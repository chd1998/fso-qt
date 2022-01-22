#include "select_truck_2.h"
#include "ui_select_truck_2.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>//表格信息显示
#include <QDebug>//输出测试程序运行是否正确
#include <QString>


Select_Truck_2::Select_Truck_2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Select_Truck_2)
{
    ui->setupUi(this);
    //**************************卡车函数**********************************
    this->model=new QStandardItemModel;//在表格中添加内容
    Select_Truck_New_Table();//表格初始化
    ui->table_Message->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格不能编辑
    ui->table_Message->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    connect(this->ui->btn_Select,SIGNAL(clicked(bool)),this,SLOT(Select_Truck_Ok()));
    connect(this->ui->line_Select_Message,SIGNAL(textChanged(QString)),this,SLOT(Select_Truck_Input()));
    connect(this->ui->table_Message, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(Click_On_Table_Message(const QModelIndex &)));//表格点击触发


    //**************************小轿车函数**********************************

    this->model_2=new QStandardItemModel;//在表格中添加内容
    Select_Car_New_Table();//表格初始化
    ui->table_Message_21->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格不能编辑
    ui->table_Message_21->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    connect(this->ui->btn_Select_2,SIGNAL(clicked(bool)),this,SLOT(Select_Car_Ok()));
    connect(this->ui->line_Select_Message_2,SIGNAL(textChanged(QString)),this,SLOT(Select_Car_Input()));

    //**************************大客车函数**********************************
    this->model_3=new QStandardItemModel;//在表格中添加内容
    Select_Bus_New_Table();//表格初始化
    ui->table_Message_3->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格不能编辑
    ui->table_Message_3->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    connect(this->ui->btn_Select_3,SIGNAL(clicked(bool)),this,SLOT(Select_Bus_Ok()));
    connect(this->ui->line_Select_Message_3,SIGNAL(textChanged(QString)),this,SLOT(Select_Bus_Input()));
}
int Select_Truck_2::line=0; //静态成员"行数"初始化
int Select_Truck_2::line_2=0; //静态成员"行数"初始化
int Select_Truck_2::line_3=0; //静态成员"行数"初始化
Select_Truck_2::~Select_Truck_2()
{
    delete ui;
}

//************查询信息功能*************

//确认按钮后查询信息
void Select_Truck_2::Select_Truck_Ok()
{
  //qDebug()<<"测试按钮响应"<<endl;
   QString Line_Message=ui->line_Select_Message->text();//读取文本框内信息
  Select_Truck_Select_Print(Line_Message);
}

//输入信息后查询
void Select_Truck_2::Select_Truck_Input()
{
   // qDebug()<<"测试输入响应"<<endl;
    QString Line_Message=ui->line_Select_Message->text();//读取文本框内信息
    Select_Truck_Select_Print(Line_Message);
}

//查询结果输出
void Select_Truck_2::Select_Truck_Select_Print(QString Line_Message)
{
    this->model->clear();//每次查询都要清除查询信息（重点）
    Select_Truck_New_Table();//表格初始化
    line=0;//初始化行数
    int index=this->ui->combo_Box_Select->currentIndex(); //得到选择的序号,共0、1、2、3

    QString temp;//用来暂存SQL语句

    if(index==0)//判断选择的序号，在选择Select的内容
    {
       temp=QString("select * from Truck where t_Id like '%1%'").arg(Line_Message);
    }
    else if(index==1)
    {
       temp=QString("select * from Truck where t_No like '%1%'").arg(Line_Message);
    }
    else if(index==2)
    {
       temp=QString("select * from Truck where t_Company like '%1%'").arg(Line_Message);
    }
    else {
       temp=QString("select * from Truck where t_Buy_Time like '%1%'").arg(Line_Message);
    }
     QSqlQuery my_Query; //查询
     if(my_Query.exec(temp))
     {
       //输出查询结果，从line=0开始输出
       while(my_Query.next()) //一直到结尾才推出循环
       {
        QString id=my_Query.value(0).toString();
        QString no=my_Query.value(1).toString();
        QString company=my_Query.value(2).toString();
        QString buy_Time=my_Query.value(3).toString();
        QString type=my_Query.value(4).toString();
        QString total_Km=my_Query.value(5).toString();
        QString oil_Consume_Km=my_Query.value(6).toString();
        QString basic_Maintenance_Fee=my_Query.value(7).toString();
        QString road_Toll=my_Query.value(8).toString();
        QString total_Cost=my_Query.value(9).toString();
        QString load_Capacity=my_Query.value(10).toString();
        //每次将一行结果输出到表格中
        Select_Truck_Message_Print(id, no,company, buy_Time,type,
                                           total_Km, oil_Consume_Km,basic_Maintenance_Fee,
                                            road_Toll,total_Cost,load_Capacity);
       }
     }
     else
     {
        Select_Truck_New_Table();//表格初始化
     }
}

