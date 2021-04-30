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

#include "appmanager.h"

#include <QDebug>
#include <QItemSelection>
#include <QPixmapCache>
#include <QSettings>

AppManager::AppManager(QWidget *parent)
{

}

AppManager::~AppManager()
{
    cancelPendingOperations();

    qDebug()<<"Cleaning up folderCrawlerThread";
    cleanUpWorker(folderCrawlerThread);

    qDebug()<<"Cleaning up newFileProcessorThread";
    cleanUpWorker(newFileProcessorThread);

    qDebug()<<"Cleaning up fileRepositoryThread";
    cleanUpWorker(fileRepositoryThread);

    qDebug()<<"Cleaning up fileViewModel";
    delete fileViewModel;

    qDebug()<<"Cleaning up catalogThread";
    cleanUpWorker(catalogThread);

    qDebug()<<"Cleaning up ui";
//    delete ui;
    qDebug()<<"Done Cleaning up.";
}

void AppManager::initialize()
{
    if (isInitialized)
    {
        qDebug() << "Already Initialized";
        return;
    }

    isInitialized = true;

    catalogThread = new QThread(this);
    catalog = new Catalog;
    catalog->moveToThread(catalogThread);

    folderCrawlerThread = new QThread(this);
    folderCrawlerWorker = new FolderCrawler;
    folderCrawlerWorker->moveToThread(folderCrawlerThread);

    fileRepositoryThread = new QThread(this);
    fileRepositoryWorker = new FileRepository;
    fileRepositoryWorker->moveToThread(fileRepositoryThread);

    newFileProcessorThread = new QThread(this);

//    newFileProcessorWorker = new Mock_NewFileProcessor;
    newFileProcessorWorker = new NewFileProcessor;
    newFileProcessorWorker->setCatalog(catalog);

    newFileProcessorWorker->moveToThread(newFileProcessorThread);

    fileFilter = new FileProcessFilter;
    fileFilter->setCatalog(catalog);
    fileFilter->moveToThread(folderCrawlerThread);

    fileViewModel = new FileViewModel(); // Do we need to set a parent here?
    fileViewModel->setCatalog(catalog);
    sortFilterProxyModel = new SortFilterProxyModel(); // Do we need to set a parent here?
    sortFilterProxyModel->setSourceModel(fileViewModel);

    thumbnailCache.moveToThread(fileRepositoryThread);
    thumbnailCache.start();

    QPixmapCache::setCacheLimit(100*1024);

    connect(this,                   &AppManager::crawl,                                 folderCrawlerWorker,    &FolderCrawler::crawl);
    connect(this,                   &AppManager::initializeFileRepository,              fileRepositoryWorker,   &FileRepository::initialize);
    connect(this,                   &AppManager::deleteAstrofilesInFolder,              fileRepositoryWorker,   &FileRepository::deleteAstrofilesInFolder);
    connect(this,                   &AppManager::loadModelFromDb,                       fileRepositoryWorker,   &FileRepository::loadModel);
    connect(this,                   &AppManager::dbAddOrUpdateAstroFile,                fileRepositoryWorker,   &FileRepository::addOrUpdateAstrofile);
    connect(this,                   &AppManager::processNewFile,                        newFileProcessorWorker, &NewFileProcessor::processNewFile);
    connect(this,                   &AppManager::dbGetDuplicates,                       fileRepositoryWorker,   &FileRepository::getDuplicateFiles);
    connect(catalogThread,          &QThread::finished,                                 catalog,                &QObject::deleteLater);
    connect(catalog,                &Catalog::AstroFilesAdded,                          fileViewModel,          &FileViewModel::AddAstroFiles);
    connect(catalog,                &Catalog::AstroFileUpdated,                         fileViewModel,          &FileViewModel::UpdateAstroFile);
    connect(this,                   &AppManager::catalogAddAstroFile,                   catalog,                &Catalog::addAstroFile);
    connect(this,                   &AppManager::catalogAddAstroFiles,                  catalog,                &Catalog::addAstroFiles);
    connect(folderCrawlerThread,    &QThread::finished,                                 folderCrawlerWorker,    &QObject::deleteLater);
    connect(folderCrawlerWorker,    &FolderCrawler::fileFound,                          fileFilter,             &FileProcessFilter::filterFile);
    connect(fileFilter,             &FileProcessFilter::shouldProcess,                  newFileProcessorWorker, &NewFileProcessor::processNewFile);
    connect(fileFilter,             &FileProcessFilter::shouldProcess,                  this,                   &AppManager::processQueued);
    connect(fileRepositoryWorker,   &FileRepository::astroFileUpdated,                  this,                   &AppManager::dbAstroFileUpdated);
    connect(fileRepositoryWorker,   &FileRepository::astroFileDeleted,                  fileViewModel,          &FileViewModel::RemoveAstroFile);
    connect(fileRepositoryWorker,   &FileRepository::astroFilesDeleted,                 fileViewModel,          &FileViewModel::RemoveAstroFiles);
    connect(fileRepositoryWorker,   &FileRepository::modelLoaded,                       this,                   &AppManager::modelLoadedFromDb);
    connect(fileRepositoryWorker,   &FileRepository::dbFailedToInitialize,              this,                   &AppManager::dbFailedToOpen);
    connect(fileRepositoryWorker,   &FileRepository::thumbnailLoaded,                   fileViewModel,          &FileViewModel::addThumbnail);
    connect(fileRepositoryThread,   &QThread::finished,                                 fileRepositoryWorker,   &QObject::deleteLater);
    connect(newFileProcessorWorker, &NewFileProcessor::astrofileProcessed,              this,                   &AppManager::astroFileProcessed);
    connect(newFileProcessorWorker, &NewFileProcessor::processingCancelled,             this,                   &AppManager::processingCancelled);
    connect(newFileProcessorThread, &QThread::finished,                                 newFileProcessorWorker, &QObject::deleteLater);
//    connect(&searchFolderDialog,    &SearchFolderDialog::searchFolderAdded,             this,                   &AppManager::searchFolderAdded);
//    connect(&searchFolderDialog,    &SearchFolderDialog::searchFolderRemoved,           this,                   &AppManager::searchFolderRemoved);
//    connect(sortFilterProxyModel,   &SortFilterProxyModel::filterMinimumDateChanged,    filterView,             &FilterView::setFilterMinimumDate);
//    connect(sortFilterProxyModel,   &SortFilterProxyModel::filterMaximumDateChanged,    filterView,             &FilterView::setFilterMaximumDate);
//    connect(sortFilterProxyModel,   &SortFilterProxyModel::filterReset,                 filterView,             &FilterView::searchFilterReset);
    connect(fileViewModel,          &FileViewModel::modelIsEmpty,                       this,                   &AppManager::setWatermark);
    connect(fileViewModel,          &FileViewModel::rowsInserted,                       this,                   &AppManager::rowsAddedToModel);
    connect(fileViewModel,          &FileViewModel::rowsRemoved,                        this,                   &AppManager::rowsRemovedFromModel);
    connect(fileViewModel,          &FileViewModel::modelReset,                         this,                   &AppManager::modelReset);
    connect(fileViewModel,          &FileViewModel::loadThumbnailFromDb,                &thumbnailCache,        &ThumbnailCache::enqueueLoadThumbnail);
    connect(&thumbnailCache,        &ThumbnailCache::dbLoadThumbnail,                   fileRepositoryWorker,   &FileRepository::loadThumbnal);
//    connect(filterView,             &FilterView::minimumDateChanged,                    sortFilterProxyModel,   &SortFilterProxyModel::setFilterMinimumDate);
//    connect(filterView,             &FilterView::maximumDateChanged,                    sortFilterProxyModel,   &SortFilterProxyModel::setFilterMaximumDate);
//    connect(filterView,             &FilterView::addAcceptedFilter,                     sortFilterProxyModel,   &SortFilterProxyModel::addAcceptedFilter);
//    connect(filterView,             &FilterView::addAcceptedInstrument,                 sortFilterProxyModel,   &SortFilterProxyModel::addAcceptedInstrument);
//    connect(filterView,             &FilterView::addAcceptedObject,                     sortFilterProxyModel,   &SortFilterProxyModel::addAcceptedObject);
//    connect(filterView,             &FilterView::addAcceptedExtension,                  sortFilterProxyModel,   &SortFilterProxyModel::addAcceptedExtension);
//    connect(filterView,             &FilterView::removeAcceptedFilter,                  sortFilterProxyModel,   &SortFilterProxyModel::removeAcceptedFilter);
//    connect(filterView,             &FilterView::removeAcceptedInstrument,              sortFilterProxyModel,   &SortFilterProxyModel::removeAcceptedInstrument);
//    connect(filterView,             &FilterView::removeAcceptedObject,                  sortFilterProxyModel,   &SortFilterProxyModel::removeAcceptedObject);
//    connect(filterView,             &FilterView::removeAcceptedExtension,               sortFilterProxyModel,   &SortFilterProxyModel::removeAcceptedExtension);
//    connect(filterView,             &FilterView::astroFileAdded,                        this,                   &AppManager::itemAddedToSortFilterView);
//    connect(filterView,             &FilterView::astroFileRemoved,                      this,                   &AppManager::itemRemovedFromSortFilterView);
//    connect(ui->astroListView,      &QWidget::customContextMenuRequested,               this,                   &AppManager::itemContextMenuRequested);
//    connect(selectionModel,         &QItemSelectionModel::selectionChanged,             this,                   &AppManager::handleSelectionChanged);

    auto foldersFromList = getSearchFolders();
    catalog->addSearchFolder(foldersFromList);
    folderCrawlerThread->start();
    fileRepositoryThread->start();
    newFileProcessorThread->start();
    catalogThread->start();

//    emit initializeFileRepository();
    emit initializeFileRepository();
    emit loadModelFromDb();

//    _watermarkMessage = "Loading Catalog...";
//    setWatermark(true);
//    emit loadModelFromDb();
//    isInitialized = true;
//    emit dbGetDuplicates();
}

