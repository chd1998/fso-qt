#include <iostream>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QLoggingCategory>
#include <QPoint>
#include <QPointer>
#include <QGraphicsPixmapItem>
//#include <QGraphicsItem>
#include <QImage>
#include <QPixmap>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QIODevice>
#include <QTextStream>
#include <QButtonGroup>
#include <stdlib.h>
#include <QDebug>
#include <windows.h>
#include <QMessageBox>
#include <QCloseEvent>
#include <fstream>
#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"
#include "atlib/atutility.h"
#include "time_sys/time_sys.h"
#include "accd/accd.h"
#include "histdisplay/histdisplay.h"


#define GB (1024*1024*1024)

double expTime=1.2;
int frameRate=200;
uint framedelay=10,groupdelay=30,fps0=0,fps1=0,fps=0;
int frameRateMax=200;
QString saveTo="e:\\",savepre="",savepred="",savepref="",ccdM,saveDir="",savepreobj,savepredf,save01="";
//QString current_date_d;
//QString current_date_t;
bool savefits=false,opened=false,live=false,display=false,display_locked=false,imgready=true,histshow_locked=false,histcalc_locked=false,savefits_locked=false,wait_Acq=false;
double temperature=0.0;
aCCD *andorCCD;
AT_64 imgW,imgH,imgStride,imageSizeBytes;
AT_H handle;
AT_64 f0=0, f1=0,df=0;
int ccdID=0,realDev=0;
long refreshtime=20;
QString fpre;
int displayRate,numBuffer;
QString CamF,CamM,CamN;
long t0=0;
int serialNo=0,fserialNo=0,sum_fserialNo=0;
bool firsttime=true,localsave=false,localfirst=false;;
QString objname="Sun";
QString obscor1="N00",obscor2="E00";
QString datatype="TiO";
uint flatcnt=0;
//QString fcnt;
QFile obslog;
QString obslogfile,obslogdir,obslogdate,logtmp,obsname;
std::ofstream outfile;
QMutex mutex,histcalc_lock,histdisplock;
quint16 imgMax=0,freedisk;
double sx=200,sy=200,ex=800,ey=800;
bool drawing=false;
int flatnum=2000,darknum=1000;
uint datanum=200;
bool continousACQ=false,diskfull=false;
QDir sdir;
QString current_date_t1,current_date_t2,current_date_t3,current_date_d;
//global vars. for histogram
QLineSeries *lineseries = nullptr;
QAreaSeries *series = nullptr;
QCategoryAxis *axisX = nullptr;
QValueAxis *axisY = nullptr;
QChart *chart= nullptr;

