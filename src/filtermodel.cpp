/*
    MIT License

    Copyright (c) 2021 Astrocat.App

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include "filtermodel.h"

TagCount::TagCount(QString tagName, int count)
{
    this->tagName = tagName;
    this->tagCount = count;
}

int TagCount::getCount() const
{
    return tagCount;
}

QString TagCount::getTagName() const
{
    return tagName;
}

void TagCount::incrementCount()
{
    tagCount++;
}

void TagCount::decrementCount()
{
    tagCount--;
}


TreeItem::TreeItem(bool isTagItem, QString groupName, TagCount* tag, TreeItem *parent)
    :  m_parentItem(parent), m_isTagItem(isTagItem), m_groupName(groupName), m_tagCount(tag ? tag->getTagName(): "None", 0)
{
//    if (isTagItem)
//    {
//        m_tagCount = *tag;
//    }
}

TreeItem::~TreeItem()
{
    qDeleteAll(m_childItems);
}

void TreeItem::appendChild(TreeItem *item)
{
    m_childItems.append(item);
}

QVector<TreeItem *> TreeItem::getChildren()
{
    return m_childItems;
}

bool TreeItem::isTagItem()
{
    return m_isTagItem;
}

QString TreeItem::getGroupName()
{
    return m_groupName;
}

TagCount *TreeItem::getTagCount()
{
    return &m_tagCount;
}

int TreeItem::childCount() const
{
    return m_childItems.count();
}

int TreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

int TreeItem::columnCount() const
{
    if (m_isTagItem)
        return 2;
    else return 1;
}

TreeItem *TreeItem::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

QVariant TreeItem::data(int column) const
{
    if (m_isTagItem)
    {
        if (column == 0)
            return m_tagCount.getTagName();
        if (column == 1)
            return m_tagCount.getCount();
    }
    else
    {
        if (column < 0 || column >= 1)
            return QVariant();
        return m_groupName;
    }
    return QVariant();
}

TreeItem *TreeItem::parentItem()
{
    return m_parentItem;
}

FilterModel::FilterModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    QList<QString> groupNames {"OBJECT", "INSTRUME", "FILTER", "DATE-OBS", "FILEEXT"};
    groups.append(groupNames);
    rootItem = new TreeItem(false, "ROOT", nullptr);
    rootItem->appendChild(new TreeItem(false, "OBJECT", nullptr, rootItem));
    rootItem->appendChild(new TreeItem(false, "INSTRUME", nullptr, rootItem));
    rootItem->appendChild(new TreeItem(false, "FILTER", nullptr, rootItem));
//    rootItem->appendChild(new TreeItem(false, "DATE-OBS", nullptr, rootItem));
    rootItem->appendChild(new TreeItem(false, "FILEEXT", nullptr, rootItem));
}

void FilterModel::addAstroFile(const AstroFile &astroFile)
{
    int id = astroFile.Id;
    if (!acceptedAstroFiles.contains(id))
    {
        acceptedAstroFiles.insert(id);
        int currentGroupRow = 0;
        for (auto a : rootItem->getChildren())
        {
            updateTagCount(astroFile, a->getGroupName(), a);
            currentGroupRow++;
        }
        insertRow(groups.count() - 1, QModelIndex());
    }

}

void FilterModel::updateTagCount(const AstroFile& astroFile, QString tag, TreeItem* treeItem)
{
    auto children = rootItem->getChildren();
    for (auto& a: children)
    {
        auto groupName = a->getGroupName();
        if (astroFile.Tags.contains(groupName))
        {
            auto tagChildren = a->getChildren();
            int tagRow = 0;
            bool tagFound = false;
            for (auto& c: tagChildren)
            {
                if (c->getTagCount()->getTagName() == astroFile.Tags[groupName])
                {
                    c->getTagCount()->incrementCount();

                    // emit that data has changed here.
                    dataChanged(createIndex(tagRow, 2, treeItem), createIndex(tagRow, 2, treeItem));
                    tagFound = true;
                    break;
                }
                tagRow++;
            }
            if (tagFound)
                continue;
            // We didn't find this tag in the tree, so let's create a new one.
            auto tagName = astroFile.Tags[groupName];
//            qDebug ()<< "Creating tag: " <<tagName;
            a->appendChild(new TreeItem(true, groupName, new TagCount(astroFile.Tags[groupName] , 1)));

            insertRow(tagRow, createIndex(tagRow, 1, treeItem));
            insertRow(tagRow, createIndex(tagRow, 2, treeItem));
        }
    }
}

QModelIndex FilterModel::index(int row, int column, const QModelIndex &parent) const
{
    TreeItem* parentItem;

    if (!parent.isValid())
    {
        parentItem = rootItem;
    }
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);

    return QModelIndex();
}

QModelIndex FilterModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int FilterModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int FilterModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;
    return 2;
}

QVariant FilterModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    switch (role)
    {
        case FilterRoles::FilterGroupRole:
        {
            return item->getGroupName();
        }
        case FilterRoles::FilterTagNameRole:
        {
            qDebug()<<"returning filtertagnamerole : " << item->getTagCount();
            return item->getTagCount()->getTagName();
        }
        case FilterRoles::FilterTagCounts:
        {
            QVariantList varList;
            for (auto a : item->getChildren())
            {
                varList.append(QVariant::fromValue(a->getTagCount()));
                a->getTagCount();
            }
//            qDebug()<<"Filter returning tagCounts: " << varList.count();
            return varList;
        }
    }

    return QVariant();
}

QHash<int, QByteArray> FilterModel::roleNames() const
{
    QHash<int, QByteArray> roles;
        roles[FilterRoles::FilterIdRole] = "id";
        roles[FilterRoles::FilterGroupRole] = "group";
        roles[FilterRoles::FilterTagNameRole] = "tagname";
        roles[FilterRoles::FilterTagValueRole] = "tagvalue";
        roles[FilterRoles::FilterTagCounts] = "tagcounts";
        return roles;        
}

void FilterModel::astroFileAdded(const AstroFile &astroFile)
{
//    qDebug() << "Adding astrofile: " <<astroFile.FileName;
    addAstroFile(astroFile);
}

