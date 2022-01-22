#include "add_truck.h"
#include "ui_add_vehicle.h"
#include<QString>
#include<QMessageBox>//表格信息显示
#include<QDebug>//输出测试程序运行是否正确
#include <QSqlQuery>
#include <QSqlError>


Add_Truck::Add_Truck(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add_Vehicle)
{
    ui->setupUi(this);

      //*********************卡车函数*************************
    this->model=new QStandardItemModel;//在表格中添加内容
    Add_Truck_Message();//表格初始化
    ui->table_Message->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格不能编辑
    ui->table_Message->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    line=0; //每开一次初始化一次数值
    //this->ui->radioButton->setFocus();
    connect(this->ui->btn_Ok,SIGNAL(clicked(bool)),this,SLOT(Add_Truck_Ok()));
    connect(this->ui->btn_Cancel,SIGNAL(clicked(bool)),this,SLOT(Add_Truck_Close()));

      //*********************小轿车函数*************************
    this->model_2=new QStandardItemModel;//在表格中添加内容
    Add_Car_Message();//表格初始化
     ui->table_Message_2->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格不能编辑
     ui->table_Message_2->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    line_2=0; //每打开一次初始化一次数值
    connect(this->ui->btn_Ok_2,SIGNAL(clicked(bool)),this,SLOT(Add_Car_Ok()));
    connect(this->ui->btn_Cancel_2,SIGNAL(clicked(bool)),this,SLOT(Add_Car_Close()));

    //*********************大客车函数*************************
    this->model_3=new QStandardItemModel;//在表格中添加内容
    Add_Bus_Message();//表格初始化
    ui->table_Message_3->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格不能编辑
    ui->table_Message_3->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    line_3=0; //每打开一次初始化一次数值
    //this->ui->radioButton->setFocus();
    connect(this->ui->btn_Ok_3,SIGNAL(clicked(bool)),this,SLOT(Add_Bus_Ok()));
    connect(this->ui->btn_Cancel_3,SIGNAL(clicked(bool)),this,SLOT(Add_Bus_Close()));

}
int Add_Truck::line=0; //初始化table行数
int Add_Truck::line_2=0; //初始化table行数
int Add_Truck::line_3=0; //初始化table行数
Add_Truck::~Add_Truck()
{
    delete ui;
}


  //*********************卡车函数*************************

//确认后保存信息
void  Add_Truck:: Add_Truck_Ok()
{
    //读取输入内容
    QString id,no,company,type,total_Km,oil_Consume_Km,road_Toll,load_Capacity;
    id=this->ui->line_Id->text();
    no=this->ui->line_No->text();
    company=this->ui->line_Company->text();
    QString buy_Time=this->ui->date_Buy_Time->date().toString("yyyy-MM-dd");
    type=this->ui->line_Type->text();
    total_Km=this->ui->line_Total_Km->text();
    oil_Consume_Km=this->ui->line_Oil_Consume_Km->text();
    road_Toll=this->ui->line_Road_Toll->text();
    load_Capacity=this->ui->line_Load_Capacity->text();

    //判断输入的内容是否符合要求
    if(id.length()<1||no.length()<1)
    {
        QMessageBox::critical(this ,"警告","请重新输入'\n'编号和牌号都不能为空");
    }
    else
    {
     QString content="车辆编号: "+id+'\n'+"车号: "+no+'\n'+"制造方: "+company +'\n'+"购买时间: "+buy_Time+'\n'
                +"型号: "+type+'\n'+"总公里数: "+total_Km+'\n'+"耗油/公里: "+oil_Consume_Km+'\n'+"过费: "+road_Toll+'\n'+
             "载重: "+load_Capacity+'\n';
     int ret =QMessageBox::information(this,"您将要添加的信息",content,QMessageBox::Ok|QMessageBox::Cancel );
     if(ret==QMessageBox::Ok)
     {

       if( Add_Truck_Insert_Table(id,no,company,buy_Time,type,total_Km,oil_Consume_Km,road_Toll,load_Capacity))
       {//如果插入成功表格显示出来
            Add_Truck_Clear_Message();
            Add_Truck_Manage_Print(id,no,company,buy_Time,type,total_Km,oil_Consume_Km,road_Toll,load_Capacity);
       }
       else {
           //如果插入失败提示信息
          QMessageBox::critical(this ,"警告","添加失败，请重新输入");
       }
     }
    }
}
//关闭窗口
void Add_Truck:: Add_Truck_Close()
{
    this->close();
}