//***********初始建立表格**************

//表格初始化
void  Select_Truck_2::Select_Truck_New_Table()
{
    this->model->setHorizontalHeaderItem(0,new QStandardItem("车辆编号"));
    //添加司机标志
    this->model->setHorizontalHeaderItem(1,new QStandardItem("车号"));
    this->model->setHorizontalHeaderItem(2,new QStandardItem("公司"));
    this->model->setHorizontalHeaderItem(3,new QStandardItem("购买时间"));
    this->model->setHorizontalHeaderItem(4,new QStandardItem("型号"));
    this->model->setHorizontalHeaderItem(5,new QStandardItem("总公里数"));
    this->model->setHorizontalHeaderItem(6,new QStandardItem("耗油/公里"));
    this->model->setHorizontalHeaderItem(7,new QStandardItem("基本维护RMB"));
    this->model->setHorizontalHeaderItem(8,new QStandardItem("过路RMB"));
    this->model->setHorizontalHeaderItem(9,new QStandardItem("每月RMB"));
    this->model->setHorizontalHeaderItem(10,new QStandardItem("载重"));

    //将视图与model绑定
    this->ui->table_Message->setModel(model);
    this->ui->table_Message->setColumnWidth(0,110);
    this->ui->table_Message->setColumnWidth(1,100);
    this->ui->table_Message->setColumnWidth(2,110);
    this->ui->table_Message->setColumnWidth(3,140);
    this->ui->table_Message->setColumnWidth(4,70);
    this->ui->table_Message->setColumnWidth(5,70);
    this->ui->table_Message->setColumnWidth(6,90);
    this->ui->table_Message->setColumnWidth(7,140);
    this->ui->table_Message->setColumnWidth(8,70);
    this->ui->table_Message->setColumnWidth(9,70);
    this->ui->table_Message->setColumnWidth(10,70);
}

//将输入的信息在表格中显示
void Select_Truck_2::Select_Truck_Message_Print(QString id,QString no,QString company,QString buy_Time,QString type,
                                              QString total_Km,QString oil_Consume_Km,QString basic_Maintenance_Fee,
                                              QString road_Toll,QString total_Cost,QString load_Capacity) const
{
    QString Manager_Message=id+"+"+no+"+"+company+"+"+buy_Time+"+"+type+"+"+total_Km+"+"+oil_Consume_Km+"+"+
            basic_Maintenance_Fee+"+"+road_Toll+"+"+total_Cost +"+"+load_Capacity;
    QStringList subs=Manager_Message.split("+");
    for(int i=0;i<11;i++)
    {
    this->model->setItem(line,i,new QStandardItem(subs.at(i)));
    }
    line++;
}



//**************************小轿车函数**********************************


//************查询信息功能*************

//确认按钮后查询信息
void Select_Truck_2::Select_Car_Ok()
{
  //qDebug()<<"测试按钮响应"<<endl;
   QString Line_Message=ui->line_Select_Message_2->text();//读取文本框内信息
  Select_Car_Select_Print(Line_Message);
}

//输入信息后查询
void Select_Truck_2::Select_Car_Input()
{
   // qDebug()<<"测试输入响应"<<endl;
    QString Line_Message=ui->line_Select_Message_2->text();//读取文本框内信息
    Select_Car_Select_Print(Line_Message);
}

//查询结果输出
void Select_Truck_2::Select_Car_Select_Print(QString Line_Message)
{
    this->model_2->clear();//每次查询都要清除查询信息（重点）
    Select_Car_New_Table();//表格初始化
    line_2=0;//初始化行数
    int index=this->ui->combo_Box_Select_2->currentIndex(); //得到选择的序号,共0、1、2、3

    QString temp;//用来暂存SQL语句

    if(index==0)//判断选择的序号，在选择Select的内容
    {
       temp=QString("select * from Car where c_Id like '%1%'").arg(Line_Message);
    }
    else if(index==1)
    {
       temp=QString("select * from Car where c_No like '%1%'").arg(Line_Message);
    }
    else if(index==2)
    {
       temp=QString("select * from Car where c_Company like '%1%'").arg(Line_Message);
    }
    else {
       temp=QString("select * from Car where c_Buy_Time like '%1%'").arg(Line_Message);
    }
     QSqlQuery my_Query; //查询
     if(my_Query.exec(temp))
     {
       //输出查询结果，从line=0开始输出
       while(my_Query.next()) //一直到结尾才推出循环
       {
        QString id=my_Query.value(0).toString();
        QString no=my_Query.value(1).toString();
        QString company=my_Query.value(2).toString();
        QString buy_Time=my_Query.value(3).toString();
        QString type=my_Query.value(4).toString();
        QString total_Km=my_Query.value(5).toString();
        QString oil_Consume_Km=my_Query.value(6).toString();
        QString basic_Maintenance_Fee=my_Query.value(7).toString();
        QString road_Toll=my_Query.value(8).toString();
        QString total_Cost=my_Query.value(9).toString();
        QString box_Num=my_Query.value(10).toString();
        //每次将一行结果输出到表格中
        Select_Car_Message_Print(id, no,company, buy_Time,type,
                                           total_Km, oil_Consume_Km,basic_Maintenance_Fee,
                                            road_Toll,total_Cost,box_Num);
       }
     }
     else
     {
        Select_Car_New_Table();//表格初始化
     }
}

