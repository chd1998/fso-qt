#include "select_truck.h"
#include "ui_select_truck.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>//表格信息显示
#include <QDebug>//输出测试程序运行是否正确
#include <QString>


Select_Truck::Select_Truck(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Select_Truck)
{
    ui->setupUi(this);

    //**************************卡车函数**********************************
    this->model=new QStandardItemModel;//在表格中添加内容
    this->add  =new Add_Driver;//实例化子窗口对象
    Select_Truck_New_Table();//表格初始化
    ui->table_Message->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格不能编辑
    ui->table_Message->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    connect(this->ui->btn_Select,SIGNAL(clicked(bool)),this,SLOT(Select_Truck_Ok()));
    connect(this->ui->line_Select_Message,SIGNAL(textChanged(QString)),this,SLOT(Select_Truck_Input()));
    connect(this->ui->btn_Delete,SIGNAL(clicked(bool)),this,SLOT(Delete_Truck_Message_Ok()));
    connect(this->ui->btn_Upadte,SIGNAL(clicked(bool)),this,SLOT(Update_Truck_Message_Ok()));
    connect(this->ui->line_Update_Message,SIGNAL(textChanged(QString)),this,SLOT(Select_Truck_Select_Update()));
    connect(this->ui->table_Message, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(Click_On_Table_Message(const QModelIndex &)));//表格点击触发
    connect(this,SIGNAL(Send_Message(QString ,QString,QString)),add,SLOT(Receive_Message(QString ,QString,QString )));//发送接受信息


    //**************************小轿车函数**********************************

    this->model_2=new QStandardItemModel;//在表格中添加内容
    Select_Car_New_Table();//表格初始化
    ui->table_Message_21->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格不能编辑
    ui->table_Message_21->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    connect(this->ui->btn_Select_2,SIGNAL(clicked(bool)),this,SLOT(Select_Car_Ok()));
    connect(this->ui->line_Select_Message_2,SIGNAL(textChanged(QString)),this,SLOT(Select_Car_Input()));
    connect(this->ui->btn_Delete_2,SIGNAL(clicked(bool)),this,SLOT(Delete_Car_Message_Ok()));
    connect(this->ui->btn_Upadte_2,SIGNAL(clicked(bool)),this,SLOT(Update_Car_Message_Ok()));
    connect(this->ui->line_Update_Message_2,SIGNAL(textChanged(QString)),this,SLOT(Select_Car_Select_Update()));  //双击触发响应

    //**************************大客车函数**********************************
    this->model_3=new QStandardItemModel;//在表格中添加内容
    Select_Bus_New_Table();//表格初始化
    ui->table_Message_3->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格不能编辑
    ui->table_Message_3->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    connect(this->ui->btn_Select_3,SIGNAL(clicked(bool)),this,SLOT(Select_Bus_Ok()));
    connect(this->ui->line_Select_Message_3,SIGNAL(textChanged(QString)),this,SLOT(Select_Bus_Input()));
    connect(this->ui->btn_Delete_3,SIGNAL(clicked(bool)),this,SLOT(Delete_Bus_Message_Ok()));
    connect(this->ui->btn_Upadte_3,SIGNAL(clicked(bool)),this,SLOT(Update_Bus_Message_Ok()));
    connect(this->ui->line_Update_Message_3,SIGNAL(textChanged(QString)),this,SLOT(Select_Bus_Select_Update()));


}
int Select_Truck::line=0; //静态成员"行数"初始化
int Select_Truck::line_2=0; //静态成员"行数"初始化
int Select_Truck::line_3=0; //静态成员"行数"初始化
Select_Truck::~Select_Truck()
{
    delete ui;
}


