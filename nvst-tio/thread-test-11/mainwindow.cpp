#include "mainwindow.h"
#include "ui_mainwindow.h"

bool pausedA=false,pausedB=false,stoppedA=true,stoppedB=true,startedA=false,startedB=false,imglocked=false,histlocked=false,histfirst=true,Alocked=false,Blocked=false,calchist_locked=false;
QMutex lockA,lockB,imglock,histlock,calchistlock;
QImage *grayimage,*grayimage16;
uint imgX0=1024,imgY0=1024,imgX,imgY,frameRate=33,histRate=200,low=20000,high=42000;
QVector<uint>vechistdata(65536,0);
uint histmax=0,histindex=32768;
//QBarSet *set = nullptr;
QLineSeries *lineseries = nullptr;
QAreaSeries *series = nullptr;
QCategoryAxis *axisX = nullptr;
QValueAxis *axisY = nullptr;
QChart *chart= nullptr;
unsigned short *destimg=nullptr,*srcimg=nullptr;
uint MAXQUEUE=100,countA=0,countA1=0,countB=0,countB1=0;
moodycamel::ConcurrentQueue<unsigned short*> imgQueue;
uint fps0=0,fps1=0,fps=0;
long t0=0,t1=0;
long dt=0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //std::vector<int>imgdata(65536,0);
    ui->setupUi(this);
    QDateTime timeT = QDateTime::currentDateTime();   //获取当前时间
    t0 = timeT.toSecsSinceEpoch();   //将当前时间转为时间戳
    QString proFullPath=QCoreApplication::applicationFilePath();
    QFileInfo tmpinfo(proFullPath);
    QString proName=tmpinfo.fileName();
    QStringList tmplist=proName.split(".");
    setWindowTitle(tmplist[0]+" - by Chen Dong @fso");
    ui->textEdit_StatusA->append("Waiting for ThreadA....");
    ui->textEdit_StatusB->append("Waiting for ThreadB....");

    ui->btn_pause_A->setEnabled(false); 
    ui->btn_resume_A->setEnabled(false);
    ui->btn_stopA->setEnabled(false);

    ui->btn_pause_B->setEnabled(false);
    ui->btn_resume_B->setEnabled(false);
    ui->btn_stopB->setEnabled(false);

    imgscene = new QGraphicsScene();
    ui->imgView->setScene(imgscene);

    //set = new QBarSet("GrayScale");
    lineseries=new QLineSeries();
    for (int i=0;i<65536;++i) {
       lineseries->append(QPointF(i,vechistdata[i]));
       //set->setColor(QColor::Rgb);
    }
    lineseries->setUseOpenGL(true);

    series = new QAreaSeries(lineseries);
    series->setName("Histogram");
    QPen pen(0x059605);
    pen.setWidth(1);
    series->setPen(pen);
    //设置横坐标
    axisX = new QCategoryAxis();
    axisX->setMin(0);
    axisX->setMax(65535);
    axisX->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    axisX->append(QString::number(0),0);
    QString tmpstring = QString::number(histindex);
    axisX->append("<font color=\"red\">"+tmpstring+"</font>", histindex);
    axisX->append(QString::number(65535),65535);

    //设置纵坐标
    axisY = new QValueAxis();
    axisY->setTickCount(3);
    axisY->setLabelFormat("%d");
    axisY->setRange(0,histmax);

    //建表
    chart = new QChart();
    chart->setContentsMargins(0,0,0,0);
    chart->setMargins(QMargins(15,0,15,0));
    chart->setAutoFillBackground(true);
    chart->addSeries(series);
    //chart->createDefaultAxes();
    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);
    chart->setTitle("Image Histrogram");
    chart->legend()->hide();
    //chart->setAnimationOptions(QChart::SeriesAnimations);
    //chart->legend()->setVisible(true);

    //表的容器
    ui->hist_chartview->setChart(chart);
    //ui->hist_chartview->resize(ui->hist_chartview->width(),ui->hist_chartview->height());
    ui->hist_chartview->resize(521,131);
    //ui->hist_chartview->setRenderHint(QPainter::Antialiasing);
    ui->hist_chartview->setVisible(true);
    //histfinished=false;

    onesecTimer = new QTimer(this);
    onesecTimer->setTimerType(Qt::PreciseTimer);
    connect(onesecTimer, &QTimer::timeout, this, &MainWindow::onesecPassed);
    onesecTimer->start(1000);
}

