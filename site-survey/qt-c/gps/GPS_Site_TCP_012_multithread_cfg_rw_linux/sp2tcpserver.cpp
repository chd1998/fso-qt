#include "sp2tcpserver.h"
#include "gpsGUI.h"

mTcpServer::mTcpServer(QObject *parent)
{
   //startMultiThreadTcpSocketServer();
}

void mTcpServer::startServer()
{
    mTcpServer* m_pTcpServer = new mTcpServer();
        //启动服务端
        if (!m_pTcpServer->listen(QHostAddress::Any, myPort))
        {
            qDebug() << "m_pTcpServer->listen() error";
        }

}

void mTcpServer::incomingConnection(qintptr handle){
    wData=lat_str+","+lon_str+","+alt_str+","+time_str+","+date_str;
    dataHandleThread* pThread=new dataHandleThread(handle,wData);
    connect(pThread, &dataHandleThread::finished, pThread, &dataHandleThread::deleteLater);
    pThread->start();

}

dataHandleThread::dataHandleThread(qintptr handle, const QString& sWriteData)
:mWriteData(sWriteData), m_nHandle(handle)
{

}



void dataHandleThread::run()
{
    QTcpSocket oTcpSocket;
    if (!oTcpSocket.setSocketDescriptor(m_nHandle))
    {
        qDebug() << "oTcpSocket.setSocketDescriptor error";
        return;
    }

    //2. 向客户端写数据
    qDebug() << "MultiThreadTcpSocketServerDemo::readDataFromClient" << &oTcpSocket;
    qDebug() << "MultiThreadTcpSocketServerDemo::writeDataToClient" << mWriteData;
    emit sendMsg2UI(mWriteData);
    oTcpSocket.write(mWriteData.toLatin1());
    oTcpSocket.disconnectFromHost();
    oTcpSocket.waitForDisconnected();
}


