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

#include "fileviewmodel.h"

#include <QBuffer>
#include <QPixmap>
#include <QPixmapCache>

FileViewModel::FileViewModel(QObject* parent) : QAbstractItemModel(parent)
{
    rc = 0;
    cc = 1;
}

FileViewModel::~FileViewModel()
{
}

void FileViewModel::setInitialModel(int count)
{
    qDebug()<<"Setting initial model";
    insertRows(0, count, QModelIndex());
}

QHash<int, QByteArray> FileViewModel::roleNames() const
{
    QHash<int, QByteArray> roles;
        roles[AstroFileRoles::IdRole] = "id";
        roles[AstroFileRoles::DisplayRole] = "display";
        roles[AstroFileRoles::DecorationRole] = "decoration";
        roles[AstroFileRoles::ObjectRole] = "object";
        roles[AstroFileRoles::InstrumentRole] = "instrument";
        roles[AstroFileRoles::FilterRole] = "filter";
        roles[AstroFileRoles::DateRole] = "date";
        roles[AstroFileRoles::FullPathRole] = "fullpath";
        roles[AstroFileRoles::RaRole] = "ra";
        roles[AstroFileRoles::DecRole] = "dec";
        roles[AstroFileRoles::CcdTempRole] = "ccd";
        roles[AstroFileRoles::ImageXSizeRole] = "imagexsize";
        roles[AstroFileRoles::ImageYSizeRole] = "imageysize";
        roles[AstroFileRoles::GainRole] = "gain";
        roles[AstroFileRoles::ExposureRole] = "exposure";
        roles[AstroFileRoles::BayerModeRole] = "bayermode";
        roles[AstroFileRoles::OffsetRole] = "offset";
        roles[AstroFileRoles::FileTypeRole] = "filetype";
        roles[AstroFileRoles::FileExtensionRole] = "fileextension";
        roles[AstroFileRoles::FileHashRole] = "filehash";
        return roles;
}

int FileViewModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return rc;
}

int FileViewModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return cc;
}

QModelIndex FileViewModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    // none of our items have valid parents, because they all are root items
    return QModelIndex();
}

QModelIndex FileViewModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (row < rc  && row >= 0 && column < cc && column >= 0)
    {
        auto a = catalog->getAstroFile(row);
        return createIndex(row, 0, a);
    }
    return QModelIndex();
}

bool FileViewModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (parent.isValid())
        return false;
    beginInsertRows(parent, row, row + count -1);
    rc+= count;
    endInsertRows();
    if (rc >= 1)
        emit modelIsEmpty(false);

    return true;
}

bool FileViewModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (parent.isValid())
        return false;

    beginRemoveRows(parent, row, row + count - 1);
    rc-= count;
    endRemoveRows();
    if (rc == 0)
        emit modelIsEmpty(true);

    // Do not "emit" astroFileDeleted, but instead call it directly.
    // Otherwise due to threading, the row will be wrong.
//    emit astroFileDeleted(row);
    catalog->deleteAstroFileRow(row);
    return true;
}

bool FileViewModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    Q_UNUSED(count);
    beginInsertColumns(parent,column, column + count -1);
    cc += count;
    endInsertColumns();
    return true;
}

bool FileViewModel::hasChildren(const QModelIndex &parent) const
{
    if (parent.isValid())
        return false;
    return true;
}

void FileViewModel::setCatalog(Catalog *cat)
{
    this->catalog = cat;
}

void FileViewModel::setCellSize(const int newSize)
{
    emit layoutAboutToBeChanged();
    int size = 400 * newSize/100;
    cellSize = QSize(size,size);
    emit layoutChanged();
}

QString imageToString(QImage img)
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer,"JPEG");
     //save image data in string
    QString image("data:image/jpg;base64,");
    image.append(QString::fromLatin1(byteArray.toBase64().data()));
    return image;
}

