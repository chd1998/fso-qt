﻿#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "time_sys.h"

#include <QDebug>
#include <QDateTime>
//#include <QElapsedTimer>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->setWindowTitle("IP Camera Client");
    this->setFixedSize(967,600);
    time_Sys *tsys = new time_Sys(NULL);
    tsys->start();
    connect(tsys,&time_Sys::onesecPassed,this,&MainWindow::showTime);
    timerGetFrame = new QTimer(this);
    timerSendData = new QTimer(this);


    //connect(timerGetFrame, SIGNAL(timeout()), this, SLOT(getFrame(int))); // 定时获取帧
    connect(timerGetFrame, &QTimer::timeout, this, [=](){getFrame(0);});
    connect(timerSendData, SIGNAL(timeout()), this, SLOT(TimeSendData())); // 定时获取帧


    connect(ui->BtnOpenCamera, SIGNAL(clicked()), this, SLOT(BtnOpenCamera()));  // 打开摄像头
    connect(ui->BtnCloseCamera, SIGNAL(clicked()), this, SLOT(BtnCloseCamera()));  // 关闭摄像头
    connect(ui->btnshot, SIGNAL(clicked()), this, SLOT(BtnShotSlot()));     // shot 1 pic by click button
    //connect(ui->btnshot, &QPushButton::clicked, this, [=](){getFrame(1);});

    connect(ui->btnConnect, SIGNAL(clicked()), this, SLOT(BtnConnectSlot())); // 连接
    connect(ui->btnOff, SIGNAL(clicked()), this, SLOT(BtnOffSlot()));     // 断开

    // 初始化套接字对象
    mSocketM = new QTcpSocket();
    ui->lineEditIp->setText("127.0.0.1");
    quint16 port=9988;
    ui->lineEditport->setText(QString::number(port));
    ui->lineEdit_save->setText("d:\\CamPic");
    QDir sdir(ui->lineEdit_save->text());
    if(!sdir.exists())
    {
        ui->textBrowser_status->append("Dir "+ui->lineEdit_save->text()+" doesn't exist, pls check...");
    }
    ui->textBrowser_status->append("Client End Initialized...");
    ui->lineEdit_time->setText("10");
    ui->lineEdit_camid->setText("0");
    ConnectFlg=0;
    firstconn=1;
    timer.start();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mSocketM;
    capture.release();
    delete [] qImg.bits();
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

void MainWindow::BtnOpenCamera()
{
    qint16 camid=ui->lineEdit_camid->text().toInt();//default is 0
    capture.open(camid);   // 打开摄像头，从摄像头中获取视频
    if (capture.isOpened())
    {
        //qDebug()<<"Open Camera succeeded...";
        ui->textBrowser_status->append("Open Camera "+QString::number(camid)+" succeeded...");
        int framerate=ui->lineEdit_time->text().toInt();
        timerGetFrame->start(framerate);   // 开始计时，超时则发出timeout()信号， 每framerate毫秒取一帧
        ui->BtnOpenCamera->setEnabled(false);
     }else
    {
        ui->textBrowser_status->append("Open Camera "+QString::number(camid)+" failed...");
        ui->BtnOpenCamera->setEnabled(true);
        //qDebug()<<"Open Camera failed...";
    }
}

// 关闭摄像头，并关闭连接
void MainWindow::BtnCloseCamera()
{
    if (capture.isOpened())
    {
        //qDebug()<<"Closing Camera...";

        timerGetFrame->stop();  // 停止取帧
        capture.release();  //释放资源
        ui->label->clear();
        //qDebug()<<"Closing Connection...";
        timerSendData->stop();  // 停止发送数据
        mSocketM->close();
        ui->textBrowser_status->append(tr("Camera Disconnected..."));
        ui->BtnOpenCamera->setEnabled(true);
    }
    else
    {
        //qDebug()<<"Closing Camera failed";
        ui->textBrowser_status->append("Camera not connected...");
    }
}

// 定时获取帧
void MainWindow::getFrame(int shotflag)
{
   //qDebug()<<"Take Picture";

   int delay = ui->lineEdit_time->text().toInt();
   Mat frame;
   capture >> frame;
   int key;
   key = waitKey(delay);
   //imwrite("camera1.bmp", frame); 保存

     //图像转换
   if (frame.channels() == 3)                             //三通道彩色图像
   {
       cv::cvtColor(frame, frame, COLOR_BGR2RGB);
       qImg = QImage((const unsigned char*)(frame.data), frame.cols, frame.rows, frame.cols*frame.channels(), QImage::Format_RGB888);
   }
   else if (frame.channels() == 1)                    //单通道（灰度图）
   {
       qImg = QImage((const unsigned char*)(frame.data), frame.cols, frame.rows, frame.cols*frame.channels(), QImage::Format_Indexed8);

       QVector<QRgb> colorTable;
       for (int k = 0; k<256; ++k)
       {
           colorTable.push_back(qRgb(k, k, k));
       }
       qImg.setColorTable(colorTable);//把qImg的颜色按像素点的颜色给设置
   }
   else
   {
       qImg = QImage((const unsigned char*)(frame.data), frame.cols, frame.rows, frame.cols*frame.channels(), QImage::Format_RGB888);
   }
   ui->label->setPixmap(QPixmap::fromImage(qImg).scaled(ui->label->width(), ui->label->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));  //改变图片的大小
   ui->label->setAlignment(Qt::AlignCenter); //设置图片的位置为居中
   tempPixmapM = QPixmap::fromImage(qImg);
   if(shotflag==1)
   {
       QDateTime cdtime =QDateTime::currentDateTime();
       QString dirpre=ui->lineEdit_save->text();
       QString cdtfn=dirpre+"\\ClientCam01-"+cdtime.toString("yyyyMMdd-hhmmsszzz")+".jpg";
       //save 1 pic when shot clicked
       ui->textBrowser_status->append("Saving "+cdtfn+" ...");
       if(!qImg.save(cdtfn,"JPG",100))
       {
            ui->textBrowser_status->append("Failed...");
       }
   }
   shotflag=0;
   //frame.release();
   //delete[] qImg.bits();
}

