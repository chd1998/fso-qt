#include "mainwindow_2.h"
#include "ui_mainwindow_2.h"

MainWindow_2::MainWindow_2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow_2)
{
    ui->setupUi(this);
}

MainWindow_2::~MainWindow_2()
{
    delete ui;
}



void MainWindow_2::on_btn_Select_Driver_2_clicked()
{
    select_Driver_2.show();
}

void MainWindow_2::on_btn_Select_Vehicle_2_clicked()
{
    select_Truck_2.show();
}
