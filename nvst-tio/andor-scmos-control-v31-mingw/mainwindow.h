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
#include "atcore.h"
#include "atutility.h"
#include "propertybrowserdelegate.h"
#include "utils.h"
#include "server.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "graphicsscene.h"
#include "aboutwindow.h"
#include "time_sys/time_sys.h"
#include "cfitsio/fitsio.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

namespace Ui {
class MainWindow;
}

extern float expTime;
extern int frameRate,crossX,crossY;
extern QString saveTo,ccdM,fpre;
extern bool savefits,drawing,nightmode;
extern double temperature;
extern quint16 imgMax;
extern double sx,sy,ex,ey;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QJsonArray propertyList;
    QMap<QString, int> validPropertyMap;
    void showTime();

private slots:
    void on_pushButtonConnect_toggled(bool checked);
    void modelItemChanged(QStandardItem *item);

    void on_btnSnap_clicked();

    void on_btnLive_toggled(bool);
    void updateGraphicsView();

    void on_actionServer_triggered();

    void processRemoteCall(Request);

    void on_actionSave_triggered();

    void on_actionExit_triggered();

    void updateCursorCoord(double x, double y);

    void on_actionAbout_triggered();

    void handle_serverStateChanged(bool isRunning);

    void on_actionDetect_Cameras_triggered();

    void selectStartpoint(double sx1,double sy1);

    void drawRect(double sx2,double sy2);


private:
    Ui::MainWindow *ui;
    QStandardItemModel *propertyBrowserModel;
    PropertyBrowserDelegate *myDelegate;
    AT_H handle;
    void loadPropertyList(QString filename);
    void initPropertyBrowser();
    void setupStatusBar();

    QLabel *labelCoordX;
    QLabel *labelCoordY;
    QLabel *labelCoordV;
    QLabel *labelCoordMV;
    QLabel *labelpicinfo;

    cv::Mat currentImage;
    QTimer *liveTimer = nullptr;
    AT_64 imageSizeBytes;
    AT_64 imageWidth;
    AT_64 imageHeight;
    AT_WC pixelEncoding[255] = {0};
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
    bool opened;
    QString fpre;
    //quint16 imgMax;
};
#endif // MAINWINDOW_H
