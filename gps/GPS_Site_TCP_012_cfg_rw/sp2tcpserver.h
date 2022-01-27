#ifndef SP2TCPSERVER_H
#define SP2TCPSERVER_H

#include <QThread>
#include <QObject>
#include <QtNetwork/qtcpserver.h>
#include <QtNetwork/qtcpsocket.h>
#include <QtNetwork/QAbstractSocket>
#include <QDebug>

class QTcpServer;
class SP2TCPServer : public QThread
{
    Q_OBJECT
public:
    explicit SP2TCPServer(QObject *parent = 0);
    ~SP2TCPServer(){};
    QTcpServer *mySP2TcpServer;
    QString clientIP;

signals:
    void sendMsg2UI(QString);

private:
    void run(void);


public slots:
    void sendData();
    void displayError(QAbstractSocket::SocketError socketError);

private:
    QString gps_Data;
    //QTcpServer *mySP2TcpServer;


private:
    //MySocket *m_socket;
    int sockDesc;


};

#endif // SP2TCPSERVER_H
