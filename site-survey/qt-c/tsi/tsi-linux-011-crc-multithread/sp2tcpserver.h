#ifndef SP2TCPSERVER_H
#define SP2TCPSERVER_H

#include <QThread>
#include <QObject>
#include <QtNetwork/qtcpserver.h>
#include <QtNetwork/qtcpsocket.h>
#include <QtNetwork/QAbstractSocket>
#include <QDebug>

class mTcpServer : QTcpServer
{
public:
    mTcpServer(QObject *parent = 0);
    //This virtual function is called by QTcpServer when a new connection is available.
    //The socketDescriptor argument is the native socket descriptor for the accepted connection.
    virtual void incomingConnection(qintptr handle);
    QString wData;
    void startServer();
};

class dataHandleThread : public QThread
{
    Q_OBJECT
public:
    explicit dataHandleThread (qintptr handle, const QString& sWriteData);
    ~dataHandleThread (){};
    //QTcpServer *mySP2TcpServer;
    void run(void);


signals:
    void sendMsg2UI(QString);


private:
    QString gps_Data;
    //QTcpServer *mySP2TcpServer;


private:
    //MySocket *m_socket;
    //int sockDesc;
    qintptr m_nHandle;
    QString mWriteData;

};


#endif // SP2TCPSERVER_H
