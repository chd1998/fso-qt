#include "taserialthread.h"

TA_SerialThread::TA_SerialThread()
{
    portCnt = 0;
    m_arrSerial = new QSerialPort[5];
}

TA_SerialThread::~TA_SerialThread()
{

}

void TA_SerialThread::run()
{

}

void TA_SerialThread::SearchPort(void)
{
    //查找可用的串口
    //const auto infos = QSerialPortInfo::availablePorts();
    //for (const QSerialPortInfo &info : infos)
    for(const QSerialPortInfo &info : QSerialPortInfo::availablePorts())
    {
        if(portCnt > 4)
            break;

        m_arrSerial[portCnt].setPort(info);
        if(m_arrSerial[portCnt].open(QIODevice::ReadWrite))
            m_arrSerial[portCnt++].close();
    }
}

void TA_SerialThread::InitPortName(uint8_t portIndex,const QString &portName)
{
    if(portIndex < portCnt)
        m_arrSerial[portIndex].setPortName(portName);//const QString &name
    else
       qDebug()<<"OpenPort port index is out of range!";
}

void TA_SerialThread::InitPortBaudRate(uint8_t portIndex,qint32 baudRate)
{
    if(portIndex < portCnt)
        m_arrSerial[portIndex].setBaudRate(baudRate);//BaudRate baudRate
    else
       qDebug()<<"OpenPort port index is out of range!";
}

void TA_SerialThread::InitPortDataBits(uint8_t portIndex,QSerialPort::DataBits dataBits)
{
    if(portIndex < portCnt)
        m_arrSerial[portIndex].setDataBits(dataBits);//DataBits dataBits
    else
       qDebug()<<"OpenPort port index is out of range!";
}

void TA_SerialThread::InitPortParity(uint8_t portIndex,QSerialPort::Parity parity)
{
    if(portIndex < portCnt)
        m_arrSerial[portIndex].setParity(parity);//Parity parity
    else
       qDebug()<<"OpenPort port index is out of range!";
}

void TA_SerialThread::InitPortStopBits(uint8_t portIndex,QSerialPort::StopBits stopBits)
{
    if(portIndex < portCnt)
    {
        m_arrSerial[portIndex].setStopBits(stopBits);//StopBits stopBits
        m_arrSerial[portIndex].setFlowControl(QSerialPort::NoFlowControl);
    }
    else
       qDebug()<<"OpenPort port index is out of range!";
}

void TA_SerialThread::OpenPort(uint8_t portIndex)
{
    if(portIndex < portCnt)
        m_arrSerial[portIndex].open(QIODevice::ReadWrite);
    else
        qDebug()<<"OpenPort port index is out of range!";
}

void TA_SerialThread::ClosePort(uint8_t portIndex)
{
    if(portIndex < portCnt)
    {
        //关闭串口
        m_arrSerial[portIndex].clear();
        m_arrSerial[portIndex].close();
        //m_arrSerial[portIndex].deleteLater();
    }
    else
        qDebug()<<"OpenPort port index is out of range!";
}

void TA_SerialThread::SendDataBuf(uint8_t portIndex,const QByteArray &str)
{
    if(portIndex < portCnt)
    {
        m_arrSerial[portIndex].write(str);
    }
    else
        qDebug()<<"SendDataBuf port index is out of range!";
}

QByteArray TA_SerialThread::GetDataBuf(uint8_t portIndex)
{
    if(portIndex < portCnt)
        return m_arrSerial[portIndex].readAll();
    else
        qDebug()<<"GetDataBuf port index is out of range!";
}
