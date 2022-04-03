QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    threada.cpp \
    threadb.cpp

HEADERS += \
    mainwindow.h \
    opencv2/core.hpp \
    opencv2/core/base.hpp \
    opencv2/core/bufferpool.hpp \
    opencv2/core/check.hpp \
    opencv2/core/core.hpp \
    opencv2/core/core_c.h \
    opencv2/core/cv_cpu_dispatch.h \
    opencv2/core/cvdef.h \
    opencv2/core/cvstd.hpp \
    opencv2/core/cvstd.inl.hpp \
    opencv2/core/cvstd_wrapper.hpp \
    opencv2/core/fast_math.hpp \
    opencv2/core/hal/interface.h \
    opencv2/core/mat.hpp \
    opencv2/core/mat.inl.hpp \
    opencv2/core/matx.hpp \
    opencv2/core/neon_utils.hpp \
    opencv2/core/operations.hpp \
    opencv2/core/optim.hpp \
    opencv2/core/ovx.hpp \
    opencv2/core/persistence.hpp \
    opencv2/core/saturate.hpp \
    opencv2/core/traits.hpp \
    opencv2/core/types.hpp \
    opencv2/core/types_c.h \
    opencv2/core/utility.hpp \
    opencv2/core/version.hpp \
    opencv2/core/vsx_utils.hpp \
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
    opencv2/opencv_modules.hpp \
    opencv2/video.hpp \
    opencv2/videoio.hpp \
    threada.h \
    threadb.h

FORMS += \
    mainwindow.ui



win32: LIBS += -L$$PWD/opencvlib/ -llibopencv_core452.dll

INCLUDEPATH += $$PWD/opencvlib
DEPENDPATH += $$PWD/opencvlib

win32: LIBS += -L$$PWD/opencvlib/ -llibopencv_highgui452.dll

INCLUDEPATH += $$PWD/opencvlib
DEPENDPATH += $$PWD/opencvlib

win32: LIBS += -L$$PWD/opencvlib/ -llibopencv_imgproc452.dll

INCLUDEPATH += $$PWD/opencvlib
DEPENDPATH += $$PWD/opencvlib
