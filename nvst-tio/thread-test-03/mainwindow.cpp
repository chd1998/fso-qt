#include "mainwindow.h"
#include "ui_mainwindow.h"

bool pausedA=false,pausedB=false,stoppedA=false,stoppedB=false,startedA=false,startedB=false,imglocked=false,histlocked=false,histfirst=true;
QMutex lockA,lockB,imglock,histlock;
QImage *grayimage,*grayimage16;
int imgX=1024,imgY=1024;
int histdata[256]{0},oldhistdata[256]{0};
int histmax=0,idx=0;
QBarSet *set = nullptr;
QBarSeries *series = nullptr;
QCategoryAxis *axisX = nullptr;
QValueAxis *axisY = nullptr;
QChart *chart= nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
}

void MainWindow::updateStatus(QString src,int count)
{

    if(src == "A")
    {
        ui->textEdit_StatusA->append(src+" , "+QString::number(count));
    }
    if(src == "B")
    {
        ui->textEdit_StatusB->append(src+" , "+QString::number(count)+": "+QString::number(histmax)+" - "+QString::number(idx));
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
    thread1 = new QThread( );
    taskA = new threadA();
    // move the task object to the thread BEFORE connecting any signal/slots
    taskA->moveToThread(thread1);
    connect(thread1, &QThread::started, taskA, &threadA::working);
    connect(taskA,&threadA::fromA,this, &MainWindow::updateStatus);
    connect(taskA,&threadA::imgReady,this, &MainWindow::updateImg);
    //connect(taskA,&threadA::imgReady,taskB, &threadB::calcHist);
    connect(taskA, &threadA::finished, thread1, &QThread::requestInterruption,Qt::DirectConnection);
    connect(taskA, &threadA::finished, this, &MainWindow::pauseThread,Qt::DirectConnection);
    connect(thread1, &QThread::finished, thread1, &QThread::deleteLater);
    if(startedB)
        connect(taskA,&threadA::imgReady,taskB, &threadB::calcHist);

    thread1->start();
    if(thread1->isRunning())
    {
        ui->textEdit_StatusA->append("ThreadA started...");
        ui->btn_start_A->setEnabled(false);
        ui->btn_pause_A->setEnabled(true);
        ui->btn_resume_A->setEnabled(false);
        ui->btn_stopA->setEnabled(true);
        startedA=true;
    }else{
        ui->textEdit_StatusA->append("ThreadA start failed...");
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
        startedA=false;
    }

    if(thread1->isFinished()){
        stoppedA=true;
        ui->textEdit_StatusA->append("ThreadA stopped...");
        ui->btn_start_A->setEnabled(true);
        ui->btn_pause_A->setEnabled(false);
        ui->btn_resume_A->setEnabled(false);
        ui->btn_stopA->setEnabled(false);
        pausedA=false;
    }
 }

void MainWindow::on_btn_start_B_clicked()
{
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
    if(startedA)
        connect(taskA,&threadA::imgReady,taskB, &threadB::calcHist);

    thread2->start();
    if(thread2->isRunning())
    {
        ui->textEdit_StatusB->append("ThreadB started...");
        ui->btn_start_B->setEnabled(false);
        ui->btn_pause_B->setEnabled(true);
        ui->btn_resume_B->setEnabled(false);
        ui->btn_stopB->setEnabled(true);
        startedB=true;
    }else{
        ui->textEdit_StatusB->append("ThreadB start failed...");
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
        startedB=false;
    }
    if(thread2->isFinished()){
        stoppedB=true;
        ui->textEdit_StatusB->append("ThreadB stopped...");
        ui->btn_start_B->setEnabled(true);
        ui->btn_pause_B->setEnabled(false);
        ui->btn_resume_B->setEnabled(false);
        ui->btn_stopB->setEnabled(false);
        pausedB=false;
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
}

void MainWindow::updateImg()
{
    imglock.lock();
    imglocked = true;
    imgscene->clear();
    ui->imgView->update();
    item = new QGraphicsPixmapItem(QPixmap::fromImage(*grayimage));
    imgscene->addItem(item);
    imgscene->setSceneRect(QRectF(0, 0, imgscene->height(), imgscene->width()));
    //scene->setSceneRect(QRectF(0, 0, imgH, imgW));
    //imgscene->setSceneRect(QRectF(0, 0, imgX,imgY));
    ui->imgView->fitInView(imgscene->sceneRect(), Qt::KeepAspectRatio);
    //ui->graphicsView->fitInView(scene->sceneRect(),Qt::IgnoreAspectRatio);
    ui->imgView->update();
    imglock.unlock();
    imglocked = false;
}

void MainWindow::updateHist()
{


    histlock.lock();
    histlocked=true;
    if(!histfirst)
    {
        delete set;
        delete series;
        delete axisX;
        delete axisY;
        //chart->removeAllSeries();
        //qDebug()<<chart;
        //delete chart;
    }
    int yRange = 0;
    //QBarSet *set = new QBarSet("GrayScale");
    set = new QBarSet("GrayScale");
    for (int i=0;i<256;i++) {
       //if(histfirst)
            set->append(histdata[i]);
       //else
       //{
         // if(oldhistdata[i] != histdata[i] )
         // {
         //   set->insert(i,histdata[i]);
          //  oldhistdata[i]=histdata[i];
         // }

       //}


       if(yRange<histdata[i]) yRange = histdata[i];
       set->setColor(QColor::Rgb);

    }
    series = new QBarSeries();
    //series->replace(set);
    series->append(set);
    series->setBarWidth(1);
    series->setUseOpenGL(true);

    //设置横坐标
    axisX = new QCategoryAxis();
    axisX->setMin(0);
    axisX->setMax(255);
    //axisX->setStartValue(0);
    //axisX->setLabelsColor(QColor(0,0,0));
    axisX->append(QString::number(0),0);
    //axisX->setLabelsColor(QColor(0,0,255));
    QString tmpstring = QString::number(idx*65536/256);
    //axisX->append("<font color=\"red\">"+tmpstring+"</font>", idx);
    axisX->append("<font color=\"red\">"+tmpstring+"</font>", idx);
    //axisX.re
    //axisX->append(QString::number(idx*65535/255),idx);
    //axisX->setLabelsColor(QColor(0,0,0));
    axisX->append(QString::number(65535),255);
    axisX->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);


    //设置纵坐标
    axisY = new QValueAxis();
    //axisY->setLabelFormat("%d");
    //axisY->setRange(0,yRange);

    //建表
    chart = new QChart();
    chart->setContentsMargins(0,0,0,0);
    chart->setMargins(QMargins(0,0,0,0));
    chart->setAutoFillBackground(true);
    chart->addSeries(series);
    //chart->createDefaultAxes();
    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);
    //chart->setTitle("Image Histrogram");
    chart->legend()->hide();
    //chart->setAnimationOptions(QChart::SeriesAnimations);
    //chart->legend()->setVisible(true);

    //表的容器
    ui->hist_chartview->setChart(chart);
    //ui->hist_chartview->resize(ui->hist_chartview->width(),ui->hist_chartview->height());
    ui->hist_chartview->resize(521,131);
    ui->hist_chartview->setRenderHint(QPainter::Antialiasing);
    ui->hist_chartview->setVisible(true);


    histlock.unlock();
    histlocked=false;
    //chart->removeAllSeries();
    //chart->removeAxis(axisX);
    //chart->removeAxis(axisY);

}

