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

#include "../src/foldercrawler.h"

class Test1 : public QObject
{
    Q_OBJECT

public:
    Test1();
    ~Test1();

private slots:
    void test_case1();

};

Test1::Test1()
{

}

Test1::~Test1()
{

}

void Test1::test_case1()
{
    FolderCrawler fc;
    QSignalSpy spy(&fc, SIGNAL(fileFound(QFileInfo)));
    fc.crawl("/Users/ozer/test2/stretch");

    for (auto sig : spy )
    {
        qDebug()<<sig;
    }
    QList<QVariant> arguments = spy.takeFirst();
    auto x = arguments.at(0);
    auto fileInfo = x.value<QFileInfo>();
    qDebug() << fileInfo.absoluteFilePath();
}

#include "tst_test1.moc"