void MainWindow::onesecPassed()
{
    fps=fps1-fps0;
    fps0=fps1;
    QDateTime timeT = QDateTime::currentDateTime();   //获取当前时间
    t1 = timeT.toSecsSinceEpoch();   //将当前时间转为时间戳
    dt = t1-t0;
    ui->label_onesec->setAlignment(Qt::AlignCenter);
    QString angle_sign=u8"°C";
    QString current_status =timeT.toString("yyyy-MM-dd hh:mm:ss")+" || Up: "+QString("%1").arg(dt,8,10,QLatin1Char('0'))+" secs. || FPS: "+QString("%1").arg(fps, 3, 10, QLatin1Char('0'))+" Frames/Sec.";
    ui->label_onesec->setText(current_status);
}

void MainWindow::updateStatus(QString src,int count,QString message)
{

    if(src == "A")
    {
        ui->textEdit_StatusA->append(src+" "+QString::number(count)+" - "+message);
        ui->textEdit_StatusA->append("After enqueue: "+QString::number(imgQueue.size_approx()));
    }
    if(src == "B")
    {
        ui->textEdit_StatusB->append(src+" "+QString::number(count)+" - histmax="+QString::number(histmax)+" histidx="+QString::number(histindex));
        ui->textEdit_StatusB->append(message);
        ui->textEdit_StatusB->append("After dequeue: "+QString::number(imgQueue.size_approx()));
    }
}

void MainWindow::pauseThread(QString src)
{
        if(src=="A")
        {
                lockA.lock();
                ui->textEdit_StatusA->append("Thread"+src+" paused...");
        }
        if(src=="B")
        {
                lockB.lock();
                ui->textEdit_StatusB->append("Thread"+src+" paused...");
        }

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug()<<event;
    ui->textEdit_StatusA->append("Waiting for ThreadA to stop...");
    ui->textEdit_StatusB->append("Waiting for ThreadB to stop...");
    QElapsedTimer t;
    t.start();
    closeP();
    while(t.elapsed()<2000)
        QCoreApplication::processEvents();
    QCoreApplication::exit(0);
}

void MainWindow::on_btn_start_A_clicked()
{
    imgX=ui->lineEdit_imgx->text().toInt();
    imgY=ui->lineEdit_imgy->text().toInt();
    //if(srcimg != NULL)
    //    delete[] srcimg;
    //srcimg=new unsigned short[imgX*imgY];
    low=ui->lineEdit_low->text().toInt();
    high=ui->lineEdit_high->text().toInt();
    if( low >= high )
    {
        low=20000;
        high=42000;
    }else{
        if(low>65535)
            low=20000;
        if(high>65535)
            high=42000;
    }
    frameRate=ui->lineEdit_framerate->text().toInt();

    thread1 = new QThread( );
    taskA = new threadA();
    // move the task object to the thread BEFORE connecting any signal/slots
    taskA->moveToThread(thread1);
    connect(thread1, &QThread::started, taskA, &threadA::working);
    connect(taskA,&threadA::fromA,this, &MainWindow::updateStatus);
    connect(taskA, &threadA::finished, thread1, &QThread::requestInterruption,Qt::DirectConnection);
    connect(taskA, &threadA::finished, this, &MainWindow::pauseThread,Qt::DirectConnection);
    connect(thread1, &QThread::finished, thread1, &QThread::deleteLater);
    //connect(taskA, &threadA::imgReady, this, &MainWindow::updateImg);
    liveTimer = new QTimer(this);
    liveTimer->setTimerType(Qt::PreciseTimer);
    connect(liveTimer, &QTimer::timeout, this, &MainWindow::updateImg);
    liveTimer->start(frameRate*2);

    thread1->start();
    if(thread1->isRunning())
    {
        ui->textEdit_StatusA->append("ThreadA started...");
        ui->btn_start_A->setEnabled(false);
        ui->btn_pause_A->setEnabled(true);
        ui->btn_resume_A->setEnabled(false);
        ui->btn_stopA->setEnabled(true);
        startedA=true;
        stoppedA=false;
        pausedA=false;

    }else{
        ui->textEdit_StatusA->append("ThreadA start failed...");
        startedA=false;
        pausedA=false;
        stoppedA=true;
    }

}


void MainWindow::on_btn_pause_A_clicked()
{

    ui->btn_pause_A->setEnabled(false);
    ui->btn_resume_A->setEnabled(true);
    ui->textEdit_StatusA->append("ThreadA paused...");
    pausedA=true;
    lockA.lock();
}


