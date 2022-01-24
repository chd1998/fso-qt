#include <iostream>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QLoggingCategory>
#include <QPoint>
#include <QPointer>
#include <QFileInfo>
#include <QButtonGroup>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"
#include "atutility.h"
#include "time_sys/time_sys.h"
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPixmap>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDir>
#include <QFileDialog>

float expTime;
int frameRate;
QString saveTo,ccdM,fpre="NVST-Dark";
quint16 imgMax=0;
bool savefits;
double temperature;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->btnLive->setEnabled(false);
    ui->btnSnap->setEnabled(false);
    ui->actionServer->setEnabled(false);
    ui->verticalLayout_2->setAlignment(Qt::AlignTop);
    QString proFullPath=QCoreApplication::applicationFilePath();
    QFileInfo tmpinfo(proFullPath);
    QString proName=tmpinfo.fileName();
    QStringList tmplist=proName.split(".");
    setWindowTitle(tmplist[0]+" --- Modified by Chen Dong @fso");
    about = new AboutWindow(this);
    this->loadPropertyList(":/profile.json");
    this->setupStatusBar();
    this->on_actionDetect_Cameras_triggered();
    ui->actionSave->setEnabled(false);
    server = new Server(this);
    connect(server, &Server::newRequest, this, &MainWindow::processRemoteCall);
    connect(server, &Server::serverStateChanged, this, &MainWindow::handle_serverStateChanged);

    scene = new GraphicsScene();
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setMouseTracking(true);
    ui->textEdit_status->setReadOnly(true);
    connect(scene, &GraphicsScene::positionMoved, this, &MainWindow::updateCursorCoord);
    tsys = new time_Sys(NULL);
    tsys->start();
    connect(tsys,&time_Sys::onesecPassed,this,&MainWindow::showTime);
    savefits=false;
    temperature=0.0;
    opened=false;
    QButtonGroup *group = new QButtonGroup(this);
    group->addButton(ui->checkBox_Dark,1);
    group->addButton(ui->checkBox_Flat,2);
    group->addButton(ui->checkBox_Data,3);
    ui->checkBox_Dark->setChecked(true);
}

void MainWindow::showTime()
{
    QDateTime current_date_time =QDateTime::currentDateTime();
    if(opened)
    {
        AT_GetFloat(handle, L"SensorTemperature", &temperature);
    }
    QString angle_sign=u8"°C";
    QString current_date =current_date_time.toString("yyyy-MM-dd hh:mm:ss")+"     Sensor Temp: "+QString::number(temperature)+angle_sign;
    ui->label_time->setText(current_date);
}

void MainWindow::updateCursorCoord(double x, double y) {
    int ix = static_cast<int>(x);
    int iy = static_cast<int>(y);
    labelCoordX->setText(QString::number(ix));
    labelCoordY->setText(QString::number(iy));
    if (!scene->items().isEmpty()) {
        if (ix > 0 && ix <= currentImage.rows && iy > 0 && iy <= currentImage.cols) {
            int v = currentImage.at<unsigned short>(cv::Point(ix, iy));
            labelCoordV->setText(QString::number(v));
        }
    }
}

void MainWindow::setupStatusBar() {
    QLabel *label;
    label = new QLabel(this);
    label->setText("X");
    label->setAlignment(Qt::AlignCenter);
    label->setMinimumWidth(20);
    labelCoordX = new QLabel(this);
    labelCoordX->setMinimumWidth(60);
    ui->statusBar->addWidget(label);
    ui->statusBar->addWidget(labelCoordX);

    label = new QLabel(this);
    label->setText("Y");
    label->setAlignment(Qt::AlignCenter);
    label->setMinimumWidth(20);
    labelCoordY = new QLabel(this);
    labelCoordY->setMinimumWidth(60);
    ui->statusBar->addWidget(label);
    ui->statusBar->addWidget(labelCoordY);

    label = new QLabel(this);
    label->setText("Value");
    label->setAlignment(Qt::AlignCenter);
    label->setMinimumWidth(20);
    labelCoordV = new QLabel(this);
    labelCoordV->setMinimumWidth(60);
    ui->statusBar->addWidget(label);
    ui->statusBar->addWidget(labelCoordV);

    label = new QLabel(this);
    label->setText("Max Value");
    label->setAlignment(Qt::AlignCenter);
    label->setMinimumWidth(20);
    labelCoordMV = new QLabel(this);
    labelCoordMV->setMinimumWidth(60);
    ui->statusBar->addWidget(label);
    ui->statusBar->addWidget(labelCoordMV);
}

