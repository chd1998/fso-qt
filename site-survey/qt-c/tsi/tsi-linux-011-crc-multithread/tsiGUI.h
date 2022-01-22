#ifndef TSIGUI_H
#define TSIGUI_H

#include <QMainWindow>
#include <QObject>
#include <QComboBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QStringList>
#include <QTimer>
#include "sp2tcpserver.h"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>

#include <QSysInfo>

#include <cstdlib>
#include <iostream>

extern QString SPName,SPParityBits,SP485Addr,devID,tsiIP,commandTSI,recvData,recvTSI_str;
extern int SPBaudRate,SPDataBits,SPStopBits,SPTimeInterval,tsiIPPort,recvTSI;
extern bool cfgStatus,dataok;
extern QDateTime current_date_time;
extern QString current_date,current_time;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void gpsDisplay();
    void gpsDisplay_2();
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QTimer         *readTimer;
    //#define time_interval 1000
    QByteArray GPS_RMC;
    QByteArray GPS_RMC_2;
    QList<QByteArray> GPS_list;
    QList<QByteArray> GPS_list_2;
    QString& latitude(QString& lat_str);
    QString& longitude(QString& lon_str);
    QString& time(QString& time_str);
    QString& date(QString& date_str);
    QString& direction(QString& direction_str);
    QString& satellite(QString& satellite_str);
    QString& getAlt(QString& alt_str);

    //int mysockDesc;
    bool opened;
    uint time_interval;

    void openSerialPort();
    void closeSerialPort();
    void startMultiThreadTcpSocketServer();
    bool openCfg();
    bool writeCfg();
    char ConvertHexChar(char ch);
    void StringToHex(QString str, QByteArray &senddata);
    void tsiDisplay();

private:
    Ui::MainWindow *ui;
    QSerialPort    *serialPortset;


public slots:
    //void on_searchButton_clicked();
    //void on_openComButton_clicked();
    void readSPData();
    //void sendData();
    void on_tsiPlainTextEdit_textChanged();
    void displayMsg(QString);
    void displayMsg01();
};

#endif // TSIGUI_H
