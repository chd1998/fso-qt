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

#define GB (1024*1024*1024)

double expTime=0.5;
int frameRate=200;
int framedelay=50;
int frameRateMax=200;
QString saveTo="e:\\",savepre="",savepred="",savepref="",ccdM,saveDir="",savepreobj,savepredf,save01="";
QString current_date_d;
QString current_date_t,current_date_t1,current_date_t2;
bool savefits=false,opened=false,live=false,display=false;
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
int serialNo=0,fserialNo=0;
bool firsttime=true,localsave=true,localfirst=true;;
QString objname="Sun";
QString obscor1="N00",obscor2="E00";
QString datatype="TiO";
int flatcnt=0;
QFile obslog;
QString obslogfile,obslogdir,obslogdate,logtmp,obsname;
std::ofstream outfile;
QMutex mutex;
quint16 imgMax=0,freedisk;
double sx=200,sy=200,ex=800,ey=800;
bool drawing=false;
int flatnum=2000,darknum=1000;
uint datanum=200;
bool continousACQ=false,fulldisk=false;
QDir sdir;

//QTextStream obsout;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);

    /*savefits=false;
    temperature=0.0;
    opened=false;
    live=false;
    framedelay=40;
    display=false;
    firsttime=true;
    //static std::ofstream outfile;
    //static QMutex mutex;*/

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
    setWindowTitle("NVST-HighSpeed-TiO-20220120-001 (Qt6+minGW64+OpenCV) --- by Chen Dong @fso");
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
            logtmp="Camera 0：ready to work...";
            outfile<<logtmp.toStdString()<<"\n";
            mutex.unlock();
            ui->textEdit_status->append(logtmp);
            connect(andorCCD,&aCCD::buf_Ready,this,&MainWindow::updateGraphicsView);
            connect(andorCCD,&aCCD::stop_Acq,this,&MainWindow::stopACQ);
            //ui->textEdit_status->append("Connected to Camera "+QString::number(ui->spinBoxCameraIndex->value()));
            ccdID=0;
            opened=true;
            expTime=0.5;
            ui->lineEdit_exposuretime->setText(QString::number(expTime));
            frameRate=frameRateMax;
            ui->lineEdit_framerate->setText(QString::number(frameRate));
            framedelay=33;
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
        }
    }else
    {
        ui->btnLive->setEnabled(false);
        ui->btnSnap->setEnabled(false);
        ui->actionServer->setEnabled(false);
        logtmp="No Camera found...";
        ui->textEdit_status->append(logtmp);
        mutex.lock();
        //outfile.open(obsname.toStdString(), std::ios::app);
        outfile<<logtmp.toStdString()<<"\n";
        mutex.unlock();
    }
    //connect(andorCCD,&aCCD::buf_Ready,this,&MainWindow::updateGraphicsView);
    scene = new GraphicsScene();
    ui->graphicsView->setScene(scene);
    ui->textEdit_status->setReadOnly(true);
    ui->graphicsView->setMouseTracking(true);
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
            fulldisk=true;
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
    QDateTime current_date_time =QDateTime::currentDateTime();
    if(opened)
    {
        AT_GetFloat(handle, L"SensorTemperature", &temperature);
    }
    long t1 = current_date_time.toSecsSinceEpoch();
    float dt = (float)(t1-t0)/3600.0;
    QString angle_sign=u8"°C";
    QString current_date =current_date_time.toString("yyyy-MM-dd hh:mm:ss")+" "+QString::number(dt,'f',4)+"hrs Temp: "+QString::number(temperature)+angle_sign;
    ui->label_time->setText(current_date);
    diskname=ui->lineEdit_saveto->text();
    quint64 freeDiskSpace = getDiskSpace(diskname, false);
    quint64 totalDiskSpace = getDiskSpace(diskname, true);
    QString diskfree="",full="",empty="";
    if(freedisk>1000)
    {
        empty="<font style='color:green;'>"+QString::number(freeDiskSpace)+"</font>";
        diskfree=diskname+" ---> "+empty+"/"+QString::number(totalDiskSpace)+" GB (Free/Total)";
    }
    else
    {
        full="<font style='color:red;'>"+QString::number(freeDiskSpace)+"</font>";
        diskfree=diskname+" ---> "+full+"/"+QString::number(totalDiskSpace)+" GB (Free/Total)";
    }
    //QString diskfree=diskname+" ---> "+QString::number(freeDiskSpace)+"/"+QString::number(totalDiskSpace)+" GB (Free/Total)";
    ui->label_freedisk->setText(diskfree);
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
    if(savefits)
    {
        labelStat->setText(" "+QString::number(fserialNo)+" Frame(s)");
    }else{
        labelStat->setText(" Stopped - "+QString("%1").arg(fserialNo, 6, 10, QLatin1Char('0'))+" Frame(s) Saved");
    }
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

    label = new QLabel(this);
    label->setText(" Saving ");
    label->setAlignment(Qt::AlignCenter);
    label->setMinimumWidth(20);
    labelStat = new QLabel(this);
    labelStat->setMinimumWidth(60);
    ui->statusBar->addWidget(label);
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
        expTime=ui->lineEdit_exposuretime->text().toFloat();
        if(expTime<=0)
            expTime=0.5;
        ui->lineEdit_exposuretime->setText(QString::number(expTime));
        frameRate=ui->lineEdit_framerate->text().toInt();
        if(frameRate<=0 || frameRate>200)
            frameRate=frameRateMax;
        ui->lineEdit_framerate->setText(QString::number(frameRate));
        /*current_date_time =QDateTime::currentDateTimeUtc();
        current_date_d =current_date_time.toString("yyyyMMdd");

        save01=saveTo+current_date_d+"\\TIO\\";

        savepreobj=save01+objname;

        savepredf=save01+fpre;

        saveDir=save01;*/
        //qDebug()<<"live pressed:"<<saveDir;
        //savepre =current_date_time.toString("hhmmss");
        //connect(andorCCD,&aCCD::stop_Acq,this,&MainWindow::stopACQ);

        //liveTimer->start(framedelay+10);
    }
    else {
        live=false;

        //andorCCD->requestInterruption();
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
    }

}