bool histfirst=true;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);

    QDateTime timeT = QDateTime::currentDateTime();   //获取当前时间
    t0 = timeT.toSecsSinceEpoch();   //将当前时间转为时间戳
    QDir dir;
    QDateTime current_date_time =QDateTime::currentDateTimeUtc();
    obslogdate=current_date_time.toString("yyyyMMdd");
    obslogdir=ui->lineEdit_savelog->text();
    if(!dir.exists(obslogdir))
    {
        dir.mkpath(obslogdir);
    }
    obsname=obslogdir+"\\TiO-"+obslogdate+".log";
    outfile.open(obsname.toStdString(), std::ios::app);

    //ui->btnLive->setEnabled(false);
    //ui->btnSnap->setEnabled(false);
    //ui->actionServer->setEnabled(false);
    //ui->verticalLayout_2->setAlignment(Qt::AlignTop);
    QString proFullPath=QCoreApplication::applicationFilePath();
    QFileInfo tmpinfo(proFullPath);
    QString proName=tmpinfo.fileName();
    QStringList tmplist=proName.split(".");
    setWindowTitle(tmplist[0]+" (Qt6+minGW64+OpenCV) - by Chen Dong @fso");
    about = new AboutWindow(this);
    //this->loadPropertyList(":/profile.json");
    this->setupStatusBar();
    this->on_actionDetect_Cameras_triggered();
    ui->actionSave->setEnabled(false);
    server = new Server(this);
    connect(server, &Server::newRequest, this, &MainWindow::processRemoteCall);
    connect(server, &Server::serverStateChanged, this, &MainWindow::handle_serverStateChanged);
    //connect ccd
    if(realDev>=1)
    {
        QPointer <aCCD> andorCCD=new aCCD(NULL);
        andorCCD->start();
        if(andorCCD->isRunning())
        {
            mutex.lock();
            //QVector<unsigned short>vecimg(imgW*imgH,0);
            //QVector<unsigned short>vechistdata(65536,0);
            QDateTime current_date_time =QDateTime::currentDateTimeUtc();
            QString todaynow =current_date_time.toString("yyyyMMdd hh:mm:ss");
            ui->textEdit_status->append(">>>"+todaynow+":");
            logtmp=">>>"+todaynow+":";
            outfile<<logtmp.toStdString()<<"\n";
            logtmp="Found "+QString::number(realDev)+" "+CamM+" "+CamN+" Device(s)...";
            ui->textEdit_status->append(logtmp);
            //qDebug("log file 1");
            outfile<<logtmp.toStdString()<<"\n";
            logtmp="Camera 0：ready to work...";
            outfile<<logtmp.toStdString()<<"\n";
            mutex.unlock();
            ui->textEdit_status->append(logtmp);

            histthread = new QThread();
            task = new histdisplay();

            // move the task object to the thread BEFORE connecting any signal/slots
            task->moveToThread(histthread);
            connect(histthread, &QThread::started, task, &histdisplay::working);
            connect(task, &histdisplay::finished, histthread, &QThread::quit);
            connect(task, &histdisplay::finished, task, &histdisplay::deleteLater);
            //connect(task, &histdisplay::finished, this,  &MainWindow::slotFinished);
            connect(histthread, &QThread::finished, histthread, &QThread::deleteLater);

            connect(andorCCD,&aCCD::buf_Ready,this,&MainWindow::updateGraphicsView,Qt::QueuedConnection);
            connect(andorCCD,&aCCD::buf_Ready,task,&histdisplay::buf2hist,Qt::DirectConnection);
            connect(andorCCD,&aCCD::stop_Acq,this,&MainWindow::stopACQ,Qt::DirectConnection);
            connect(task,&histdisplay::draw_hist,this,&MainWindow::drawHist,Qt::BlockingQueuedConnection);
            //connect(andorCCD, &aCCD::pause_Acq, this, &MainWindow::periodicSave,Qt::DirectConnection);
            //*histimg = QImage(imgH,imgW,QImage::Format_Indexed8);

            histthread->start();
            //connect(andorCCD,&aCCD::buf_Ready,this,&MainWindow::drawHist);
            //ui->textEdit_status->append("Connected to Camera "+QString::number(ui->spinBoxCameraIndex->value()));
            ccdID=0;
            opened=true;
            expTime=1.2;
            ui->lineEdit_exposuretime->setText(QString::number(expTime));
            frameRate=frameRateMax;
            ui->lineEdit_framerate->setText(QString::number(frameRate));
            groupdelay=30;
            ui->lineEdit_groupdelay->setText(QString::number(groupdelay));
            framedelay=10;
            numBuffer=10;
            objname="Sun";
            ui->lineEdit_objname->setText(objname);
            obscor1="N00";
            obscor2="E00";
            ui->lineEdit_cor1->setText(obscor1);
            ui->lineEdit_cor2->setText(obscor2);
            ui->btnLive->setEnabled(true);
            ui->btnSnap->setEnabled(false);
            ui->actionServer->setEnabled(false);
            ui->lineEdit_objname->setEnabled(false);
            ui->lineEdit_cor1->setEnabled(false);
            ui->lineEdit_cor2->setEnabled(false);
            labelStat->setText("Ready for Acquistion...");

        }else
        {
            ui->btnLive->setEnabled(false);
            ui->btnSnap->setEnabled(false);
            ui->actionServer->setEnabled(false);
            logtmp="Open Camera Failed...";
            ui->textEdit_status->append(logtmp);
            mutex.lock();
            outfile<<logtmp.toStdString()<<"\n";
            mutex.unlock();
            ui->lineEdit_exposuretime->setEnabled(false);
            ui->lineEdit_framerate->setEnabled(false);
            ui->lineEdit_objname->setEnabled(false);
            ui->lineEdit_cor1->setEnabled(false);
            ui->lineEdit_cor2->setEnabled(false);
            ui->lineEdit_darknum->setEnabled(false);
            ui->lineEdit_flatnum->setEnabled(false);
            ui->lineEdit_datanum->setEnabled(false);
            ui->lineEdit_saveto->setEnabled(false);
            ui->lineEdit_savelog->setEnabled(false);
            ui->lineEdit_groupdelay->setEnabled(false);
            labelStat->setText("No Camera Found...");
        }
    }else
    {
        ui->btnLive->setEnabled(false);
        ui->btnSnap->setEnabled(false);
        ui->actionServer->setEnabled(false);
        ui->lineEdit_exposuretime->setEnabled(false);
        ui->lineEdit_framerate->setEnabled(false);
        ui->lineEdit_objname->setEnabled(false);
        ui->lineEdit_cor1->setEnabled(false);
        ui->lineEdit_cor2->setEnabled(false);
        ui->lineEdit_darknum->setEnabled(false);
        ui->lineEdit_flatnum->setEnabled(false);
        ui->lineEdit_datanum->setEnabled(false);
        ui->lineEdit_saveto->setEnabled(false);
        ui->lineEdit_savelog->setEnabled(false);
        ui->lineEdit_groupdelay->setEnabled(false);
        labelStat->setText("No Camera Found...");

    }
    //connect(andorCCD,&aCCD::buf_Ready,this,&MainWindow::updateGraphicsView);
    scene = new GraphicsScene();
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    ui->graphicsView->setScene(scene);

    ui->textEdit_status->setReadOnly(true);
    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff );
    connect(scene, &GraphicsScene::positionMoved, this, &MainWindow::updateCursorCoord);
    connect(scene, &GraphicsScene::RectPoint1, this, &MainWindow::selectStartpoint);
    connect(scene, &GraphicsScene::RectPoint2, this, &MainWindow::drawRect);
    tsys = new time_Sys(NULL);
    tsys->start();
    connect(tsys,&time_Sys::onesecPassed,this,&MainWindow::showTime);
    //connect(ui->lineEdit_exposuretime,&QLineEdit::returnPressed, this,&MainWindow::on_exptime_changed);
    QButtonGroup *group = new QButtonGroup(this);

    group->addButton(ui->checkBox_Flat,1);
    group->addButton(ui->checkBox_Dark,2);
    group->addButton(ui->checkBox_Data,3);
    ui->checkBox_Dark->setChecked(true);
    if(ui->checkBox_Dark->isChecked()){
        fpre="dark";
        datanum=ui->lineEdit_darknum->text().toUInt();
    }
    if(ui->checkBox_Flat->isChecked()){
        fpre="FLAT";
        datanum=ui->lineEdit_flatnum->text().toUInt();
    }
    if(ui->checkBox_Data->isChecked()){
        fpre="T";
        datatype="TiO";
        datanum=ui->lineEdit_datanum->text().toUInt();
    }
    ui->lineEdit_flatnum->setEnabled(false);
    ui->lineEdit_datanum->setEnabled(false);
    ui->lineEdit_darknum->setEnabled(true);
    ui->lineEdit_objname->setEnabled(false);
    ui->lineEdit_cor1->setEnabled(false);
    ui->lineEdit_cor2->setEnabled(false);

    //set cache for display
    //setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    QPixmapCache::setCacheLimit(102400);    //default=10240kB

    //绘制直方图
    QVector<unsigned short>vechistdata0(65536,0);
    int histmax0=1000;
    int histidx0=32768;
    lineseries=new QLineSeries();
    for (int i=0;i<65536;++i) {
       lineseries->append(QPointF(i,vechistdata0[i]));
       //set->setColor(QColor::Rgb);
    }
    lineseries->setUseOpenGL(true);

    series = new QAreaSeries(lineseries);
    series->setName("Histogram");
    QPen pen(Qt::black);
    pen.setWidth(1);
    series->setPen(pen);
    QBrush brush;
    brush.setColor(Qt::black);//画刷颜色
    brush.setStyle(Qt::SolidPattern);//画刷填充样式，斜网格
    series->setBrush(brush);
    //设置横坐标
    axisX = new QCategoryAxis();
    axisX->setMin(0);
    axisX->setMax(65535);
    axisX->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    axisX->append(QString::number(0),0);
    QString tmpstring = QString::number(histidx0);
    axisX->append("<font color=\"red\">"+tmpstring+"</font>", histidx0);
    axisX->append(QString::number(65535),65535);

    //设置纵坐标
    axisY = new QValueAxis();
    axisY->setTickCount(3);
    axisY->setLabelFormat("%7d");
    axisY->setRange(0,histmax0);

    //建表
    chart = new QChart();
    chart->setContentsMargins(0,0,0,0);
    //左上右下
    chart->setMargins(QMargins(20,0,20,0));
    chart->setAutoFillBackground(true);
    chart->addSeries(series);
    //chart->createDefaultAxes();
    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);
    chart->setTitle("Image Histogram");
    chart->legend()->hide();
    //chart->setAnimationOptions(QChart::SeriesAnimations);
    //chart->legend()->setVisible(true);
    //表的容器
    int w=ui->chart_graphicsView->width();
    int h=ui->chart_graphicsView->height();
    ui->chart_graphicsView->resize(w,h);
    ui->chart_graphicsView->setChart(chart);
    ui->chart_graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->chart_graphicsView->setVisible(true);

    //obslog open

    if(outfile.is_open()){
        //qDebug("log file opened...");
        outfile<<"========================================"<<"\n";
        outfile.close();
    }
    setFixedSize(this->width(),this->height());
    //outfile<<obslogdir.toStdString();
}


