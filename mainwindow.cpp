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

#include "mainwindow.h"
#include "searchfolderdialog.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      isInitialized(false)
{
    ui->setupUi(this);

    folderCrawlerThread = new QThread(this);
    folderCrawlerWorker = new FolderCrawler;
    folderCrawlerWorker->moveToThread(folderCrawlerThread);

    fileRepositoryThread = new QThread(this);
    fileRepositoryWorker = new FileRepository;
    fileRepositoryWorker->moveToThread(fileRepositoryThread);

    fitsProcessorThread = new QThread(this);
    fitsProcessorWorker = new FitsProcessor;
    fitsProcessorWorker->moveToThread(fitsProcessorThread);

    fileViewModel = new FileViewModel(ui->astroListView);
    sortFilterProxyModel = new SortFilterProxyModel(ui->astroListView);
    sortFilterProxyModel->setSourceModel(fileViewModel);
    ui->astroListView->setViewMode(QListView::IconMode);
    ui->astroListView->setIconSize(QSize(100,100));
    ui->astroListView->setResizeMode(QListView::Adjust);
    ui->astroListView->setModel(sortFilterProxyModel);
    filterWidget = new FilterWidget(ui->scrollAreaWidgetContents_2);

    connect(this,                   &MainWindow::crawl,                                 folderCrawlerWorker,    &FolderCrawler::crawl);
    connect(this,                   &MainWindow::getAstroFile,                          fileRepositoryWorker,   &FileRepository::getAstrofile);
    connect(this,                   &MainWindow::getAllAstroFiles,                      fileRepositoryWorker,   &FileRepository::getAllAstrofiles);
    connect(this,                   &MainWindow::insertAstroFile,                       fileRepositoryWorker,   &FileRepository::insertAstrofile);
    connect(this,                   &MainWindow::dbAddTags,                             fileRepositoryWorker,   &FileRepository::addTags);
    connect(this,                   &MainWindow::dbAddThumbnail,                        fileRepositoryWorker,   &FileRepository::addThumbnail);
    connect(this,                   &MainWindow::dbGetThumbnails,                       fileRepositoryWorker,   &FileRepository::getThumbnails);
    connect(this,                   &MainWindow::initializeFileRepository,              fileRepositoryWorker,   &FileRepository::initialize);
    connect(this,                   &MainWindow::getAllAstroFileTags,                   fileRepositoryWorker,   &FileRepository::getTags);
    connect(this,                   &MainWindow::deleteAstrofilesInFolder,              fileRepositoryWorker,   &FileRepository::deleteAstrofilesInFolder);
    connect(this,                   &MainWindow::processFitsFile,                       fitsProcessorWorker,    &FitsProcessor::processFitsFile);
    connect(folderCrawlerWorker,    &FolderCrawler::fileFound,                          this,                   &MainWindow::newFileFound);
    connect(folderCrawlerThread,    &QThread::finished,                                 folderCrawlerWorker,    &QObject::deleteLater);
    connect(fileRepositoryWorker,   &FileRepository::getAstroFileFinished,              this,                   &MainWindow::getAstroFileFinished);
    connect(fileRepositoryWorker,   &FileRepository::getAllAstroFilesFinished,          this,                   &MainWindow::getAllAstroFilesFinished);
    connect(fileRepositoryWorker,   &FileRepository::getThumbnailFinished,              this,                   &MainWindow::getThumbnailFinished);
    connect(fileRepositoryWorker,   &FileRepository::getTagsFinished,                   this,                   &MainWindow::getAllAstroFileTagsFinished);
    connect(fileRepositoryWorker,   &FileRepository::getTagsFinished,                   filterWidget,           &FilterWidget::setAllTags);
    connect(fileRepositoryWorker,   &FileRepository::getThumbnailFinished,              fileViewModel,          &FileViewModel::getThumbnailFinished);
    connect(fileRepositoryWorker,   &FileRepository::astroFileDeleted,                  fileViewModel,          &FileViewModel::RemoveAstroFile);
    connect(fileRepositoryThread,   &QThread::finished,                                 fileRepositoryWorker,   &QObject::deleteLater);
    connect(fitsProcessorWorker,    &FitsProcessor::processFitsFileFinished,            this,                   &MainWindow::processFitsFileFinished);
    connect(fitsProcessorThread,    &QThread::finished,                                 fitsProcessorWorker,    &QObject::deleteLater);
    connect(fileViewModel,          &FileViewModel::getThumbnail,                       fileRepositoryWorker,   &FileRepository::getThumbnail);
    connect(&searchFolderDialog,    &SearchFolderDialog::searchFolderAdded,             folderCrawlerWorker,    &FolderCrawler::crawl);
    connect(&searchFolderDialog,    &SearchFolderDialog::searchFolderRemoved,           this,                   &MainWindow::searchFolderRemoved);
    connect(sortFilterProxyModel,   &SortFilterProxyModel::filterMinimumDateChanged,    filterWidget,           &FilterWidget::setFilterMinimumDate);
    connect(sortFilterProxyModel,   &SortFilterProxyModel::filterMaximumDateChanged,    filterWidget,           &FilterWidget::setFilterMaximumDate);
    connect(sortFilterProxyModel,   &SortFilterProxyModel::filterReset,                 filterWidget,           &FilterWidget::searchFilterReset);
    connect(sortFilterProxyModel,   &SortFilterProxyModel::astroFileAccepted,           filterWidget,           &FilterWidget::addAstroFileTags);
    connect(filterWidget,           &FilterWidget::minimumDateChanged,                  sortFilterProxyModel,   &SortFilterProxyModel::setFilterMinimumDate);
    connect(filterWidget,           &FilterWidget::maximumDateChanged,                  sortFilterProxyModel,   &SortFilterProxyModel::setFilterMaximumDate);
    connect(filterWidget,           &FilterWidget::addAcceptedFilter,                   sortFilterProxyModel,   &SortFilterProxyModel::addAcceptedFilter);
    connect(filterWidget,           &FilterWidget::addAcceptedInstrument,               sortFilterProxyModel,   &SortFilterProxyModel::addAcceptedInstrument);
    connect(filterWidget,           &FilterWidget::addAcceptedObject,                   sortFilterProxyModel,   &SortFilterProxyModel::addAcceptedObject);
    connect(filterWidget,           &FilterWidget::removeAcceptedFilter,                sortFilterProxyModel,   &SortFilterProxyModel::removeAcceptedFilter);
    connect(filterWidget,           &FilterWidget::removeAcceptedInstrument,            sortFilterProxyModel,   &SortFilterProxyModel::removeAcceptedInstrument);
    connect(filterWidget,           &FilterWidget::removeAcceptedObject,                sortFilterProxyModel,   &SortFilterProxyModel::removeAcceptedObject);

    // Enable the tester during development and debugging. Disble before committing
    //tester = new QAbstractItemModelTester(fileViewModel, QAbstractItemModelTester::FailureReportingMode::Fatal, this);
}

