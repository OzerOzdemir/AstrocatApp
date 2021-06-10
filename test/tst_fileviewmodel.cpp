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
#include <fileviewmodel.h>
#include <filterview.h>
#include <mainwindow.h>
#include <sortfilterproxymodel.h>

class TestFileViewModel : public QObject
{
    Q_OBJECT

public:
    TestFileViewModel();
    ~TestFileViewModel();

private slots:
    void test_case1();

};

TestFileViewModel::TestFileViewModel()
{    
}

TestFileViewModel::~TestFileViewModel()
{
}

void TestFileViewModel::test_case1()
{
    FileViewModel model;

    QList<AstroFile> list;

    QElapsedTimer timer;
    timer.start();
    qDebug()<<"Setting up test data";
    for (int i = 1; i < 1000 * 1000; i++)
    {
        QImage tiny(20, 20, QImage::Format::Format_RGB32);
        AstroFile astroFile;
        astroFile.Id = i;
        astroFile.processStatus = FailedToProcess;
        astroFile.FileName = QString("File_%1").arg(i);
        astroFile.FullPath = QString("/path/File_%1").arg(i);
        astroFile.Tags.insert({{"key1", "value1"}, {"key2", "value2"}});
        astroFile.thumbnail = QImage();
        astroFile.tinyThumbnail = tiny;
        list.append(astroFile);
    }
    qDebug() << "time:" << timer.elapsed() << "milliseconds";
    qDebug()<<"Setting up sortfilterproxymodel";
    SortFilterProxyModel sortFilterProxyModel;
    sortFilterProxyModel.setSourceModel(&model);
    qDebug() << "time:" << timer.elapsed() << "milliseconds";

    qDebug()<<"Calling setInitialModel";
    model.setInitialModel(list);
    qDebug() << "time:" << timer.elapsed() << "milliseconds";

    qDebug()<<"Done.";
}

#include "tst_fileviewmodel.moc"
