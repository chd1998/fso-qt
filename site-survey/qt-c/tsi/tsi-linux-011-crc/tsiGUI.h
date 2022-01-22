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


extern QString SPName,SPParityBits,SP485Addr,devID,tsiIP,commandTSI,recvData,recvTSI_str;
extern int SPBaudRate,SPDataBits,SPStopBits,SPTimeInterval,tsiIPPort,recvTSI;
extern bool cfgStatus,dataok;
extern QDateTime current_date_time;
extern QString current_date;

//using std::cout;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void tsiDisplay();
    //void gpsDisplay_2();
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QTimer         *readTimer;
    //#define time_interval 1000


    //int mysockDesc;
    bool opened;
    uint time_interval;
    bool openCfg();
    bool writeCfg();
    void openSerialPort();
    void closeSerialPort();

    char ConvertHexChar(char ch);
    void StringToHex(QString str, QByteArray &senddata);
    //QString crc16ForModbus(const QString &hexText);

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

#endif // TSIGUI_H