void MainWindow::displayOK(){
    display=true;
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
        //if((fpre=="FLAT" && (fserialNo % datanum)==0 && fserialNo>0) || (fpre=="dark" && (fserialNo % datanum)==0 && fserialNo>0))
        //    break;
        //if(fpre=="FLAT" && fserialNo > 2000)
            //break;
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
    ui->textEdit_status->append(datatype+" Acquisition Stopped...");
    logtmp=datatype+" Acquisition Stopped...";
    outfile<<logtmp.toStdString()<<"\n";
    QString numf = QString("%1").arg(fserialNo, 8, 10, QLatin1Char('0'));
    logtmp=datatype+" accquired "+numf+" frames...";
    ui->textEdit_status->append(logtmp);
    outfile<<logtmp.toStdString()<<"\n";
    if(fulldisk)
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
}

void MainWindow::on_btnSnap_pressed() {
    //if(!savefits && live && opened)
    if(!savefits  && live && opened && !fulldisk)
    {
        localfirst=true;
        QDateTime current_date_time =QDateTime::currentDateTimeUtc();
        current_date_d =current_date_time.toString("yyyyMMdd");
        current_date_t1 =current_date_time.toString("hhmmss");
        savefits=true;
        saveTo=ui->lineEdit_saveto->text();

        //dir example
        //e:\20211008\TIO\dark\062905\062905
        //e:\20211008\TIO\FLAT00
        //e:\20211008\TIO\12882\071655\071655
        if(ui->checkBox_Dark->isChecked()){
            fpre="dark";
            //savepred=saveTo+current_date_d+"\\TIO\\dark\\"+current_date_t+"\\"+current_date_t;
            datanum=ui->lineEdit_darknum->text().toUInt();
            if(datanum < 1000)
                datanum=1000;
        }
        if(ui->checkBox_Flat->isChecked()){
            fpre="FLAT";
            //savepref=saveTo+current_date_d+"\\TIO\\FLAT";
            datanum=ui->lineEdit_flatnum->text().toUInt();
            if(datanum < 2000)
                datanum=2000;
        }
        if(ui->checkBox_Data->isChecked()){
            fpre="T";
            //savepre=saveTo+current_date_d+"\\TIO\\"+ui->lineEdit_objname->text()+"\\"+current_date_t;
            datanum=ui->lineEdit_datanum->text().toUInt();
            if(datanum<=0)
                continousACQ=true;
            //if(datanum>200)
            //    datanum=200;
        }
        savepred=saveTo+current_date_d+"\\TIO\\dark\\"+current_date_t1+"\\"+current_date_t1;
        savepref=saveTo+current_date_d+"\\TIO\\FLAT";
        savepre=saveTo+current_date_d+"\\TIO\\"+ui->lineEdit_objname->text()+"\\"+current_date_t1;
        if(fpre=="T")
                saveDir=savepre;
        if(fpre=="FLAT")
                saveDir=savepref;
        if(fpre=="dark")
                saveDir=savepred;
        qDebug()<<"snap pressed:"<<savepred<<" "<<savepref<<" "<<savepre;
        qDebug()<<"Saving Dir:"<<saveDir;

        expTime=ui->lineEdit_exposuretime->text().toFloat();
        if(expTime<=0)
            expTime=0.5;
        ui->lineEdit_exposuretime->setText(QString::number(expTime));
        frameRate=ui->lineEdit_framerate->text().toInt();

        if(frameRate<=0 || frameRate>200)
            frameRate=frameRateMax;
        ui->lineEdit_framerate->setText(QString::number(frameRate));

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
        logtmp=datatype+" Acquisition Stopped...";
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

    }else
    {
        ui->btnSnap->setText("Start Acquisition");
        localfirst=true;
        ui->textEdit_status->append("Waiting for Saving Data in 2 secs. ...");
        QElapsedTimer t1;
        t1.start();
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
        savefits=false;
        ui->btnSnap->setEnabled(true);
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
        logtmp=datatype+" accquired "+numf+" frames...";
        ui->textEdit_status->append(logtmp);
        if(fulldisk)
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
        fserialNo=0;
        serialNo=0;
        if(fpre=="FLAT")
            flatcnt=flatcnt+1;
    }
}


