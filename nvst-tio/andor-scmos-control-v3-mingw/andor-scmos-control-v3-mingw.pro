#-------------------------------------------------
#
# Project created by QtCreator 2019-03-11T16:54:53
# Modified on 2021-06-23 17:58:33
# By Chen Dong @home00
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = andor-scmos-control-004
TEMPLATE = app

INCLUDEPATH += "C:\Program Files\Andor SDK3"


LIBS += -L"C:\Program Files\Andor SDK3" -latcorem -latutilitym

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    propertybrowserdelegate.cpp \
    time_sys/time_sys.cpp \
    utils.cpp \
    server.cpp \
    graphicsscene.cpp \
    aboutwindow.cpp

HEADERS += \
    cfitsio/fitsio.h \
    cfitsio/group.h \
    cfitsio/longnam.h \
        mainwindow.h \
    propertybrowserdelegate.h \
    time_sys/time_sys.h \
    utils.h \
    server.h \
    graphicsscene.h \
    aboutwindow.h

FORMS += \
        mainwindow.ui \
    serverwindow.ui \
    aboutwindow.ui

RESOURCES += \
    assets.qrc

win32:RC_ICONS += "./assets/icon.ico"

INCLUDEPATH += $$PWD/. \
                $$PWD/cfitsio \
                $$PWD/opencvlib \
                D:\opencv\build\x64\mingw\bin
DEPENDPATH += $$PWD/. \
                $$PWD/cfitsio \
                $$PWD/opencvlib
LIBS += -L$$PWD/cfitsio/ -lcfitsio

#LIBS += -L$$PWD/opencvlib/ -llibopencv_core452.dll \
#               -llibopencv_highgui452.dll \
#               -llibopencv_imgcodecs452.dll \
#               -llibopencv_imgproc452.dll


win32: LIBS += -L$$PWD/opencvlib/ -llibopencv_core452.dll \
                                  -llibopencv_highgui452.dll \
                                  -llibopencv_imgcodecs452.dll \
                                  -llibopencv_imgproc452.dll

INCLUDEPATH += $$PWD/opencvlib
DEPENDPATH += $$PWD/opencvlib