MainWindow::~MainWindow()
{
    qDebug()<<"Cleaning up folderCrawlerThread";
    cleanUpWorker(folderCrawlerThread);

    qDebug()<<"Cleaning up fitsProcessorThread";
    fitsProcessorWorker->cancel();
    cleanUpWorker(fitsProcessorThread);

    qDebug()<<"Cleaning up fileRepositoryThread";
    cleanUpWorker(fileRepositoryThread);

    qDebug()<<"Cleaning up fileViewModel";
    delete fileViewModel;
    qDebug()<<"Cleaning up ui";
    delete ui;
    qDebug()<<"Done Cleaning up.";
}

void MainWindow::initialize()
{
    if (isInitialized)
        return;

    folderCrawlerThread->start();
    fileRepositoryThread->start();
    fitsProcessorThread->start();

    emit initializeFileRepository();
    emit getAllAstroFileTags();
    emit getAllAstroFiles();
    emit dbGetThumbnails();
    isInitialized = true;
}

void MainWindow::cleanUpWorker(QThread* thread)
{
    thread->quit();
    thread->wait();
    delete thread;
}

void MainWindow::newFileFound(const QFileInfo fileInfo)
{
    AstroFile astroFile;
    astroFile.FullPath = fileInfo.absoluteFilePath();
    astroFile.CreatedTime = fileInfo.birthTime();
    astroFile.LastModifiedTime = fileInfo.lastModified();
    astroFile.DirectoryPath = fileInfo.canonicalPath();
    astroFile.FileType = fileInfo.suffix();
    astroFile.FileName = fileInfo.baseName();

    if (fileViewModel->astroFileExists(fileInfo.absoluteFilePath()))
    {
        qDebug() << "File already in DB";
    }
    else
    {
        QImage img;
        fileViewModel->addAstroFile(astroFile, img);
        emit insertAstroFile(astroFile);
        emit processFitsFile(astroFile);
        emit getAstroFile(fileInfo.absoluteFilePath());
    }
}

void MainWindow::getAstroFileFinished(const AstroFile astroFile)
{
//    qDebug() << "AstroFile from DB: " << astroFile.FullPath;

}

void MainWindow::processFitsFileFinished(const AstroFile astroFile, const QImage& img, long nX, long nY )
{
    emit dbAddTags(astroFile);
    QImage thumbnail = makeThumbnail(img);
    emit dbAddThumbnail(astroFile, thumbnail);

    fileViewModel->addAstroFile(astroFile, img.scaled( 400, 400, Qt::KeepAspectRatio));
}

void MainWindow::searchFolderRemoved(const QString folder)
{
    // The source folder was removed by the user. We will need to remove all images in this source folder from the db.
    emit deleteAstrofilesInFolder(folder);
}

void MainWindow::on_pushButton_clicked()
{
//    emit GetAllAstroFileTags();
//    QDate date(2020, 11, 04);
//    sortFilterProxyModel->setFilterMinimumDate(date);
//    sortFilterProxyModel->invalidate();
}

void MainWindow::on_imageSizeSlider_valueChanged(int value)
{
    emit fileViewModel->setCellSize(value);
}

void MainWindow::getAllAstroFilesFinished(const QList<AstroFile> & files)
{
    fileViewModel->setInitialAstrofiles(files);
}

void MainWindow::getAllAstroFileTagsFinished(const QMap<QString, QSet<QString>> & tags)
{
    QMapIterator<QString, QSet<QString>> iter(tags);

    while(iter.hasNext())
    {
        iter.next();
        qDebug() << "===" << iter.key();
        QSetIterator setiter(iter.value());
        while (setiter.hasNext())
        {
            qDebug() << "------" << setiter.next();
        }
    }
}

void MainWindow::getThumbnailFinished(const AstroFile& astroFile, const QPixmap& pixmap)
{
}

QImage MainWindow::makeThumbnail(const QImage &image)
{
    QImage small =image.scaled( QSize(200, 200), Qt::KeepAspectRatio);
    return small;
}

void MainWindow::on_actionFolders_triggered()
{
    searchFolderDialog.exec();
}