void MainWindow::BtnShotSlot()
{
    if (capture.isOpened())
    {
        ui->btnshot->setEnabled(false);
        //ui->textBrowser_status->append("Saving 1 Pic...");
        MainWindow::getFrame(1);
        ui->btnshot->setEnabled(true);
    }
    else
    {
        ui->textBrowser_status->append("Open Camera first...");
    }
}

// 如果连接定时发送数据
void MainWindow::TimeSendData()
{
    if (ConnectFlg==1  &&  capture.isOpened())
    {
        //qDebug()<<"Send Data to Server";
        timerSendData->start();
        QBuffer  buffer;
        buffer.open(QIODevice::ReadWrite);
        tempPixmapM.save(&buffer, "bmp");
        quint32 pix_len = (quint32)buffer.data().size();
        quint32 imgsize=pix_len*4;
        //qDebug("image size:%d", pix_len);
        //ui->textBrowser_status->append("Image : "+QString::number(imgsize)+" Bytes...");
        QByteArray dataArray;
        dataArray.append(buffer.data());
        quint32 write_len = mSocketM->write(dataArray);
        quint32 bufsize=write_len*4;
        //ui->textBrowser_status->append("Sending "+QString::number(bufsize)+" Bytes...");
    }
    else
    {   if(firstconn==1)
        {
            if (!capture.isOpened())
            {
                ui->textBrowser_status->append("Pls Open Camera...");
            }
            if (ConnectFlg==0)
            {
                ui->textBrowser_status->append("Pls Connect to "+ui->lineEditIp->text()+":"+ui->lineEditport->text()+"...");
            }
        }
        firstconn=0;
        //ui->BtnOpenCamera->setEnabled(true);
    }
}


void MainWindow::BtnConnectSlot()
{
   //检测链接成功信号关联槽函数
   connect(mSocketM, SIGNAL(connected()), this, SLOT(connect_suc()));
   //检测掉线信号
   connect(mSocketM, SIGNAL(disconnected()), this, SLOT(client_dis()));
   //连接服务器，设置ip和端口号
   try{
       mSocketM->connectToHost(ui->lineEditIp->text(), ui->lineEditport->text().toInt()); //ui->ipEdit->text()：ip，ui->portEdit->text().toInt()：端口号
       if(mSocketM->waitForConnected(1000)) //wait for connection in 1000ms
       {
           //QString  ui->lineEdit->text();
           ui->textBrowser_status->append("Connected to "+ui->lineEditIp->text()+":"+ui->lineEditport->text()+" ...");
           ConnectFlg=1;
       }
       else
       {
           mSocketM->disconnect();
           ui->textBrowser_status->append("Connect to "+ui->lineEditIp->text()+":"+ui->lineEditport->text()+" Failed...");
       }
   }catch(Exception ce)
   {
       ui->textBrowser_status->append("Connect to "+ui->lineEditIp->text()+":"+ui->lineEditport->text()+" Failed...");
   }
}


void MainWindow::BtnOffSlot()
{
   if(ConnectFlg==1)
   {
       mSocketM->close();
       mSocketM->disconnectFromHost();
       //capture.release();
       //delete [] qImg.bits();
       //mSocketM=NULL;
       ui->btnConnect->setEnabled(true);
       ui->textBrowser_status->append("DisConnected from Server...");
       ConnectFlg=0;
   }
   else
   {
       ui->textBrowser_status->append("Not Connected to Server...");
   }
}

void MainWindow::connect_suc()
{
    ui->btnConnect->setEnabled(false);//如果连接成功则连接按钮不能按下
    ConnectFlg = 1;
    timerSendData->start(ui->lineEdit_time->text().toInt());
}
void MainWindow::client_dis()
{
    ui->btnOff->setEnabled(true);//如果连接没有成功则连接按钮还可以按下
    ui->btnConnect->setEnabled(true);//如果连接成功则连接按钮不能按下
    ConnectFlg = 0;
    timerSendData->stop();
    mSocketM->disconnect();
    ui->textBrowser_status->append("DisConnected from Server...");

}

