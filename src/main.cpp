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

//#include "mainwindow.h"

//#include <QApplication>
//#include <QSettings>

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);

//    QSettings::setDefaultFormat(QSettings::IniFormat);
//    QCoreApplication::setApplicationName("Astrocat");
//    QCoreApplication::setOrganizationName("Astrocat");
//    QCoreApplication::setOrganizationDomain("astrocat.app");
//    MainWindow w;
//    w.initialize();
//    w.show();
//    return a.exec();
//}

#include "appmanager.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSettings>
#include <QQmlContext>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QCoreApplication::setApplicationName("Astrocat");
    QCoreApplication::setOrganizationName("Astrocat");
    QCoreApplication::setOrganizationDomain("astrocat.app");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    AppManager* appManager = new AppManager;
    appManager->initialize();

    engine.rootContext()->setContextProperty("appManager", appManager);

    engine.load(url);

    return app.exec();
}
