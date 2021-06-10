QT       += core gui sql testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

DEFINES += CURRENT_APP_VERSION=\"\\\"$${VERSION}\\\"\"

SOURCES += \
    ../src/aboutwindow.cpp \
    ../src/autostretcher.cpp \
    ../src/filerepository.cpp \
    ../src/fileviewmodel.cpp \
    ../src/filterview.cpp \
    ../src/fitsfile.cpp \
    ../src/fitsprocessor.cpp \
    ../src/foldercrawler.cpp \
    ../src/imageprocessor.cpp \
    ../src/mainwindow.cpp \
    ../src/newfileprocessor.cpp \
    ../src/searchfolderdialog.cpp \
    ../src/sortfilterproxymodel.cpp \
    ../src/xisfprocessor.cpp

HEADERS += \
    ../src/aboutwindow.h \
    ../src/astrofile.h \
    ../src/autostretcher.h \
    ../src/fileprocessor.h \
    ../src/filerepository.h \
    ../src/fileviewmodel.h \
    ../src/filterview.h \
    ../src/fitsfile.h \
    ../src/fitsprocessor.h \
    ../src/foldercrawler.h \
    ../src/imageprocessor.h \
    ../src/mainwindow.h \
    ../src/newfileprocessor.h \
    ../src/searchfolderdialog.h \
    ../src/sortfilterproxymodel.h \
    ../src/xisfprocessor.h

FORMS += \
    ../src/aboutwindow.ui \
    ../src/mainwindow.ui \
    ../src/searchfolderdialog.ui

SOURCES += \
    tst_test1.cpp \
    tst_NewFileProcessor.cpp \
    tst_fileviewmodel.cpp \
    tstmain.cpp

LIBS += -L$$PWD/../external/build/libs/ -llcms -llz4 -lpcl -lRFC6234 -lcfitsio -lzlib

win32 {
    LIBS += -L$$PWD/../external/build/libs/Release
    LIBS += -luser32 -luserenv -ladvapi32
    DEFINES += __PCL_WINDOWS WIN32 WIN64 __PCL_NO_WIN32_MINIMUM_VERSIONS UNICODE _UNICODE _WINDOWS _NDEBUG
    QMAKE_CXXFLAGS = "/EHsc /MP"
}
macx {
    DEFINES += __PCL_MACOSX
}
linux {
DEFINES += __PCL_LINUX
}

INCLUDEPATH += $$PWD/../src
INCLUDEPATH += $$PWD/../external/cfitsio
INCLUDEPATH += $$PWD/../external/pcl/include
