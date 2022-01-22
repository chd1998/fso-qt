#include "login_management.h"
#include "ui_login_management.h"
#include "QDebug"
#include "QString"
#include "QMessageBox"
#include "apply_account.h"
#include <QSqlQuery>
#include <QSqlError>
#include "mainwindow.h" //主窗口
#include <QApplication>



login_Management::login_Management(QWidget *parent) :
    QMainWindow (parent),
    ui(new Ui::login_Management)
{
    ui->setupUi(this);
    Open_Database();
    connect(this->ui->btn_Login,SIGNAL(clicked(bool)),this,SLOT(Login()));
}

login_Management::~login_Management()
{
    delete ui;
}
//************连接数据库************
bool login_Management::Open_Database()
{
     this->ui->lineEdit->setFocus();
    //QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");   //数据库驱动类型为SQL Server
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");   //数据库驱动类型为SQL Server
    db.setHostName("127.0.0.1");                            //选择本地主机，127.0.0.1
    db.setDatabaseName("Vehicle_Management");
    db.setUserName("Vehicle_Management");                       //登录用户
    db.setPassword("songzhe123");                              //密码
    if(!db.open())                                             //打开数据库
    {
        qDebug()<<db.lastError().text();
        QMessageBox::critical(0, QObject::tr("Database error"), db.lastError().text());
        return false ;                                   //打开失败
    }
    return true;
}
//登录操作
void login_Management::Login()
{
    QSqlQuery mysql;
    QString name=this->ui->lineEdit->text();
    QString passwd=this->ui->lineEdit_2->text();
    QString str=QString("select * from Account where a_Id='%1' and a_Password='%2'").arg(name).arg(passwd);
    mysql.exec(str);
    if(name==""||passwd=="")
       { QMessageBox::critical(this,"登录情况","请输入账号或密码");
        return ;}
   if(mysql.first())
      {
       QMessageBox::information(this,"登录情况","登录成功");//后接管理界面
       this->hide();
         if(name.at(0)=='9')
         {
          w.setWindowTitle("车辆管理系统_管理员版");
          w.show();
         }
         else
         {
              w_2.setWindowTitle("车辆管理系统");
              w_2.show();

         }
      }
    else
        QMessageBox::critical(this,"登录情况","密码不正确！");
}

//申请账号
void login_Management::on_btn_Forget_clicked()
{
      Apply_Account apply;
      apply.exec();
}