//***********初始建立表格**************

//表格初始化
void  Select_Truck_2::Select_Car_New_Table()
{
    this->model_2->setHorizontalHeaderItem(0,new QStandardItem("车辆编号"));
    this->model_2->setHorizontalHeaderItem(1,new QStandardItem("车号"));
    this->model_2->setHorizontalHeaderItem(2,new QStandardItem("公司"));
    this->model_2->setHorizontalHeaderItem(3,new QStandardItem("购买时间"));
    this->model_2->setHorizontalHeaderItem(4,new QStandardItem("型号"));
    this->model_2->setHorizontalHeaderItem(5,new QStandardItem("总公里数"));
    this->model_2->setHorizontalHeaderItem(6,new QStandardItem("耗油/公里"));
    this->model_2->setHorizontalHeaderItem(7,new QStandardItem("基本维护RMB"));
    this->model_2->setHorizontalHeaderItem(8,new QStandardItem("过路RMB"));
    this->model_2->setHorizontalHeaderItem(9,new QStandardItem("每月RMB"));
    this->model_2->setHorizontalHeaderItem(10,new QStandardItem("箱数"));

    //将视图与model_2绑定
    this->ui->table_Message_21->setModel(model_2);
    this->ui->table_Message_21->setColumnWidth(0,110);
    this->ui->table_Message_21->setColumnWidth(1,100);
    this->ui->table_Message_21->setColumnWidth(2,80);
    this->ui->table_Message_21->setColumnWidth(3,140);
    this->ui->table_Message_21->setColumnWidth(4,70);
    this->ui->table_Message_21->setColumnWidth(5,70);
    this->ui->table_Message_21->setColumnWidth(6,90);
    this->ui->table_Message_21->setColumnWidth(7,140);
    this->ui->table_Message_21->setColumnWidth(8,70);
    this->ui->table_Message_21->setColumnWidth(9,70);
    this->ui->table_Message_21->setColumnWidth(10,70);
}



//将输入的信息在表格中显示
void Select_Truck_2::Select_Car_Message_Print(QString id,QString no,QString company,QString buy_Time,QString type,
                                              QString total_Km,QString oil_Consume_Km,QString basic_Maintenance_Fee,
                                              QString road_Toll,QString total_Cost,QString box_Num) const
{
    QString Manager_Message=id+"+"+no+"+"+company+"+"+buy_Time+"+"+type+"+"+total_Km+"+"+oil_Consume_Km+"+"+
            basic_Maintenance_Fee+"+"+road_Toll+"+"+total_Cost +"+"+box_Num;
    QStringList subs=Manager_Message.split("+");
    for(int i=0;i<11;i++)
    {
    this->model_2->setItem(line_2,i,new QStandardItem(subs.at(i)));
    }
    line_2++;
}


//**************************大客车函数**********************************

//************查询信息功能*************

//确认按钮后查询信息
void Select_Truck_2::Select_Bus_Ok()
{
  //qDebug()<<"测试按钮响应"<<endl;
   QString Line_Message=ui->line_Select_Message_3->text();//读取文本框内信息
  Select_Bus_Select_Print(Line_Message);
}

//输入信息后查询
void Select_Truck_2::Select_Bus_Input()
{
   // qDebug()<<"测试输入响应"<<endl;
    QString Line_Message=ui->line_Select_Message_3->text();//读取文本框内信息
    Select_Bus_Select_Print(Line_Message);
}

