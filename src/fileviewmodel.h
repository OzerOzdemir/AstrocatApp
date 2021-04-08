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

#ifndef FILEVIEWMODEL_H
#define FILEVIEWMODEL_H

#include "astrofile.h"

#include <QAbstractItemModel>
#include <QImage>

enum AstroFileRoles
{
    IdRole = Qt::UserRole,
    InstrumentRole,
    ObjectRole,
    FilterRole,
    DateRole,
    FullPathRole,
    RaRole,
    DecRole,
    CcdTempRole,
    ImageXSizeRole,
    ImageYSizeRole,
    GainRole,
    ExposureRole,
    BayerModeRole,
    OffsetRole,
    FileTypeRole,
    FileExtensionRole,
    FileHashRole
};

class FileViewModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    FileViewModel(QObject* parent = nullptr);
    ~FileViewModel();
    bool astroFileExists(const QString fullPath);
    AstroFile getAstroFileById(int id);
    int getAstroFileIdByPath(const QString& fullPath);

    // QAbstractItemModel interface
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;
    bool insertColumns(int column, int count, const QModelIndex &parent) override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    bool hasChildren(const QModelIndex &parent) const override;

public slots:
    void setCellSize(const int newSize);
    void removeAstroFile(AstroFile astroFile);
    void setInitialModel(const QList<AstroFile>& files);
    void addAstroFile(const AstroFile& astroFile);
    void addThumbnail(const AstroFile& astroFile);

signals:
    void modelIsEmpty(bool isEmpty);
    void loadThumbnailFromDb(const AstroFile& astroFile) const;

private:
    int rc;
    int cc;

    // We should try to get rid of the doulbe collection of AstroFiles here.
    QList<AstroFile> fileList;
    QMap<QString, int> filePathToIdMap;
    QMap<int, AstroFile> fileIdMap;
    int getRowForAstroFile(const AstroFile& astroFile);
    QModelIndex getIndexForAstroFile(const AstroFile& astroFile);
    QSize cellSize = QSize(200, 200);
};

#endif // FILEVIEWMODEL_H
