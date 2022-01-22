#ifndef TASERIALTHREAD_H
#define TASERIALTHREAD_H

#include <QThread>
#include <QtSerialPort>
#include <QSerialPortInfo>
#include <QString>

class TA_SerialThread:public QThread
{
public:
    TA_SerialThread();
    ~TA_SerialThread();

public:
    uint8_t portCnt;
    QSerialPort *m_arrSerial;//ptr point to an array,maxium port num is 5

public:
    void SearchPort(void);
    void InitPortName(uint8_t portIndex,const QString &portName);
    void InitPortBaudRate(uint8_t portIndex,qint32 baudRate);
    void InitPortDataBits(uint8_t portIndex,QSerialPort::DataBits dataBits);
    void InitPortParity(uint8_t portIndex,QSerialPort::Parity parity);
    void InitPortStopBits(uint8_t portIndex,QSerialPort::StopBits stopBits);
    void OpenPort(uint8_t portIndex);
    void ClosePort(uint8_t portIndex);
    void SendDataBuf(uint8_t portIndex,const QByteArray &str);
    QByteArray GetDataBuf(uint8_t portIndex);

private slots:
    void run();
};
#endif // TASERIALTHREAD_H
