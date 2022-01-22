#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "apply_account.h"//申请账号
#include"add_truck.h"//添加卡车
#include "QDebug"
#include "QString"
#include "QMessageBox"
#include "apply_account.h"
#include <QSqlQuery>
#include <QSqlError>
#include "mainwindow.h" //主窗口
#include <QApplication>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this->ui->btn_Add_Truck,SIGNAL(clicked(bool)),this,SLOT(Main_Add_Truck()));
    connect(this->ui->btn_Select_Truck,SIGNAL(clicked(bool)),this,SLOT(Main_Select_Truck()));
    connect(this->ui->btn_Select_Driver,SIGNAL(clicked(bool)),this,SLOT(Select_My_Driver()));

    //连接数据库
   //QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");   //数据库驱动类型为SQL Server
   QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");   //数据库驱动类型为mysql
   db.setHostName("127.0.0.1");                            //选择本地主机，127.0.0.1
   db.setDatabaseName("Vehicle_Management");
   db.setUserName("Vehicle_Management");                       //登录用户
   db.setPassword("songzhe123");                              //密码
   if(!db.open())                                             //打开数据库
   {
       qDebug()<<db.lastError().text();
       QMessageBox::critical(0, QObject::tr("Database error"), db.lastError().text());                               //打开失败
   }
    qDebug()<<"连接成功"<<Qt::endl;
}

MainWindow::~MainWindow()
{
    delete ui;
}

//查询司机触发
void MainWindow::Select_My_Driver()
{
    select_Driver.show();
}
void MainWindow::on_action_triggered()
{
    Apply_Account apply;
    apply.exec();
}
void MainWindow::Main_Add_Truck()
{
    add.show();
}
void MainWindow::Main_Select_Truck()
{
    select.show();
}