//将输入的数据插入数据库中
bool Add_Truck::Add_Truck_Insert_Table(QString id,QString no,QString company,QString buy_Time,QString type,
                                       QString total_Km,QString oil_Consume_Km,QString road_Toll,QString load_Capacity) const
{
    double total_Cost;

    int index=this->ui->oil_Price->currentIndex();
    if(index==0)
    {
        total_Cost=6.22*oil_Consume_Km.toDouble()+1500;
    }
    else if(index==1)
    {
        total_Cost=6.63*oil_Consume_Km.toDouble()+1500;
    }
    else if(index==2)
    {
       total_Cost=7.05*oil_Consume_Km.toDouble()+1500;
    }
    else {
        total_Cost=6.25*oil_Consume_Km.toDouble()+1500;
    }


    QString str = QString("insert into Truck(t_Id, t_No, t_Company,t_Buy_Time,t_Type,t_Total_Km,t_oil_Consume_Km,t_Road_Toll,t_Load_Capacity,t_Total_Cost) values('%1', "
                          "'%2', '%3','%4','%5','%6','%7','%8','%9','%10')").arg(id).arg(no).arg(company)
            .arg(buy_Time).arg(type).arg(total_Km).arg(oil_Consume_Km).arg(road_Toll).arg(load_Capacity).arg(total_Cost);
    QSqlQuery query;
    if(query.exec(str))
    {
      return true;
    }
    return false;
}



//表格初始化
void  Add_Truck::Add_Truck_Message()
{
    this->model->setHorizontalHeaderItem(0,new QStandardItem("车辆编号"));
    this->model->setHorizontalHeaderItem(1,new QStandardItem("车号"));
    this->model->setHorizontalHeaderItem(2,new QStandardItem("公司"));
    this->model->setHorizontalHeaderItem(3,new QStandardItem("购买时间"));
    this->model->setHorizontalHeaderItem(4,new QStandardItem("型号"));
    this->model->setHorizontalHeaderItem(5,new QStandardItem("总公里数"));
    this->model->setHorizontalHeaderItem(6,new QStandardItem("耗油/公里"));
    this->model->setHorizontalHeaderItem(7,new QStandardItem("过费"));
    this->model->setHorizontalHeaderItem(8,new QStandardItem("载重"));

    //将视图与model绑定
    this->ui->table_Message->setModel(model);
    this->ui->table_Message->setColumnWidth(0,110);
    this->ui->table_Message->setColumnWidth(1,100);
    this->ui->table_Message->setColumnWidth(2,80);
    this->ui->table_Message->setColumnWidth(3,110);
    this->ui->table_Message->setColumnWidth(4,70);
    this->ui->table_Message->setColumnWidth(5,90);
    this->ui->table_Message->setColumnWidth(6,90);
    this->ui->table_Message->setColumnWidth(7,70);
    this->ui->table_Message->setColumnWidth(8,70);

}
//初始化输入框内容
void  Add_Truck::Add_Truck_Clear_Message()
{
    this->ui->line_Id->clear();
    this->ui->line_No->clear();
    this->ui->line_Company->clear();
   // this->ui->line_Buy_Time->clear();
    this->ui->line_Type->clear();
    this->ui->line_Total_Km->clear();
    this->ui->line_Oil_Consume_Km->clear();
    this->ui->line_Road_Toll->clear();
    this->ui->line_Load_Capacity->clear();

}

//将输入的信息在表格中显示
void Add_Truck:: Add_Truck_Manage_Print(QString id,QString no,QString company,QString buy_Time,QString type,
                                        QString total_Km,QString oil_Consume_Km,QString road_Toll,QString load_Capacity) const
{
    QString Manager_Message=id+"+"+no+"+"+company+"+"+buy_Time+"+"+type+"+"+total_Km+"+"+oil_Consume_Km+"+"+road_Toll+"+"+load_Capacity;
    QStringList subs=Manager_Message.split("+");
    for(int i=0;i<9;i++)
    {
    this->model->setItem(line,i,new QStandardItem(subs.at(i)));
    }
    line++;
}


  //*********************小轿车函数*************************