//获取磁盘容量
quint64 MainWindow::getDiskSpace(QString iDriver, bool flag)
{
    //iDriver为盘符(例如"C\"),flag为true则求磁盘总容量，为false则求磁盘剩余容量
    LPCWSTR strDriver=(LPCWSTR)iDriver.utf16();
    ULARGE_INTEGER freeDiskSpaceAvailable, totalDiskSpace, totalFreeDiskSpace;

    //调用函数获取磁盘参数(单位为字节Byte),转化为GB，需要除以(1024*1024*1024)
    GetDiskFreeSpaceEx( strDriver, &freeDiskSpaceAvailable, &totalDiskSpace, &totalFreeDiskSpace) ;
    if(flag)
    {
        return (quint64) totalDiskSpace.QuadPart/GB;
    }
    else
    {
        freedisk=(quint64) totalFreeDiskSpace.QuadPart/GB;
        if(freedisk<=3)
            diskfull=true;
        return (quint64) totalFreeDiskSpace.QuadPart/GB;
    }
}

void MainWindow::selectStartpoint(double sa,double sb){
    sx=sa;
    sy=sb;
    //ui->textEdit_status->append("Start Point: "+QString::number(sx)+" "+QString::number(sy));
}

void MainWindow::drawRect(double ea,double eb){
    ex=ea;
    ey=eb;
    //ui->textEdit_status->append("End Point: "+QString::number(ex)+" "+QString::number(ey));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug()<<event;
    savefits=false;
    live=false;
    opened=false;
    AT_Flush(handle);
    //AT_Close(handle);
    //Do my jobs on button x clicked
    qDebug("closing...");
    QDateTime current_date_time =QDateTime::currentDateTimeUtc();
    QString todaynow =current_date_time.toString("yyyyMMdd hh:mm:ss");
    ui->textEdit_status->append(">>>"+todaynow+":");
    mutex.lock();
    outfile.open(obsname.toStdString(), std::ios::app);
    logtmp=">>>"+todaynow+":";
    outfile<<logtmp.toStdString()<<"\n";
    ui->textEdit_status->append("Closing in 5 secs. ...");
    logtmp="Closing in 5 secs. ...";
    outfile<<logtmp.toStdString()<<"\n";
    outfile<<"========================================"<<"\n";
    outfile<<" "<<"\n";
    if(outfile.is_open()){
        //qDebug("log file opened...");
        outfile.close();
    }
    mutex.unlock();
    ui->textEdit_status->append("pls wait for 5 secs. to release all resources...");

    AT_FinaliseUtilityLibrary();
    AT_FinaliseLibrary();
    /*if(andorCCD->isRunning())
        andorCCD->requestInterruption();
    andorCCD->wait();
    if(tsys->isRunning())
        tsys->requestInterruption();
    tsys->wait();
    andorCCD->quit();
    tsys->quit();
    if(outfile.is_open())
        outfile.close();*/
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<5000)
        QCoreApplication::processEvents();
    //delete andorCCD;
    //delete tsys;
    QApplication::exit(0);

}

void MainWindow::showTime()
{
    QString diskname;

    fps=fps1-fps0;
    fps0=fps1;
    QDateTime current_date_time =QDateTime::currentDateTime();
    if(opened)
    {
        AT_GetFloat(handle, L"SensorTemperature", &temperature);
    }
    long t1 = current_date_time.toSecsSinceEpoch();
    float dt = (float)(t1-t0)/3600.0;
    QString angle_sign=u8"°C";
    QString current_date =current_date_time.toString("yyyy-MM-dd hh:mm:ss")+" || Up: "+QString::number(dt,'f',4)+"hrs || Sensor: "+QString::number(temperature)+angle_sign+" || FPS: "+QString("%1").arg(fps, 3, 10, QLatin1Char('0'))+" Frames/Sec.";
    ui->label_time->setText(current_date);
    diskname=ui->lineEdit_saveto->text();
    quint64 freeDiskSpace = getDiskSpace(diskname, false);
    quint64 totalDiskSpace = getDiskSpace(diskname, true);
    QString diskfree="",full="",empty="";
    if(freedisk>1000)
    {
        empty="<font style='color:green;'>"+QString::number(freeDiskSpace)+"</font>";
        diskfree=empty+"/"+QString::number(totalDiskSpace)+" GB (Free/Total)";
    }
    else
    {
        full="<font style='color:red;'>"+QString::number(freeDiskSpace)+"</font>";
        diskfree=full+"/"+QString::number(totalDiskSpace)+" GB (Free/Total)";
    }
    ui->label_freedisk->setText(diskfree);
    /*if(savefits)
    {
        labelStat->setText(" "+QString("%1").arg(fserialNo, 8, 10, QLatin1Char('0'))+" Frame(s)");
    }else{
        labelStat->setText(" Stopped - "+QString("%1").arg(sum_fserialNo, 8, 10, QLatin1Char('0'))+" Frame(s) Saved");
    }*/

}

void MainWindow::updateCursorCoord(double x, double y) {
    int ix = static_cast<int>(x);
    int iy = static_cast<int>(y);
    labelCoordX->setText(QString::number(ix));
    labelCoordY->setText(QString::number(iy));
    if (!scene->items().isEmpty()) {
        if (ix > 0 && ix <= currentImage.rows && iy > 0 && iy <= currentImage.cols) {
            int v = currentImage.at<unsigned short>(cv::Point(iy, ix));
            //int v = CV_IMAGE_ELEM(currentImage,CV_16U,iy,ix);
            labelCoordV->setText(QString::number(v));
        }
    }
    labelCoordMV->setText(QString::number(imgMax));
}

void MainWindow::setupStatusBar() {
    QLabel *label;
    label = new QLabel(this);
    label->setText("X");
    label->setAlignment(Qt::AlignCenter);
    label->setMinimumWidth(20);
    labelCoordX = new QLabel(this);
    labelCoordX->setMinimumWidth(60);
    ui->statusBar->addWidget(label);
    ui->statusBar->addWidget(labelCoordX);

    label = new QLabel(this);
    label->setText("Y");
    label->setAlignment(Qt::AlignCenter);
    label->setMinimumWidth(20);
    labelCoordY = new QLabel(this);
    labelCoordY->setMinimumWidth(60);
    ui->statusBar->addWidget(label);
    ui->statusBar->addWidget(labelCoordY);

    label = new QLabel(this);
    label->setText("Value");
    label->setAlignment(Qt::AlignCenter);
    label->setMinimumWidth(20);
    labelCoordV = new QLabel(this);
    labelCoordV->setMinimumWidth(60);
    ui->statusBar->addWidget(label);
    ui->statusBar->addWidget(labelCoordV);

    label = new QLabel(this);
    label->setText("Max Value");
    label->setAlignment(Qt::AlignCenter);
    label->setMinimumWidth(20);
    labelCoordMV = new QLabel(this);
    labelCoordMV->setMinimumWidth(60);
    ui->statusBar->addWidget(label);
    ui->statusBar->addWidget(labelCoordMV);

    labelinfo = new QLabel(this);
    //label->setText(" Saving Status ");
    labelinfo->setText(QString::number(imgH)+"x"+QString::number(imgW)+" 16bits");
    labelinfo->setAlignment(Qt::AlignCenter);
    labelinfo->setMinimumWidth(20);
    labelStat = new QLabel(this);
    labelStat->setMinimumWidth(60);
    ui->statusBar->addWidget(labelinfo);
    ui->statusBar->addWidget(labelStat);
}

