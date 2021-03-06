#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QJsonArray>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QLabel>
#include <fstream>
#include <iostream>
#include <QMutex>
#include <QDir>
#include <QtCharts>
//#include <QtCharts/QChartView>
//#include <QtCharts/QBarSet>
//#include <QtCharts/QBarSeries>
#include "atlib/atcore.h"
#include "atlib/atutility.h"
#include "propertybrowserdelegate.h"
#include "utils.h"
#include "server.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "graphicsscene.h"
#include "aboutwindow.h"
#include "time_sys/time_sys.h"
#include "cfitsio/fitsio.h"
#include "histdisplay/histdisplay.h"
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

QT_USE_NAMESPACE

//using namespace QtCharts;

namespace Ui {
class MainWindow;
}

extern double expTime;
extern int frameRate;
extern uint framedelay,groupdelay;
extern int frameRateMax;
extern int realDev;
extern QString saveTo,savepred,savepref,savepre,ccdM,saveDir,savepreobj,savepredf,save01;
extern bool savefits,opened,live,display,display_locked,imgready,histshow_locked,histcalc_locked,savefits_locked,wait_Acq;
extern uint fps0,fps1,fps;
extern double temperature;
extern AT_64 imgW,imgH,imgStride,imageSizeBytes;
extern AT_H handle;
extern AT_64 f0,f1,df;
extern int ccdID;
extern long refreshtime;
extern QString fpre;
extern int numBuffer;
extern QString CamF,CamM,CamN;
extern long t0;
extern int serialNo,fserialNo,sum_fserialNo;
extern uint datanum;
extern bool firsttime,localsave,localfirst;
extern QString objname;
extern QString obscor1,obscor2;
extern QString datatype;
extern uint flatcnt;
extern QFile obslog;
extern QString obslogfile,obslogdir,obslogdate,obsname;
extern std::ofstream outfile;
extern QMutex mutex,histcalc_lock,histdisplock;
extern quint16 imgMax,freedisk;
extern double sx,sy,ex,ey;
extern bool drawing,continousACQ,diskfull;
extern QString current_date_d;
extern QString current_date_t1,current_date_t2,current_date_t3;
extern QDir sdir;

//global vars. for histogram
extern QLineSeries *lineseries ;
extern QAreaSeries *series;
extern QCategoryAxis *axisX ;
extern QValueAxis *axisY ;
extern QChart *chart;
extern bool histfirst;


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QJsonArray propertyList;
    QMap<QString, int> validPropertyMap;
    void showTime();
    QTextStream obsout;
    QString logtmp;
    //static QMutex mutex;
    //static std::ofstream outfile;
    quint64 getDiskSpace(QString, bool);
    QImage buf2image(unsigned short *);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    //void on_pushButtonConnect_pressed();
    //void modelItemChanged(QStandardItem *item);

    void on_btnSnap_pressed();

    void on_btnLive_pressed();

    void updateGraphicsView(unsigned short*,uint,QString,uint);

    void displayOK();

    //void periodicSave(uint);

    void stopACQ();

    void on_actionServer_triggered();

    void processRemoteCall(Request);

    void on_actionSave_triggered();

    void on_actionExit_triggered();

    void updateCursorCoord(double x, double y);

    void on_actionAbout_triggered();

    void handle_serverStateChanged(bool isRunning);

    void on_actionDetect_Cameras_triggered();

    //void on_exptime_changed();

    void selectStartpoint(double sx1,double sy1);

    void drawRect(double sx2,double sy2);


    //void showimgMax();

    //void aboutToQuit();

    void on_checkBox_Data_clicked();

    void on_checkBox_Dark_clicked();

    void on_checkBox_Flat_clicked();

    void drawHist(QVector<uint>,uint,uint);

    void on_textEdit_status_textChanged();

signals:
    //void histReady(cv::Mat);
    //void histReady(QImage);

private:
    Ui::MainWindow *ui;
    //QStandardItemModel *propertyBrowserModel;
    //PropertyBrowserDelegate *myDelegate;
    //AT_H handle;
    //void loadPropertyList(QString filename);
    //void initPropertyBrowser();
    void setupStatusBar();

    QLabel *labelCoordX;
    QLabel *labelCoordY;
    QLabel *labelCoordV;
    QLabel *labelCoordMV;
    QLabel *labelStat;
    QLabel *labelinfo;
    QChart *chart;

    QThread* histthread;
    histdisplay* task;

    cv::Mat currentImage;
    QTimer *liveTimer = nullptr;
    QTimer *saveTimer = nullptr;
    AT_64 imageSizeBytes;
    AT_64 imageWidth;
    AT_64 imageHeight;
    AT_WC pixelEncoding[255] = {0};
    AT_WC pixelReadout[255] = {0};
    AT_64 imageStride;
    int bufferSize;
    unsigned char *buffer = nullptr;
    unsigned short *unpackedBuffer = nullptr;
    QGraphicsPixmapItem *item = nullptr;
    GraphicsScene *scene = nullptr;
    //QChartView *chart_graphicsView;
    Server *server = nullptr;
    void snap(QString filename, QString metadata);
    //void drawHist(QImage);
    AboutWindow *about;
    time_Sys* tsys;
    //QTextStream obsout;
    //bool opened;

};
#endif // MAINWINDOW_H
