#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "time_sys.h"

#include <QTcpServer>
#include <QDebug>
#include <QString>
#include <QDateTime>
#include <QThread>
//#include <QElapsedTimer>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("IP Camera Server");
    this->setFixedSize(967,600);
    //show current time
    time_Sys *tsys = new time_Sys(NULL);
    tsys->start();
    connect(tsys,&time_Sys::onesecPassed,this,&MainWindow::showTime);

    connect(ui->btnPhto, SIGNAL(clicked()), this, SLOT(BtnPhtoSlot()));  //拍照
    connect(ui->btn_listen, SIGNAL(clicked()), this, SLOT(BtnListenSlot()));  //监听
    connect(ui->btn_listen_clsconn,SIGNAL(clicked()), this, SLOT(BtnclsconnSlot()));
    //初始化套接字对象

    mServerM = new QTcpServer();
    connect(mServerM, SIGNAL(newConnection()), this, SLOT(BtnConnectSlot())); //连接客户端
   //connect(mServerM,&QTcpServer::newConnection,this,&MainWindow::server_New_Connect);
    ui->lineEdit_IP->setText("127.0.0.1");
    quint16 port=9988;
    ui->lineEdit_port->setText(QString::number(port));
    ui->lineEdit_save->setText("d:\\CamPic");
    QDir sdir(ui->lineEdit_save->text());
    if(!sdir.exists())
    {
        ui->textBrowser_status->append("Dir "+ui->lineEdit_save->text()+" doesn't exist, pls check...");
    }
    ui->textBrowser_status->append("Server End Initialized...");
    ui->lineEdit_time->setText("10");
    ui->btn_listen_clsconn->setEnabled(false);
    shotFlg=0; //don't shot
    serverstatus=0;//server not start
    timer.start();
}

MainWindow::~MainWindow()
{
    delete ui;
    mServerM->close();
    delete mServerM;
}

void MainWindow::showTime()
{
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy-MM-dd hh:mm:ss");
    qint32 uptime=timer.elapsed();
    qint32 upsecs=(uptime/1000) % 60;
    qint32 upmins=(uptime/1000)%3600/60;
    qint32 uphrs=(uptime/1000)%(24*3600)/3600;
    qint32 updays=(uptime/1000)/(24*3600);
    QString ups=QString("%1").arg(upsecs,2,10,QLatin1Char('0'));
    QString upm=QString("%1").arg(upmins,2,10,QLatin1Char('0'));
    QString uph=QString("%1").arg(uphrs,2,10,QLatin1Char('0'));
    QString upd=QString("%1").arg(updays,4,10,QLatin1Char('0'));
    ui->label_time->setText(current_date+"    Up: "+upd+"Days, "+uph+":"+upm+":"+ups);
}

void MainWindow::BtnPhtoSlot()
{
    if (mServerM && mServerM->isListening())
    {
        shotFlg=1;
    }
    else
    {
        ui->textBrowser_status->append("Waiting for Client...");
    }

}

void MainWindow::BtnclsconnSlot()
{
    try{
        if (serverstatus)
        {
           mSocketM->disconnectFromHost();
           mSocketM->close();
           mSocketM=NULL;
           mServerM->close();
           ui->textBrowser_status->append("Server Closed...");
           ui->btn_listen->setEnabled(true);
           ui->btn_listen_clsconn->setEnabled(false);
           serverstatus=0;
        }
        else
        {
            ui->textBrowser_status->append("Server has been closed...");
            ui->btn_listen->setEnabled(true);
            ui->btn_listen_clsconn->setEnabled(false);
            serverstatus=0;
        }
    }catch(Exception edis)
    {
        ui->textBrowser_status->append("Failed to Close Server...");
    }
}