//确认后保存信息
void  Add_Truck:: Add_Car_Ok()
{
    //读取输入内容
    QString id,no,company,type,total_Km,oil_Consume_Km,road_Toll,box_Num;
    id=this->ui->line_Id_2->text();
    no=this->ui->line_No_2->text();
    company=this->ui->line_Company_2->text();
    QString buy_Time=this->ui->date_Buy_Time->date().toString("yyyy-MM-dd");
    type=this->ui->line_Type_2->text();
    total_Km=this->ui->line_Total_Km_2->text();
    oil_Consume_Km=this->ui->line_Oil_Consume_Km_2->text();
    road_Toll=this->ui->line_Road_Toll_2->text();
    box_Num=this->ui->line_Box_Num_2->text();

    //判断输入的内容是否符合要求
    if(id.length()<1||no.length()<1)
    {
        QMessageBox::critical(this ,"警告","请重新输入'\n'编号和牌号都不能为空");
    }
    else
    {
     QString content="车辆编号: "+id+'\n'+"车号: "+no+'\n'+"制造方: "+company +'\n'+"购买时间: "+buy_Time+'\n'
                +"型号: "+type+'\n'+"总公里数: "+total_Km+'\n'+"耗油/公里: "+oil_Consume_Km+'\n'+"过费: "+road_Toll+'\n'+
             "箱数: "+box_Num+'\n';
     int ret =QMessageBox::information(this,"您将要添加的信息",content,QMessageBox::Ok|QMessageBox::Cancel );
     if(ret==QMessageBox::Ok)
     {

       if( Add_Car_Insert_Table(id,no,company,buy_Time,type,total_Km,oil_Consume_Km,road_Toll,box_Num))
       {//如果插入成功表格显示出来
            Add_Car_Clear_Message();
            Add_Car_Manage_Print(id,no,company,buy_Time,type,total_Km,oil_Consume_Km,road_Toll,box_Num);
       }
       else {
           //如果插入失败提示信息
          QMessageBox::critical(this ,"警告","添加失败，请重新输入");
       }
     }
    }
}
//关闭窗口
void Add_Truck:: Add_Car_Close()
{
    this->close();
}

//将输入的数据插入数据库中
bool Add_Truck::Add_Car_Insert_Table(QString id,QString no,QString company,QString buy_Time,QString type,
                                       QString total_Km,QString oil_Consume_Km,QString road_Toll,QString box_Num) const
{
    double total_Cost;

    int index=this->ui->oil_Price->currentIndex();
    if(index==0)
    {
        total_Cost=6.22*oil_Consume_Km.toDouble()+1000;
    }
    else if(index==1)
    {
        total_Cost=6.63*oil_Consume_Km.toDouble()+1000;
    }
    else if(index==2)
    {
       total_Cost=7.05*oil_Consume_Km.toDouble()+1000;
    }
    else {
        total_Cost=6.25*oil_Consume_Km.toDouble()+1000;
    }


    QString str = QString("insert into Car(c_Id, c_No, c_Company,c_Buy_Time,c_Type,c_Total_Km,c_oil_Consume_Km,c_Road_Toll,c_Box_Num,c_Total_Cost) values('%1', "
                          "'%2', '%3','%4','%5','%6','%7','%8','%9','%10')").arg(id).arg(no).arg(company)
            .arg(buy_Time).arg(type).arg(total_Km).arg(oil_Consume_Km).arg(road_Toll).arg(box_Num).arg(total_Cost);
    QSqlQuery query;
    if(query.exec(str))
    {
      return true;
    }
    return false;
}



