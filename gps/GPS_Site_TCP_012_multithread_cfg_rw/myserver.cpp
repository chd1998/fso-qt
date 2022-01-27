// myserver.cpp

#include "myserver.h"
#include "mythread.h"
#include "gpsGUI.h"

MyServer::MyServer(QObject *parent) :
    QTcpServer(parent)
{
}

void MyServer::startServer()
{
  QHostAddress ipserver;
  if(gpsIP == "*")
      ipserver=QHostAddress::AnyIPv4;
  else
      ipserver.setAddress(gpsIP);
  //ipserver.setAddress("127.0.0.1");
  if(!this->listen(ipserver, myPort))
    {
        qDebug() << "Could not start server";
        assert(false);
    }
    else
    {
        qDebug() << "Listening on port " << myPort << "...";
    }
    //qDebug() << this->isListening();
}

// This function is called by QTcpServer when a new connection is available.
void MyServer::incomingConnection(qintptr socketDescriptor)
{
    // We have a new connection
    qDebug() << socketDescriptor << " Connecting...";

    // Every new connection will be run in a newly created thread
    MyThread *thread = new MyThread(socketDescriptor, this);
    connect(thread,&MyThread::sendMsg2Server,this,&MyServer::recvMsgfromThread);
    // connect signal/slot
    // once a thread is not needed, it will be beleted later
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    //this->connect(&thread,&MyThread::sendMsg2UI,&MainWindow,&MainWindow::displayMsg);
    thread->start();
    //emit sendMsg2UI(lat_str+","+lon_str+","+alt_str+","+time_str+","+date_str+"\n");
    emit newConnection();
}

void MyServer::recvMsgfromThread(QString myMsg)
{
    emit sendMsg2UI(myMsg);
}
