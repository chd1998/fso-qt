#ifndef SP2TCPSOCKET_H
#define SP2TCPSOCKET_H

#include <QTcpSocket>
#include <QHostAddress>

class sp2TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    sp2TcpSocket(int socketdesc,QTcpSocket *parent = NULL);
    ~sp2TcpSocket();

private slots:
    void WriteData();
    void SocketErr(QAbstractSocket::SocketError socketError);
private:
    QString m_recvDataStr;
};

#endif // SP2TCPSOCKET_H
