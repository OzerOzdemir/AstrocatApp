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

#ifndef APPMANAGER_H
#define APPMANAGER_H

#include "fileprocessfilter.h"
#include "filerepository.h"
#include "fileviewmodel.h"
#include "filtermodel.h"
#include "foldercrawler.h"
#include "newfileprocessor.h"
#include "sortfilterproxymodel.h"
#include "thumbnailcache.h"

#include <QObject>
#include <QFileInfo>
#include <QThread>
#include <QItemSelection>
#include <QLabel>

class AppManager : public QObject
{
    Q_OBJECT
public:
    AppManager(QWidget *parent = nullptr);
    ~AppManager();
    void initialize();
    Q_INVOKABLE void hello(QString msg);
    Q_INVOKABLE SortFilterProxyModel* getModel();
    Q_INVOKABLE FilterModel* getFilterModel();

public slots:
//    void newFileFound(const QFileInfo fileInfo);
    void searchFolderAdded(const QString folder);
    void searchFolderRemoved(const QString folder);

signals:
    void crawl(QString rootFolder);
    void deleteAstrofilesInFolder(const QString fullPath);
    void dbAddOrUpdateAstroFile(const AstroFile& astroFile);
    void dbAddTags(const AstroFile& astroFile);
    void dbAddThumbnail(const AstroFile& astroFile, const QImage& image);
    void dbUpdateProcessStatus(const AstroFile& astroFile);
    void initializeFileRepository();
    void loadModelFromDb();
    void loadModelIntoViewModel(const QList<AstroFile> &files);
    void resetModel();
    void dbGetDuplicates();

    void extractTags(const AstroFile& astroFile);
    void extractThumbnail(const AstroFile& astroFile);
    void processNewFile(const QFileInfo& fileInfo);

    void catalogAddAstroFile(const AstroFile &file);
    void catalogAddAstroFiles(const QList<AstroFile> &files);

private slots:
    void on_imageSizeSlider_valueChanged(int value);
    void on_actionFolders_triggered();
    void handleSelectionChanged(QItemSelection selection);
    void modelLoadedFromDb(const QList<AstroFile> &files);

    void astroFileProcessed(const AstroFile& astroFile);
    void processingCancelled(const QFileInfo& fileInfo);
    void processQueued(const QFileInfo &fileInfo);

    void on_actionAbout_triggered();
    void setWatermark(bool shoudSet);

    void rowsAddedToModel(const QModelIndex &parent, int first, int last);
    void rowsRemovedFromModel(const QModelIndex &parent, int first, int last);
    void modelReset();
    void itemAddedToSortFilterView(int numberAdded);
    void itemRemovedFromSortFilterView(int numberRemoved);
    void itemContextMenuRequested(const QPoint &pos);

    void reveal();
    void remove();
    void on_duplicatesButton_clicked();

    void dbFailedToOpen(const QString message);
    void dbAstroFileUpdated(const AstroFile& astroFile);
//    void dbAstroFileDeleted(const AstroFile& astroFile);

private:
    bool isInitialized;

    QThread* folderCrawlerThread;
    FolderCrawler* folderCrawlerWorker;

    QThread* fileRepositoryThread;
    FileRepository* fileRepositoryWorker;

    QThread* newFileProcessorThread;
    NewFileProcessor* newFileProcessorWorker;

    FileViewModel* fileViewModel;
    SortFilterProxyModel* sortFilterProxyModel;

    QThread* catalogThread;
    Catalog* catalog;
    FileProcessFilter* fileFilter;

    ThumbnailCache thumbnailCache;

    FilterModel* filterModel;

    void cancelPendingOperations();
    void cleanUpWorker(QThread* thread);
    QList<QString> getSearchFolders();
};

#endif // APPMANAGER_H
