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

#ifndef FILTERMODEL_H
#define FILTERMODEL_H

#include "astrofile.h"

#include <QAbstractItemModel>

enum FilterRoles
{
    FilterIdRole = Qt::UserRole,
    FilterGroupRole,
    FilterTagNameRole,
    FilterTagValueRole,
    FilterTagCounts
};

class TreeItem;

class TagCount : public QObject
{
    Q_OBJECT
public:
    TagCount(QString tagName, int count);
    Q_INVOKABLE int getCount() const;
    Q_INVOKABLE QString getTagName() const;
    void incrementCount();
    void decrementCount();
private:
    QString tagName;
    int tagCount;
    TreeItem* m_parentItem;
};

class TreeItem
{
public:
    explicit TreeItem(bool isTagItem, QString groupName, TagCount* tag, TreeItem *parentItem = nullptr);
    ~TreeItem();

    void appendChild(TreeItem *child);
    QVector<TreeItem*>  getChildren();
    bool isTagItem();
    QString getGroupName();
    TagCount* getTagCount();

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeItem *parentItem();

private:
    QVector<TreeItem*> m_childItems;
//    QVector<QVariant> m_itemData;
    TreeItem *m_parentItem;
    bool m_isTagItem;
    QString m_groupName;
    TagCount m_tagCount;
};



class FilterModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit FilterModel(QObject *parent = nullptr);

    void addAstroFile(const AstroFile& astroFile);
    void updateTagCount(const AstroFile& astroFile, QString tag, TreeItem* treeItem);

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const;

public slots:
    void astroFileAdded(const AstroFile& astroFile);

private:
//    QMap<QString, QMap<QString,int>> fileTags;
    QSet<int> acceptedAstroFiles;
    QList<QString> groups;
    TreeItem *rootItem;
};

#endif // FILTERMODEL_H