MainWindow::~MainWindow() {
    //AT_FinaliseLibrary();
    //stop time thread
    /*if(tsys->isRunning())
    {
        tsys->requestInterruption();
        tsys->quit();
        tsys->wait();
    }
    delete tsys;
    if(andorCCD->isRunning())
    {
            andorCCD->requestInterruption();
            andorCCD->quit();
            andorCCD->wait();
     }
    delete andorCCD;*/
    delete ui;
    delete server;
}



void MainWindow::on_btnLive_pressed() {
    if (!live && opened) {
        ui->actionSave->setEnabled(false);
        //ui->btnSnap->setEnabled(false);
        //ui->propertyBrowserView->setEnabled(false);
        ui->actionServer->setEnabled(false);
        server->setVisible(false);
        live=true;
        QDateTime current_date_time =QDateTime::currentDateTimeUtc();
        QString todaynow =current_date_time.toString("yyyyMMdd hh:mm:ss");
        ui->textEdit_status->append(">>>"+todaynow+":");
        mutex.lock();
        outfile.open(obsname.toStdString(), std::ios::app);
        logtmp=">>>"+todaynow+":";
        outfile<<logtmp.toStdString()<<"\n";
        ui->textEdit_status->append("Live Video Started...");
        logtmp="Live Video Started...";
        outfile<<logtmp.toStdString()<<"\n";
        if(outfile.is_open()){
            //qDebug("log file opened...");
            outfile.close();
        }
        mutex.unlock();
        ui->btnLive->setText("Stop Live");
        ui->btnSnap->setEnabled(true);
        //connect(andorCCD,&aCCD::buf_Ready,this,&MainWindow::updateGraphicsView);
        liveTimer = new QTimer(parent());
        connect(liveTimer, &QTimer::timeout, this, &MainWindow::displayOK);
        liveTimer->start(framedelay);

        //saveTimer = new QTimer(parent());
        //connect(saveTimer, &QTimer::timeout, this, &MainWindow::periodicSave);
        //saveTimer->start(groupdelay*1000);

        expTime=ui->lineEdit_exposuretime->text().toFloat();
        if(expTime<=0)
            expTime=1.2;
        ui->lineEdit_exposuretime->setText(QString::number(expTime));
        frameRate=ui->lineEdit_framerate->text().toInt();
        if(frameRate<=0 || frameRate>200)
            frameRate=frameRateMax;
        ui->lineEdit_framerate->setText(QString::number(frameRate));
        ui->lineEdit_exposuretime->setEnabled(false);
        ui->lineEdit_framerate->setEnabled(false);
        ui->lineEdit_groupdelay->setEnabled(false);

        labelinfo->setText(QString::number(imgH)+"x"+QString::number(imgW)+" 16bits");
    }
    else {
        live=false;
        //ui->textEdit_status->append("Live Stopped...");
        if(savefits)
            ui->textEdit_status->append("Waiting for Saving Data...");
        while(savefits)
        {
            QCoreApplication::processEvents();
        }
        //andorCCD->requestInterruption();
        savefits=false;
        ui->textEdit_status->append("Saving Data Stopped...");
        if(fpre=="T")
            datatype="TiO";
        else
            datatype=fpre;
        QDateTime current_date_time =QDateTime::currentDateTimeUtc();
        QString todaynow =current_date_time.toString("yyyyMMdd hh:mm:ss");
        ui->textEdit_status->append(">>>"+todaynow+":");
        logtmp=">>>"+todaynow+":";
        mutex.lock();
        outfile.open(obsname.toStdString(), std::ios::app);
        outfile<<logtmp.toStdString()<<"\n";
        ui->textEdit_status->append(datatype+" Acquisition Stopped...");
        logtmp=datatype+" Acquisition Stopped...";
        outfile<<logtmp.toStdString()<<"\n";
        ui->btnSnap->setEnabled(false);
        //liveTimer->stop();
        //delete liveTimer;
        //liveTimer = nullptr;
        ui->actionSave->setEnabled(true);
        ui->actionServer->setEnabled(true);
        ui->btnLive->setText("Start Live");
        ui->textEdit_status->append("Live Video Stopped...");
        logtmp="Live Video Stopped...";
        outfile<<logtmp.toStdString()<<"\n";
        if(outfile.is_open()){
            //qDebug("log file opened...");
            outfile.close();
        }
        mutex.unlock();
        ui->btnSnap->setText("Start Acquisition");
        ui->btnSnap->setEnabled(false);
        ui->lineEdit_exposuretime->setEnabled(true);
        ui->lineEdit_framerate->setEnabled(true);
        ui->lineEdit_saveto->setEnabled(true);
        ui->lineEdit_savelog->setEnabled(true);
        ui->lineEdit_groupdelay->setEnabled(true);

        if(ui->checkBox_Data->isChecked())
        {
            ui->lineEdit_objname->setEnabled(true);
            ui->lineEdit_cor1->setEnabled(true);
            ui->lineEdit_cor2->setEnabled(true);
            ui->lineEdit_datanum->setEnabled(true);
            ui->lineEdit_darknum->setEnabled(false);
            ui->lineEdit_flatnum->setEnabled(false);
        }
        if(ui->checkBox_Flat->isChecked())
        {
            ui->lineEdit_objname->setEnabled(false);
            ui->lineEdit_cor1->setEnabled(false);
            ui->lineEdit_cor2->setEnabled(false);
            ui->lineEdit_datanum->setEnabled(false);
            ui->lineEdit_darknum->setEnabled(false);
            ui->lineEdit_flatnum->setEnabled(true);
        }
        if(ui->checkBox_Dark->isChecked())
        {
            ui->lineEdit_objname->setEnabled(false);
            ui->lineEdit_cor1->setEnabled(false);
            ui->lineEdit_cor2->setEnabled(false);
            ui->lineEdit_datanum->setEnabled(false);
            ui->lineEdit_darknum->setEnabled(true);
            ui->lineEdit_flatnum->setEnabled(false);
        }

    }

}

