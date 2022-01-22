#include "sp2tcpserver.h"
#include "tsiGUI.h"

SP2TCPServer::SP2TCPServer(QObject *parent) : QThread(parent)
{
    qDebug()<<"TCP Server initialized...";
    mySP2TcpServer = new QTcpServer(this);
    connect(mySP2TcpServer, &QTcpServer::newConnection, this, &SP2TCPServer::sendData);
    connect(mySP2TcpServer, &QTcpServer::acceptError, this, &SP2TCPServer::displayError);
    QHostAddress ipserver;
    if(tsiIP == "*")
        ipserver=QHostAddress::AnyIPv4;
    else
        ipserver.setAddress(tsiIP);
    if (!mySP2TcpServer->listen(ipserver, tsiIPPort))
    {
        qDebug("Server listen on Port %d Failed...", tsiIPPort);
        assert(false);
    }
    else
    {
        qDebug("Server listen on Port %d ...", tsiIPPort);
        assert(true);
    }
}

void SP2TCPServer::run()
{
    uint idx=0;
    while(!isInterruptionRequested())
    {
        //qDebug()<<"Server Running..."<<idx;
        emit sendMsg2UI(QString::number(idx)+" - "+clientIP+" - "+current_date+" "+QString::number(recvTSI)+"\n");
        msleep(1000);
        idx++;
    }
}

void SP2TCPServer::sendData()
{
    QTcpSocket* gClientConnection = mySP2TcpServer->nextPendingConnection();
    clientIP=gClientConnection->peerAddress().toString();
    gClientConnection->write(current_date.toLatin1()+" "+devID.toLatin1()+" "+recvTSI_str.toLatin1());
    gClientConnection->disconnectFromHost();
    QString tmpMsg;
    tmpMsg=clientIP+" - "+current_date+" "+QString::number(recvTSI);
    emit sendMsg2UI(tmpMsg);
}

void SP2TCPServer::displayError(QAbstractSocket::SocketError socketError)
{
    qDebug()<<socketError;
    QString tmpMsg;
    tmpMsg="Error occured while accepting incoming connection...";
    emit sendMsg2UI(tmpMsg);
}