//表格初始化
void  Add_Truck::Add_Car_Message()
{
    this->model_2->setHorizontalHeaderItem(0,new QStandardItem("车辆编号"));
    this->model_2->setHorizontalHeaderItem(1,new QStandardItem("车号"));
    this->model_2->setHorizontalHeaderItem(2,new QStandardItem("公司"));
    this->model_2->setHorizontalHeaderItem(3,new QStandardItem("购买时间"));
    this->model_2->setHorizontalHeaderItem(4,new QStandardItem("型号"));
    this->model_2->setHorizontalHeaderItem(5,new QStandardItem("总公里数"));
    this->model_2->setHorizontalHeaderItem(6,new QStandardItem("耗油/公里"));
    this->model_2->setHorizontalHeaderItem(7,new QStandardItem("过费"));
    this->model_2->setHorizontalHeaderItem(8,new QStandardItem("箱数"));

    //将视图与model_2绑定
    this->ui->table_Message_2->setModel(model_2);
    this->ui->table_Message_2->setColumnWidth(0,110);
    this->ui->table_Message_2->setColumnWidth(1,100);
    this->ui->table_Message_2->setColumnWidth(2,80);
    this->ui->table_Message_2->setColumnWidth(3,110);
    this->ui->table_Message_2->setColumnWidth(4,70);
    this->ui->table_Message_2->setColumnWidth(5,90);
    this->ui->table_Message_2->setColumnWidth(6,90);
    this->ui->table_Message_2->setColumnWidth(7,70);
    this->ui->table_Message_2->setColumnWidth(8,70);

}
//初始化输入框内容
void  Add_Truck::Add_Car_Clear_Message()
{
    this->ui->line_Id_2->clear();
    this->ui->line_No_2->clear();
    this->ui->line_Company_2->clear();
   // this->ui->line_Buy_Time->clear();
    this->ui->line_Type_2->clear();
    this->ui->line_Total_Km_2->clear();
    this->ui->line_Oil_Consume_Km_2->clear();
    this->ui->line_Road_Toll_2->clear();
    this->ui->line_Box_Num_2->clear();

}

//将输入的信息在表格中显示
void Add_Truck:: Add_Car_Manage_Print(QString id,QString no,QString company,QString buy_Time,QString type,
                                        QString total_Km,QString oil_Consume_Km,QString road_Toll,QString box_Num) const
{
    QString Manager_Message=id+"+"+no+"+"+company+"+"+buy_Time+"+"+type+"+"+total_Km+"+"+oil_Consume_Km+"+"+road_Toll+"+"+box_Num;
    QStringList subs=Manager_Message.split("+");
    for(int i=0;i<9;i++)
    {
    this->model_2->setItem(line_2,i,new QStandardItem(subs.at(i)));
    }
    line_2++;
}



  //*********************大客车函数*************************


//确认后保存信息
void  Add_Truck:: Add_Bus_Ok()
{
    //读取输入内容
    QString id,no,company,type,total_Km,oil_Consume_Km,road_Toll,max_Passengers;
    id=this->ui->line_Id_3->text();
    no=this->ui->line_No_3->text();
    company=this->ui->line_Company_3->text();
    QString buy_Time=this->ui->date_Buy_Time->date().toString("yyyy-MM-dd");
    type=this->ui->line_Type_3->text();
    total_Km=this->ui->line_Total_Km_3->text();
    oil_Consume_Km=this->ui->line_Oil_Consume_Km_3->text();
    road_Toll=this->ui->line_Road_Toll_3->text();
    max_Passengers=this->ui->line_Max_Passengers_3->text();

    //判断输入的内容是否符合要求
    if(id.length()<1||no.length()<1)
    {
        QMessageBox::critical(this ,"警告","请重新输入'\n'编号和牌号都不能为空");
    }
    else
    {
     QString content="车辆编号: "+id+'\n'+"车号: "+no+'\n'+"制造方: "+company +'\n'+"购买时间: "+buy_Time+'\n'
                +"型号: "+type+'\n'+"总公里数: "+total_Km+'\n'+"耗油/公里: "+oil_Consume_Km+'\n'+"过费: "+road_Toll+'\n'+
             "最大载客量: "+max_Passengers+'\n';
     int ret =QMessageBox::information(this,"您将要添加的信息",content,QMessageBox::Ok|QMessageBox::Cancel );
     if(ret==QMessageBox::Ok)
     {

       if( Add_Bus_Insert_Table(id,no,company,buy_Time,type,total_Km,oil_Consume_Km,road_Toll,max_Passengers))
       {//如果插入成功表格显示出来
            Add_Bus_Clear_Message();
            Add_Bus_Manage_Print(id,no,company,buy_Time,type,total_Km,oil_Consume_Km,road_Toll,max_Passengers);
       }
       else {
           //如果插入失败提示信息
          QMessageBox::critical(this ,"警告","添加失败，请重新输入");
       }
     }
    }
}
//关闭窗口
void Add_Truck:: Add_Bus_Close()
{
    this->close();
}