void MainWindow::stopACQ(){
    ui->textEdit_status->append("Waiting for Saving Data...");
    QElapsedTimer t1;
    t1.start();
    while(t1.elapsed()<5000)
    {
        QCoreApplication::processEvents();
        if(continousACQ ){
            if(fpre=="T" && !localsave)
                break;
        }
        else
        {
            if((fserialNo % datanum)==0 && fserialNo>0)
            {
                break;
            }
        }

        //if((fpre=="FLAT" && (fserialNo % 2000)==0 && fserialNo>0) || (fpre=="dark" && (fserialNo % 1000)==0 && fserialNo>0))
        if((fpre=="FLAT" && (fserialNo % datanum)==0 && fserialNo>0) || (fpre=="dark" && (fserialNo % datanum)==0 && fserialNo>0))
        {
            break;
        }
    }
    savefits=false;
    if(fpre=="T")
        datatype="TiO";
    else
        datatype=fpre;
    QDateTime current_date_time =QDateTime::currentDateTimeUtc();
    QString todaynow =current_date_time.toString("yyyyMMdd hh:mm:ss");
    ui->textEdit_status->append(">>>"+todaynow+":");
    logtmp=">>>"+todaynow+":";
    mutex.lock();
    outfile.open(obsname.toStdString(), std::ios::app);
    outfile<<logtmp.toStdString()<<"\n";
    ui->textEdit_status->append(datatype+" Stop Singal Received...");
    logtmp=datatype+" Stop Singal Received...";
    outfile<<logtmp.toStdString()<<"\n";
    if(datatype=="FLAT")
        ui->textEdit_status->append(datatype+" Count "+QString::number(flatcnt)+" Acquisition Stopped...");
    else
        ui->textEdit_status->append(datatype+" Acquisition Stopped...");
    if(datatype=="FLAT")
        logtmp=datatype+" Count "+QString::number(flatcnt)+" Acquisition Stopped...";
    else
        logtmp=datatype+" Acquisition Stopped...";
    outfile<<logtmp.toStdString()<<"\n";
    QString numf = QString("%1").arg(fserialNo, 8, 10, QLatin1Char('0'));
    if(datatype=="FLAT")
        logtmp=datatype+" Count "+QString::number(flatcnt)+" accquired "+numf+" frames...";
    else
        logtmp=datatype+" acquired "+numf+" frames...";
    ui->textEdit_status->append(logtmp);
    outfile<<logtmp.toStdString()<<"\n";
    if(diskfull)
    {
        logtmp="Acquisition Disk "+saveTo+" is full...";
        outfile<<logtmp.toStdString()<<"\n";
        ui->textEdit_status->append(logtmp);
    }
    //outfile<<ccdM.toStdString()<<"\n";
    if(outfile.is_open()){
        //qDebug("log file opened...");
        outfile.close();
    }
    mutex.unlock();
    ui->btnSnap->setEnabled(true);
    ui->btnSnap->setText("Start Acquistion");
    fserialNo=0;
    serialNo=0;
    if(fpre=="FLAT")
        flatcnt=flatcnt+1;
    ui->lineEdit_saveto->setEnabled(true);
    ui->lineEdit_savelog->setEnabled(true);
    if(ui->checkBox_Data->isChecked())
    {
        ui->lineEdit_objname->setEnabled(true);
        ui->lineEdit_cor1->setEnabled(true);
        ui->lineEdit_cor2->setEnabled(true);
        ui->lineEdit_datanum->setEnabled(true);
        ui->lineEdit_darknum->setEnabled(false);
        ui->lineEdit_flatnum->setEnabled(false);
    }
    if(ui->checkBox_Flat->isChecked())
    {
        ui->lineEdit_objname->setEnabled(false);
        ui->lineEdit_cor1->setEnabled(false);
        ui->lineEdit_cor2->setEnabled(false);
        ui->lineEdit_datanum->setEnabled(false);
        ui->lineEdit_darknum->setEnabled(false);
        ui->lineEdit_flatnum->setEnabled(true);
    }
    if(ui->checkBox_Dark->isChecked())
    {
        ui->lineEdit_objname->setEnabled(false);
        ui->lineEdit_cor1->setEnabled(false);
        ui->lineEdit_cor2->setEnabled(false);
        ui->lineEdit_datanum->setEnabled(false);
        ui->lineEdit_darknum->setEnabled(true);
        ui->lineEdit_flatnum->setEnabled(false);
    }
}

