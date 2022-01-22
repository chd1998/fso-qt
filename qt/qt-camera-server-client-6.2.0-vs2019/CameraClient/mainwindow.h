#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTcpSocket>
#include <QTcpServer>
#include <QBuffer>
#include <iostream>
#include <QThread>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <QElapsedTimer>

using namespace  cv;

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
    void BtnOpenCamera();
    void BtnCloseCamera();
    void TimeSendData();
    void BtnConnectSlot();
    void BtnOffSlot();
    void BtnShotSlot();
    void connect_suc();
    void client_dis();

    void getFrame(int i=0);

public slots:
    void showTime();

private:
    Ui::MainWindow *ui;

    QImage      qImg;
    QTcpSocket* mSocketM;
    QPixmap     tempPixmapM;
    QTimer     *timerGetFrame;    // 定时器用于定时取帧，上面说的隔一段时间就去取就是用这个实现
    QTimer     *timerSendData;    // 定时器用于定时取帧，上面说的隔一段时间就去取就是用这个实现
    VideoCapture capture;//有溢出

    quint32    ConnectFlg;   // 连接标记
    QElapsedTimer timer;
    quint16    firstconn;
};
#endif // MAINWINDOW_H
