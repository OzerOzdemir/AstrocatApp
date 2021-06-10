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

#include <QtTest>
#include <QFileInfo>

#include "newfileprocessor.h"
#include "foldercrawler.h"

class Mock_NewFileProcessor : public QObject
{
    Q_OBJECT

public:
    Mock_NewFileProcessor();
    ~Mock_NewFileProcessor();

private slots:
    void test_case1();

    void test_invalidFiles();
    void test_invalidFiles_data();

};

Mock_NewFileProcessor::Mock_NewFileProcessor()
{

}

Mock_NewFileProcessor::~Mock_NewFileProcessor()
{

}

void Mock_NewFileProcessor::test_case1()
{
//    NewFileProcessor proc;
//    FolderCrawler fc;
//    QSignalSpy spy(&fc, SIGNAL(fileFound(QFileInfo)));
//    fc.crawl("/Users/ozer/test2/stretch");

//    for (auto& sig : spy )
//    {
//        auto x = sig.at(0);
//        auto fileInfo = x.value<QFileInfo>();
//        qDebug() << fileInfo.absoluteFilePath();
//        QBENCHMARK
//        {
//            proc.processNewFile(fileInfo);
//        }
//    }
}

void Mock_NewFileProcessor::test_invalidFiles_data()
{
    QTest::addColumn<QString>("filePath");
    QTest::addColumn<AstroFileProcessStatus>("expectedResult");

    QTest::newRow("invalid FITS path") <<  "/nonexistent.fits" << AstroFileProcessStatus::FailedToProcess;
    QTest::newRow("invalid XISF path") <<  "/nonexistent.fits" << AstroFileProcessStatus::FailedToProcess;
    QTest::newRow("NoAccess FITS path") <<  "/Users/ozer/test2/testcases/NoAccess.fits" << AstroFileProcessStatus::FailedToProcess;
    QTest::newRow("NoAccess XISF path") <<  "/Users/ozer/test2/testcases/NoAccess.xisf" << AstroFileProcessStatus::FailedToProcess;
    QTest::newRow("Zero Byte FITS file") <<  "/Users/ozer/test2/testcases/zerobyte.fits" << AstroFileProcessStatus::FailedToProcess;
    QTest::newRow("Zero Byte XISF file") <<  "/Users/ozer/test2/testcases/zerobyte.xisf" << AstroFileProcessStatus::FailedToProcess;
    QTest::newRow("Zero Byte PNG file") <<  "/Users/ozer/test2/testcases/zerobyte.png" << AstroFileProcessStatus::FailedToProcess;
}

void Mock_NewFileProcessor::test_invalidFiles()
{
//    QFETCH(QString, filePath);
//    QFETCH(AstroFileProcessStatus, expectedResult);

//    NewFileProcessor proc;
//    QSignalSpy spy(&proc, SIGNAL(astrofileProcessed(const AstroFile&)));

//    QFileInfo fileInfo;
//    fileInfo.setFile(filePath);
//    proc.processNewFile(fileInfo);
//    auto s = spy.takeFirst();
//    auto x = s.takeFirst();
//    auto astroFile = x.value<AstroFile>();

//    QCOMPARE(astroFile.FullPath, filePath);
//    QCOMPARE(astroFile.processStatus, expectedResult);
}

#include "tst_NewFileProcessor.moc"
