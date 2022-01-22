/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_2D_MAINWINDOW_H
#define UI_2D_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QPushButton *BtnOpenCamera;
    QLabel *label_2;
    QLineEdit *lineEditIp;
    QLabel *label_3;
    QLineEdit *lineEditport;
    QLabel *label_4;
    QPushButton *btnConnect;
    QPushButton *btnOff;
    QPushButton *BtnCloseCamera;
    QTextBrowser *textBrowser_status;
    QLabel *label_time;
    QLabel *label_5;
    QLineEdit *lineEdit_save;
    QPushButton *btnshot;
    QLabel *label_captime;
    QLineEdit *lineEdit_time;
    QLabel *label_unit;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(967, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 50, 511, 541));
        label->setFrameShape(QFrame::Box);
        label->setAlignment(Qt::AlignCenter);
        BtnOpenCamera = new QPushButton(centralwidget);
        BtnOpenCamera->setObjectName(QStringLiteral("BtnOpenCamera"));
        BtnOpenCamera->setGeometry(QRect(610, 30, 75, 23));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(550, 90, 54, 12));
        lineEditIp = new QLineEdit(centralwidget);
        lineEditIp->setObjectName(QStringLiteral("lineEditIp"));
        lineEditIp->setGeometry(QRect(630, 79, 171, 31));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(820, 90, 71, 16));
        lineEditport = new QLineEdit(centralwidget);
        lineEditport->setObjectName(QStringLiteral("lineEditport"));
        lineEditport->setGeometry(QRect(850, 80, 61, 31));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(540, 230, 54, 16));
        btnConnect = new QPushButton(centralwidget);
        btnConnect->setObjectName(QStringLiteral("btnConnect"));
        btnConnect->setGeometry(QRect(600, 200, 91, 23));
        btnOff = new QPushButton(centralwidget);
        btnOff->setObjectName(QStringLiteral("btnOff"));
        btnOff->setGeometry(QRect(710, 200, 91, 23));
        BtnCloseCamera = new QPushButton(centralwidget);
        BtnCloseCamera->setObjectName(QStringLiteral("BtnCloseCamera"));
        BtnCloseCamera->setGeometry(QRect(780, 30, 75, 23));
        textBrowser_status = new QTextBrowser(centralwidget);
        textBrowser_status->setObjectName(QStringLiteral("textBrowser_status"));
        textBrowser_status->setGeometry(QRect(540, 250, 411, 341));
        label_time = new QLabel(centralwidget);
        label_time->setObjectName(QStringLiteral("label_time"));
        label_time->setGeometry(QRect(190, 20, 131, 16));
        label_time->setAlignment(Qt::AlignCenter);
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(550, 170, 54, 20));
        lineEdit_save = new QLineEdit(centralwidget);
        lineEdit_save->setObjectName(QStringLiteral("lineEdit_save"));
        lineEdit_save->setGeometry(QRect(630, 160, 281, 31));
        btnshot = new QPushButton(centralwidget);
        btnshot->setObjectName(QStringLiteral("btnshot"));
        btnshot->setGeometry(QRect(820, 200, 91, 23));
        label_captime = new QLabel(centralwidget);
        label_captime->setObjectName(QStringLiteral("label_captime"));
        label_captime->setGeometry(QRect(550, 120, 71, 21));
        lineEdit_time = new QLineEdit(centralwidget);
        lineEdit_time->setObjectName(QStringLiteral("lineEdit_time"));
        lineEdit_time->setGeometry(QRect(630, 120, 171, 31));
        label_unit = new QLabel(centralwidget);
        label_unit->setObjectName(QStringLiteral("label_unit"));
        label_unit->setGeometry(QRect(810, 120, 91, 31));
        QFont font;
        font.setFamily(QStringLiteral("Courier New"));
        font.setPointSize(12);
        label_unit->setFont(font);
        label_unit->setFrameShadow(QFrame::Raised);
        label_unit->setLineWidth(1);
        label_unit->setAlignment(Qt::AlignCenter);
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Video/Image", Q_NULLPTR));
        BtnOpenCamera->setText(QApplication::translate("MainWindow", "Open Cam", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "Server IP", Q_NULLPTR));
        lineEditIp->setText(QApplication::translate("MainWindow", "127.0.0.1", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "Port", Q_NULLPTR));
        lineEditport->setText(QApplication::translate("MainWindow", "9988", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "Status", Q_NULLPTR));
        btnConnect->setText(QApplication::translate("MainWindow", "Connect", Q_NULLPTR));
        btnOff->setText(QApplication::translate("MainWindow", "DisConnect", Q_NULLPTR));
        BtnCloseCamera->setText(QApplication::translate("MainWindow", "Close Cam", Q_NULLPTR));
        label_time->setText(QApplication::translate("MainWindow", "Current Time", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "Save to", Q_NULLPTR));
        lineEdit_save->setText(QApplication::translate("MainWindow", "d:\\ClientPic", Q_NULLPTR));
        btnshot->setText(QApplication::translate("MainWindow", "Shot", Q_NULLPTR));
        label_captime->setText(QApplication::translate("MainWindow", "Capture Time", Q_NULLPTR));
        lineEdit_time->setText(QApplication::translate("MainWindow", "50", Q_NULLPTR));
        label_unit->setText(QApplication::translate("MainWindow", "ms/Frame", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_2D_MAINWINDOW_H