//点击获取表格信息
void Select_Truck::Click_On_Table_Message(const QModelIndex &index)
{
    QString cellText = index.data().toString();
    int row=index.row();//选中的行号
    if (index.isValid())
    {
           QModelIndex dex=model->index(row,0);//获取车辆编号
           QString vehicle_Id=model->data(dex).toString();
           //qDebug()<<vehicle_Id;
           QString select=QString("select * from Driver where d_Id ='%1'").arg(vehicle_Id);
           QSqlQuery my_Query; //查询
           my_Query.exec(select);
          if(my_Query.first())
          {
             QString name=my_Query.value(3).toString();
             QString gender=my_Query.value(4).toString();
             QString fee_Km=my_Query.value(5).toString();
             QString salary=my_Query.value(8).toString();
               //每次将一行结果输出到表格中
              QString content="姓名: "+name+'\n'+"性别: "+gender+'\n'+"费用/公里: "+fee_Km+'\n'+"工资: "+salary+'\n';
              int ret =QMessageBox::information(this,"司机信息",content,QMessageBox::Ok);
            //   qDebug()<<"测试"<<" ";
          }
          else
          {


              //获取一行信息的代码，此处只需要获取编号
                  QModelIndex dex0=model->index(row,0);          //model 是事先与tableView绑定好的
                  QString id=model->data(dex0).toString();
                  QModelIndex dex1=model->index(row,1);          //model 是事先与tableView绑定好的
                  QString no=model->data(dex1).toString();
                  QModelIndex dex2=model->index(row,4);          //model 是事先与tableView绑定好的
                  QString type=model->data(dex2).toString();

                 // qDebug()<<id<<no<<type<<"Click_On_Table_Message";
                  emit Send_Message(id,no,type);//向Add_Driver发送信息
                  add->exec();


          }
   }





           //message=message.trimmed(); //去掉QString 中多余的空格 ，此处不需要
           //qDebug()<<message<<endl; 验证结果
           //获取一行信息的代码，此处只需要获取编号
//           for(int i=0;i<2;i++)
//           {
//               QModelIndex dex=model->index(row,i);          //model 是事先与tableView绑定好的
//               QString message=model->data(dex).toString();
//           }

}

//************查询信息功能*************

//确认按钮后查询信息
void Select_Truck::Select_Truck_Ok()
{
  //qDebug()<<"测试按钮响应"<<endl;
   QString Line_Message=ui->line_Select_Message->text();//读取文本框内信息
  Select_Truck_Select_Print(Line_Message);
}

//输入信息后查询
void Select_Truck::Select_Truck_Input()
{
   // qDebug()<<"测试输入响应"<<endl;
    QString Line_Message=ui->line_Select_Message->text();//读取文本框内信息
    Select_Truck_Select_Print(Line_Message);
}

//查询结果输出
void Select_Truck::Select_Truck_Select_Print(QString Line_Message)
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

