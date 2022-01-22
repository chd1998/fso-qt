#include "sp2tcpserver.h"
#include "gpsGUI.h"

SP2TCPServer::SP2TCPServer(QObject *parent) : QThread(parent)
{
    qDebug()<<"TCP Server initialized...";
    mySP2TcpServer = new QTcpServer(this);
    connect(mySP2TcpServer, &QTcpServer::newConnection, this, &SP2TCPServer::sendData);
    connect(mySP2TcpServer, &QTcpServer::acceptError, this, &SP2TCPServer::displayError);
    QHostAddress ipserver;
    if(gpsIP == "*")
        ipserver=QHostAddress::AnyIPv4;
    else
        ipserver.setAddress(gpsIP);
    if (!mySP2TcpServer->listen(ipserver, myPort))
    {
        qDebug("Server listen on Port %d Failed...", myPort);
        //QString tmpMsg=siteID+" : "+"Server listen on "+QString::number(myPort)+" failed...";
        //emit sendMsg2UI(tmpMsg);
        assert(false);
    }
    else
    {
        qDebug("Server listen on Port %d ...", myPort);
        //QString tmpMsg=siteID+" : "+"Server listen on "+QString::number(myPort)+"...";
        //emit sendMsg2UI(tmpMsg);
        assert(true);
    }
}

void SP2TCPServer::run()
{
    uint idx=0;
    while(!isInterruptionRequested())
    {
        //qDebug()<<"Server Running..."<<idx;
        emit sendMsg2UI(QString::number(idx)+" - "+clientIP+" : "+lat_str+","+lon_str+","+alt_str+","+time_str+","+date_str);
        msleep(1000);
        idx++;
    }
}

void SP2TCPServer::sendData()
{
    QTcpSocket* gClientConnection = mySP2TcpServer->nextPendingConnection();
    clientIP=gClientConnection->peerAddress().toString();
    gClientConnection->write(siteID.toLatin1()+","+lat_str_ip.toLatin1()+","+lon_str_ip.toLatin1()+","+alt_str.toLatin1()+","+time_str.toLatin1()+","+date_str.toLatin1()+"\n");
    //gClientConnection->write(lon_str.toLatin1());
    //gClientConnection->write(time_str.toLatin1());
    //gClientConnection->write(date_str.toLatin1());
    gClientConnection->disconnectFromHost();
    QString tmpMsg;
    tmpMsg=siteID+" : "+clientIP+","+lat_str+","+lon_str+","+alt_str+","+time_str+","+date_str;
    emit sendMsg2UI(tmpMsg);
}

void SP2TCPServer::displayError(QAbstractSocket::SocketError socketError)
{
    qDebug()<<socketError;
    QString tmpMsg;
    tmpMsg="Error occured while accepting incoming connection...";
    emit sendMsg2UI(tmpMsg);
}
