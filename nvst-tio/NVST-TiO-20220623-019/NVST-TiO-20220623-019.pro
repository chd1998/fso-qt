#-------------------------------------------------
#
# Project created by QtCreator 2019-03-11T16:54:53
# Modified on 2021-06-23 17:58:33
# By Chen Dong @home00
#
#-------------------------------------------------

QT       += core gui network
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

CONFIG += c++17

TARGET = NVST-TiO-20220623-019
TEMPLATE = app

#QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO -= -O1
#QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO -= -O2
QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO -= -O3
#QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO -= -Od
#QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO -= -Od

UI_DIR=./

INCLUDEPATH += "C:\Program Files\Andor SDK3"

#                D:\opencv\build\include

LIBS += -L"C:\Program Files\Andor SDK3" -latcorem -latutilitym

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += QT_NO_DEBUG_OUTPUT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    accd/accd.cpp \
    histdisplay/histdisplay.cpp \
        main.cpp \
        mainwindow.cpp \
    time_sys/time_sys.cpp \
    utils.cpp \
    server.cpp \
    graphicsscene.cpp \
    aboutwindow.cpp

HEADERS += \
    accd/accd.h \
    atlib/atcore.h \
    atlib/atutility.h \
    cfitsio/fitsio.h \
    cfitsio/group.h \
    cfitsio/longnam.h \
    histdisplay/histdisplay.h \
    mainwindow.h \
    opencv2/core.hpp \
    opencv2/core/base.hpp \
    opencv2/core/core.hpp \
    opencv2/core/core_c.h \
    opencv2/highgui.hpp \
    opencv2/highgui/highgui.hpp \
    opencv2/highgui/highgui_c.h \
    opencv2/imgcodecs.hpp \
    opencv2/imgcodecs/imgcodecs.hpp \
    opencv2/imgcodecs/imgcodecs_c.h \
    opencv2/imgproc.hpp \
    opencv2/imgproc/imgproc.hpp \
    opencv2/imgproc/imgproc_c.h \
    opencv2/imgproc/segmentation.hpp \
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
                $$PWD/opencv2 \
#                D:\opencv\build\x64\mingw\bin
DEPENDPATH += $$PWD/. \
                $$PWD/cfitsio \
                $$PWD/opencvlib
LIBS += -L$$PWD/cfitsio/ -lcfitsio


#win32: LIBS += -L$$PWD/opencvlib/ -llibopencv_core455.dll \
#               -llibopencv_highgui455.dll \
#               -llibopencv_imgcodecs455.dll \
#               -llibopencv_imgproc455.dll

#INCLUDEPATH += $$PWD/opencvlib
#DEPENDPATH += $$PWD/opencvlib

DISTFILES += \
    atlib/atcore.dll \
    atlib/atcorem.lib \
    atlib/atutility.dll \
    atlib/atutilitym.lib \
    cfitsio/cfitsio.dll \
    cfitsio/cfitsio.lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/opencvlib/ -llibopencv_core455.dll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/opencvlib/ -llibopencv_core455.dll


INCLUDEPATH += $$PWD/opencvlib
DEPENDPATH += $$PWD/opencvlib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/opencvlib/ -llibopencv_highgui455.dll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/opencvlib/ -llibopencv_highgui455.dll

INCLUDEPATH += $$PWD/opencvlib
DEPENDPATH += $$PWD/opencvlib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/opencvlib/ -llibopencv_imgcodecs455.dll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/opencvlib/ -llibopencv_imgcodecs455.dll

INCLUDEPATH += $$PWD/opencvlib
DEPENDPATH += $$PWD/opencvlib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/opencvlib/ -llibopencv_imgproc455.dll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/opencvlib/ -llibopencv_imgproc455.dll

INCLUDEPATH += $$PWD/opencvlib
DEPENDPATH += $$PWD/opencvlib
