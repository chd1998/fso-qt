#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QCategoryAxis>
#include <QCloseEvent>
#include <QMutex>
#include <QImage>
//#include "opencv2/core.hpp"
//#include "opencv2/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
#include "simimage.h"

extern unsigned short *simBuff;
//extern QImage grayimage;
extern bool display,stopped,paused;
extern int imgX,imgY,runround,refreshrate;
extern QMutex tlock;

QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString path;
    bool opened;
    QThread* thread;
    simImage* task;
    //void drawHist(unsigned short*);
    //void showHist(cv::Mat);
    void closeEvent(QCloseEvent *event);

//signals:
    //void histReady(QString);

private slots:
    void drawHist(QImage);

    void slotFinished();

    void on_pushButton_start_clicked();

    void on_pushButton_resume_clicked();

    void on_pushButton_pause_clicked();

private:
    Ui::MainWindow *ui;
    void showHistogram(QImage);
};
#endif // MAINWINDOW_H