void MainWindow::updateGraphicsView(unsigned short* buf,uint buflen) {
    if(live && display)
    {
        //currentImage.release();
        currentImage = cv::Mat(static_cast<int>(imgH), static_cast<int>(imgW), CV_16UC1, buf);
        QString dispM;
        //double minVal;
        //double maxVal;
        //cv::Point minLoc;
        //cv::Point maxLoc;
        //minMaxLoc(currentImage,&minVal,&maxVal,&minLoc, &maxLoc);

        imgMax=*std::max_element(buf,buf+buflen);
        labelCoordMV->setText(QString::number(imgMax));
        cv::Mat lossyImage;
        cv::normalize(currentImage, lossyImage, 0, 255, cv::NORM_MINMAX, CV_8U);
        if(!lossyImage.data)
        {
            display=false;
            return;
        }
        if(drawing && live)
            rectangle(lossyImage, cv::Point(static_cast<int>(sx), static_cast<int>(sy)), cv::Point(static_cast<int>(ex),static_cast<int>(ey)), RGB(0,0,0), 4,8,0);
        //rectangle(lossyImage, cv::Point(static_cast<int>(sx), static_cast<int>(sy)), cv::Point(static_cast<int>(ex),static_cast<int>(ey)), cv::Scalar(255, 0, 255), 4, 8, 0);
        //QImage qimage = QImage(static_cast<unsigned char *>(lossyImage.data), lossyImage.cols, lossyImage.rows, static_cast<int>(lossyImage.step), QImage::Format_Grayscale8);
        QImage* qimage= new QImage(static_cast<unsigned char *>(lossyImage.data), lossyImage.cols, lossyImage.rows, static_cast<int>(lossyImage.step), QImage::Format_Grayscale8);
        scene->clear();
        ui->graphicsView->update();
        item = new QGraphicsPixmapItem(QPixmap::fromImage(*qimage));
        scene->addItem(item);
        scene->setSceneRect(QRectF(0, 0, imgH, imgW));
        //scene->setSceneRect(QRectF(0, 0, lossyImage.cols, lossyImage.rows));
        ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
        ui->graphicsView->update();
        currentImage.release();
        lossyImage.release();
        delete qimage;
        if(savefits)
        {
            dispM="";
            QString numf = QString("%1").arg(fserialNo, 8, 10, QLatin1Char('0'));
            if(fpre=="T")
                datatype="TiO";
            else
                datatype=fpre;
            dispM=datatype+" Accquired "+numf+" Frame(s)...";
            ui->textEdit_status->setFocus();
            QTextCursor storeCursorPos = ui->textEdit_status->textCursor();
            ui->textEdit_status->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
            ui->textEdit_status->moveCursor(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
            ui->textEdit_status->moveCursor(QTextCursor::End, QTextCursor::KeepAnchor);
            ui->textEdit_status->textCursor().removeSelectedText();
            ui->textEdit_status->textCursor().deletePreviousChar();
            ui->textEdit_status->setTextCursor(storeCursorPos);
            ui->textEdit_status->append(dispM);
        }
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
    realDev=deviceCount-2;
    QDateTime current_date_time =QDateTime::currentDateTimeUtc();
    QString todaynow =current_date_time.toString("yyyyMMdd hh:mm:ss");
    ui->textEdit_status->append(">>>"+todaynow+":");
    logtmp=">>>"+todaynow+":";
    outfile<<logtmp.toStdString()<<"\n";
    logtmp="Found "+QString::number(realDev)+" Device(s)...";
    ui->textEdit_status->append(logtmp);  
    mutex.lock();
    //qDebug("log file 1");
    outfile<<logtmp.toStdString()<<"\n";
    //outfile<<ccdM.toStdString()<<"\n";
    //outfile<<"test";
    //mutex.unlock();
    mutex.unlock();
    //AT_Close(handle);
    AT_FinaliseLibrary();
}