void MainWindow::on_btnSnap_pressed() {
    //if(!savefits && live && opened)
    if(!savefits  && live && opened && !diskfull )
    {
        localfirst=true;
        //localsave=true;
        savefits=true;


        QDateTime current_date_time =QDateTime::currentDateTimeUtc();
        current_date_d =current_date_time.toString("yyyyMMdd");
        current_date_t1 =current_date_time.toString("hhmmss");

        saveTo=ui->lineEdit_saveto->text();
        //dir example
        //e:\20211008\TIO\dark\062905\062905
        //e:\20211008\TIO\FLAT00
        //e:\20211008\TIO\12882\071655\071655
        if(ui->checkBox_Dark->isChecked()){
            fpre="dark";
            //savepred=saveTo+current_date_d+"\\TIO\\dark\\"+current_date_t+"\\"+current_date_t;
            datanum=ui->lineEdit_darknum->text().toUInt();
            if((datanum % 1000) != 0 )
                datanum=1000;
        }
        if(ui->checkBox_Flat->isChecked()){
            fpre="FLAT";
            //savepref=saveTo+current_date_d+"\\TIO\\FLAT";
            flatcnt=0;
            datanum=ui->lineEdit_flatnum->text().toUInt();
            if((datanum % 2000) != 0 )
                datanum=2000;
        }
        if(ui->checkBox_Data->isChecked()){
            fpre="T";
            //savepre=saveTo+current_date_d+"\\TIO\\"+ui->lineEdit_objname->text()+"\\"+current_date_t;
            datanum=ui->lineEdit_datanum->text().toUInt();
            if(datanum<=0)
                continousACQ=true;
            //if(datanum<0)
                //datanum=200;
        }
        //QDir tmpdir;
        savepred=saveTo+current_date_d+"\\TIO\\dark";
        savepref=saveTo+current_date_d+"\\TIO\\FLAT";
        //savepre=saveTo+current_date_d+"\\TIO\\"+ui->lineEdit_objname->text()+"\\"+current_date_t1;
        savepre=saveTo+current_date_d+"\\TIO\\"+ui->lineEdit_objname->text();
        if(fpre=="T")
        {
            saveDir=savepre;
            localsave=true;
            localfirst=true;
            //fcnt = current_date_t1;
            //if(!tmpdir.exists(saveDir))
                //tmpdir.mkpath(saveDir);
        }
        if(fpre=="FLAT")
                saveDir=savepref;
        if(fpre=="dark")
                saveDir=savepred;
        qDebug()<<"snap pressed:"<<savepred<<" "<<savepref<<" "<<savepre;
        qDebug()<<"Saving Dir:"<<saveDir;


        expTime=ui->lineEdit_exposuretime->text().toFloat();
        if(expTime<=0)
            expTime=1.2;
        ui->lineEdit_exposuretime->setText(QString::number(expTime));

        frameRate=ui->lineEdit_framerate->text().toInt();
        if(frameRate<=0 || frameRate>200)
            frameRate=frameRateMax;
        ui->lineEdit_framerate->setText(QString::number(frameRate));

        groupdelay=ui->lineEdit_groupdelay->text().toInt();
        if(groupdelay<20 || groupdelay>60)
            groupdelay=30;
        ui->lineEdit_groupdelay->setText(QString::number(groupdelay));

        numBuffer=10;
        if(fpre=="T")
            datatype="TiO";
        else
            datatype=fpre;
        current_date_time =QDateTime::currentDateTimeUtc();
        QString todaynow =current_date_time.toString("yyyyMMdd hh:mm:ss");
        ui->textEdit_status->append(">>>"+todaynow+":");
        logtmp=">>>"+todaynow+":";
        outfile.open(obsname.toStdString(), std::ios::app);
        mutex.lock();       
        outfile<<logtmp.toStdString()<<"\n";
        ui->textEdit_status->append(datatype+" Acquisition Started...");
        logtmp=datatype+" Acquisition Started...";
        outfile<<logtmp.toStdString()<<"\n";
        if(outfile.is_open()){
            //qDebug("log file opened...");
            outfile.close();
        }
        mutex.unlock();
        ui->btnSnap->setText("Stop Acquisition");
        ui->btnSnap->setEnabled(true);
        objname=ui->lineEdit_objname->text();
        if(QString(objname).isEmpty())
            objname="Sun";
        obscor1=ui->lineEdit_cor1->text();
        if(QString(obscor1).isEmpty())
            obscor1="N00";
        obscor2=ui->lineEdit_cor2->text();
        if(QString(obscor2).isEmpty())
            obscor2="E00";

        //current_date_time =QDateTime::currentDateTimeUtc();
        //current_date_d =current_date_time.toString("yyyyMMdd");
        //current_date_t1 =current_date_time.toString("hhmmss");
        ui->lineEdit_exposuretime->setEnabled(false);
        ui->lineEdit_framerate->setEnabled(false);
        ui->lineEdit_objname->setEnabled(false);
        ui->lineEdit_cor1->setEnabled(false);
        ui->lineEdit_cor2->setEnabled(false);
        ui->lineEdit_darknum->setEnabled(false);
        ui->lineEdit_flatnum->setEnabled(false);
        ui->lineEdit_datanum->setEnabled(false);
        ui->lineEdit_saveto->setEnabled(false);
        ui->lineEdit_savelog->setEnabled(false);
        ui->lineEdit_groupdelay->setEnabled(false);
    }else
    {
        ui->btnSnap->setText("Start Acquisition");
        ui->btnSnap->setEnabled(true);
        //localfirst=false;

        ui->textEdit_status->append("Waiting for Saving Data...");
        //QElapsedTimer t1;
        //t1.start();
        while(savefits)
        {
            QCoreApplication::processEvents();
            qDebug("Waiting...");
            //if(fpre=="dark" && (fserialNo % 999)==0)
            if(fpre=="dark" && (fserialNo % (datanum-1))==0)
                break;
            if(fpre=="FLAT" && (fserialNo % (datanum-1))==0)
                break;
            if(fpre=="T" && serialNo == 0 )
                break;
        }
        ui->textEdit_status->append("Saving Data Finished...");
        savefits=false;
        localsave=false;
        localfirst=false;

        if(fpre=="T")
            datatype="TiO";
        else
            datatype=fpre;
        mutex.lock();
        outfile.open(obsname.toStdString(), std::ios::app);
        QDateTime current_date_time =QDateTime::currentDateTimeUtc();
        QString todaynow =current_date_time.toString("yyyyMMdd hh:mm:ss");
        ui->textEdit_status->append(">>>"+todaynow+":");
        logtmp=">>>"+todaynow+":";
        outfile<<logtmp.toStdString()<<"\n";
        //outfile<<ccdM.toStdString()<<"\n";
        ui->textEdit_status->append(datatype+" Acquisition Stopped...");
        logtmp=datatype+" Acquisition Stopped...";
        outfile<<logtmp.toStdString()<<"\n";
        QString numf = QString("%1").arg(fserialNo, 8, 10, QLatin1Char('0'));
        logtmp=datatype+" acquired "+numf+" frame(s)...";
        ui->textEdit_status->append(logtmp);
        if(diskfull)
        {
            ui->textEdit_status->append(saveTo+" is full...");
            logtmp=saveTo+" is full...";
            outfile<<logtmp.toStdString()<<"\n";
        }
        outfile<<logtmp.toStdString()<<"\n";
        if(outfile.is_open()){
            //qDebug("log file opened...");
            outfile.close();
        }
        mutex.unlock();
        /*QElapsedTimer t;
        t.start();
        while(t.elapsed()<5000)
            QCoreApplication::processEvents();*/
        sum_fserialNo=sum_fserialNo+fserialNo;
        fserialNo=0;
        serialNo=0;
        if(fpre=="FLAT")
            flatcnt=flatcnt+1;
        //ui->lineEdit_objname->setEnabled(true);
        //ui->lineEdit_cor1->setEnabled(true);
        //ui->lineEdit_cor2->setEnabled(true);
        //saveTimer->stop();
        //disconnect(saveTimer, &QTimer::timeout, this, &MainWindow::periodicSave);
        //saveTimer->deleteLater();

        //delete saveTimer;
        ui->lineEdit_saveto->setEnabled(true);
        ui->lineEdit_savelog->setEnabled(true);
        if(ui->checkBox_Data->isChecked())
        {
            ui->lineEdit_objname->setEnabled(true);
            ui->lineEdit_cor1->setEnabled(true);
            ui->lineEdit_cor2->setEnabled(true);
            ui->lineEdit_datanum->setEnabled(true);
            ui->lineEdit_darknum->setEnabled(false);
            ui->lineEdit_flatnum->setEnabled(false);
        }
        if(ui->checkBox_Flat->isChecked())
        {
            ui->lineEdit_objname->setEnabled(false);
            ui->lineEdit_cor1->setEnabled(false);
            ui->lineEdit_cor2->setEnabled(false);
            ui->lineEdit_datanum->setEnabled(false);
            ui->lineEdit_darknum->setEnabled(false);
            ui->lineEdit_flatnum->setEnabled(true);
        }
        if(ui->checkBox_Dark->isChecked())
        {
            ui->lineEdit_objname->setEnabled(false);
            ui->lineEdit_cor1->setEnabled(false);
            ui->lineEdit_cor2->setEnabled(false);
            ui->lineEdit_datanum->setEnabled(false);
            ui->lineEdit_darknum->setEnabled(true);
            ui->lineEdit_flatnum->setEnabled(false);
        }
    }
}