void MainWindow::BtnConnectSlot()
{
    serverstatus=1;
    //connect(mServerM, SIGNAL(newConnection()), this, SLOT(BtnConnectSlot())); //连接客户端
    mSocketM = mServerM->nextPendingConnection(); //与客户端通信的套接字
    m_pClientSocketList.push_back(mSocketM);
    //关联接收客户端数据信号readyRead信号（客户端有数据就会发readyRead信号）
    ui->textBrowser_status->append("Client "+mSocketM->peerAddress().toString().remove("::ffff:")+" Connected...");
    connect(mSocketM, SIGNAL(readyRead()), this, SLOT(connect_suc()));
    //检测掉线信号
    connect(mSocketM, SIGNAL(disconnected()), this, SLOT(client_dis()));
    ui->btn_listen->setEnabled(false);
    ui->btn_listen_clsconn->setEnabled(true);
}

void MainWindow::connect_suc()
{
    if(serverstatus==1)
    {
        //可以实现同时读取多个客户端发送过来的消息
        QByteArray array;
        qint32 frate=ui->label_time->text().toInt();
        while (mSocketM->waitForReadyRead(frate)) {
            array.append((QByteArray)mSocketM->readAll());
            //qint32 asize=array.size();
            //ui->textBrowser_status->append("Reading "+QString::number(asize)+" Bytes...");
        }

        QBuffer buffer(&array);
        buffer.open(QIODevice::ReadOnly);
        QPixmap picture;//method 1
        picture.loadFromData(array, "bmp");
        if (!picture.isNull())
        {
            //qDebug() << "pic OK";
            ui->label->setPixmap(picture.scaled(ui->label->width(), ui->label->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            //ui->label->setPixmap(QPixmap::fromImage(array).scaled(ui->label->width(), ui->label->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));  //改变图片的大小
            ui->label->setAlignment(Qt::AlignCenter); //设置图片的位置为居中
            //ui->textBrowser_status->append("Pic Ok...");
            if(shotFlg==1)
            {
                QDateTime cdtime =QDateTime::currentDateTime();
                QString dirpre=ui->lineEdit_save->text();
                QString cdtfn=dirpre+"\\ServerCam01-"+cdtime.toString("yyyyMMdd-hhmmsszzz")+".jpg";
                ui->textBrowser_status->append("Saving "+cdtfn+" ...");
                if (!picture.save(cdtfn,"jpg"))
                {
                    ui->textBrowser_status->append("Failed ...");
                }
                shotFlg=0;
            }
        }
        else
        {
            QDateTime cdtime =QDateTime::currentDateTime();
            ui->textBrowser_status->append(cdtime.toString("yyyyMMdd-hhmmss")+" : Pic Error...");
            //qDebug() << "pic error";
        }
    }
}
void MainWindow::client_dis()
{
        serverstatus=0;
        //QTcpSocket *clientconn= static_cast<QTcpSocket*>(QObject::sender());
        QTcpSocket *clientconn = static_cast<QTcpSocket*>(QObject::sender());//掉线对象
        //m_pClientSocketList.removeOne(clientconn);
        //obj->flush();
        //obj->close();
        clientconn->disconnect();
        clientconn->disconnectFromHost();
        clientconn->deleteLater();
        clientconn->close();
        clientconn=nullptr;
        ui->textBrowser_status->append("Connection from Clients "+mSocketM->peerAddress().toString().remove("::ffff:")+" Closed...");//关闭连接成功
}

void MainWindow::BtnListenSlot()
{
    QHostAddress IP(ui->lineEdit_IP->text());//服务器端ip
    quint16 port =ui->lineEdit_port->text().toUInt();//服务器端端口号port
    if(mServerM->listen(QHostAddress::Any, port))//监听客户端ip和port
    {
        ui->textBrowser_status->append("Listening on "+IP.toString()+":"+QString::number(port));//监听成功
        ui->btn_listen->setEnabled(false);
        ui->btn_listen_clsconn->setEnabled(true);
    }
    else
    {
        if(!mServerM->isListening())
        {
            ui->textBrowser_status->append("Listening on "+IP.toString()+":"+QString::number(port)+" Failed...");//监听失败
        }
        else
        {
             ui->textBrowser_status->append("Already Listening on "+IP.toString()+":"+QString::number(port)+"...");//监听失败
             ui->btn_listen->setEnabled(false);
             ui->btn_listen_clsconn->setEnabled(true);
        }
    }
}

