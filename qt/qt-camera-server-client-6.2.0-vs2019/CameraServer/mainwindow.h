#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include<QBuffer>
#include <QTcpServer>
#include <QTcpSocket>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <QElapsedTimer>

using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void BtnPhtoSlot();
    void BtnConnectSlot();
    void BtnListenSlot();
    void BtnclsconnSlot();
    void connect_suc();
    void client_dis();

public slots:
    void showTime();
    //void server_New_Connect();

private:
    Ui::MainWindow *ui;

    QTcpServer* mServerM;
    QTcpSocket *mSocketM;
    QList<QTcpSocket*> m_pClientSocketList;
    qint32 shotFlg;
    qint32 serverstatus;
    QElapsedTimer timer;
};
#endif // MAINWINDOW_H
