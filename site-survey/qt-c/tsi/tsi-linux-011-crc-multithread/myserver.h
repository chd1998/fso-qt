#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include "mythread.h"

class MyServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = 0);
    void startServer();
    QString serverMsg;
signals:
    void sendMsg2UI(QString);

public slots:
    void recvMsgfromThread(QString);

protected:
    void incomingConnection(qintptr socketDescriptor);

};

#endif // MYSERVER_H