void AppManager::hello(QString msg)
{
    qDebug()<<msg;
}

SortFilterProxyModel* AppManager::getModel()
{
    return this->sortFilterProxyModel;
}

void AppManager::searchFolderAdded(const QString folder)
{

}

void AppManager::searchFolderRemoved(const QString folder)
{

}

void AppManager::on_imageSizeSlider_valueChanged(int value)
{

}

void AppManager::on_actionFolders_triggered()
{

}

void AppManager::handleSelectionChanged(QItemSelection selection)
{

}

void AppManager::modelLoadedFromDb(const QList<AstroFile> &files)
{
    emit catalogAddAstroFiles(files);
}

void AppManager::astroFileProcessed(const AstroFile &astroFile)
{

}

void AppManager::processingCancelled(const QFileInfo &fileInfo)
{

}

void AppManager::processQueued(const QFileInfo &fileInfo)
{

}

void AppManager::on_actionAbout_triggered()
{

}

void AppManager::setWatermark(bool shoudSet)
{

}

void AppManager::rowsAddedToModel(const QModelIndex &parent, int first, int last)
{

}

void AppManager::rowsRemovedFromModel(const QModelIndex &parent, int first, int last)
{

}

void AppManager::modelReset()
{

}

void AppManager::itemAddedToSortFilterView(int numberAdded)
{

}

void AppManager::itemRemovedFromSortFilterView(int numberRemoved)
{

}

void AppManager::itemContextMenuRequested(const QPoint &pos)
{

}

void AppManager::reveal()
{

}

void AppManager::remove()
{

}

void AppManager::on_duplicatesButton_clicked()
{

}

void AppManager::dbFailedToOpen(const QString message)
{

}

void AppManager::dbAstroFileUpdated(const AstroFile &astroFile)
{

}

void AppManager::cancelPendingOperations()
{
    catalog->removeAllSearchFolders();
    thumbnailCache.cancel();
    fileFilter->cancel();
    folderCrawlerWorker->cancel();
    newFileProcessorWorker->cancel();
    fileRepositoryWorker->cancel();
}

void AppManager::cleanUpWorker(QThread *thread)
{
    thread->quit();
    thread->wait();
    delete thread;
}

QList<QString> AppManager::getSearchFolders()
{
    QSettings settings;
    auto foldersFromList = settings.value("SearchFolders").value<QList<QString>>();
    return foldersFromList;
}