MainWindow::~MainWindow() {
    //AT_FinaliseLibrary();
    //stop time thread
    if(tsys->isRunning())
    {
        tsys->requestInterruption();
        tsys->quit();
        tsys->wait();
    }
    delete ui;
    delete server;
}

void MainWindow::on_pushButtonConnect_toggled(bool checked) {
    if (checked) {
        AT_InitialiseLibrary();
        int ret = AT_Open(ui->spinBoxCameraIndex->value(), &handle);
        if (ret == AT_SUCCESS) {
            ui->pushButtonConnect->setText("Disconnect");
            ui->pushButtonConnect->setToolTip("Click to disconnect the camera");
            ui->spinBoxCameraIndex->setEnabled(false);
            this->initPropertyBrowser();
            ui->btnLive->setEnabled(true);
            ui->btnSnap->setEnabled(false);
            ui->actionServer->setEnabled(true);
            ui->textEdit_status->append("Connected to Camera "+QString::number(ui->spinBoxCameraIndex->value()));
            opened=true;
            //AT_GetFloat(handle, L"SensorTemperature", &temperature);
        }  // TODO: error handle
    } else {
        server->on_btnServerStart_clicked(false);
        server->setVisible(false);
        ui->btnLive->setChecked(false);
        AT_Close(handle);
        AT_FinaliseLibrary();
        ui->spinBoxCameraIndex->setEnabled(true);
        ui->pushButtonConnect->setText("Connect");
        ui->pushButtonConnect->setToolTip("Click to connect the camera");
        ui->btnLive->setEnabled(false);
        ui->btnSnap->setEnabled(false);
        ui->actionServer->setEnabled(false);
        ui->textEdit_status->append("Camera "+QString::number(ui->spinBoxCameraIndex->value())+" disconnected...");
        opened=false;
    }
}

void MainWindow::modelItemChanged(QStandardItem *item) {
    if (myDelegate->isInit)
        return;

    disconnect(propertyBrowserModel, &QStandardItemModel::itemChanged, this, &MainWindow::modelItemChanged);

    QJsonObject feature = propertyList[item->index().row()].toObject();
    AT_WC *featureName = new AT_WC[256];
    AT_WC *enumVal = new AT_WC[1024];
    feature["identifier"].toString().toWCharArray(featureName);
    featureName[feature["identifier"].toString().length()] = 0;
    QString type = feature["type"].toString();
    if (type == "integer") {
        int newVal = item->data(Qt::EditRole).toInt();
        AT_SetInt(handle, featureName, static_cast<AT_64>(newVal));
        AT_64 refreshVal = static_cast<AT_64>(newVal);
        AT_GetInt(handle, featureName, &refreshVal);
        item->setData(static_cast<int>(refreshVal), Qt::EditRole);
    } else if (type == "float") {
        double newVal = item->data(Qt::EditRole).toDouble();
        AT_SetFloat(handle, featureName, newVal);
        double refreshVal = newVal;
        AT_GetFloat(handle, featureName, &refreshVal);
        item->setData(refreshVal, Qt::EditRole);
    } else if (type == "bool") {
        AT_BOOL newVal = item->data(Qt::CheckStateRole).toBool();
        AT_SetBool(handle, featureName, newVal);
        AT_BOOL refreshVal = newVal;
        AT_GetBool(handle, featureName, &refreshVal);
        item->setData(refreshVal, Qt::CheckStateRole);
    } else if (type == "enum") {
        item->data(Qt::EditRole).toString().toWCharArray(enumVal);
        enumVal[item->data(Qt::EditRole).toString().length()] = 0;
        AT_SetEnumString(handle, featureName, enumVal);
        int refreshVal = 0;
        AT_GetEnumIndex(handle, featureName, &refreshVal);
        QStringList enumList = item->data(Qt::UserRole+1).toStringList();
        item->setData(enumList[refreshVal], Qt::EditRole);
    }
    delete [] enumVal;
    delete [] featureName;
    connect(propertyBrowserModel, &QStandardItemModel::itemChanged, this, &MainWindow::modelItemChanged);
}

