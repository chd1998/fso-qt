#include "select_driver_2.h"
#include "ui_select_driver_2.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>//表格信息显示
#include <QDebug>//输出测试程序运行是否正确
#include <QString>


Select_Driver_2::Select_Driver_2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Select_Driver_2)
{
    ui->setupUi(this);
    this->model=new QStandardItemModel;//在表格中添加内容
    Select__Driver_New_Table();//表格初始化
    ui->table_Message->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格不能编辑
    ui->table_Message->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    connect(this->ui->btn_Select,SIGNAL(clicked(bool)),this,SLOT(Select__Driver_Ok()));
    connect(this->ui->line_Select_Message,SIGNAL(textChanged(QString)),this,SLOT(Select__Driver_Input()));
}
int Select_Driver_2::line=0; //静态成员"行数"初始化
Select_Driver_2::~Select_Driver_2()
{
    delete ui;
}

//确认按钮后查询信息
void Select_Driver_2::Select__Driver_Ok()
{
  //qDebug()<<"测试按钮响应"<<endl;
   QString Line_Message=ui->line_Select_Message->text();//读取文本框内信息
  Select__Driver_Select_Print(Line_Message);
}

//输入信息后查询
void Select_Driver_2::Select__Driver_Input()
{
   // qDebug()<<"测试输入响应"<<endl;
    QString Line_Message=ui->line_Select_Message->text();//读取文本框内信息
    Select__Driver_Select_Print(Line_Message);
}

//查询结果输出
void Select_Driver_2::Select__Driver_Select_Print(QString Line_Message)
{
    this->model->clear();//每次查询都要清除查询信息（重点）
    Select__Driver_New_Table();//表格初始化
    line=0;//初始化行数
    int index=this->ui->combo_Box_Select->currentIndex(); //得到选择的序号,共0、1、2、3

    QString temp;//用来暂存SQL语句

    if(index==0)//判断选择的序号，在选择Select的内容
    {
       temp=QString("select * from Driver where d_Id like '%1%'").arg(Line_Message);
    }
    else if(index==1)
    {
       temp=QString("select * from Driver where d_No like '%1%'").arg(Line_Message);
    }
    else if(index==2)
    {
       temp=QString("select * from Driver where d_Type like '%1%'").arg(Line_Message);
    }
    else {
       temp=QString("select * from Driver where d_Name like '%1%'").arg(Line_Message);
    }
     QSqlQuery my_Query; //查询
     if(my_Query.exec(temp))
     {
       //输出查询结果，从line=0开始输出
       while(my_Query.next()) //一直到结尾才推出循环
       {
        QString id=my_Query.value(0).toString();
        QString no=my_Query.value(1).toString();
        QString type=my_Query.value(2).toString();
        QString name=my_Query.value(3).toString();
        QString gender=my_Query.value(4).toString();
        QString km=my_Query.value(5).toString();
        QString fee_Km=my_Query.value(6).toString();
        QString base_Salary=my_Query.value(7).toString();
        QString salary=my_Query.value(8).toString();
        //每次将一行结果输出到表格中
        Select__Driver_Message_Print(id,no, type,name,gender,km,fee_Km, base_Salary,salary) ;
       }
     }
     else
     {
        Select__Driver_New_Table();//表格初始化
     }
}

//***********初始建立表格**************

//表格初始化
void  Select_Driver_2::Select__Driver_New_Table()
{
    this->model->setHorizontalHeaderItem(0,new QStandardItem("车辆编号"));
    //添加司机标志
    this->model->setHorizontalHeaderItem(1,new QStandardItem("车号"));
    this->model->setHorizontalHeaderItem(2,new QStandardItem("车辆类型"));
    this->model->setHorizontalHeaderItem(3,new QStandardItem("姓名"));
    this->model->setHorizontalHeaderItem(4,new QStandardItem("性别"));
    this->model->setHorizontalHeaderItem(5,new QStandardItem("行驶里程"));
    this->model->setHorizontalHeaderItem(6,new QStandardItem("耗油/公里"));
    this->model->setHorizontalHeaderItem(7,new QStandardItem("基本工资"));
    this->model->setHorizontalHeaderItem(8,new QStandardItem("工资"));

    //将视图与model绑定
    this->ui->table_Message->setModel(model);
    this->ui->table_Message->setColumnWidth(0,110);
    this->ui->table_Message->setColumnWidth(1,110);
    this->ui->table_Message->setColumnWidth(2,100);
    this->ui->table_Message->setColumnWidth(3,60);
    this->ui->table_Message->setColumnWidth(4,70);
    this->ui->table_Message->setColumnWidth(5,70);
    this->ui->table_Message->setColumnWidth(6,90);
    this->ui->table_Message->setColumnWidth(7,110);
    this->ui->table_Message->setColumnWidth(8,70);
}


//将输入的信息在表格中显示
void Select_Driver_2::Select__Driver_Message_Print(QString id,QString no,QString type,QString name,QString gender,
                                                QString km,QString fee_Km,QString base_Salary, QString salary) const
{
    QString Manager_Message=id+"+"+no+"+"+type+"+"+name+"+"+gender+"+"+km+"+"+fee_Km+"+"+
            base_Salary+"+"+salary;
    QStringList subs=Manager_Message.split("+");
    for(int i=0;i<9;i++)
    {
    this->model->setItem(line,i,new QStandardItem(subs.at(i)));
    }
    line++;
}