void MainWindow::on_btn_resume_A_clicked()
{

    ui->btn_pause_A->setEnabled(true);
    ui->btn_resume_A->setEnabled(false);
    ui->textEdit_StatusA->append("ThreadA resumed...");
    pausedA=false;
    lockA.unlock();
}

void MainWindow::on_btn_stopA_pressed()
{
    if(pausedA)
        lockA.unlock();
    if(thread1->isRunning())
    {
        thread1->disconnect();
        thread1->quit();
        thread1->requestInterruption();
        thread1->wait();
        while(thread1->isRunning() )
        {
            QCoreApplication::processEvents();
            qDebug()<<"Waiting...";
        }
        thread1->deleteLater();
    }

    if(thread1->isFinished()){
        stoppedA=true;
        ui->textEdit_StatusA->append("ThreadA stopped...");
        ui->btn_start_A->setEnabled(true);
        ui->btn_pause_A->setEnabled(false);
        ui->btn_resume_A->setEnabled(false);
        ui->btn_stopA->setEnabled(false);
        startedA=false;
        pausedA=false;
        stoppedA=true;
    }else
    {
        startedA=true;
        pausedA=false;
        stoppedA=false;
    }

 }

void MainWindow::on_btn_start_B_clicked()
{
    imgX=ui->lineEdit_imgx->text().toInt();
    imgY=ui->lineEdit_imgy->text().toInt();
    //if(destimg != NULL)
    //    delete[] destimg;
    //destimg=new unsigned short[imgX*imgY];
    histRate=ui->lineEdit_histrate->text().toInt();
    thread2 = new QThread( );
    taskB = new threadB();
    // move the task object to the thread BEFORE connecting any signal/slots
    taskB->moveToThread(thread2);
    connect(thread2, &QThread::started, taskB, &threadB::working);
    connect(taskB,&threadB::fromB,this, &MainWindow::updateStatus);
    connect(taskB,&threadB::histReady,this, &MainWindow::updateHist);
    connect(taskB, &threadB::finished, thread2, &QThread::requestInterruption,Qt::DirectConnection);
    connect(taskB, &threadB::finished, this, &MainWindow::pauseThread,Qt::DirectConnection);
    connect(thread2, &QThread::finished, thread2, &QThread::deleteLater);
    //if(startedA)
        //connect(taskA,&threadA::imgReady,taskB, &threadB::calcHist);

    thread2->start();
    if(thread2->isRunning())
    {
        ui->textEdit_StatusB->append("ThreadB started...");
        ui->btn_start_B->setEnabled(false);
        ui->btn_pause_B->setEnabled(true);
        ui->btn_resume_B->setEnabled(false);
        ui->btn_stopB->setEnabled(true);
        startedB=true;
        stoppedB=false;
        pausedB=false;
    }else{
        ui->textEdit_StatusB->append("ThreadB start failed...");
        startedB=false;
        stoppedB=true;
        pausedB=false;
    }
}


void MainWindow::on_btn_pause_B_clicked()
{
    ui->btn_pause_B->setEnabled(false);
    ui->btn_resume_B->setEnabled(true);
    ui->textEdit_StatusB->append("ThreadB paused...");
    pausedB=true;
    lockB.lock();
}


void MainWindow::on_btn_resume_B_clicked()
{
    ui->btn_pause_B->setEnabled(true);
    ui->btn_resume_B->setEnabled(false);
    ui->textEdit_StatusA->append("ThreadB resumed...");
    pausedB=false;
    lockB.unlock();
}

void MainWindow::on_btn_stopB_pressed()
{
    if(pausedB)
        lockB.unlock();
    if(thread2->isRunning())
    {
        thread2->disconnect();
        thread2->quit();
        thread2->requestInterruption();
        thread2->wait();
        while(thread2->isRunning())
        {
            QCoreApplication::processEvents();
            qDebug()<<"Waiting...";
        }
        thread2->deleteLater();

    }
    if(thread2->isFinished()){
        stoppedB=true;
        ui->textEdit_StatusB->append("ThreadB stopped...");
        ui->btn_start_B->setEnabled(true);
        ui->btn_pause_B->setEnabled(false);
        ui->btn_resume_B->setEnabled(false);
        ui->btn_stopB->setEnabled(false);
        startedB=false;
        pausedB=false;
        stoppedB=true;
    }else{
        startedB=true;
        pausedB=false;
        stoppedB=false;
    }


}

