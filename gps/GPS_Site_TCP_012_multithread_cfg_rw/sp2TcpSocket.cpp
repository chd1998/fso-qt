#include "sp2TcpSocket.h"

sp2TcpSocket::sp2TcpSocket(int sock, QTcpSocket *parent) : QTcpSocket(parent)
{
    this->setSocketDescriptor(sock);
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SocketErr(QAbstractSocket::SocketError)));
}

sp2TcpSocket::~sp2TcpSocket()
{

}

// 将会移进线程里面运行
void sp2TcpSocket::WriteData()
{
    TcpSocket *socket = (sp2TcpSocket*)sender();
    QString recvStr = socket->readAll();
    m_recvDataStr += recvStr;
    qDebug()<<m_recvDataStr<<socket->localAddress().toString();
    // 对数据的处理
}

void TcpSocket::SocketErr(QAbstractSocket::SocketError socketError)
{
    TcpSocket *socket = (TcpSocket*)sender();
}