void MainWindow::loadPropertyList(QString filename) {
    QFile file;
    file.setFileName(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray raw = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(raw);
    this->propertyList = doc.array();
}

void MainWindow::initPropertyBrowser() {
    myDelegate = new PropertyBrowserDelegate(this, &propertyList);
    myDelegate->isInit = true;
    propertyBrowserModel = new QStandardItemModel(propertyList.size(), 2, this);
    propertyBrowserModel->setHorizontalHeaderLabels({"Feature", "Value"});
    AT_WC *featureName = new AT_WC[256];
    for(int row = 0; row < propertyList.size(); row++) {
        QJsonValue feature = propertyList.at(row);

        QModelIndex index0 = propertyBrowserModel->index(row, 0, QModelIndex());
        propertyBrowserModel->setData(index0, feature["identifier"], Qt::EditRole);
        propertyBrowserModel->setData(index0, feature["description"], Qt::ToolTipRole);

        QModelIndex index1 = propertyBrowserModel->index(row, 1, QModelIndex());
        propertyBrowserModel->setData(index1, feature["description"], Qt::ToolTipRole);

        validPropertyMap.insert(feature["identifier"].toString(), row);

        feature["identifier"].toString().toWCharArray(featureName);
        featureName[feature["identifier"].toString().length()] = 0;

        if (feature["type"] == "integer") {
            AT_64 val = 0;
            AT_64 max = 65536;
            AT_64 min = 0;
            AT_GetInt(handle, featureName, &val);
            AT_GetIntMin(handle, featureName, &min);
            AT_GetIntMax(handle, featureName, &max);
            propertyBrowserModel->setData(index1, static_cast<int>(val), Qt::EditRole);
            propertyBrowserModel->setData(index1, static_cast<int>(min), Qt::UserRole+1);  // Min
            propertyBrowserModel->setData(index1, static_cast<int>(max), Qt::UserRole+2);  // Max
        } else if (feature["type"] == "float") {
            double val = 0.;
            double min = 0.;
            double max = 65536.;

            AT_GetFloat(handle, featureName, &val);
            AT_GetFloatMax(handle, featureName, &max);
            AT_GetFloatMin(handle, featureName, &min);

            propertyBrowserModel->setData(index1, val);
            propertyBrowserModel->setData(index1, min, Qt::UserRole+1);  // Min
            propertyBrowserModel->setData(index1, max, Qt::UserRole+2);  // Max
        } else if (feature["type"] == "bool") {
            AT_BOOL val = false;
            AT_GetBool(handle, featureName, &val);
            propertyBrowserModel->setData(index1, val, Qt::CheckStateRole);
        } else if (feature["type"] == "enum") {
            int enumCount = 0;
            int enumIndex = 0;
            AT_GetEnumCount(handle, featureName, &enumCount);
            AT_GetEnumIndex(handle, featureName, &enumIndex);
            QStringList enumStringList;
            for (int i = 0; i < enumCount; i++) {
                AT_WC str[255];
                QString s;
                AT_GetEnumStringByIndex(handle, featureName, i, str, 255);
                s = QString().fromWCharArray(str);
                enumStringList.append(s);
            }

            propertyBrowserModel->setData(index1, enumStringList[enumIndex], Qt::EditRole);
            propertyBrowserModel->setData(index1, enumStringList, Qt::UserRole+1);
        } else if (feature["type"] == "string") {
            AT_WC str[255] = {0};
            AT_GetString(handle, featureName, str, 255);
            QString s = QString().fromWCharArray(str);
            propertyBrowserModel->setData(index1, s, Qt::EditRole);
        }
    }

    delete [] featureName;

    myDelegate->isInit = false;

    connect(propertyBrowserModel, &QStandardItemModel::itemChanged, this, &MainWindow::modelItemChanged);

    ui->propertyBrowserView->setModel(propertyBrowserModel);
    ui->propertyBrowserView->setItemDelegate(myDelegate);
    ui->propertyBrowserView->setEditTriggers(QAbstractItemView::AllEditTriggers);
}

void MainWindow::on_btnSnap_clicked() {
    if(!savefits)
    {
        savefits=true;
        ui->textEdit_status->append("Acquisition Started...");
        ui->btnSnap->setText("Stop Acquisition");
        if(ui->checkBox_Dark->isChecked())
            fpre="NVST-Dark";
        if(ui->checkBox_Flat->isChecked())
            fpre="NVST-Flat";
        if(ui->checkBox_Data->isChecked())
            fpre="NVST-Data";

    }else
    {
        savefits=false;
        ui->btnSnap->setText("Start Acquisition");
        ui->textEdit_status->append("Acquisition Stopped...");
    }
}

void MainWindow::on_btnLive_toggled(bool checked) {
    expTime=ui->lineEdit_exposuretime->text().toFloat();
    //QString frametmp=ui->lineEdit_framerate->text();
    //frameRate=frametmp.toInt();
    //float framedelay=1/frameRate;
    if (checked) {
        ui->actionSave->setEnabled(false);
        //ui->btnSnap->setEnabled(false);
        ui->propertyBrowserView->setEnabled(false);
        ui->actionServer->setEnabled(false);
        server->setVisible(false);
        AT_SetBool(handle, L"Overlap", true);
        AT_GetInt(handle, L"ImageSizeBytes", &imageSizeBytes);
        int pixelEncodingIndex;
        AT_GetEnumIndex(handle, L"PixelEncoding", &pixelEncodingIndex);
        AT_GetEnumStringByIndex(handle, L"PixelEncoding", pixelEncodingIndex, pixelEncoding, 255);
        AT_GetInt(handle, L"AOIHeight", &imageHeight);
        AT_GetInt(handle, L"AOIWidth", &imageWidth);

        AT_GetInt(handle, L"AOIStride", &imageStride);
        AT_SetEnumString(handle,L"ElectronicShutteringMode",L"Global");
        AT_SetEnumeratedString(handle, L"TriggerMode",L"Software");
        AT_SetEnumeratedString(handle, L"CycleMode", L"Continuous");
        AT_SetBool(handle, L"SensorCooling", AT_TRUE);
        AT_SetEnumString(handle, L"PixelEncoding", L"Mono16");
        AT_SetEnumString(handle, L"SimplePreAmpGainControl",L"16-bit (low noise & high well capacity)");

        bufferSize = static_cast<int>(imageSizeBytes);
        delete [] buffer;
        delete [] unpackedBuffer;
        buffer = new unsigned char[bufferSize];
        AT_QueueBuffer(handle, buffer, bufferSize);
        unpackedBuffer = new unsigned short[imageWidth * imageHeight];
        AT_SetFloat(handle, L"ExposureTime", expTime);
        AT_Command(handle, L"AcquisitionStart");
        liveTimer = new QTimer(parent());
        connect(liveTimer, &QTimer::timeout, this, &MainWindow::updateGraphicsView);
        //liveTimer->start(20);
        ui->textEdit_status->append("Live Video Started...");
        ui->btnLive->setText("Stop Live");
        ui->btnSnap->setEnabled(true);
        liveTimer->start(30);
    } else {
        liveTimer->stop();
        delete liveTimer;
        liveTimer = nullptr;
        AT_Command(handle, L"AcquisitionStop");
        AT_Flush(handle);
        AT_SetEnumeratedString(handle, L"TriggerMode",L"Internal");
        ui->actionSave->setEnabled(true);
        //ui->btnSnap->setEnabled(true);
        ui->propertyBrowserView->setEnabled(true);
        ui->actionServer->setEnabled(true);
        ui->btnLive->setText("Start Live");
        ui->btnSnap->setEnabled(false);
        ui->textEdit_status->append("Live Video Stopped...");
        savefits=false;
        ui->btnSnap->setText("Start Acquisition");
        ui->textEdit_status->append("Acquisition Stopped...");
    }

}

void MainWindow::updateGraphicsView() {
    AT_Command(handle, L"SoftwareTrigger");
    AT_WaitBuffer(handle, &buffer, &bufferSize, AT_INFINITE);

    AT_InitialiseUtilityLibrary();

    AT_ConvertBuffer(buffer, reinterpret_cast<unsigned char *>(unpackedBuffer), imageHeight, imageWidth, imageStride, pixelEncoding, L"Mono16");

    AT_FinaliseUtilityLibrary();
    AT_QueueBuffer(handle, buffer, bufferSize);

    if(savefits)
    {
        saveTo=ui->lineEdit_saveto->text();
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyyMMdd-hhmmsszzz");
        QString fn=saveTo+"\\"+fpre+"-"+current_date+".fits";
        char *fitsname;
        QByteArray tmp=fn.toUtf8();
        fitsname=tmp.data();
        fitsfile *fptr;
        long fpixel = 1, naxis = 2, nelements;
        long naxes[2] = { long(imageWidth), long(imageHeight) };
        float exposure;
        int bitpix=USHORT_IMG;
        int status = 0; /* initialize status before calling fitsio routines */
        /* create new fits file */
        fits_create_file(&fptr, fitsname, &status);
        //ui->textEdit_status->append("Saving "+fitsname);
        if(status == 0)
        {
            qDebug("create file ok! %d", status);
            fits_create_img(fptr, bitpix, naxis, naxes, &status);
            /* Write a keyword; must pass the ADDRESS of the value */
            exposure = expTime;
            fits_update_key(fptr, TFLOAT, "EXPOSURE", &exposure,"Exposure Time(s)", &status);
            nelements = imageWidth*imageHeight; /* number of pixels to write */
            //tmpbuf=new uchar[nelements];
            //memcpy(tmpbuf,pBuf,sizeof(*pBuf));
            /* Write the array of integers to the image */
            fits_write_img(fptr, TUSHORT, fpixel, nelements, buffer, &status);
            qDebug("Saving  %s... %d",qPrintable(fn),status);
            fits_close_file(fptr, &status); /* close the file */
            //msleep(5);
            //fits_report_error(stderr, status); /* print out any error messages */
            //qDebug("Saving  %s... %d",qPrintable(fn),status);
            //ccdM="Saving "+fn+"...";
            if(status==0)
            {
                ui->textEdit_status->append(fn+" saved...");
            }else
            {
                ui->textEdit_status->append(fn+" failed...");
            }
        }
        else
        {
            qDebug("create file failed! %d",status);
            status=1;
        }
    }
    currentImage = cv::Mat(static_cast<int>(imageWidth), static_cast<int>(imageHeight), CV_16UC1, unpackedBuffer);
    double minVal;
    double maxVal;
    cv::Point minLoc;
    cv::Point maxLoc;
    minMaxLoc(currentImage,&minVal,&maxVal,&minLoc, &maxLoc);
    labelCoordMV->setText(QString::number(maxVal));
    cv::Mat lossyImage;
    cv::normalize(currentImage, lossyImage, 0., 255, cv::NORM_MINMAX, CV_8U);
    QImage qimage = QImage(static_cast<unsigned char *>(lossyImage.data), lossyImage.cols, lossyImage.rows, static_cast<int>(lossyImage.step), QImage::Format_Grayscale8);
    scene->clear();
    item = new QGraphicsPixmapItem(QPixmap::fromImage(qimage));
    scene->addItem(item);
    scene->setSceneRect(QRectF(0, 0, lossyImage.cols, lossyImage.rows));
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    ui->graphicsView->update();
}

void MainWindow::on_actionServer_triggered() {
    server->show();
    server->activateWindow();
}

void MainWindow::handle_serverStateChanged(bool isRunning) {
    if (isRunning) {
        ui->btnLive->setEnabled(false);
        ui->btnSnap->setEnabled(false);
        ui->propertyBrowserView->setEnabled(false);
    } else {
        ui->btnLive->setEnabled(true);
        ui->btnSnap->setEnabled(true);
        ui->propertyBrowserView->setEnabled(true);
    }
}

void MainWindow::processRemoteCall(Request req) {
    std::cout << "processRemoteCall" << std::endl;
    std::cout << "Id: " << req.id.toStdString() << std::endl;
    std::cout << "Method: " << req.method.toStdString() << std::endl;
    for (QString &p: req.parameters)
        std::cout << "Parameter: " << p.toStdString() << std::endl;
    if (req.method == "snap") {
        snap(req.parameters[0], req.parameters[1]);
    }
    server->ack(0);
}

void MainWindow::snap(QString filename, QString metadata) {
    AT_64 imageSizeBytes;
    AT_GetInt(handle, L"ImageSizeBytes", &imageSizeBytes);
    int pixelEncodingIndex;
    AT_GetEnumIndex(handle, L"PixelEncoding", &pixelEncodingIndex);
    AT_WC pixelEncoding[255] = {0};
    AT_GetEnumStringByIndex(handle, L"PixelEncoding", pixelEncodingIndex, pixelEncoding, 255);
    AT_64 imageHeight;
    AT_GetInt(handle, L"AOIHeight", &imageHeight);
    AT_64 imageWidth;
    AT_GetInt(handle, L"AOIWidth", &imageWidth);
    AT_64 imageStride;
    AT_GetInt(handle, L"AOIStride", &imageStride);

    int bufferSize = static_cast<int>(imageSizeBytes);
    delete [] buffer;
    buffer = new unsigned char[bufferSize];
    AT_QueueBuffer(handle, buffer, bufferSize);
    AT_Command(handle, L"AcquisitionStart");
    AT_WaitBuffer(handle, &buffer, &bufferSize, AT_INFINITE);
    AT_Command(handle, L"AcquisitionStop");
    AT_Flush(handle);

    AT_InitialiseUtilityLibrary();
    delete [] unpackedBuffer;
    unpackedBuffer = new unsigned short[imageWidth * imageHeight];
    AT_ConvertBuffer(buffer, reinterpret_cast<unsigned char *>(unpackedBuffer), imageWidth, imageHeight, imageStride, pixelEncoding, L"Mono16");
    AT_FinaliseUtilityLibrary();

    if (!metadata.isEmpty()) {
        QFile metadataFile(QDir(server->saveDir).filePath(filename + ".txt"));
        metadataFile.open(QFile::WriteOnly);
        metadataFile.write(metadata.toStdString().c_str());
    }
    currentImage = cv::Mat(static_cast<int>(imageWidth), static_cast<int>(imageHeight), CV_16UC1, unpackedBuffer);
    ui->actionSave->setEnabled(true);
    //snap to local
    cv::imwrite(cv::String(QDir(server->saveDir).filePath(filename + ".tif").toStdString()), currentImage);
    //snap to remote
    cv::imwrite(cv::String(QDir(ui->lineEdit_saveto->text()).filePath(filename + ".tif").toStdString()), currentImage);
    cv::Mat lossyImage;
    cv::normalize(currentImage, lossyImage, 0., 255, cv::NORM_MINMAX, CV_8U);
    QImage qimage = QImage(static_cast<unsigned char *>(lossyImage.data), lossyImage.cols, lossyImage.rows, static_cast<int>(lossyImage.step), QImage::Format_Grayscale8);
    scene->clear();
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(qimage));
    scene->addItem(item);
    scene->setSceneRect(QRectF(0, 0, lossyImage.cols, lossyImage.rows));
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    ui->graphicsView->update();
}