//查询结果输出
void Select_Truck_2::Select_Bus_Select_Print(QString Line_Message)
{
    this->model_3->clear();//每次查询都要清除查询信息（重点）
    Select_Bus_New_Table();//表格初始化
    line_3=0;//初始化行数
    int index=this->ui->combo_Box_Select_3->currentIndex(); //得到选择的序号,共0、1、2、3

    QString temp;//用来暂存SQL语句

    if(index==0)//判断选择的序号，在选择Select的内容
    {
       temp=QString("select * from Bus where b_Id like '%1%'").arg(Line_Message);
    }
    else if(index==1)
    {
       temp=QString("select * from Bus where b_No like '%1%'").arg(Line_Message);
    }
    else if(index==2)
    {
       temp=QString("select * from Bus where b_Company like '%1%'").arg(Line_Message);
    }
    else {
       temp=QString("select * from Bus where b_Buy_Time like '%1%'").arg(Line_Message);
    }
     QSqlQuery my_Query; //查询
     if(my_Query.exec(temp))
     {
       //输出查询结果，从line=0开始输出
       while(my_Query.next()) //一直到结尾才推出循环
       {
        QString id=my_Query.value(0).toString();
        QString no=my_Query.value(1).toString();
        QString company=my_Query.value(2).toString();
        QString buy_Time=my_Query.value(3).toString();
        QString type=my_Query.value(4).toString();
        QString total_Km=my_Query.value(5).toString();
        QString oil_Consume_Km=my_Query.value(6).toString();
        QString basic_Maintenance_Fee=my_Query.value(7).toString();
        QString road_Toll=my_Query.value(8).toString();
        QString total_Cost=my_Query.value(9).toString();
        QString max_Passengers=my_Query.value(10).toString();
        //每次将一行结果输出到表格中
        Select_Bus_Message_Print(id, no,company, buy_Time,type,
                                           total_Km, oil_Consume_Km,basic_Maintenance_Fee,
                                            road_Toll,total_Cost,max_Passengers);
       }
     }
     else
     {
        Select_Bus_New_Table();//表格初始化
     }
}

//***********初始建立表格**************

//表格初始化
void  Select_Truck_2::Select_Bus_New_Table()
{
    this->model_3->setHorizontalHeaderItem(0,new QStandardItem("车辆编号"));
    this->model_3->setHorizontalHeaderItem(1,new QStandardItem("车号"));
    this->model_3->setHorizontalHeaderItem(2,new QStandardItem("公司"));
    this->model_3->setHorizontalHeaderItem(3,new QStandardItem("购买时间"));
    this->model_3->setHorizontalHeaderItem(4,new QStandardItem("型号"));
    this->model_3->setHorizontalHeaderItem(5,new QStandardItem("总公里数"));
    this->model_3->setHorizontalHeaderItem(6,new QStandardItem("耗油/公里"));
    this->model_3->setHorizontalHeaderItem(7,new QStandardItem("基本维护RMB"));
    this->model_3->setHorizontalHeaderItem(8,new QStandardItem("过路RMB"));
    this->model_3->setHorizontalHeaderItem(9,new QStandardItem("每月RMB"));
    this->model_3->setHorizontalHeaderItem(10,new QStandardItem("最大载客量"));

    //将视图与model_3绑定
    this->ui->table_Message_3->setModel(model_3);
    this->ui->table_Message_3->setColumnWidth(0,110);
    this->ui->table_Message_3->setColumnWidth(1,100);
    this->ui->table_Message_3->setColumnWidth(2,80);
    this->ui->table_Message_3->setColumnWidth(3,140);
    this->ui->table_Message_3->setColumnWidth(4,70);
    this->ui->table_Message_3->setColumnWidth(5,70);
    this->ui->table_Message_3->setColumnWidth(6,90);
    this->ui->table_Message_3->setColumnWidth(7,110);
    this->ui->table_Message_3->setColumnWidth(8,90);
    this->ui->table_Message_3->setColumnWidth(9,90);
    this->ui->table_Message_3->setColumnWidth(10,110);
}


//将输入的信息在表格中显示
void Select_Truck_2::Select_Bus_Message_Print(QString id,QString no,QString company,QString buy_Time,QString type,
                                              QString total_Km,QString oil_Consume_Km,QString basic_Maintenance_Fee,
                                              QString road_Toll,QString total_Cost,QString max_Passengers) const
{
    QString Manager_Message=id+"+"+no+"+"+company+"+"+buy_Time+"+"+type+"+"+total_Km+"+"+oil_Consume_Km+"+"+
            basic_Maintenance_Fee+"+"+road_Toll+"+"+total_Cost +"+"+max_Passengers;
    QStringList subs=Manager_Message.split("+");
    for(int i=0;i<11;i++)
    {
    this->model_3->setItem(line_3,i,new QStandardItem(subs.at(i)));
    }
    line_3++;
}




