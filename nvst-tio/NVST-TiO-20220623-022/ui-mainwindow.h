/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_2D_MAINWINDOW_H
#define UI_2D_MAINWINDOW_H

#include <QtCharts/QChartView>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionServer;
    QAction *actionLogging;
    QAction *actionSave;
    QAction *actionExit;
    QAction *actionAbout;
    QAction *actionDetect_Cameras;
    QWidget *centralWidget;
    QGraphicsView *graphicsView;
    QLabel *label_saveto;
    QLineEdit *lineEdit_saveto;
    QPushButton *btnLive;
    QLabel *label_time;
    QLabel *label_exposuretime;
    QLineEdit *lineEdit_exposuretime;
    QLineEdit *lineEdit_framerate;
    QPushButton *btnSnap;
    QLabel *label_status;
    QTextEdit *textEdit_status;
    QCheckBox *checkBox_Flat;
    QCheckBox *checkBox_Dark;
    QCheckBox *checkBox_Data;
    QLabel *label_obj;
    QLineEdit *lineEdit_objname;
    QLabel *label_freedisk;
    QLineEdit *lineEdit_cor1;
    QLineEdit *lineEdit_cor2;
    QLabel *label_savelog;
    QLineEdit *lineEdit_savelog;
    QLineEdit *lineEdit_flatnum;
    QLineEdit *lineEdit_darknum;
    QLineEdit *lineEdit_datanum;
    QLabel *label_dunit_2;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_dunit_5;
    QLabel *label_dunit_6;
    QChartView *chart_graphicsView;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuServer;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(937, 763);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        actionServer = new QAction(MainWindow);
        actionServer->setObjectName(QString::fromUtf8("actionServer"));
        actionLogging = new QAction(MainWindow);
        actionLogging->setObjectName(QString::fromUtf8("actionLogging"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionDetect_Cameras = new QAction(MainWindow);
        actionDetect_Cameras->setObjectName(QString::fromUtf8("actionDetect_Cameras"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setEnabled(true);
        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(340, 10, 581, 512));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy1);
        graphicsView->setMinimumSize(QSize(512, 512));
        graphicsView->viewport()->setProperty("cursor", QVariant(QCursor(Qt::CrossCursor)));
        graphicsView->setMouseTracking(false);
        graphicsView->setAutoFillBackground(false);
        graphicsView->setFrameShape(QFrame::NoFrame);
        graphicsView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        graphicsView->setBackgroundBrush(brush);
        label_saveto = new QLabel(centralWidget);
        label_saveto->setObjectName(QString::fromUtf8("label_saveto"));
        label_saveto->setGeometry(QRect(10, 100, 61, 16));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_saveto->sizePolicy().hasHeightForWidth());
        label_saveto->setSizePolicy(sizePolicy2);
        lineEdit_saveto = new QLineEdit(centralWidget);
        lineEdit_saveto->setObjectName(QString::fromUtf8("lineEdit_saveto"));
        lineEdit_saveto->setGeometry(QRect(80, 100, 61, 20));
        btnLive = new QPushButton(centralWidget);
        btnLive->setObjectName(QString::fromUtf8("btnLive"));
        btnLive->setGeometry(QRect(10, 320, 131, 31));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(btnLive->sizePolicy().hasHeightForWidth());
        btnLive->setSizePolicy(sizePolicy3);
        btnLive->setCheckable(true);
        label_time = new QLabel(centralWidget);
        label_time->setObjectName(QString::fromUtf8("label_time"));
        label_time->setGeometry(QRect(10, 10, 311, 21));
        sizePolicy3.setHeightForWidth(label_time->sizePolicy().hasHeightForWidth());
        label_time->setSizePolicy(sizePolicy3);
        label_time->setLayoutDirection(Qt::LeftToRight);
        label_time->setFrameShape(QFrame::Panel);
        label_time->setFrameShadow(QFrame::Raised);
        label_time->setLineWidth(2);
        label_exposuretime = new QLabel(centralWidget);
        label_exposuretime->setObjectName(QString::fromUtf8("label_exposuretime"));
        label_exposuretime->setGeometry(QRect(10, 40, 61, 16));
        sizePolicy2.setHeightForWidth(label_exposuretime->sizePolicy().hasHeightForWidth());
        label_exposuretime->setSizePolicy(sizePolicy2);
        lineEdit_exposuretime = new QLineEdit(centralWidget);
        lineEdit_exposuretime->setObjectName(QString::fromUtf8("lineEdit_exposuretime"));
        lineEdit_exposuretime->setGeometry(QRect(80, 40, 61, 20));
        lineEdit_framerate = new QLineEdit(centralWidget);
        lineEdit_framerate->setObjectName(QString::fromUtf8("lineEdit_framerate"));
        lineEdit_framerate->setGeometry(QRect(80, 70, 61, 20));
        btnSnap = new QPushButton(centralWidget);
        btnSnap->setObjectName(QString::fromUtf8("btnSnap"));
        btnSnap->setGeometry(QRect(170, 320, 151, 31));
        sizePolicy3.setHeightForWidth(btnSnap->sizePolicy().hasHeightForWidth());
        btnSnap->setSizePolicy(sizePolicy3);
        btnSnap->setCheckable(false);
        btnSnap->setChecked(false);
        btnSnap->setAutoRepeatDelay(50);
        btnSnap->setAutoRepeatInterval(50);
        label_status = new QLabel(centralWidget);
        label_status->setObjectName(QString::fromUtf8("label_status"));
        label_status->setGeometry(QRect(10, 360, 331, 16));
        sizePolicy2.setHeightForWidth(label_status->sizePolicy().hasHeightForWidth());
        label_status->setSizePolicy(sizePolicy2);
        QFont font;
        font.setBold(true);
        label_status->setFont(font);
        textEdit_status = new QTextEdit(centralWidget);
        textEdit_status->setObjectName(QString::fromUtf8("textEdit_status"));
        textEdit_status->setGeometry(QRect(10, 390, 311, 311));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(textEdit_status->sizePolicy().hasHeightForWidth());
        textEdit_status->setSizePolicy(sizePolicy4);
        checkBox_Flat = new QCheckBox(centralWidget);
        checkBox_Flat->setObjectName(QString::fromUtf8("checkBox_Flat"));
        checkBox_Flat->setGeometry(QRect(10, 170, 71, 21));
        checkBox_Dark = new QCheckBox(centralWidget);
        checkBox_Dark->setObjectName(QString::fromUtf8("checkBox_Dark"));
        checkBox_Dark->setGeometry(QRect(10, 200, 51, 21));
        checkBox_Data = new QCheckBox(centralWidget);
        checkBox_Data->setObjectName(QString::fromUtf8("checkBox_Data"));
        checkBox_Data->setGeometry(QRect(10, 230, 71, 21));
        label_obj = new QLabel(centralWidget);
        label_obj->setObjectName(QString::fromUtf8("label_obj"));
        label_obj->setGeometry(QRect(10, 260, 81, 21));
        lineEdit_objname = new QLineEdit(centralWidget);
        lineEdit_objname->setObjectName(QString::fromUtf8("lineEdit_objname"));
        lineEdit_objname->setGeometry(QRect(90, 260, 231, 20));
        label_freedisk = new QLabel(centralWidget);
        label_freedisk->setObjectName(QString::fromUtf8("label_freedisk"));
        label_freedisk->setGeometry(QRect(150, 100, 171, 51));
        label_freedisk->setFrameShape(QFrame::StyledPanel);
        lineEdit_cor1 = new QLineEdit(centralWidget);
        lineEdit_cor1->setObjectName(QString::fromUtf8("lineEdit_cor1"));
        lineEdit_cor1->setGeometry(QRect(90, 290, 101, 20));
        lineEdit_cor2 = new QLineEdit(centralWidget);
        lineEdit_cor2->setObjectName(QString::fromUtf8("lineEdit_cor2"));
        lineEdit_cor2->setGeometry(QRect(210, 290, 111, 20));
        label_savelog = new QLabel(centralWidget);
        label_savelog->setObjectName(QString::fromUtf8("label_savelog"));
        label_savelog->setGeometry(QRect(40, 130, 31, 20));
        lineEdit_savelog = new QLineEdit(centralWidget);
        lineEdit_savelog->setObjectName(QString::fromUtf8("lineEdit_savelog"));
        lineEdit_savelog->setGeometry(QRect(80, 130, 61, 20));
        lineEdit_flatnum = new QLineEdit(centralWidget);
        lineEdit_flatnum->setObjectName(QString::fromUtf8("lineEdit_flatnum"));
        lineEdit_flatnum->setGeometry(QRect(60, 170, 81, 20));
        lineEdit_darknum = new QLineEdit(centralWidget);
        lineEdit_darknum->setObjectName(QString::fromUtf8("lineEdit_darknum"));
        lineEdit_darknum->setGeometry(QRect(60, 200, 81, 20));
        lineEdit_datanum = new QLineEdit(centralWidget);
        lineEdit_datanum->setObjectName(QString::fromUtf8("lineEdit_datanum"));
        lineEdit_datanum->setGeometry(QRect(60, 230, 81, 20));
        label_dunit_2 = new QLabel(centralWidget);
        label_dunit_2->setObjectName(QString::fromUtf8("label_dunit_2"));
        label_dunit_2->setGeometry(QRect(150, 170, 171, 81));
        label_dunit_2->setFrameShape(QFrame::Box);
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(150, 70, 171, 21));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 290, 81, 21));
        label_dunit_5 = new QLabel(centralWidget);
        label_dunit_5->setObjectName(QString::fromUtf8("label_dunit_5"));
        label_dunit_5->setGeometry(QRect(150, 40, 91, 21));
        label_dunit_6 = new QLabel(centralWidget);
        label_dunit_6->setObjectName(QString::fromUtf8("label_dunit_6"));
        label_dunit_6->setGeometry(QRect(10, 70, 71, 21));
        chart_graphicsView = new QChartView(centralWidget);
        chart_graphicsView->setObjectName(QString::fromUtf8("chart_graphicsView"));
        chart_graphicsView->setGeometry(QRect(340, 530, 581, 171));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 937, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuServer = new QMenu(menuBar);
        menuServer->setObjectName(QString::fromUtf8("menuServer"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuServer->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionSave);
        menuFile->addAction(actionExit);
        menuServer->addAction(actionServer);
        menuServer->addAction(actionDetect_Cameras);
        menuHelp->addAction(actionAbout);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
#if QT_CONFIG(tooltip)
        MainWindow->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
        actionServer->setText(QCoreApplication::translate("MainWindow", "Server", nullptr));
        actionLogging->setText(QCoreApplication::translate("MainWindow", "Logging", nullptr));
        actionSave->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
#if QT_CONFIG(shortcut)
        actionSave->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionExit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
#if QT_CONFIG(shortcut)
        actionExit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionAbout->setText(QCoreApplication::translate("MainWindow", "About", nullptr));
        actionDetect_Cameras->setText(QCoreApplication::translate("MainWindow", "Detect Cameras", nullptr));
        label_saveto->setText(QCoreApplication::translate("MainWindow", "Save Data ", nullptr));
        lineEdit_saveto->setText(QCoreApplication::translate("MainWindow", "e:\\", nullptr));
        btnLive->setText(QCoreApplication::translate("MainWindow", "Start Live", nullptr));
        label_time->setText(QCoreApplication::translate("MainWindow", "Current Time", nullptr));
        label_exposuretime->setText(QCoreApplication::translate("MainWindow", "Exposure", nullptr));
        lineEdit_exposuretime->setText(QCoreApplication::translate("MainWindow", "1.2", nullptr));
        lineEdit_framerate->setText(QCoreApplication::translate("MainWindow", "200", nullptr));
        btnSnap->setText(QCoreApplication::translate("MainWindow", "Start Acquisition", nullptr));
        label_status->setText(QCoreApplication::translate("MainWindow", "Log (Andor Neo for NVST - TiO)", nullptr));
        checkBox_Flat->setText(QCoreApplication::translate("MainWindow", "FLAT", nullptr));
        checkBox_Dark->setText(QCoreApplication::translate("MainWindow", "dark", nullptr));
        checkBox_Data->setText(QCoreApplication::translate("MainWindow", "Data", nullptr));
        label_obj->setText(QCoreApplication::translate("MainWindow", "Obs. Object", nullptr));
        lineEdit_objname->setText(QCoreApplication::translate("MainWindow", "Sun", nullptr));
        label_freedisk->setText(QCoreApplication::translate("MainWindow", "Free Space  ", nullptr));
        lineEdit_cor1->setText(QCoreApplication::translate("MainWindow", "N00", nullptr));
        lineEdit_cor2->setText(QCoreApplication::translate("MainWindow", "E00", nullptr));
        label_savelog->setText(QCoreApplication::translate("MainWindow", "Log", nullptr));
        lineEdit_savelog->setText(QCoreApplication::translate("MainWindow", "d:\\log", nullptr));
        lineEdit_flatnum->setText(QCoreApplication::translate("MainWindow", "2000", nullptr));
        lineEdit_darknum->setText(QCoreApplication::translate("MainWindow", "1000", nullptr));
        lineEdit_datanum->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_dunit_2->setText(QCoreApplication::translate("MainWindow", "Frames(0-nonStop)", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Frarmes/Group (Max 200)", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Coordinate", nullptr));
        label_dunit_5->setText(QCoreApplication::translate("MainWindow", "ms", nullptr));
        label_dunit_6->setText(QCoreApplication::translate("MainWindow", "Acquire", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuServer->setTitle(QCoreApplication::translate("MainWindow", "Tools", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_2D_MAINWINDOW_H