QVariant FileViewModel::data(const QModelIndex &index, int role) const
{
    if (index.row() >= rc)
    {
        return QVariant();
    }
    AstroFile a(*catalog->getAstroFile(index.row()));

    switch (role)
    {
        case Qt::DisplayRole:
        {
            return a.FileName;
        }
        case Qt::DecorationRole:
        {
            QPixmap pixmap;
            if (!QPixmapCache::find(QString::number(a.Id), &pixmap))
            {
//                qDebug()<<"Requesting thumb from db for: " << a.Id;
                emit loadThumbnailFromDb(a);
                auto img = a.tinyThumbnail.scaled( cellSize*0.9, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                return img;
            }
            else
            {
//                qDebug()<<"Showing thumb for: " << a.Id;
                QImage image = pixmap.toImage();
                QImage small = image.scaled( cellSize*0.9, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                return small;
            }
        }
        case AstroFileRoles::DisplayRole:
        {
            return a.FileName;
        }
        case AstroFileRoles::DecorationRole:
        {
            QPixmap pixmap;
            if (!QPixmapCache::find(QString::number(a.Id), &pixmap))
            {
    //                qDebug()<<"Requesting thumb from db for: " << a.Id;
                emit loadThumbnailFromDb(a);
                auto img = a.tinyThumbnail.scaled( cellSize*0.9, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                return imageToString(img);
            }
            else
            {
    //                qDebug()<<"Showing thumb for: " << a.Id;
                QImage image = pixmap.toImage();
                QImage small = image.scaled( cellSize*0.9, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                return imageToString(small);
            }
        }
        case Qt::SizeHintRole:
        {
            return cellSize;
        }
        case AstroFileRoles::IdRole:
        {
            return a.Id;
        }
        case AstroFileRoles::ObjectRole:
        {
            return a.Tags["OBJECT"];
        }
        case AstroFileRoles::InstrumentRole:
        {
            return a.Tags["INSTRUME"];
        }
        case AstroFileRoles::FilterRole:
        {
            return a.Tags["FILTER"];
        }
        case AstroFileRoles::DateRole:
        {
            return a.Tags["DATE-OBS"];
        }
        case AstroFileRoles::FullPathRole:
        {
            return a.FullPath;
        }
        case AstroFileRoles::RaRole:
        {
            return a.Tags["RA"];
        }
        case AstroFileRoles::DecRole:
        {
            return a.Tags["DEC"];
        }
        case AstroFileRoles::CcdTempRole:
        {
            return a.Tags["CCD-TEMP"];
        }
        case AstroFileRoles::ImageXSizeRole:
        {
            return a.Tags["NAXIS1"];
        }
        case AstroFileRoles::ImageYSizeRole:
        {
            return a.Tags["NAXIS2"];
        }
        case AstroFileRoles::GainRole:
        {
            return a.Tags["GAIN"];
        }
        case AstroFileRoles::ExposureRole:
        {
            return a.Tags["EXPTIME"];
        }
        case AstroFileRoles::BayerModeRole:
        {
            return a.Tags["BAYERPAT"];
        }
        case AstroFileRoles::OffsetRole:
        {
            return a.Tags["BLKLEVEL"];
        }
        case AstroFileRoles::FileTypeRole:
        {
            return a.FileType;
        }
        case AstroFileRoles::FileExtensionRole:
        {
            return a.FileExtension;
        }
        case AstroFileRoles::FileHashRole:
        {
            return a.FileHash;
        }
    }

    return QVariant();
}

void FileViewModel::AddAstroFiles(int numberAdded)
{
    insertRows(rc, numberAdded, QModelIndex());
}

void FileViewModel::UpdateAstroFile(AstroFile astroFile, int row)
{
    auto index = createIndex(row, 0);
    emit dataChanged(index, index);
}

void FileViewModel::RemoveAstroFile(const AstroFile& astroFile)
{
//    qDebug()<<"Removed from model: " << astroFile.FullPath;
    int row = catalog->astroFileIndex(astroFile);
    removeRow(row);
}

void FileViewModel::RemoveAstroFiles(const QList<AstroFile> &astroFiles)
{
    int row = catalog->astroFileIndex(astroFiles.first());
    removeRows(row, astroFiles.count(), QModelIndex());
}

void FileViewModel::addThumbnail(const AstroFile &astroFile)
{
    int row = catalog->astroFileIndex(astroFile);
    auto index = createIndex(row, 0);
//    qDebug()<<"Inserting into PixmapCache: " << astroFile.Id << " row: " << row;
    QPixmapCache::insert(QString::number(astroFile.Id), QPixmap::fromImage(astroFile.thumbnail));
    emit dataChanged(index, index, {AstroFileRoles::DecorationRole});
}