void MainWindow::on_actionSave_triggered() {
    QFileDialog fd(this);
    QString fn = fd.getSaveFileName(this, "Save file", "andor.tif", "16-bit original image (*.tif);;8-bit lossy image (*.bmp)");
    std::cout << fn.toStdString() << std::endl;
    if (fn.endsWith(".tif")) {
        cv::imwrite(cv::String(fn.toStdString()), currentImage);
    } else if (fn.endsWith(".bmp")) {
        cv::Mat lossyImage;
        cv::normalize(currentImage, lossyImage, 0., 255, cv::NORM_MINMAX, CV_8U);
        cv::imwrite(cv::String(fn.toStdString()), lossyImage);
    }
}

void MainWindow::on_actionExit_triggered()
{
    if (ui->pushButtonConnect->isChecked())
        ui->pushButtonConnect->setChecked(false);
    this->close();
}

void MainWindow::on_actionAbout_triggered() {
    about->show();
}

void MainWindow::on_actionDetect_Cameras_triggered() {
    AT_64 deviceCount;
    int realDev=0;
    AT_FinaliseLibrary();
    AT_InitialiseLibrary();
    AT_GetInt(1, L"DeviceCount", &deviceCount);
    if (deviceCount > 2) {
        ui->spinBoxCameraIndex->setMinimum(0);
        ui->spinBoxCameraIndex->setRange(0, static_cast<int>(deviceCount - 3));
        ui->spinBoxCameraIndex->setEnabled(true);
        ui->pushButtonConnect->setEnabled(true);
    } else {
        ui->spinBoxCameraIndex->setEnabled(false);
        ui->pushButtonConnect->setEnabled(false);
    }
    if(deviceCount>2)
    {
        realDev=deviceCount-2;
    }
    ccdM="Found "+QString::number(realDev)+" Device(s)...";
    ui->textEdit_status->append(ccdM);
}
