#-------------------------------------------------
#
# Project created by QtCreator 2019-07-02T12:39:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = New_Vehicle_Management
TEMPLATE = app

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
    login_management.cpp \
    apply_account.cpp \
    select_truck.cpp \
    add_truck.cpp \
    select_car.cpp \
    select_bus.cpp \
    add_driver.cpp \
    select_driver.cpp \
    mainwindow_2.cpp \
    select_truck_2.cpp \
    select_driver_2.cpp

HEADERS += \
        mainwindow.h \
    login_management.h \
    apply_account.h \
    select_truck.h \
    add_truck.h \
    select_car.h \
    select_bus.h \
    add_driver.h \
    select_driver.h \
    mainwindow_2.h \
    select_truck_2.h \
    select_driver_2.h

FORMS += \
        mainwindow.ui \
    login_management.ui \
    apply_account.ui \
    select_truck.ui \
    add_vehicle.ui \
    add_driver.ui \
    select_driver.ui \
    mainwindow_2.ui \
    select_truck_2.ui \
    select_driver_2.ui

QT += sql #连接数据库
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
