#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "threada.h"
#include "threadb.h"
#include <QTimer>
#include <QThread>
#include <QMutex>
#include <QtCharts/QtCharts>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include "concurrentqueue.h"

extern  QMutex lockA,lockB,imglock,histlock,calchistlock;
extern  bool pausedA,pausedB,stoppedA,stoppedB,startedA,startedB,imglocked,histlocked,histfirst,Alocked,Blocked,calchist_locked;
extern  uint imgX,imgX0,imgY,imgY0,frameRate,histRate,low,high;
extern  QImage *grayimage,*grayimage16;
//extern  int *histdata;
extern QVector<uint>vechistdata;
extern uint histmax,histindex;
//extern QBarSet *set ;
extern QLineSeries *lineseries ;
extern QAreaSeries *series;
extern QCategoryAxis *axisX ;
extern QValueAxis *axisY ;
extern QChart *chart;
extern unsigned short *srcimg,*destimg;
extern moodycamel::ConcurrentQueue<unsigned short*> imgQueue;
extern uint MAXQUEUE,countA,countA1,countB,countB1;
extern long t0,t1;
extern long dt;
extern uint fps0,fps1,fps;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void closeEvent(QCloseEvent *event);
    void closeP();

    QThread* thread1;
    QThread* thread2;
    threadA* taskA;
    threadB* taskB;
    QTimer *liveTimer = nullptr,*onesecTimer=nullptr;

    QGraphicsPixmapItem *item = nullptr;
    QGraphicsScene *imgscene = nullptr;



    //static bool histfinished;



private:
    Ui::MainWindow *ui;

private slots:
    void updateStatus(QString,int,QString);
    //void updateImg(unsigned short *);
    void updateImg();
    void updateHist(QVector<uint>,uint,uint);
    void pauseThread(QString);
    void on_btn_start_A_clicked();
    void on_btn_pause_A_clicked();
    void on_btn_resume_A_clicked();
    void on_btn_start_B_clicked();
    void on_btn_pause_B_clicked();
    void on_btn_resume_B_clicked();
    void on_btn_exit_clicked();
    void on_btn_stopA_pressed();
    void on_btn_stopB_pressed();
    void onesecPassed();
};
#endif // MAINWINDOW_H
