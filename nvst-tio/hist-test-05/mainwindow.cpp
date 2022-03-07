#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "simimage.h"
#include <QThread>

using namespace std;
bool display=true,stopped=false,paused=false;
int imgX=1024,imgY=1024,runround=0,refreshrate=1000;
unsigned short* simBuff;
QMutex tlock;
//QImage grayimage;


void MainWindow::showHistogram(QImage grayimage){
    //  对直方图进行数据处理,求出各个灰度值的像素数
    int yRange = 0;
    int data[256]{0};
    int max=0,idx=0;
    for(int i = 0; i <grayimage.height(); i++){
            for(int j = 0; j < grayimage.width(); j++){
                //int index=cv::saturate_cast<int>(grayimage.ptr<cv::Vec3b>(i)[j][0]);
                //int index=grayimage.ptr<cv::Vec3b>(i)[j][0];
                int index=grayimage.pixelIndex(i,j);
                ++data[index];
                if(data[index]>max)
                {
                    max=data[index];
                    idx=index;
                }
            }
        }
   qDebug()<<max<<" "<<idx;
    //设置每个“柱”的颜色，值，宽度等
    QBarSet *set = new QBarSet("灰度值");
    for (int i=0;i<256;i++) {
       set->append(data[i]);
       if(yRange<data[i]) yRange = data[i];
       set->setColor(QColor::Rgb);
    }
    QBarSeries *series = new QBarSeries();
    //series->replace(set);
    series->append(set);
    series->setBarWidth(1);

    //设置横坐标
    QCategoryAxis *axisX = new QCategoryAxis();
    axisX->setMin(0);
    axisX->setMax(255);
    //axisX->setStartValue(0);
    //axisX->setLabelsColor(QColor(0,0,0));
    axisX->append(QString::number(0),0);
    //axisX->setLabelsColor(QColor(0,0,255));
    QString tmpstring = QString::number(idx*65535/256);
    axisX->append("<font color=\"red\">"+tmpstring+"</font>", idx);
    //axisX->append(QString::number(idx*65535/255),idx);
    //axisX->setLabelsColor(QColor(0,0,0));
    axisX->append(QString::number(65535),255);
    axisX->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);


    //设置纵坐标
    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%d");
    axisY->setRange(0,yRange);

    //建表
    QChart *chart = new QChart();
    chart->addSeries(series);
    //chart->createDefaultAxes();
    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);
    chart->setTitle("灰度直方图");
    //chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);

    //表的容器
    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
    ui->chartView->setVisible(true);
}

void MainWindow::drawHist(QImage currentimage)
{
    if(thread->isRunning())
    {
        ui->textEdit_picinfo->append("Round "+QString::number(runround)+": drawing Histogram...");
        qDebug()<<currentimage.size();

        QImage qimg=currentimage.convertToFormat(QImage::Format_Indexed8);
        qDebug()<<qimg.pixelIndex(512,512);
        //qimg=qimg.convertToFormat(QImage::Format_Grayscale16);
        QPixmap pixmap = QPixmap::fromImage(qimg);
        pixmap.scaled(ui->label->size(), Qt::KeepAspectRatio);
        ui->label->clear();
        ui->label->setScaledContents(true);
        ui->label->setPixmap(pixmap);
        //cv::Mat tmpimage = cv::Mat(qimg.height(), qimg.width(), CV_8U, (void*)qimg.constBits(), qimg.bytesPerLine());
        //showHistogram(tmpimage);
        showHistogram(qimg);
        runround++;
    }else
    {
        ui->textEdit_picinfo->append("thread not running...");
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    opened=false;
    paused=false;
    //int round=0;

    setWindowTitle("Histogram for GrayScale Images");
    thread = new QThread( );
    task = new simImage();

    // move the task object to the thread BEFORE connecting any signal/slots
    task->moveToThread(thread);

     //connect(thread, SIGNAL(started()), task, SLOT(doWork()));
     //connect(task, SIGNAL(workFinished()), thread, SLOT(quit()));
     // automatically delete thread and task object when work is done:
     //connect(task, SIGNAL(workFinished()), task, SLOT(deleteLater()));
     //connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
     connect(thread, &QThread::started, task, &simImage::generateImg);
     connect(task,&simImage::imgReady,this, &MainWindow::drawHist);
     connect(task, &simImage::finished, thread, &QThread::quit,Qt::DirectConnection);
     connect(task, &simImage::finished, task, &simImage::deleteLater);
     connect(task, &simImage::finished, this,  &MainWindow::slotFinished);
     connect(thread, &QThread::finished, thread, &QThread::deleteLater);
     ui->pushButton_resume->setEnabled(false);
     ui->pushButton_pause->setEnabled(false);
     //grayimage = QImage(imgX,imgY,QImage::Format_Grayscale16);
     //thread->start();
    //connect(this,&MainWindow::histReady,this,&MainWindow::drawHist);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotFinished()
{


}

void MainWindow::closeEvent(QCloseEvent *event)
{
   qDebug()<<event;
   //display=false;
   stopped=true;
   //paused=true;
   qDebug()<<"Closing...";
   ui->textEdit_picinfo->append("Pls wait for 1 sec. for closing thread...");
   if(paused)
       tlock.unlock();
   if(thread->isRunning()){
       //thread->disconnect();
       thread->requestInterruption();
       thread->quit();
       thread->wait();
       //QElapsedTimer t;
       //t.start();
       while(!thread->isFinished())
       {
           qDebug()<<"Waiting for thread to quit...";
           QCoreApplication::processEvents();
       }
   }

   qDebug()<<"Closed!";
   QApplication::exit(0);
}

void MainWindow::on_pushButton_start_clicked()
{
    if(!thread->isRunning())
    {

        display=true;
        imgX=ui->lineEdit_imgX->text().toInt();
        imgY=ui->lineEdit_imgY->text().toInt();
        if(imgX<=0)
            imgX=1024;
        if(imgY<=0)
            imgY=1024;
        refreshrate=ui->lineEdit_refresh->text().toInt();
        if(refreshrate<=0)
            refreshrate=1000;
        //unsigned short *simBuff = new unsigned short [imgX][imgY];
        //ui->textEdit_picinfo->append("start clicked...");
        thread->start();
        if(thread->isRunning())
             ui->textEdit_picinfo->append("Thread Started...");
        else
             ui->textEdit_picinfo->append("Starting Thread Failed...");
        ui->pushButton_pause->setEnabled(true);
        ui->pushButton_start->setEnabled(false);

    }
    else
    {
        ui->textEdit_picinfo->append("Thread Already Running...");
    }

}



void MainWindow::on_pushButton_resume_clicked()
{
    ui->textEdit_picinfo->append("Thread Resumed...");
    ui->pushButton_resume->setEnabled(false);
    ui->pushButton_pause->setEnabled(true);

    imgX=ui->lineEdit_imgX->text().toInt();
    imgY=ui->lineEdit_imgY->text().toInt();
    if(imgX<=0)
        imgX=1024;
    if(imgY<=0)
        imgY=1024;
    refreshrate=ui->lineEdit_refresh->text().toInt();
    if(refreshrate<=0)
        refreshrate=1000;

    paused=false;
    tlock.unlock();
}


void MainWindow::on_pushButton_pause_clicked()
{
    ui->textEdit_picinfo->append("Thread Paused...");
    ui->pushButton_resume->setEnabled(true);
    ui->pushButton_pause->setEnabled(false);
    paused=true;
    tlock.lock();
}

