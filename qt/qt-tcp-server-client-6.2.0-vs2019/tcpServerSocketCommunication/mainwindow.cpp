#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //服务器参数初始化
    server = new QTcpServer();//初始化QTcpServer对象

    //当服务器被客户端访问时，会触发newConnection信号，可以利用该信号作为触发信号，绑定槽函数server_New_Connect
    connect(server,&QTcpServer::newConnection,this,&MainWindow::server_New_Connect);//关联信号和槽函数
    ui->lineEdit_IP->setText("127.0.0.1");//设置默认IP
    ui->lineEdit_Port->setText("6666");//设置默认端口号
}

MainWindow::~MainWindow()
{
    server->close();
    server->deleteLater();
    delete ui;
}

void MainWindow::on_pushButton_Listen_clicked()
{
    //侦听一个端口，客户端可以使用这个端口访问服务器
    QHostAddress IP(ui->lineEdit_IP->text());//服务器端ip
    quint16 port =ui->lineEdit_Port->text().toUInt();//服务器端端口号port
    if(server->listen(IP,port))//监听客户端ip和port
    {
        ui->labelStat->setText("Listening Succeeded！");//监听成功
    }
    else
    {
        //监听失败
        ui->labelStat->setText("Listening Failed！");
    }
}

void MainWindow::on_pushButton_Send_clicked()
{
    //long len=QString(ui->textEdit_Send->toPlainText().toLatin1()).length();
    long len=QString(ui->textEdit_Send->toPlainText()).length();
    qint64 writeResult = socket->write(ui->textEdit_Send->toPlainText().toLocal8Bit());//发送socket数据包
    bool BoolFlush = socket->flush();//从缓冲区读写数据，如果从缓冲区读写了数据，则返回true。没有数据读写，则返回false
    if(writeResult!=-1 && BoolFlush==1)
    {
        //返回值不为-1，则发送数据包成功
        if(writeResult==0)
        {
            ui->labelStat->setText("Write Data returned 0");
        }
        QString tmpString="Succeeded in Writing Data！"+QString::number(len)+" Bytes";
        ui->labelStat->setText(tmpString);
        ui->textEdit_Send->clear();
    }

}

void MainWindow::server_New_Connect()
{
    //获取客户端连接
    socket =server->nextPendingConnection();

    //连接QTcpSocket的信号槽，以读取新数据
    //&QTcpSocket::readyRead表示服务器接收到客户端数据后，自动触发readyRead信号
    //&QTcpSocket::disconnected表示服务器接收到客户端断开连接后，自动触发disconnected信号
    QObject::connect(socket,&QTcpSocket::readyRead,this,&MainWindow::socket_Read_Data);
    QObject::connect(socket,&QTcpSocket::disconnected,this,&MainWindow::socket_Disconnect);
}

void MainWindow::socket_Read_Data()
{
    QByteArray buffer;
    buffer=socket->readAll();//读取缓冲区数据
    ui->textEdit_Receive->append(QString::fromLocal8Bit(buffer));//显示接收到的内容
}

void MainWindow::socket_Disconnect()
{
    socket->close();
}

void MainWindow::on_textEdit_Receive_textChanged()
{
    ui->textEdit_Receive->moveCursor(QTextCursor::End);
}