//将输入的数据插入数据库中
bool Add_Truck::Add_Bus_Insert_Table(QString id,QString no,QString company,QString buy_Time,QString type,
                                       QString total_Km,QString oil_Consume_Km,QString road_Toll,QString max_Passengers) const
{
    double total_Cost;

    int index=this->ui->oil_Price->currentIndex();
    if(index==0)
    {
        total_Cost=6.22*oil_Consume_Km.toDouble()+2000;
    }
    else if(index==1)
    {
        total_Cost=6.63*oil_Consume_Km.toDouble()+2000;
    }
    else if(index==2)
    {
       total_Cost=7.05*oil_Consume_Km.toDouble()+2000;
    }
    else {
        total_Cost=6.25*oil_Consume_Km.toDouble()+2000;
    }


    QString str = QString("insert into Bus(b_Id, b_No, b_Company,b_Buy_Time,b_Type,b_Total_Km,b_oil_Consume_Km,b_Road_Toll,b_Max_Passengers,b_Total_Cost) values('%1', "
                          "'%2', '%3','%4','%5','%6','%7','%8','%9','%10')").arg(id).arg(no).arg(company)
            .arg(buy_Time).arg(type).arg(total_Km).arg(oil_Consume_Km).arg(road_Toll).arg(max_Passengers).arg(total_Cost);
    QSqlQuery query;
    if(query.exec(str))
    {
      return true;
    }
    return false;
}



//表格初始化
void  Add_Truck::Add_Bus_Message()
{
    this->model_3->setHorizontalHeaderItem(0,new QStandardItem("车辆编号"));
    this->model_3->setHorizontalHeaderItem(1,new QStandardItem("车号"));
    this->model_3->setHorizontalHeaderItem(2,new QStandardItem("公司"));
    this->model_3->setHorizontalHeaderItem(3,new QStandardItem("购买时间"));
    this->model_3->setHorizontalHeaderItem(4,new QStandardItem("型号"));
    this->model_3->setHorizontalHeaderItem(5,new QStandardItem("总公里数"));
    this->model_3->setHorizontalHeaderItem(6,new QStandardItem("耗油/公里"));
    this->model_3->setHorizontalHeaderItem(7,new QStandardItem("过费"));
    this->model_3->setHorizontalHeaderItem(8,new QStandardItem("最大载客量"));

    //将视图与model_3绑定
    this->ui->table_Message_3->setModel(model_3);
    this->ui->table_Message_3->setColumnWidth(0,110);
    this->ui->table_Message_3->setColumnWidth(1,100);
    this->ui->table_Message_3->setColumnWidth(2,80);
    this->ui->table_Message_3->setColumnWidth(3,110);
    this->ui->table_Message_3->setColumnWidth(4,70);
    this->ui->table_Message_3->setColumnWidth(5,90);
    this->ui->table_Message_3->setColumnWidth(6,90);
    this->ui->table_Message_3->setColumnWidth(7,70);
    this->ui->table_Message_3->setColumnWidth(8,120);

}
//初始化输入框内容
void  Add_Truck::Add_Bus_Clear_Message()
{
    this->ui->line_Id_3->clear();
    this->ui->line_No_3->clear();
    this->ui->line_Company_3->clear();
   // this->ui->line_Buy_Time->clear();
    this->ui->line_Type_3->clear();
    this->ui->line_Total_Km_3->clear();
    this->ui->line_Oil_Consume_Km_3->clear();
    this->ui->line_Road_Toll_3->clear();
    this->ui->line_Max_Passengers_3->clear();

}

//将输入的信息在表格中显示
void Add_Truck:: Add_Bus_Manage_Print(QString id,QString no,QString company,QString buy_Time,QString type,
                                        QString total_Km,QString oil_Consume_Km,QString road_Toll,QString max_Passengers) const
{
    QString Manager_Message=id+"+"+no+"+"+company+"+"+buy_Time+"+"+type+"+"+total_Km+"+"+oil_Consume_Km+"+"+road_Toll+"+"+max_Passengers;
    QStringList subs=Manager_Message.split("+");
    for(int i=0;i<9;i++)
    {
    this->model_3->setItem(line_3,i,new QStandardItem(subs.at(i)));
    }
    line_3++;
}
