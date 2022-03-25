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
#include <QDir>
#include <fstream>
#include <iostream>
#include <QMutex>
#include "atcore.h"
#include "atutility.h"
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

namespace Ui {
class MainWindow;
}

extern double expTime;
extern int frameRate;
extern int framedelay;
extern int frameRateMax;
extern int realDev;
extern QString saveTo,savepred,savepref,savepre,ccdM,saveDir,savepreobj,savepredf,save01;
extern bool savefits,opened,live,display;
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
extern int flatcnt;
extern QFile obslog;
extern QString obslogfile,obslogdir,obslogdate,obsname;
extern std::ofstream outfile;
extern QMutex mutex;
extern quint16 imgMax,freedisk;
extern double sx,sy,ex,ey;
extern bool drawing,continousACQ,fulldisk;
extern QString current_date_d;
extern QString current_date_t,current_date_t1,current_date_t2;
extern QDir sdir;

//extern QTextStream obsout;

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
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    //void on_pushButtonConnect_pressed();
    //void modelItemChanged(QStandardItem *item);

    void on_btnSnap_pressed();

    void on_btnLive_pressed();

    void updateGraphicsView(unsigned short* buf,uint buflen);

    void displayOK();
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

    void on_checkBox_Flat_clicked();

    void on_checkBox_Dark_clicked();

    void on_checkBox_Data_clicked();

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
    cv::Mat currentImage;
    QTimer *liveTimer = nullptr;
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
    Server *server = nullptr;
    void snap(QString filename, QString metadata);
    AboutWindow *about;
    time_Sys* tsys;
    //QTextStream obsout;
    //bool opened;

};
#endif // MAINWINDOW_H
