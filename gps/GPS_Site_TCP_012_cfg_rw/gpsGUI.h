#ifndef GPSGUI_H
#define GPSGUI_H

#include <QMainWindow>
#include <QObject>
#include <QComboBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QStringList>
#include <QTimer>

#include "sp2tcpserver.h"


extern QString myIP;
extern uint myPort;
extern QString lat_str,lat_str_ip;      //纬度
extern QString lon_str,lon_str_ip;     //经度
extern QString time_str;          //时间
extern QString date_str;          //日期
extern QString alt_str;             //高度
extern QString NetTxt;
extern QString SPName,SPParityBits,siteID,gpsIP;
extern int SPBaudRate,SPDataBits,SPStopBits,SPTimeInterval,gpsIPPort;
extern bool cfgStatus;

//using std::cout;

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
    bool openCfg();
    bool writeCfg();
    void openSerialPort();
    void closeSerialPort();

private:
    Ui::MainWindow *ui;
    QSerialPort    *serialPortset;


public slots:
    //void on_searchButton_clicked();
    //void on_openComButton_clicked();
    void readSPData();
    //void sendData();
    void on_gpsPlainTextEdit_textChanged();
    void displayMsg(QString);
};

#endif // GPSGUI_H
