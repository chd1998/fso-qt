#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "threada.h"
#include "threadb.h"
#include <QThread>
#include <QMutex>
extern  QMutex lockA,lockB;
extern  bool pausedA,pausedB,stoppedA,stoppedB;

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

    QThread* thread1;
    QThread* thread2;
    threadA* taskA;
    threadB* taskB;



private:
    Ui::MainWindow *ui;

private slots:
    void updateStatus(QString,int);
    void pauseThread(QString);
    void on_btn_start_A_clicked();
    void on_btn_pause_A_clicked();
    void on_btn_resume_A_clicked();
    void on_btn_start_B_clicked();
    void on_btn_pause_B_clicked();
    void on_btn_resume_B_clicked();
    void on_btn_stop_pressed();
    void on_btn_exit_clicked();
};
#endif // MAINWINDOW_H
