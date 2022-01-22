#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QHostAddress>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket();//初始化客户端对象

    //连接信号槽
    //关联readyRead信号和socket_Read_Data槽函数。服务器有数据发送时，会自动触发readyRead信号
    QObject::connect(socket,&QTcpSocket::readyRead,this,&MainWindow::socket_Read_Data);

    ui->lineEdit_IP->setText("127.0.0.1");//设置默认IP。或者127.0.0.1也行，该ip所有本地计算机通用
    ui->lineEdit_Port->setText("6666");//客户端的端口号要与服务器的端口号一致
}

MainWindow::~MainWindow()
{
    //析构函数
    delete this->socket;//删除客户端对象
    delete ui;
}

//连接槽函数
void MainWindow::on_pushButton_Connect_clicked()
{
    socket->abort();//取消已有连接
    socket->connectToHost(ui->lineEdit_IP->text(),ui->lineEdit_Port->text().toInt());//连接服务器

    if(!socket->waitForConnected(30000))
    {
        qDebug()<<tr("Connect to Server Failed！");
        ui->label_Stat->setText("Connect to Server Failed！");
    }
    else
    {
        qDebug()<<tr("Server Connected！");
        ui->label_Stat->setText("Server Connected！");
    }
}

//发送按钮槽函数
void MainWindow::on_pushButton_Send_clicked()
{
    //long len=QString(ui->textEdit_Send->toPlainText().toLatin1()).length();
    long len=QString(ui->textEdit_Send->toPlainText()).length();
    qint64 writeResult = socket->write(ui->textEdit_Send->toPlainText().toLocal8Bit());
    bool BoolFlush = socket->flush();
    if(writeResult!=-1 && BoolFlush==1)
    {
        //返回值不为-1，则发送数据包成功
        if(writeResult==0)
        {
            ui->label_Stat->setText("Write Data returned 0...");
        }
        QString tmpString="Succeeded in Writing Data！"+QString::number(len)+" Bytes";
        ui->label_Stat->setText(tmpString);
        ui->textEdit_Send->clear();
    }
}

//读缓冲区槽函数
void MainWindow::socket_Read_Data()
{
    QByteArray buffer;
    buffer=socket->readAll();//读取缓冲区数据
    ui->textEdit_Receive->append(QString::fromLocal8Bit(buffer));//显示缓冲区数据
}

//断开连接槽函数
void MainWindow::socket_Disconnected()
{
    socket->close();
}

void MainWindow::on_textEdit_Receive_textChanged()
{
    ui->textEdit_Receive->moveCursor(QTextCursor::End);
}
