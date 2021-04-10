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

#ifndef CATALOG_H
#define CATALOG_H

#include "astrofile.h"

#include <QObject>
#include <QFileInfo>

class Catalog : public QObject
{
    Q_OBJECT
public:
    explicit Catalog(QObject *parent = nullptr);
    ~Catalog();

    void addSearchFolder(const QString& folder);
    void addSearchFolder(const QList<QString>& folders);


//    void hideAstroFile(const AstroFile& astroFile);
//    void unhideAstroFile(const AstroFile& astroFile);

//    void removeAstroFilesInFolder(const QString& folder);

//    /*
//     * The Folder Crawler will ask for this.
//     * There might be a large number of incoming requests for this
//     * while there is also a large number of requests for "addAstroFile"
//     * coming from the db.
//     */
    bool shouldProcessFile(const QFileInfo& fileInfo);


    int getNumberOfItems();
    int astroFileIndex(const AstroFile& astroFile); // Returns the 0-based row number of the object. -1 on failure
    AstroFile* getAstroFile(int row);

public slots:
    void addAstroFile(const AstroFile& astroFile);
    void addAstroFiles(const QList<AstroFile>& files);

    void deleteAstroFile(const AstroFile& astroFile);
    void deleteAstroFiles(const QList<AstroFile>& files);

signals:
//    void AstroFileAdded(AstroFile astroFile, int row);
    void AstroFilesAdded(int numberAdded);
    void AstroFileUpdated(AstroFile astroFile, int row);
    void AstroFileRemoved(AstroFile astroFile, int row);

private:
    QRecursiveMutex listMutex;

    QList<QString> searchFolders;
    QList<AstroFile*> astroFiles;
    QMap<QString, AstroFile*> filePathToIdMap;

    AstroFile* getAstroFileByPath(QString path);
    void impAddAstroFile(const AstroFile& astroFile, bool shouldEmit = true);

};

#endif // CATALOG_H