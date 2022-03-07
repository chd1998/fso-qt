#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;


void MainWindow::showHistogram(cv::Mat grayimage){
    //  对直方图进行数据处理,求出各个灰度值的像素数
    int yRange = 0;
    int data[256]{0};
    int max=0,idx=0;
    for(int i = 0; i <grayimage.cols; i++){
            for(int j = 0; j < grayimage.rows; j++){
                //int index=cv::saturate_cast<int>(grayimage.ptr<cv::Vec3b>(i)[j][0]);
                int index=grayimage.ptr<cv::Vec3b>(i)[j][0];
                if(index<0) index=0;
                if(index>255) index=255;
                ++data[index];
                if(data[index]>max)
                {
                    max=data[index];
                    idx=index;
                }
            }
        }
   qDebug()<<max<<" "<<idx;
    //设置每个“柱”的颜色，值，宽度等
    QBarSet *set = new QBarSet("灰度值");
    for (int i=0;i<256;i++) {
       set->append(data[i]);
       if(yRange<data[i]) yRange = data[i];
       set->setColor(QColor::Rgb);
    }
    QBarSeries *series = new QBarSeries();
    //series->replace(set);
    series->append(set);
    series->setBarWidth(1);

    //设置横坐标
    QCategoryAxis *axisX = new QCategoryAxis();
    axisX->setMin(0);
    axisX->setMax(255);
    //axisX->setStartValue(0);
    //axisX->setLabelsColor(QColor(0,0,0));
    axisX->append(QString::number(0),0);
    //axisX->setLabelsColor(QColor(0,0,255));
    QString tmpstring = QString::number(idx*65535/255);
    axisX->append("<font color=\"red\">"+tmpstring+"</font>", idx);
    //axisX->append(QString::number(idx*65535/255),idx);
    //axisX->setLabelsColor(QColor(0,0,0));
    axisX->append(QString::number(65535),255);
    axisX->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);


    //设置纵坐标
    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%d");
    axisY->setRange(0,yRange);

    //建表
    QChart *chart = new QChart();
    chart->addSeries(series);
    //chart->createDefaultAxes();
    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);
    chart->setTitle("灰度直方图");
    //chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);

    //表的容器
    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
    ui->chartView->setVisible(true);
}

void MainWindow::drawHist(QString path)
{
    if(opened)
    {
        try{
            QImage qimg;
            cv::Mat grayimage = cv::imread(cv::format("%s", path.toLocal8Bit().constData()),0);
            cv::normalize(grayimage, grayimage, 0, 256, cv::NORM_MINMAX, CV_8U);
            /*double minVal;
            double maxVal;
            cv::Point minLoc;
            cv::Point maxLoc;
            minMaxLoc(grayimage,&minVal,&maxVal,&minLoc, &maxLoc);
            qDebug()<<minVal<<" "<<maxVal;*/
            //cv::namedWindow("origin");
            //cv::imshow("origin",grayimage);
            qimg.load(path);
            qimg=qimg.convertToFormat(QImage::Format_Indexed8);
            //qimg=qimg.convertToFormat(QImage::Format_Grayscale16);
            QPixmap pixmap = QPixmap::fromImage(qimg);
            pixmap.scaled(ui->label->size(), Qt::KeepAspectRatio);
            ui->label->clear();
            ui->label->setScaledContents(true);
            ui->label->setPixmap(pixmap);
            //ui->label->setPixmap(QPixmap::fromImage(qimg));
            ui->textEdit_picinfo->append(QString::number(pixmap.width())+" "+QString::number(pixmap.height())+" "+path);
            showHistogram(grayimage);
        }catch(cv::Exception exception)
        {
            qDebug("Error in cv::imread image");
        }
        //qimg= QImage::fromData(data);
        //qimg=qimg.convertToFormat(QImage::Format_Grayscale8);

    }else
    {
        ui->textEdit_picinfo->append("Failed to open "+path+" file...");
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    opened=false;
    setWindowTitle("Histogram for GrayScale Images");
    connect(this,&MainWindow::histReady,this,&MainWindow::drawHist);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_open_pressed()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Open Image"));
    fileDialog->setDirectory(".");
    fileDialog->setNameFilter(tr("Image Files(*.jpg *.png *.bmp *.tiff)"));
    //fileDialog->setFilter(QDir::AllEntries);
    //fileDialog->setDefaultSuffix("jpg");
    if(fileDialog->exec() == QDialog::Accepted) {
            path = fileDialog->selectedFiles()[0];
            //QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
            //ui->pic_label->setText(path+" Selected");
            opened=true;
    } else {
            //QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
            //ui->pic_label->setText("Nothing Selected");
            opened=false;
    }
    emit histReady(path);
}

