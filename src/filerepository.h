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

#ifndef FILEREPOSITORY_H
#define FILEREPOSITORY_H

#include "astrofile.h"

#include <QObject>
#include <QSqlDatabase>

class FileRepository : public QObject
{
    Q_OBJECT
public:
    FileRepository(QObject *parent = nullptr);
    void cancel();

public slots:
    void deleteAstrofilesInFolder(const QString& fullPath);
    void initialize();
    void loadModel();
    void addOrUpdateAstrofile(const AstroFile& afi);
    void getDuplicateFiles();
    void getDuplicateFilesByFileHash();
    void getDuplicateFilesByImageHash();
    void loadThumbnal(const AstroFile& afi);

signals:
    void getAllAstroFilesFinished(const QList<AstroFile>& astroFiles );
    void getTagsFinished(const QMap<QString, QSet<QString>>& tags);
    void astroFileDeleted(const AstroFile& astroFile);
    void astroFilesDeleted(const QList<AstroFile>& astroFiles);
    void modelLoaded(const QList<AstroFile>& astroFile);
    void dbFailedToInitialize(const QString& message);
    void astroFileUpdated(const AstroFile& astroFile);
    void thumbnailLoaded(const AstroFile& astrofile);
    void modelLoadingGotAstrofiles();
    void modelLoadingGotTags();
    void modelLoadingGotThumbnails();

private:
    QSqlDatabase db;
    void createTables();
    void createDatabase();
    void migrateDatabase();
    void migrateFromVersion(int oldVersion);
    int insertAstrofile(const AstroFile& afi);
    void addTags(const AstroFile& astroFile);
    void addThumbnail(const AstroFile& astroFile);
    QList<AstroFile> getAstrofilesInFolder(const QString& fullPath);
    QMap<int, AstroFile> _getAllAstrofiles();
    QMap<int, QImage> _getAllThumbnails();

    volatile bool cancelSignaled = false;
};

#endif // FILEREPOSITORY_H
