#include "mainwindow.h"
#include "ui_mainwindow.h"

bool pausedA=false,pausedB=false,stoppedA=false,stoppedB=false;
QMutex lockA,lockB;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->textEdit_StatusA->append("Waiting for ThreadA....");
    ui->textEdit_StatusB->append("Waiting for ThreadB....");
    ui->btn_pause_A->setEnabled(false);
    ui->btn_pause_B->setEnabled(false);
    ui->btn_resume_A->setEnabled(false);
    ui->btn_resume_B->setEnabled(false);



}

void MainWindow::updateStatus(QString src,int count)
{

    if(src == "A")
    {
        ui->textEdit_StatusA->append(src+" , "+QString::number(count));
    }
    if(src == "B")
    {
        ui->textEdit_StatusB->append(src+" , "+QString::number(count));
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
    if(thread1->isRunning()){
        //thread->disconnect();
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
    if(thread2->isRunning()){
        //thread->disconnect();
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

void MainWindow::on_btn_start_A_clicked()
{
    thread1 = new QThread( );
    taskA = new threadA();
    // move the task object to the thread BEFORE connecting any signal/slots
    taskA->moveToThread(thread1);
    connect(thread1, &QThread::started, taskA, &threadA::working);
    connect(taskA,&threadA::fromA,this, &MainWindow::updateStatus);
    connect(taskA, &threadA::finished, thread1, &QThread::requestInterruption,Qt::DirectConnection);
    connect(taskA, &threadA::finished, this, &MainWindow::pauseThread,Qt::DirectConnection);
    connect(thread1, &QThread::finished, thread1, &QThread::deleteLater);

    thread1->start();
    if(thread1->isRunning())
    {
        ui->textEdit_StatusA->append("ThreadA started...");
        ui->btn_start_A->setEnabled(false);
        ui->btn_pause_A->setEnabled(true);
        ui->btn_resume_A->setEnabled(false);
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


void MainWindow::on_btn_start_B_clicked()
{
    thread2 = new QThread( );
    taskB = new threadB();
    // move the task object to the thread BEFORE connecting any signal/slots
    taskB->moveToThread(thread2);
    connect(thread2, &QThread::started, taskB, &threadB::working);
    connect(taskB,&threadB::fromB,this, &MainWindow::updateStatus);
    connect(taskB, &threadB::finished, thread2, &QThread::requestInterruption,Qt::DirectConnection);
    connect(taskB, &threadB::finished, this, &MainWindow::pauseThread,Qt::DirectConnection);
    connect(thread2, &QThread::finished, thread2, &QThread::deleteLater);

    thread2->start();
    if(thread2->isRunning())
    {
        ui->textEdit_StatusB->append("ThreadB started...");
        ui->btn_start_B->setEnabled(false);
        ui->btn_pause_B->setEnabled(true);
        ui->btn_resume_B->setEnabled(false);
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


void MainWindow::on_btn_stop_pressed()
{
    if(pausedA)
        lockA.unlock();
    if(pausedB)
        lockB.unlock();
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

    if(thread1->isFinished()){
        stoppedA=true;
        ui->textEdit_StatusA->append("ThreadA stopped...");
        ui->btn_start_A->setEnabled(true);
        ui->btn_pause_A->setEnabled(false);
        ui->btn_resume_A->setEnabled(false);
        pausedA=false;
    }
    if(thread2->isFinished()){
        stoppedB=true;
        ui->textEdit_StatusB->append("ThreadB stopped...");
        ui->btn_start_B->setEnabled(true);
        ui->btn_pause_B->setEnabled(false);
        ui->btn_resume_B->setEnabled(false);
        pausedB=false;
    }
}


void MainWindow::on_btn_exit_clicked()
{
    if((stoppedA && stoppedB) || (!thread1->isRunning() && !thread2->isRunning()))
    {
        QCoreApplication::exit(0);
    }
    else{
        if(thread1->isRunning() || !stoppedA)
            ui->textEdit_StatusA->append("pls stop ThreadA first...");
        if(thread2->isRunning() || !stoppedB)
            ui->textEdit_StatusB->append("pls stop ThreadB first...");
    }
}

