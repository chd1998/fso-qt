#include "apply_account.h"
#include "ui_apply_account.h"
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include "QMessageBox"
Apply_Account::Apply_Account(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Apply_Account)
{
    ui->setupUi(this);
    this->ui->lineEdit->setFocus();
    connect(this->ui->btn_ok,SIGNAL(clicked(bool)),this,SLOT(clicked_Ok()));
    connect(this->ui->btn_no,SIGNAL(clicked(bool)),this,SLOT(clicked_No()));
}

Apply_Account::~Apply_Account()
{
    delete ui;
}
//密码长度不能小于3，不能大于6；
//关闭操作
void Apply_Account::clicked_No()
{
   this->close();
}
//确认操作
void Apply_Account::clicked_Ok()
{
   QSqlQuery sql;
   QString s;
   QString Id=this->ui->lineEdit->text();
   QString passwd=this->ui->lineEdit_2->text();
   QString passwd2=this->ui->lineEdit_3->text();
   s=QString("insert into Account values('%1','%2')").arg(Id).arg(passwd);
   if(Id.length()<3||passwd.length()<=3)
      { QMessageBox::critical(this,"注册情况","登录账号不小于3位，密码不能小于3位！");
       return;}
   if(passwd!=passwd2)
     { QMessageBox::critical(this,"提示","两次密码不同");return ;}
   if(sql.exec(s))
   {
       QMessageBox::information(this,"注册情况","注册成功");
       this->close();
   }
   else
       QMessageBox::critical(this,"注册情况","注册失败");

}
