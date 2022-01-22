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
    QPushButton *btnPhto;
    QLabel *label_stat;
    QLabel *label_port;
    QLineEdit *lineEdit_port;
    QPushButton *btn_listen;
    QLabel *label_IP;
    QLineEdit *lineEdit_IP;
    QLabel *label_save;
    QLineEdit *lineEdit_save;
    QTextBrowser *textBrowser_status;
    QLabel *label_time;
    QPushButton *btn_listen_clsconn;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(933, 600);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 50, 491, 531));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setFrameShape(QFrame::Box);
        label->setAlignment(Qt::AlignCenter);
        btnPhto = new QPushButton(centralwidget);
        btnPhto->setObjectName(QStringLiteral("btnPhto"));
        btnPhto->setGeometry(QRect(650, 230, 271, 23));
        label_stat = new QLabel(centralwidget);
        label_stat->setObjectName(QStringLiteral("label_stat"));
        label_stat->setGeometry(QRect(510, 250, 41, 31));
        label_port = new QLabel(centralwidget);
        label_port->setObjectName(QStringLiteral("label_port"));
        label_port->setGeometry(QRect(650, 100, 31, 31));
        lineEdit_port = new QLineEdit(centralwidget);
        lineEdit_port->setObjectName(QStringLiteral("lineEdit_port"));
        lineEdit_port->setGeometry(QRect(700, 99, 221, 31));
        btn_listen = new QPushButton(centralwidget);
        btn_listen->setObjectName(QStringLiteral("btn_listen"));
        btn_listen->setGeometry(QRect(650, 150, 111, 23));
        label_IP = new QLabel(centralwidget);
        label_IP->setObjectName(QStringLiteral("label_IP"));
        label_IP->setGeometry(QRect(650, 60, 31, 21));
        lineEdit_IP = new QLineEdit(centralwidget);
        lineEdit_IP->setObjectName(QStringLiteral("lineEdit_IP"));
        lineEdit_IP->setGeometry(QRect(700, 60, 221, 31));
        label_save = new QLabel(centralwidget);
        label_save->setObjectName(QStringLiteral("label_save"));
        label_save->setGeometry(QRect(650, 190, 51, 31));
        lineEdit_save = new QLineEdit(centralwidget);
        lineEdit_save->setObjectName(QStringLiteral("lineEdit_save"));
        lineEdit_save->setGeometry(QRect(700, 190, 221, 31));
        textBrowser_status = new QTextBrowser(centralwidget);
        textBrowser_status->setObjectName(QStringLiteral("textBrowser_status"));
        textBrowser_status->setGeometry(QRect(510, 280, 411, 301));
        label_time = new QLabel(centralwidget);
        label_time->setObjectName(QStringLiteral("label_time"));
        label_time->setGeometry(QRect(130, 10, 271, 31));
        label_time->setAlignment(Qt::AlignCenter);
        btn_listen_clsconn = new QPushButton(centralwidget);
        btn_listen_clsconn->setObjectName(QStringLiteral("btn_listen_clsconn"));
        btn_listen_clsconn->setGeometry(QRect(810, 150, 111, 23));
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Video/Image", Q_NULLPTR));
        btnPhto->setText(QApplication::translate("MainWindow", "Shot ", Q_NULLPTR));
        label_stat->setText(QApplication::translate("MainWindow", "Status", Q_NULLPTR));
        label_port->setText(QApplication::translate("MainWindow", "Port", Q_NULLPTR));
        lineEdit_port->setText(QApplication::translate("MainWindow", "9988", Q_NULLPTR));
        btn_listen->setText(QApplication::translate("MainWindow", "Listen", Q_NULLPTR));
        label_IP->setText(QApplication::translate("MainWindow", "IP", Q_NULLPTR));
        lineEdit_IP->setText(QApplication::translate("MainWindow", "127.0.0.1", Q_NULLPTR));
        label_save->setText(QApplication::translate("MainWindow", "Save to", Q_NULLPTR));
        lineEdit_save->setText(QApplication::translate("MainWindow", "d:\\CamPic", Q_NULLPTR));
        label_time->setText(QApplication::translate("MainWindow", "Current Time", Q_NULLPTR));
        btn_listen_clsconn->setText(QApplication::translate("MainWindow", "Close Connection", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_2D_MAINWINDOW_H
