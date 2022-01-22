#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cuda.h>
#include <cuda_runtime.h>
#include <cufft.h>
#include "device_launch_parameters.h"
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <string>
#include <complex>
#include <device_functions.h>
#include <cuComplex.h>
#include <chrono>
#include "QFileDialog"
#include "QString"

using namespace std;

extern "C"
cufftComplex* FFT_GPU(cufftComplex* signal1, cufftComplex* signal2, int len_c);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->risunok->replot();
    ui->risunok->clearGraphs();
    ui->risunok->addGraph();
    ui->risunok->yAxis->setLabel("VKF");
    ui->risunok->xAxis->setLabel("time(ms)");
    this->setWindowTitle("VKF with CUDA");
}

MainWindow::~MainWindow()
{
    delete ui;
}

QVector<double> abs(cufftComplex* a, int len)
{
    QVector<double> rez(len);
    for(size_t i = 0; i < len; i++)
    {
        rez[i] = sqrt(pow(a[i].x,2) + pow(a[i].y,2));
    }
    return rez;
}

void swap(cufftComplex *v1, cufftComplex *v2)
{
    cufftComplex tmp = *v1;
    *v1 = *v2;
    *v2 = tmp;
}

void fftshift(cufftComplex* a, int len)
{
    int k = 0;
        int c = (int) floor((float)len/2);
        // For odd and for even numbers of element use different algorithm
        if (len % 2 == 0)
        {
            for (k = 0; k < c; k++)
                swap(&a[k], &a[k+c]);
        }
        else
        {
            cufftComplex tmp = a[0];
            for (k = 0; k < c; k++)
            {
                a[k] = a[c + k + 1];
                a[c + k + 1] = a[k + 1];
            }
            a[c] = tmp;
        }
}

void MainWindow::on_pb_VKF_exec_clicked()
{
    int deviceCount;
        cudaGetDeviceCount(&deviceCount);
        if (deviceCount == 0)
        {
            statusBar()->showMessage("No Device to Support CUDA", 3000);
        }
        else
            cout << "deviceCount = " << deviceCount << endl;
    cudaSetDevice(0);

    auto start = std::chrono::high_resolution_clock::now();
    ifstream* fileSource1 = new ifstream;
    ifstream* fileSource2 = new ifstream;


    QString filename1 = ui->le_filename1->text();
    QString filename2 = ui->le_filename2->text();

    if(filename1 == "" || filename2 == "") {
        statusBar()->showMessage("File path error", 8000);
    }

    fileSource1->open(filename1.toStdString(), ios::binary);
    fileSource2->open(filename2.toStdString(), ios::binary);

    if (fileSource1->is_open() && fileSource2->is_open()) {
        int fileSize = ui->le_razm->text().toInt();
        int len_c = fileSize / 2;

        char* sourceMass1 = new char[fileSize];
        char* sourceMass2 = new char[fileSize];
        fileSource1->read(sourceMass1, fileSize);
        fileSource2->read(sourceMass2, fileSize);

        cufftComplex* signal1 = new cufftComplex[len_c];
        cufftComplex* signal2 = new cufftComplex[len_c];

        for (int i = 0, j = 0; j < len_c; i = i + 2, j++)
        {
            signal1[j].x = sourceMass1[i];
            signal1[j].y = sourceMass1[i + 1];
            signal2[j].x = sourceMass2[i];
            signal2[j].y = sourceMass2[i + 1];
        }
        auto t1 = (double) std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count()/1000000;
        ui->te_info->append("Time to copy from file to RAM: "+QString::number(t1)+" ms");

        auto t2 = std::chrono::high_resolution_clock::now();
        cufftComplex* result_of_IFFT;
        result_of_IFFT = FFT_GPU(signal1, signal2, len_c);
        auto t3 = (double) std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - t2).count() / 1000000;
        ui->te_info->append("Time of execution of VKF: "+QString::number(t3)+" ms");

        fftshift(result_of_IFFT, len_c);
        QVector<double> abs_result(len_c);
        abs_result = abs(result_of_IFFT, len_c);

        double Fd = ui->le_fd->text().toInt();

        QVector<double> osb_x(len_c);

        for(int i = 0; i < len_c; i++)
        {
            osb_x[i] = double((-len_c + 2*i))/Fd*1000;
        }

        ui->risunok->graph(0)->setPen(QPen(Qt::blue)); //set the color of the graph line (blue)
        ui->risunok->graph(0)->setName("The real part");
        ui->risunok->xAxis->setRange(osb_x[0], osb_x[len_c-1]);
        double max = *std::max_element(abs_result.begin(), abs_result.end());
        ui->risunok->yAxis->setRange(0, max);
        ui->risunok->graph(0)->setData(osb_x, abs_result);
        ui->risunok->replot();
    }
    else
        statusBar()->showMessage("File open error", 3000);
}

void MainWindow::on_openFile1_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,tr("Select signal recording file"));
    ui->le_filename1->setText(path);
}

void MainWindow::on_openFile2_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,tr("Select signal recording file"));
    ui->le_filename2->setText(path);
}