void MainWindow::drawHist(QVector<uint>vechistdata,uint histmax,uint histidx)
{

    if(live)
    {
        histdisplock.lock();
        histshow_locked=true;
        //qDebug("here inside drawHist...");
        //ui->textEdit_status->append("inside hist show");
        chart->removeSeries(series);
        delete lineseries;
        delete series;
        lineseries=new QLineSeries();
        //series->clear();
        for (int i=0;i<vechistdata.size();++i) {
           lineseries->append(QPointF(i,vechistdata[i]));
        }
        lineseries->setColor(Qt::black);

        series = new QAreaSeries(lineseries);
        series->setName("Histogram");
        QPen pen(Qt::black);
        pen.setWidth(1);
        series->setPen(pen);
        QBrush brush;
        brush.setColor(Qt::black);//画刷颜色
        brush.setStyle(Qt::SolidPattern);//画刷填充样式
        series->setBrush(brush);

        chart->removeAxis(axisX);
        delete axisX;
        axisX = new QCategoryAxis();
        axisX->setMin(0);
        axisX->setMax(65535);
        axisX->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
        axisX->append(QString::number(0),0);
        QString tmpstring = QString::number(histidx);
        axisX->append("<font color=\"red\">"+tmpstring+"</font>", histidx);
        axisX->append(QString::number(65535),65535);

        chart->removeAxis(axisY);
        delete axisY;
        axisY = new QValueAxis();
        axisY->setTickCount(3);
        axisY->setLabelFormat("%7d");
        axisY->setRange(0,histmax);

        chart->addSeries(series);
        chart->addAxis(axisX,Qt::AlignBottom);
        chart->addAxis(axisY,Qt::AlignLeft);

        ui->chart_graphicsView->setVisible(true);
        ui->chart_graphicsView->update();

        histdisplock.unlock();

        /*QElapsedTimer t1;
        t1.start();
        while(t1.elapsed()<50)
        {
            QCoreApplication::processEvents();
        }*/
        histshow_locked=false;
    }
}

void MainWindow::displayOK(){
        display=true;
        if(savefits)
                {
                    ccdM="";
                    QString numf = QString("%1").arg(fserialNo, 8, 10, QLatin1Char('0'));
                    if(fpre=="T")
                        datatype="TiO";
                    else
                        datatype=fpre;
                    ccdM=datatype+" Acquired "+numf+" Frame(s)...";
                    ui->textEdit_status->setFocus();
                    QTextCursor storeCursorPos = ui->textEdit_status->textCursor();
                    ui->textEdit_status->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
                    ui->textEdit_status->moveCursor(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
                    ui->textEdit_status->moveCursor(QTextCursor::End, QTextCursor::KeepAnchor);
                    ui->textEdit_status->textCursor().removeSelectedText();
                    ui->textEdit_status->textCursor().deletePreviousChar();
                    ui->textEdit_status->setTextCursor(storeCursorPos);
                    ui->textEdit_status->append(ccdM);
                    labelStat->setText(datatype+" : Saving "+QString("%1").arg(fserialNo, 8, 10, QLatin1Char('0'))+" Frame(s)");
                }else{
                    labelStat->setText(" Stopped - "+QString("%1").arg(sum_fserialNo, 8, 10, QLatin1Char('0'))+" Frame(s) Saved");
                }

}

/*void MainWindow::periodicSave(uint dt)
{
    uint wait_time_ms=(groupdelay-dt)*1000;
    QElapsedTimer t;
    t.start();
    while(!t.hasExpired(wait_time_ms))
    {
        QCoreApplication::processEvents();
    }
    wait_Acq=false;
}*/

void MainWindow::updateGraphicsView(unsigned short* buf,uint buflen) {
    if(live)
    {
        QMutex displock;
        displock.lock();
        display_locked=true;
        //if(live && NULL != buf && display )

        //QImage *qimage;
        currentImage = cv::Mat(static_cast<int>(imgH), static_cast<int>(imgW), CV_16UC1, buf);
        double minVal;
        double maxVal;
        cv::Point minLoc;
        cv::Point maxLoc;
        minMaxLoc(currentImage,&minVal,&maxVal,&minLoc, &maxLoc);
        imgMax=maxVal;
        //}

        labelCoordMV->setText(QString::number(imgMax));
        //cv::Mat lossyImage=cv::Mat::zeros(imgW,imgH,CV_8U);
        cv::Mat lossyImage;
        //if(currentImage.data)
        cv::normalize(currentImage, lossyImage, 0, 255, cv::NORM_MINMAX, CV_8U);
        //else
        //{
            //histcalc_lock.unlock();
            //return;
        //}
        if(drawing && live)
            rectangle(lossyImage, cv::Point(static_cast<int>(sx), static_cast<int>(sy)), cv::Point(static_cast<int>(ex),static_cast<int>(ey)), RGB(0,0,0), 4,8,0);
        //rectangle(lossyImage, cv::Point(static_cast<int>(sx), static_cast<int>(sy)), cv::Point(static_cast<int>(ex),static_cast<int>(ey)), cv::Scalar(255, 0, 255), 4, 8, 0);
        //QImage qimage = QImage(static_cast<unsigned char *>(lossyImage.data), lossyImage.cols, lossyImage.rows, static_cast<int>(lossyImage.step), QImage::Format_Grayscale8);
        //qimage= new QImage(static_cast<unsigned char *>(lossyImage.data), lossyImage.cols, lossyImage.rows, static_cast<int>(lossyImage.step), QImage::Format_Indexed8);

        scene->clear();
        ui->graphicsView->viewport()->update();
        //item = new QGraphicsPixmapItem(QPixmap::fromImage(*qimage));
        item = new QGraphicsPixmapItem(QPixmap::fromImage(QImage(static_cast<unsigned char *>(lossyImage.data), lossyImage.cols, lossyImage.rows, static_cast<int>(lossyImage.step), QImage::Format_Indexed8)));
        scene->addItem(item);
        //scene->setSceneRect(QRectF(0, 0, scene->height(), scene->width()));
        //scene->setSceneRect(QRectF(0, 0, imgH, imgW));
        scene->setSceneRect(QRectF(0, 0, lossyImage.cols, lossyImage.rows));
        ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
        //ui->graphicsView->fitInView(scene->sceneRect(),Qt::IgnoreAspectRatio);
        ui->graphicsView->viewport()->update();
        //ui->graphicsView->;
        currentImage.release();
        //delete qimage;

        /*if(savefits)
        {
            ccdM="";
            QString numf = QString("%1").arg(fserialNo, 8, 10, QLatin1Char('0'));
            if(fpre=="T")
                datatype="TiO";
            else
                datatype=fpre;
            ccdM=datatype+" Acquired "+numf+" Frame(s)...";
            ui->textEdit_status->setFocus();
            QTextCursor storeCursorPos = ui->textEdit_status->textCursor();
            ui->textEdit_status->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
            ui->textEdit_status->moveCursor(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
            ui->textEdit_status->moveCursor(QTextCursor::End, QTextCursor::KeepAnchor);
            ui->textEdit_status->textCursor().removeSelectedText();
            ui->textEdit_status->textCursor().deletePreviousChar();
            ui->textEdit_status->setTextCursor(storeCursorPos);
            ui->textEdit_status->append(ccdM);
            labelStat->setText(" "+QString("%1").arg(fserialNo, 8, 10, QLatin1Char('0'))+" Frame(s)");
        }else{
            labelStat->setText(" Stopped - "+QString("%1").arg(sum_fserialNo, 8, 10, QLatin1Char('0'))+" Frame(s) Saved");
        }*/
        fps1=fps1+1;
        displock.unlock();
        display_locked=false;
        display=false;
    }



}

void MainWindow::on_actionServer_triggered() {
    server->show();
    server->activateWindow();
}

void MainWindow::handle_serverStateChanged(bool isRunning) {
    if (isRunning) {
        ui->btnLive->setEnabled(false);
        ui->btnSnap->setEnabled(false);
        //ui->propertyBrowserView->setEnabled(false);
    } else {
        ui->btnLive->setEnabled(true);
        ui->btnSnap->setEnabled(true);
        //ui->propertyBrowserView->setEnabled(true);
    }
}

void MainWindow::processRemoteCall(Request req) {
    std::cout << "processRemoteCall" << std::endl;
    std::cout << "Id: " << req.id.toStdString() << std::endl;
    std::cout << "Method: " << req.method.toStdString() << std::endl;
    for (QString &p: req.parameters)
        std::cout << "Parameter: " << p.toStdString() << std::endl;
    if (req.method == "snap") {
        snap(req.parameters[0], req.parameters[1]);
    }
    server->ack(0);
}

void MainWindow::snap(QString filename, QString metadata) {
    AT_64 imageSizeBytes;
    AT_GetInt(handle, L"ImageSizeBytes", &imageSizeBytes);
    int pixelEncodingIndex;
    AT_GetEnumIndex(handle, L"PixelEncoding", &pixelEncodingIndex);
    AT_WC pixelEncoding[255] = {0};
    AT_GetEnumStringByIndex(handle, L"PixelEncoding", pixelEncodingIndex, pixelEncoding, 255);
    AT_64 imageHeight;
    AT_GetInt(handle, L"AOIHeight", &imageHeight);
    AT_64 imageWidth;
    AT_GetInt(handle, L"AOIWidth", &imageWidth);
    AT_64 imageStride;
    AT_GetInt(handle, L"AOIStride", &imageStride);

    int bufferSize = static_cast<int>(imageSizeBytes);
    delete [] buffer;
    buffer = new unsigned char[bufferSize];
    AT_QueueBuffer(handle, buffer, bufferSize);
    AT_Command(handle, L"AcquisitionStart");
    AT_WaitBuffer(handle, &buffer, &bufferSize, AT_INFINITE);
    AT_Command(handle, L"AcquisitionStop");
    AT_Flush(handle);

    AT_InitialiseUtilityLibrary();
    delete [] unpackedBuffer;
    unpackedBuffer = new unsigned short[imageWidth * imageHeight];
    AT_ConvertBuffer(buffer, reinterpret_cast<unsigned char *>(unpackedBuffer), imageWidth, imageHeight, imageStride, pixelEncoding, L"Mono16");
    AT_FinaliseUtilityLibrary();

    if (!metadata.isEmpty()) {
        QFile metadataFile(QDir(server->saveDir).filePath(filename + ".txt"));
        metadataFile.open(QFile::WriteOnly);
        metadataFile.write(metadata.toStdString().c_str());
    }
    currentImage = cv::Mat(static_cast<int>(imageWidth), static_cast<int>(imageHeight), CV_16UC1, unpackedBuffer);
    ui->actionSave->setEnabled(true);
    //snap to local
    cv::imwrite(cv::String(QDir(server->saveDir).filePath(filename + ".tif").toStdString()), currentImage);
    //snap to remote
    cv::imwrite(cv::String(QDir(ui->lineEdit_saveto->text()).filePath(filename + ".tif").toStdString()), currentImage);
    cv::Mat lossyImage;
    cv::normalize(currentImage, lossyImage, 0., 255, cv::NORM_MINMAX, CV_8U);
    QImage qimage = QImage(static_cast<unsigned char *>(lossyImage.data), lossyImage.cols, lossyImage.rows, static_cast<int>(lossyImage.step), QImage::Format_Grayscale8);
    scene->clear();
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(qimage));
    scene->addItem(item);
    scene->setSceneRect(QRectF(0, 0, lossyImage.cols, lossyImage.rows));
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    ui->graphicsView->update();
}