void MainWindow::on_btn_exit_clicked()
{
    ui->textEdit_StatusA->append("Waiting for ThreadA to stop...");
    ui->textEdit_StatusB->append("Waiting for ThreadB to stop...");
    closeP();
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<2000)
        QCoreApplication::processEvents();
    QCoreApplication::exit(0);
}


void MainWindow::closeP()
{
    stoppedA=true;
    stoppedB=true;
    //paused=true;
    qDebug()<<"Closing...";
    ui->textEdit_StatusA->append("Pls wait for 1 sec. for closing thread...");
    ui->textEdit_StatusB->append("Pls wait for 1 sec. for closing thread...");
    if(pausedA)
        lockA.unlock();
    if(pausedB)
        lockB.unlock();
    if(startedA){
        thread1->disconnect();
        thread1->requestInterruption();
        thread1->quit();
        thread1->wait();
        //QElapsedTimer t;
        //t.start();
        while(!thread1->isFinished())
        {
            qDebug()<<"Waiting for threadA to quit...";
            QCoreApplication::processEvents();
        }
    }
    if(startedB){
        thread2->disconnect();
        thread2->requestInterruption();
        thread2->quit();
        thread2->wait();
        //QElapsedTimer t;
        //t.start();
        while(!thread2->isFinished())
        {
            qDebug()<<"Waiting for threadB to quit...";
            QCoreApplication::processEvents();
        }
    }
    pausedA=false;
    pausedB=false;
    startedA=false;
    startedB=false;
}

void MainWindow::updateImg()
{
    //imglock.lock();
    //imglocked = true;
    //QElapsedTimer t;
    //t.start();




    if(!pausedA  && startedA && !stoppedA )
    {
        grayimage16 = new QImage(imgX,imgY,QImage::Format_Grayscale16);
        if(imgQueue.try_dequeue(destimg))
        {
            *grayimage16 = QImage(reinterpret_cast< uchar* >( destimg ),imgX,imgY,QImage::Format_Grayscale16).scaled(imgscene->width(),imgscene->height());
            imgscene->clear();
            ui->imgView->update();
            item = new QGraphicsPixmapItem(QPixmap::fromImage(*grayimage16));
            imgscene->addItem(item);
            imgscene->setSceneRect(QRectF(0, 0, imgY, imgX));
            ui->imgView->fitInView(imgscene->sceneRect(), Qt::KeepAspectRatio);
            ui->imgView->update();

            //delete item;
        }else
        {
            ui->textEdit_StatusA->append("Image dequeue in display failed, pls wait...");
        }
        countA++;
        fps1++;
        delete grayimage16;
    }



    //imglock.unlock();
    //imglocked = false;
    //while(t.elapsed()<frameRate)
        //QCoreApplication::processEvents();
}

void MainWindow::updateHist(QVector<uint> tmphistdata,uint tmphistmax,uint tmphistindex)
{
    //histlock.lock();
    //histlocked=true;
    if(startedB && !pausedB )
    {
        chart->removeSeries(series);
        delete lineseries;
        delete series;
        lineseries=new QLineSeries();

        for (int i=0;i<65536;++i) {
           lineseries->append(QPointF(i,tmphistdata[i]));
        }
        lineseries->setColor(Qt::black);

        series = new QAreaSeries(lineseries);
        series->setName("Histogram");
        QPen pen(Qt::black);
        pen.setWidth(1);
        series->setPen(pen);
        QBrush brush;
        brush.setColor(Qt::black);//画刷颜色
        brush.setStyle(Qt::SolidPattern);//画刷填充样式，斜网格
        series->setBrush(brush);

        chart->removeAxis(axisX);
        delete axisX;
        axisX = new QCategoryAxis();
        axisX->setMin(0);
        axisX->setMax(65535);
        axisX->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
        axisX->append(QString::number(0),0);
        QString tmpstring = QString::number(tmphistindex);
        axisX->append("<font color=\"red\">"+tmpstring+"</font>", tmphistindex);
        axisX->append(QString::number(65535),65535);

        chart->removeAxis(axisY);
        delete axisY;
        axisY = new QValueAxis();
        axisY->setTickCount(3);
        axisY->setLabelFormat("%d");
        axisY->setRange(0,tmphistmax);

        chart->addSeries(series);
        chart->addAxis(axisX,Qt::AlignBottom);
        chart->addAxis(axisY,Qt::AlignLeft);

        ui->hist_chartview->setVisible(true);
        ui->hist_chartview->update();
        countB++;
    }

    //histlock.unlock();
    //histlocked=false;

}

