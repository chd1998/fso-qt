#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "threada.h"
#include "threadb.h"
#include <QThread>
#include <QMutex>
#include <QtCharts/QtCharts>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

extern  QMutex lockA,lockB,imglock,histlock;
extern  bool pausedA,pausedB,stoppedA,stoppedB,startedA,startedB,imglocked,histlocked,histfirst;
extern  int imgX,imgY,frameRate,histRate;
extern  QImage *grayimage,*grayimage16;
//extern  int *histdata;
extern std::vector<unsigned short>vecimg;
extern std::vector<unsigned short>vechistdata;
extern int histmax,histindex;
extern QBarSet *set ;
extern QBarSeries *series ;
extern QCategoryAxis *axisX ;
extern QValueAxis *axisY ;
extern QChart *chart;
extern unsigned short * myImage,*myImageBack;

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


    QGraphicsPixmapItem *item = nullptr;
    QGraphicsScene *imgscene = nullptr;



private:
    Ui::MainWindow *ui;

private slots:
    void updateStatus(QString,int);
    void updateImg();
    void updateHist(std::vector<unsigned short>,int,int);
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
};
#endif // MAINWINDOW_H
