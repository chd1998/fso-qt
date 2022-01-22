#include "add_driver.h"
#include "ui_add_driver.h"
#include"select_truck.h"
#include<QMessageBox>//表格信息显示
#include<QDebug>
#include <QSqlQuery>
#include <QSqlError>
Add_Driver::Add_Driver(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add_Driver)
{
    ui->setupUi(this);
    connect(this->ui->btn_ok,SIGNAL(clicked(bool)),this,SLOT(Add_Driver_Message()));
    connect(this->ui->btn_no,SIGNAL(clicked(bool)),this,SLOT(close()));
}
Add_Driver::~Add_Driver()
{
    delete ui;
}


//向数据库中插入司机信息
void Add_Driver::Add_Driver_Message()
{
    int ret =QMessageBox::information(this,"添加信息","确认是否添加司机",QMessageBox::Ok|QMessageBox::Cancel );
    if(ret==QMessageBox::Ok)
    {
    QString name=this->ui->line_Name->text();
    QString gender=this->ui->btn_Group_Gender->checkedButton()->text();
    double km=this->ui->line_Km->text().toDouble();
    double fee_Km=this->ui->line_Fee_Km->text().toDouble();
    double salary=km*fee_Km+5000;
    Receive_Message(id,no,type);
    QString insert_Driver_Message=QString("insert into Driver (d_Id,d_No,d_Type,d_Name,d_Gender,d_Km,d_Fee_Km,d_Salary) "
                                          "values('%1','%2','%3','%4','%5',%6,%7,%8)").arg(id).arg(no).arg(type).arg(name)
            .arg(gender).arg(km).arg(fee_Km).arg(salary);
    QSqlQuery query;
    if(query.exec(insert_Driver_Message))
    {
        QMessageBox::information(this,"提示","添加成功",QMessageBox::Ok);
        this->close();
    }
    else
    {
        QMessageBox::critical(this,"提示","添加失败",QMessageBox::Ok|QMessageBox::Cancel );
    }
   // qDebug()<<name<<gender<<km<<fee_Km<<id<<no<<type<<salary<<"Add_Driver_Message";
    }
    else {
        return;
    }
}

//接受信息
void Add_Driver::Receive_Message(QString id,QString no,QString type)
{
    this->id=id;
    this->no=no;
    this->type=type;
  //  qDebug()<<id<<no<<type<<"Receive_Message";
}