void MainWindow::on_actionSave_triggered() {
    QFileDialog fd(this);
    QString fn = fd.getSaveFileName(this, "Save file", "andor.tif", "16-bit original image (*.tif);;8-bit lossy image (*.bmp)");
    std::cout << fn.toStdString() << std::endl;
    if (fn.endsWith(".tif")) {
        cv::imwrite(cv::String(fn.toStdString()), currentImage);
    } else if (fn.endsWith(".bmp")) {
        cv::Mat lossyImage;
        cv::normalize(currentImage, lossyImage, 0., 255, cv::NORM_MINMAX, CV_8U);
        cv::imwrite(cv::String(fn.toStdString()), lossyImage);
    }
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionAbout_triggered() {
    about->show();
}

void MainWindow::on_actionDetect_Cameras_triggered() {
    AT_64 deviceCount;
    realDev=0;
    AT_FinaliseLibrary();
    AT_InitialiseLibrary();
    AT_GetInt(AT_HANDLE_SYSTEM, L"DeviceCount", &deviceCount);
    qDebug("device count: %lld", deviceCount-2);
    QDateTime current_date_time =QDateTime::currentDateTimeUtc();
    QString todaynow =current_date_time.toString("yyyyMMdd hh:mm:ss");
    ui->textEdit_status->append(">>>"+todaynow+":");
    realDev=deviceCount-2;
    if(realDev>0)
        ui->textEdit_status->append("Found "+QString::number(realDev)+" Camera");
    else
        ui->textEdit_status->append("No Camera Found...");


    //AT_Close(handle);
    AT_FinaliseLibrary();
}

void MainWindow::on_checkBox_Data_clicked()
{
    ui->lineEdit_objname->setEnabled(true);
    ui->lineEdit_cor1->setEnabled(true);
    ui->lineEdit_cor2->setEnabled(true);
    ui->lineEdit_datanum->setEnabled(true);
    ui->lineEdit_darknum->setEnabled(false);
    ui->lineEdit_flatnum->setEnabled(false);
}

void MainWindow::on_checkBox_Dark_clicked()
{
    ui->lineEdit_objname->setEnabled(false);
    ui->lineEdit_cor1->setEnabled(false);
    ui->lineEdit_cor2->setEnabled(false);
    ui->lineEdit_datanum->setEnabled(false);
    ui->lineEdit_darknum->setEnabled(true);
    ui->lineEdit_flatnum->setEnabled(false);
}

void MainWindow::on_checkBox_Flat_clicked()
{
    ui->lineEdit_objname->setEnabled(false);
    ui->lineEdit_cor1->setEnabled(false);
    ui->lineEdit_cor2->setEnabled(false);
    ui->lineEdit_datanum->setEnabled(false);
    ui->lineEdit_darknum->setEnabled(false);
    ui->lineEdit_flatnum->setEnabled(true);
}