//更新输入时查询信息
void Select_Truck::Select_Truck_Select_Update()
{
    QString Line_Message=this->ui->line_Update_Message->text();
    this->model->clear();//每次查询都要清除查询信息（重点）
    Select_Truck_New_Table();//表格初始化
    line=0;//初始化行数
    QSqlQuery my_Query; //查询
    QString temp=QString("select * from Truck where t_Id like '%1%'").arg(Line_Message);
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



//***********更新学生信息************

//确认按钮后更新按钮
void Select_Truck::Update_Truck_Message_Ok()
{
    QString message=this->ui->line_Update_Message->text();
    int index=this->ui->combo_Box_Update->currentIndex();
    QString select_Message; //存入被修改人的信息
    QString SQL;
    if(index==0)
    {
        select_Message="t_Id";
    }
    else if(index==1)
    {
        select_Message="t_No";
    }
    else if(index==2)
    {
        select_Message="t_Company";
    }
    else {
        select_Message="t_Buy_Time";
    }
    if(message!="")
    {
        Update_Truck_Message_Judge(select_Message,message);
    }
    else {
        QMessageBox::critical(this ,"警告","修改失败，请重新输入");
    }
}

//判断需要改变什么
void Select_Truck::Update_Truck_Message_Judge(QString select_Message,QString message)
{
    QString SQL_Update;  //赋值SQL执行语句
    QString SQL;
    QSqlQuery my_Update; //定义SQL执行变量
    SQL=QString("select * from Truck where %1='%2'").arg(select_Message).arg(message);
    my_Update.exec(SQL);
    if(!my_Update.first())
    {
        QMessageBox::critical(this ,"警告","修改失败，请重新输入");
        return ;
    }
    int ret =QMessageBox::information(this,"是否更新","请谨慎考虑",QMessageBox::Ok|QMessageBox::Cancel );
    if(ret!=QMessageBox::Ok)
    {
        return ;
    }
    //获得需要改变的状态
    bool index_No=this->ui->check_No_1->isChecked();
    bool index_Company=this->ui->check_Company_1->isChecked();
    bool index_Buy_Time=this->ui->check_Buy_Time_1->isChecked();
    bool index_Type=this->ui->check_Type_1->isChecked();
    bool index_Total_Km=this->ui->check_Total_Km_1->isChecked();
    bool index_Consum_Km=this->ui->check_Oil_Consum_Km_1->isChecked();
    bool index_Basic_Maintenance_Fee=this->ui->check_Basic_Maintenance_Fee_1->isChecked();
    bool index_Road_Toll=this->ui->check_Road_Toll_1->isChecked();
    bool index_Load_Capacity=this->ui->check_Load_Capacity_1->isChecked();
    //获得文本框信息
    QString line_No=this->ui->line_No_1->text();
    QString line_Company=this->ui->line_Company_1->text();
    QString line_Buy_Time=this->ui->line_Buy_Time_1->text();
    QString line_Type=this->ui->line_Type_1->text();
    QString line_Total_Km=this->ui->line_Total_Km_1->text();
    QString line_Consume_Km=this->ui->line_Oil_Consume_Km_1->text();
    QString line_Basic_Maintenance_Fee=this->ui->check_Basic_Maintenance_Fee_1->text();
    QString line_Road_Toll=this->ui->line_Road_Toll_1->text();
    QString line_Load_Capacity=this->ui->line_Load_Capacity_1->text();


    //判断并进行SQL语句
    QString line_Id=this->ui->line_Update_Message->text();
    if(index_No==1)
    {
          if(line_No.length()<1)
          {
             QMessageBox::critical(this ,"警告","修改失败,车号不能为空");
              return;
          }
        SQL_Update=QString("update Truck set t_No='%1' where %2='%3'").arg(line_No).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    if(index_Company==1)
    {
        SQL_Update=QString("update Truck set t_Company='%1' where %2='%3'").arg(line_Company).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    if(index_Road_Toll==1)
    {
        SQL_Update=QString("update Truck set t_Road_Toll='%1' where %2='%3'").arg(line_Road_Toll).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    if(index_Buy_Time==1)
    {
        SQL_Update=QString("update Truck set t_Buy_Time='%1' where %2='%3'").arg(line_Buy_Time).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    if(index_Type==1)
    {
        SQL_Update=QString("update Truck set t_Type='%1' where %2='%3'").arg(line_Type).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    if(index_Total_Km==1)
    {
        SQL_Update=QString("update Truck set t_Total_Km='%1' where %2='%3'").arg(line_Total_Km).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    if(index_Consum_Km==1)
    {
        SQL_Update=QString("update Truck set t_Consum_Km='%1' where %2='%3'").arg(line_Consume_Km).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    if(index_Basic_Maintenance_Fee==1)
    {
        SQL_Update=QString("update Truck set t_Basic_Maintenance_Fee='%1' where %2='%3'").arg(line_Basic_Maintenance_Fee).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    if(index_Load_Capacity==1)
    {
        SQL_Update=QString("update Truck set t_Load_Capacity='%1' where %2='%3'").arg(line_Load_Capacity).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    QString Line_Message=ui->line_Select_Message->text();//读取文本框内信息
    //Select_Truck_Select_Print(Line_Message);
    Select_Truck_Select_Update(); //更新后显示结果
    QMessageBox::information(this ,"提示","修改成功");
    Select_Truck_Clear_Message();//清除修改输入框内容
}
//***********删除学生信息************

//确认按钮后删除信息
void Select_Truck::Delete_Truck_Message_Ok()
{
    QString message=this->ui->line_Delete_Message->text();//读取文本栏的信息
    int index=this->ui->combo_Box_Delete->currentIndex();//得到选择的序号
    QString SQL;
    QString SQL_Select;
    if(index==0)
    {
        SQL=QString("delete from Truck where t_Id='%1'").arg(message);
        SQL_Select=QString("select * from Truck where t_Id='%1'").arg(message);
    }
    else if(index==1)
    {
        SQL=QString("delete from Truck where t_No='%1'").arg(message);
        SQL_Select=QString("select * from Truck where t_No='%1'").arg(message);
    }
    else if(index==2)
    {
        SQL=QString("delete from Truck where t_Company='%1'").arg(message);
        SQL_Select=QString("select * from Truck where t_Company='%1'").arg(message);
    }
    else {
        SQL=QString("delete from Truck where t_Buy_Time='%1'").arg(message);
        SQL_Select=QString("select * from Truck where t_Buy_Time='%1'").arg(message);
    }
    QSqlQuery my_Delete; //定义执行SQL语句变量
    QSqlQuery my_Select; //查询是否存在所要删除信息
    my_Select.exec(SQL_Select);
    if(my_Select.first())   //********有些问题，提示不完全*********
    {
        int ret =QMessageBox::information(this,"是否删除","请谨慎考虑",QMessageBox::Ok|QMessageBox::Cancel );
        if(ret==QMessageBox::Ok)
        {
        my_Delete.exec(SQL);//执行SQL删除语句
        QMessageBox::information(this ,"提示","删除成功");
        QString Line_Message=ui->line_Select_Message->text();//读取文本框内信息
        Select_Truck_Select_Print(Line_Message);//执行一次查询线束删除后信息
        }
        else return ;
    }
    else
        {
        QMessageBox::critical(this ,"提示","删除信息失败");
    }

}

//***********初始建立表格**************

//表格初始化
void  Select_Truck::Select_Truck_New_Table()
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

//初始化输入框内容
void  Select_Truck::Select_Truck_Clear_Message()
{
    this->ui->line_No_1->clear();
    this->ui->line_Company_1->clear();
    this->ui->line_Type_1->clear();
    this->ui->line_Total_Km_1->clear();
    this->ui->line_Oil_Consume_Km_1->clear();
    this->ui->line_Road_Toll_1->clear();
    this->ui->line_Buy_Time_1->clear();
}

//将输入的信息在表格中显示
void Select_Truck::Select_Truck_Message_Print(QString id,QString no,QString company,QString buy_Time,QString type,
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
void Select_Truck::Select_Car_Ok()
{
  //qDebug()<<"测试按钮响应"<<endl;
   QString Line_Message=ui->line_Select_Message_2->text();//读取文本框内信息
  Select_Car_Select_Print(Line_Message);
}

//输入信息后查询
void Select_Truck::Select_Car_Input()
{
   // qDebug()<<"测试输入响应"<<endl;
    QString Line_Message=ui->line_Select_Message_2->text();//读取文本框内信息
    Select_Car_Select_Print(Line_Message);
}

//查询结果输出
void Select_Truck::Select_Car_Select_Print(QString Line_Message)
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

//更新输入时查询信息
void Select_Truck::Select_Car_Select_Update()
{
    QString Line_Message=this->ui->line_Update_Message_2->text();
    this->model_2->clear();//每次查询都要清除查询信息（重点）
    Select_Car_New_Table();//表格初始化
    line_2=0;//初始化行数
    QSqlQuery my_Query; //查询
    QString temp=QString("select * from Car where c_Id like '%1%'").arg(Line_Message);
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



//***********更新学生信息************

//确认按钮后更新按钮
void Select_Truck::Update_Car_Message_Ok()
{
    QString message=this->ui->line_Update_Message_2->text();
    int index=this->ui->combo_Box_Update_2->currentIndex();
    QString select_Message; //存入被修改人的信息
    QString SQL;
    if(index==0)
    {
        select_Message="c_Id";
    }
    else if(index==1)
    {
        select_Message="c_No";
    }
    else if(index==2)
    {
        select_Message="c_Company";
    }
    else {
        select_Message="c_Buy_Time";
    }
    if(message!="")
    {
        Update_Car_Message_Judge(select_Message,message);
    }
    else {
        QMessageBox::critical(this ,"警告","修改失败，请重新输入");
    }
}

//判断需要改变什么
void Select_Truck::Update_Car_Message_Judge(QString select_Message,QString message)
{
    QString SQL_Update;  //赋值SQL执行语句
    QString SQL;
    QSqlQuery my_Update; //定义SQL执行变量
    SQL=QString("select * from Car where %1='%2'").arg(select_Message).arg(message);
    my_Update.exec(SQL);
    if(!my_Update.first())
    {
        QMessageBox::critical(this ,"警告","修改失败，请重新输入");
        return ;
    }
    int ret =QMessageBox::information(this,"是否更新","请谨慎考虑",QMessageBox::Ok|QMessageBox::Cancel );
    if(ret!=QMessageBox::Ok)
    {
        return ;
    }
    //获得需要改变的状态
    bool index_No=this->ui->check_No_2->isChecked();
    bool index_Company=this->ui->check_Company_2->isChecked();
    bool index_Buy_Time=this->ui->check_Buy_Time_2->isChecked();
    bool index_Type=this->ui->check_Type_2->isChecked();
    bool index_Total_Km=this->ui->check_Total_Km_2->isChecked();
    bool index_Consum_Km=this->ui->check_Oil_Consum_Km_2->isChecked();
    bool index_Basic_Maintenance_Fee=this->ui->check_Basic_Maintenance_Fee_2->isChecked();
    bool index_Road_Toll=this->ui->check_Road_Toll_2->isChecked();
    bool index_Box_Num=this->ui->check_Box_Num_2->isChecked();

    //获得文本框信息
    QString line_No=this->ui->line_No_20->text();
    QString line_Company=this->ui->line_Company_20->text();
    QString line_Buy_Time=this->ui->line_Buy_Time_2->text();
    QString line_Type=this->ui->line_Type_20->text();
    QString line_Total_Km=this->ui->line_Total_Km_20->text();
    QString line_Consume_Km=this->ui->line_Oil_Consume_Km_20->text();
    QString line_Basic_Maintenance_Fee=this->ui->check_Basic_Maintenance_Fee_2->text();
    QString line_Road_Toll=this->ui->line_Road_Toll_20->text();
    QString line_Box_Num=this->ui->line_Box_Num_2->text();

    //判断并进行SQL语句
    QString line_Id=this->ui->line_Update_Message_2->text();
    if(index_No==1)
    {
          if(line_No.length()<1)
          {
             QMessageBox::critical(this ,"警告","修改失败,车号不能为空");
              return;
          }
        SQL_Update=QString("update Car set c_No='%1' where %2='%3'").arg(line_No).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    if(index_Company==1)
    {
        SQL_Update=QString("update Car set c_Company='%1' where %2='%3'").arg(line_Company).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    if(index_Road_Toll==1)
    {
        SQL_Update=QString("update Car set c_Road_Toll='%1' where %2='%3'").arg(line_Road_Toll).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    if(index_Buy_Time==1)
    {
        SQL_Update=QString("update Car set c_Buy_Time='%1' where %2='%3'").arg(line_Buy_Time).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    if(index_Type==1)
    {
        SQL_Update=QString("update Car set c_Type='%1' where %2='%3'").arg(line_Type).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    if(index_Total_Km==1)
    {
        SQL_Update=QString("update Car set c_Total_Km='%1' where %2='%3'").arg(line_Total_Km).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    if(index_Consum_Km==1)
    {
        SQL_Update=QString("update Car set c_Consum_Km='%1' where %2='%3'").arg(line_Consume_Km).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    if(index_Basic_Maintenance_Fee==1)
    {
        SQL_Update=QString("update Car set c_Basic_Maintenance_Fee='%1' where %2='%3'").arg(line_Basic_Maintenance_Fee).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    if(index_Basic_Maintenance_Fee==1)
    {
        SQL_Update=QString("update Car set c_Basic_Maintenance_Fee='%1' where %2='%3'").arg(line_Basic_Maintenance_Fee).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    if(index_Box_Num==1)
    {
        SQL_Update=QString("update Car set c_Box_Num='%1' where %2='%3'").arg(line_Basic_Maintenance_Fee).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    QString Line_Message=ui->line_Select_Message->text();//读取文本框内信息
    Select_Car_Select_Update(); //更新后显示结果
    QMessageBox::information(this ,"提示","修改成功");
    Select_Car_Clear_Message();//清除修改输入框内容
}
//***********删除学生信息************

//确认按钮后删除信息
void Select_Truck::Delete_Car_Message_Ok()
{
    QString message=this->ui->line_Delete_Message->text();//读取文本栏的信息
    int index=this->ui->combo_Box_Delete->currentIndex();//得到选择的序号
    QString SQL;
    QString SQL_Select;
    if(index==0)
    {
        SQL=QString("delete from Car where c_Id='%1'").arg(message);
        SQL_Select=QString("select * from Car where c_Id='%1'").arg(message);
    }
    else if(index==1)
    {
        SQL=QString("delete from Car where c_No='%1'").arg(message);
        SQL_Select=QString("select * from Car where c_No='%1'").arg(message);
    }
    else if(index==2)
    {
        SQL=QString("delete from Car where c_Company='%1'").arg(message);
        SQL_Select=QString("select * from Car where c_Company='%1'").arg(message);
    }
    else {
        SQL=QString("delete from Car where c_Buy_Time='%1'").arg(message);
        SQL_Select=QString("select * from Car where c_Buy_Time='%1'").arg(message);
    }
    QSqlQuery my_Delete; //定义执行SQL语句变量
    QSqlQuery my_Select; //查询是否存在所要删除信息
    my_Select.exec(SQL_Select);
    if(my_Select.first())   //********有些问题，提示不完全*********
    {
        int ret =QMessageBox::information(this,"是否删除","请谨慎考虑",QMessageBox::Ok|QMessageBox::Cancel );
        if(ret==QMessageBox::Ok)
        {
        my_Delete.exec(SQL);//执行SQL删除语句
        QMessageBox::information(this ,"提示","删除成功");
        QString Line_Message=ui->line_Select_Message->text();//读取文本框内信息
        Select_Car_Select_Print(Line_Message);//执行一次查询线束删除后信息
        }
        else return ;
    }
    else
        {
        QMessageBox::critical(this ,"提示","删除信息失败");
    }

}

//***********初始建立表格**************

//表格初始化
void  Select_Truck::Select_Car_New_Table()
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

//初始化输入框内容
void  Select_Truck::Select_Car_Clear_Message()
{
    this->ui->line_No_20->clear();
    this->ui->line_Company_20->clear();
    this->ui->line_Type_20->clear();
    this->ui->line_Total_Km_20->clear();
    this->ui->line_Oil_Consume_Km_20->clear();
    this->ui->line_Road_Toll_20->clear();
    this->ui->line_Buy_Time_2->clear();
}

//将输入的信息在表格中显示
void Select_Truck::Select_Car_Message_Print(QString id,QString no,QString company,QString buy_Time,QString type,
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
void Select_Truck::Select_Bus_Ok()
{
  //qDebug()<<"测试按钮响应"<<endl;
   QString Line_Message=ui->line_Select_Message_3->text();//读取文本框内信息
  Select_Bus_Select_Print(Line_Message);
}

//输入信息后查询
void Select_Truck::Select_Bus_Input()
{
   // qDebug()<<"测试输入响应"<<endl;
    QString Line_Message=ui->line_Select_Message_3->text();//读取文本框内信息
    Select_Bus_Select_Print(Line_Message);
}

//查询结果输出
void Select_Truck::Select_Bus_Select_Print(QString Line_Message)
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

//更新输入时查询信息
void Select_Truck::Select_Bus_Select_Update()
{
    QString Line_Message=this->ui->line_Update_Message_3->text();
    this->model_3->clear();//每次查询都要清除查询信息（重点）
    Select_Bus_New_Table();//表格初始化
    line_3=0;//初始化行数
    QSqlQuery my_Query; //查询
    QString temp=QString("select * from Bus where b_Id like '%1%'").arg(Line_Message);
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



//***********更新学生信息************

//确认按钮后更新按钮
void Select_Truck::Update_Bus_Message_Ok()
{
    QString message=this->ui->line_Update_Message_3->text();
    int index=this->ui->combo_Box_Update_3->currentIndex();
    QString select_Message; //存入被修改人的信息
    QString SQL;
    if(index==0)
    {
        select_Message="b_Id";
    }
    else if(index==1)
    {
        select_Message="b_No";
    }
    else if(index==2)
    {
        select_Message="b_Company";
    }
    else {
        select_Message="b_Buy_Time";
    }
    if(message!="")
    {
        Update_Bus_Message_Judge(select_Message,message);
    }
    else {
        QMessageBox::critical(this ,"警告","修改失败，请重新输入");
    }
}

//判断需要改变什么
void Select_Truck::Update_Bus_Message_Judge(QString select_Message,QString message)
{
    QString SQL_Update;  //赋值SQL执行语句
    QString SQL;
    QSqlQuery my_Update; //定义SQL执行变量
    SQL=QString("select * from Bus where %1='%2'").arg(select_Message).arg(message);
    my_Update.exec(SQL);
    if(!my_Update.first())
    {
        QMessageBox::critical(this ,"警告","修改失败，请重新输入");
        return ;
    }
    int ret =QMessageBox::information(this,"是否更新","请谨慎考虑",QMessageBox::Ok|QMessageBox::Cancel );
    if(ret!=QMessageBox::Ok)
    {
        return ;
    }
    //获得需要改变的状态
    bool index_No=this->ui->check_No_3->isChecked();
    bool index_Company=this->ui->check_Company_3->isChecked();
    bool index_Buy_Time=this->ui->check_Buy_Time_3->isChecked();
    bool index_Type=this->ui->check_Type_3->isChecked();
    bool index_Total_Km=this->ui->check_Total_Km_3->isChecked();
    bool index_Consum_Km=this->ui->check_Oil_Consum_Km_3->isChecked();
    bool index_Basic_Maintenance_Fee=this->ui->check_Basic_Maintenance_Fee_3->isChecked();
    bool index_Road_Toll=this->ui->check_Road_Toll_3->isChecked();
    bool index_Max_Passengers=this->ui->check_Max_Passengers_3->isChecked();

    //获得文本框信息
    QString line_No=this->ui->line_No_3->text();
    QString line_Company=this->ui->line_Company_3->text();
    QString line_Buy_Time=this->ui->line_Buy_Time_3->text();
    QString line_Type=this->ui->line_Type_3->text();
    QString line_Total_Km=this->ui->line_Total_Km_3->text();
    QString line_Consume_Km=this->ui->line_Oil_Consume_Km_3->text();
    QString line_Basic_Maintenance_Fee=this->ui->check_Basic_Maintenance_Fee_3->text();
    QString line_Road_Toll=this->ui->line_Road_Toll_3->text();
    QString line_Max_Passengers=this->ui->line_Max_Passengers_3->text();

    //判断并进行SQL语句
    QString line_Id=this->ui->line_Update_Message_3->text();
    if(index_No==1)
    {
          if(line_No.length()<1)
          {
             QMessageBox::critical(this ,"警告","修改失败,车号不能为空");
              return;
          }
        SQL_Update=QString("update Bus set b_No='%1' where %2='%3'").arg(line_No).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    if(index_Company==1)
    {
        SQL_Update=QString("update Bus set b_Company='%1' where %2='%3'").arg(line_Company).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    if(index_Road_Toll==1)
    {
        SQL_Update=QString("update Bus set b_Road_Toll='%1' where %2='%3'").arg(line_Road_Toll).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    if(index_Buy_Time==1)
    {
        SQL_Update=QString("update Bus set b_Buy_Time='%1' where %2='%3'").arg(line_Buy_Time).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    if(index_Type==1)
    {
        SQL_Update=QString("update Bus set b_Type='%1' where %2='%3'").arg(line_Type).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    if(index_Total_Km==1)
    {
        SQL_Update=QString("update Bus set b_Total_Km='%1' where %2='%3'").arg(line_Total_Km).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    if(index_Consum_Km==1)
    {
        SQL_Update=QString("update Bus set b_Consum_Km='%1' where %2='%3'").arg(line_Consume_Km).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    if(index_Basic_Maintenance_Fee==1)
    {
        SQL_Update=QString("update Bus set b_Basic_Maintenance_Fee='%1' where %2='%3'").arg(line_Basic_Maintenance_Fee).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    if(index_Basic_Maintenance_Fee==1)
    {
        SQL_Update=QString("update Bus set b_Basic_Maintenance_Fee='%1' where %2='%3'").arg(line_Basic_Maintenance_Fee).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    if(index_Max_Passengers==1)
    {
        SQL_Update=QString("update Bus set b_Max_Passengers='%1' where %2='%3'").arg(line_Max_Passengers).arg(select_Message).arg(message);
        my_Update.exec(SQL_Update);
    }
    QString Line_Message=ui->line_Select_Message->text();//读取文本框内信息
    Select_Bus_Select_Update(); //更新后显示结果
    QMessageBox::information(this ,"提示","修改成功");
    Select_Bus_Clear_Message();//清除修改输入框内容
}
//***********删除学生信息************

//确认按钮后删除信息
void Select_Truck::Delete_Bus_Message_Ok()
{
    QString message=this->ui->line_Delete_Message->text();//读取文本栏的信息
    int index=this->ui->combo_Box_Delete->currentIndex();//得到选择的序号
    QString SQL;
    QString SQL_Select;
    if(index==0)
    {
        SQL=QString("delete from Bus where b_Id='%1'").arg(message);
        SQL_Select=QString("select * from Bus where b_Id='%1'").arg(message);
    }
    else if(index==1)
    {
        SQL=QString("delete from Bus where b_No='%1'").arg(message);
        SQL_Select=QString("select * from Bus where b_No='%1'").arg(message);
    }
    else if(index==2)
    {
        SQL=QString("delete from Bus where b_Company='%1'").arg(message);
        SQL_Select=QString("select * from Bus where b_Company='%1'").arg(message);
    }
    else {
        SQL=QString("delete from Bus where b_Buy_Time='%1'").arg(message);
        SQL_Select=QString("select * from Bus where b_Buy_Time='%1'").arg(message);
    }
    QSqlQuery my_Delete; //定义执行SQL语句变量
    QSqlQuery my_Select; //查询是否存在所要删除信息
    my_Select.exec(SQL_Select);
    if(my_Select.first())   //********有些问题，提示不完全*********
    {
        int ret =QMessageBox::information(this,"是否删除","请谨慎考虑",QMessageBox::Ok|QMessageBox::Cancel );
        if(ret==QMessageBox::Ok)
        {
        my_Delete.exec(SQL);//执行SQL删除语句
        QMessageBox::information(this ,"提示","删除成功");
        QString Line_Message=ui->line_Select_Message->text();//读取文本框内信息
        Select_Bus_Select_Print(Line_Message);//执行一次查询线束删除后信息
        }
        else return ;
    }
    else
        {
        QMessageBox::critical(this ,"提示","删除信息失败");
    }

}

//***********初始建立表格**************

//表格初始化
void  Select_Truck::Select_Bus_New_Table()
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

//初始化输入框内容
void  Select_Truck::Select_Bus_Clear_Message()
{
    this->ui->line_No_3->clear();
    this->ui->line_Company_3->clear();
    this->ui->line_Type_3->clear();
    this->ui->line_Total_Km_3->clear();
    this->ui->line_Oil_Consume_Km_3->clear();
    this->ui->line_Road_Toll_3->clear();
    this->ui->line_Buy_Time_3->clear();
    this->ui->line_Max_Passengers_3->clear();
}

//将输入的信息在表格中显示
void Select_Truck::Select_Bus_Message_Print(QString id,QString no,QString company,QString buy_Time,QString type,
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



