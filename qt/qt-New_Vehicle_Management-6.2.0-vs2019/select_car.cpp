//#include "select_car.h"
//#include "ui_select_truck.h"
//#include <QSqlQuery>
//#include <QSqlError>
//#include <QDebug>
//#include <QMessageBox>//表格信息显示
//#include <QDebug>//输出测试程序运行是否正确
//#include <QString>

//Select_Car::Select_Car(QWidget *parent) :
//    QDialog(parent),
//    ui(new Ui::Select_Truck)
//{
//    ui->setupUi(this);

//    //**************************小轿车函数**********************************
//    this->model_2=new QStandardItemModel;
//    Select_Car_New_Table();//表格初始化
//    ui->table_Message_21->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格不能编辑
//    ui->table_Message_21->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
//    connect(this->ui->btn_Select_2,SIGNAL(clicked(bool)),this,SLOT(Select_Car_Ok()));
//    connect(this->ui->line_Select_Message_2,SIGNAL(textChanged(QString)),this,SLOT(Select_Car_Input()));
//    connect(this->ui->btn_Delete_2,SIGNAL(clicked(bool)),this,SLOT(Delete_Car_Message_Ok()));
//    connect(this->ui->btn_Upadte_2,SIGNAL(clicked(bool)),this,SLOT(Update_Car_Message_Ok()));
//    connect(this->ui->line_Update_Message_2,SIGNAL(textChanged(QString)),this,SLOT(Select_Car_Select_Update()));







//    //**************************大客车函数**********************************



//}
//int Select_Car::line_2=0; //静态成员"行数"初始化
//Select_Car::~Select_Car()
//{
//    delete ui;
//}


////************查询信息功能*************

////确认按钮后查询信息
//void Select_Car::Select_Car_Ok()
//{
//  //qDebug()<<"测试按钮响应"<<endl;
//   QString Line_Message=ui->line_Select_Message_2->text();//读取文本框内信息
//  Select_Car_Select_Print(Line_Message);
//}

////输入信息后查询
//void Select_Car::Select_Car_Input()
//{
//   // qDebug()<<"测试输入响应"<<endl;
//    QString Line_Message=ui->line_Select_Message_2->text();//读取文本框内信息
//    Select_Car_Select_Print(Line_Message);
//}

////查询结果输出
//void Select_Car::Select_Car_Select_Print(QString Line_Message)
//{
//    this->model_2->clear();//每次查询都要清除查询信息（重点）
//    Select_Car_New_Table();//表格初始化
//    line_2=0;//初始化行数
//    int index=this->ui->combo_Box_Select_2->currentIndex(); //得到选择的序号,共0、1、2、3

//    QString temp;//用来暂存SQL语句

//    if(index==0)//判断选择的序号，在选择Select的内容
//    {
//       temp=QString("select * from Car where c_Id like '%1%'").arg(Line_Message);
//    }
//    else if(index==1)
//    {
//       temp=QString("select * from Car where c_No like '%1%'").arg(Line_Message);
//    }
//    else if(index==2)
//    {
//       temp=QString("select * from Car where c_Company like '%1%'").arg(Line_Message);
//    }
//    else {
//       temp=QString("select * from Car where c_Buy_Time like '%1%'").arg(Line_Message);
//    }
//     QSqlQuery my_Query; //查询
//     if(my_Query.exec(temp))
//     {
//       //输出查询结果，从line=0开始输出
//       while(my_Query.next()) //一直到结尾才推出循环
//       {
//        QString id=my_Query.value(0).toString();
//        QString no=my_Query.value(1).toString();
//        QString company=my_Query.value(2).toString();
//        QString buy_Time=my_Query.value(3).toString();
//        QString type=my_Query.value(4).toString();
//        QString total_Km=my_Query.value(5).toString();
//        QString oil_Consume_Km=my_Query.value(6).toString();
//        QString basic_Maintenance_Fee=my_Query.value(7).toString();
//        QString road_Toll=my_Query.value(8).toString();
//        QString total_Cost=my_Query.value(9).toString();
//        QString box_Num=my_Query.value(10).toString();
//        //每次将一行结果输出到表格中
//        Select_Car_Message_Print(id, no,company, buy_Time,type,
//                                           total_Km, oil_Consume_Km,basic_Maintenance_Fee,
//                                            road_Toll,total_Cost,box_Num);
//       }
//     }
//     else
//     {
//        Select_Car_New_Table();//表格初始化
//     }
//}

////更新输入时查询信息
//void Select_Car::Select_Car_Select_Update()
//{
//    QString Line_Message=this->ui->line_Update_Message_2->text();
//    this->model_2->clear();//每次查询都要清除查询信息（重点）
//    Select_Car_New_Table();//表格初始化
//    line_2=0;//初始化行数
//    QSqlQuery my_Query; //查询
//    QString temp=QString("select * from Car where c_Id like '%1%'").arg(Line_Message);
//    if(my_Query.exec(temp))
//    {
//      //输出查询结果，从line=0开始输出
//      while(my_Query.next()) //一直到结尾才推出循环
//      {
//       QString id=my_Query.value(0).toString();
//       QString no=my_Query.value(1).toString();
//       QString company=my_Query.value(2).toString();
//       QString buy_Time=my_Query.value(3).toString();
//       QString type=my_Query.value(4).toString();
//       QString total_Km=my_Query.value(5).toString();
//       QString oil_Consume_Km=my_Query.value(6).toString();
//       QString basic_Maintenance_Fee=my_Query.value(7).toString();
//       QString road_Toll=my_Query.value(8).toString();
//       QString total_Cost=my_Query.value(9).toString();
//       QString box_Num=my_Query.value(10).toString();
//       //每次将一行结果输出到表格中
//                     Select_Car_Message_Print(id, no,company, buy_Time,type,
//                                                        total_Km, oil_Consume_Km,basic_Maintenance_Fee,
//                                                         road_Toll,total_Cost,box_Num);
//      }
//    }
//    else
//    {
//       Select_Car_New_Table();//表格初始化
//    }
//}



////***********更新学生信息************

////确认按钮后更新按钮
//void Select_Car::Update_Car_Message_Ok()
//{
//    QString message=this->ui->line_Update_Message_2->text();
//    int index=this->ui->combo_Box_Update_2->currentIndex();
//    QString select_Message; //存入被修改人的信息
//    QString SQL;
//    if(index==0)
//    {
//        select_Message="c_Id";
//    }
//    else if(index==1)
//    {
//        select_Message="c_No";
//    }
//    else if(index==2)
//    {
//        select_Message="c_Company";
//    }
//    else {
//        select_Message="c_Buy_Time";
//    }
//    if(message!="")
//    {
//        Update_Car_Message_Judge(select_Message,message);
//    }
//    else {
//        QMessageBox::critical(this ,"警告","修改失败，请重新输入");
//    }
//}

////判断需要改变什么
//void Select_Car::Update_Car_Message_Judge(QString select_Message,QString message)
//{
//    QString SQL_Update;  //赋值SQL执行语句
//    QString SQL;
//    QSqlQuery my_Update; //定义SQL执行变量
//    SQL=QString("select * from Car where %1='%2'").arg(select_Message).arg(message);
//    my_Update.exec(SQL);
//    if(!my_Update.first())
//    {
//        QMessageBox::critical(this ,"警告","修改失败，请重新输入");
//        return ;
//    }
//    int ret =QMessageBox::information(this,"是否更新","请谨慎考虑",QMessageBox::Ok|QMessageBox::Cancel );
//    if(ret!=QMessageBox::Ok)
//    {
//        return ;
//    }
//    //获得需要改变的状态
//    bool index_No=this->ui->check_No_2->isChecked();
//    bool index_Company=this->ui->check_Company_2->isChecked();
//    bool index_Buy_Time=this->ui->check_Buy_Time_2->isChecked();
//    bool index_Type=this->ui->check_Type_2->isChecked();
//    bool index_Total_Km=this->ui->check_Total_Km_2->isChecked();
//    bool index_Consum_Km=this->ui->check_Oil_Consum_Km_2->isChecked();
//    bool index_Basic_Maintenance_Fee=this->ui->check_Basic_Maintenance_Fee_2->isChecked();
//    bool index_Road_Toll=this->ui->check_Road_Toll_2->isChecked();
//    //获得文本框信息
//    QString line_No=this->ui->line_No_20->text();
//    QString line_Company=this->ui->line_Company_20->text();
//    QString line_Buy_Time=this->ui->line_Buy_Time_2->text();
//    QString line_Type_=this->ui->line_Type_20->text();
//    QString line_Total_Km_=this->ui->line_Total_Km_20->text();
//    QString line_Consume_Km=this->ui->line_Oil_Consume_Km_20->text();
//    QString line_Basic_Maintenance_Fee=this->ui->check_Basic_Maintenance_Fee_2->text();
//    QString line_Road_Toll=this->ui->line_Road_Toll_20->text();
//    //判断并进行SQL语句
//    QString line_Id=this->ui->line_Update_Message_2->text();
//    if(index_No==1)
//    {
//          if(line_No.length()<1)
//          {
//             QMessageBox::critical(this ,"警告","修改失败,车号不能为空");
//              return;
//          }
//        SQL_Update=QString("update Car set c_No='%1' where %2='%3'").arg(line_No).arg(select_Message).arg(message);
//        my_Update.exec(SQL_Update);
//    }
//    if(index_Company==1)
//    {
//        SQL_Update=QString("update Car set c_Company='%1' where %2='%3'").arg(line_Company).arg(select_Message).arg(message);
//        my_Update.exec(SQL_Update);
//    }
//    if(index_Road_Toll==1)
//    {
//        SQL_Update=QString("update Car set c_Road_Toll='%1' where %2='%3'").arg(line_Road_Toll).arg(select_Message).arg(message);
//        my_Update.exec(SQL_Update);
//    }
//    if(index_Buy_Time==1)
//    {
//        SQL_Update=QString("update Car set c_Buy_Time='%1' where %2='%3'").arg(line_Buy_Time).arg(select_Message).arg(message);
//        my_Update.exec(SQL_Update);
//    }
//    if(index_Type==1)
//    {
//        SQL_Update=QString("update Car set c_Type='%1' where %2='%3'").arg(index_Type).arg(select_Message).arg(message);
//        my_Update.exec(SQL_Update);
//    }
//    if(index_Total_Km==1)
//    {
//        SQL_Update=QString("update Car set c_Total_Km='%1' where %2='%3'").arg(index_Total_Km).arg(select_Message).arg(message);
//        my_Update.exec(SQL_Update);
//    }
//    if(index_Consum_Km==1)
//    {
//        SQL_Update=QString("update Car set c_Consum_Km='%1' where %2='%3'").arg(line_Consume_Km).arg(select_Message).arg(message);
//        my_Update.exec(SQL_Update);
//    }
//    if(index_Basic_Maintenance_Fee==1)
//    {
//        SQL_Update=QString("update Car set c_Basic_Maintenance_Fee='%1' where %2='%3'").arg(line_Basic_Maintenance_Fee).arg(select_Message).arg(message);
//        my_Update.exec(SQL_Update);
//    }
//    QString Line_Message=ui->line_Select_Message->text();//读取文本框内信息
//    Select_Car_Select_Update(); //更新后显示结果
//    QMessageBox::information(this ,"提示","修改成功");
//    Select_Car_Clear_Message();//清除修改输入框内容
//}
////***********删除学生信息************

////确认按钮后删除信息
//void Select_Car::Delete_Car_Message_Ok()
//{
//    QString message=this->ui->line_Delete_Message->text();//读取文本栏的信息
//    int index=this->ui->combo_Box_Delete->currentIndex();//得到选择的序号
//    QString SQL;
//    QString SQL_Select;
//    if(index==0)
//    {
//        SQL=QString("delete from Car where c_Id='%1'").arg(message);
//        SQL_Select=QString("select * from Car where c_Id='%1'").arg(message);
//    }
//    else if(index==1)
//    {
//        SQL=QString("delete from Car where c_No='%1'").arg(message);
//        SQL_Select=QString("select * from Car where c_No='%1'").arg(message);
//    }
//    else if(index==2)
//    {
//        SQL=QString("delete from Car where c_Company='%1'").arg(message);
//        SQL_Select=QString("select * from Car where c_Company='%1'").arg(message);
//    }
//    else {
//        SQL=QString("delete from Car where c_Buy_Time='%1'").arg(message);
//        SQL_Select=QString("select * from Car where c_Buy_Time='%1'").arg(message);
//    }
//    QSqlQuery my_Delete; //定义执行SQL语句变量
//    QSqlQuery my_Select; //查询是否存在所要删除信息
//    my_Select.exec(SQL_Select);
//    if(my_Select.first())   //********有些问题，提示不完全*********
//    {
//        int ret =QMessageBox::information(this,"是否删除","请谨慎考虑",QMessageBox::Ok|QMessageBox::Cancel );
//        if(ret==QMessageBox::Ok)
//        {
//        my_Delete.exec(SQL);//执行SQL删除语句
//        QMessageBox::information(this ,"提示","删除成功");
//        QString Line_Message=ui->line_Select_Message->text();//读取文本框内信息
//        Select_Car_Select_Print(Line_Message);//执行一次查询线束删除后信息
//        }
//        else return ;
//    }
//    else
//        {
//        QMessageBox::critical(this ,"提示","删除信息失败");
//    }

//}

////***********初始建立表格**************

////表格初始化
//void  Select_Car::Select_Car_New_Table()
//{
//    this->model_2->setHorizontalHeaderItem(0,new QStandardItem("车辆编号"));
//    this->model_2->setHorizontalHeaderItem(1,new QStandardItem("车号"));
//    this->model_2->setHorizontalHeaderItem(2,new QStandardItem("公司"));
//    this->model_2->setHorizontalHeaderItem(3,new QStandardItem("购买时间"));
//    this->model_2->setHorizontalHeaderItem(4,new QStandardItem("型号"));
//    this->model_2->setHorizontalHeaderItem(5,new QStandardItem("总公里数"));
//    this->model_2->setHorizontalHeaderItem(6,new QStandardItem("耗油/公里"));
//    this->model_2->setHorizontalHeaderItem(8,new QStandardItem("基本维护RMB"));
//    this->model_2->setHorizontalHeaderItem(7,new QStandardItem("过路RMB"));
//    this->model_2->setHorizontalHeaderItem(9,new QStandardItem("每月RMB"));
//    this->model_2->setHorizontalHeaderItem(10,new QStandardItem("箱数"));

//    //将视图与model_2绑定
//    this->ui->table_Message_21->setModel(model_2);
//    this->ui->table_Message_21->setColumnWidth(0,110);
//    this->ui->table_Message_21->setColumnWidth(1,100);
//    this->ui->table_Message_21->setColumnWidth(2,110);
//    this->ui->table_Message_21->setColumnWidth(3,140);
//    this->ui->table_Message_21->setColumnWidth(4,70);
//    this->ui->table_Message_21->setColumnWidth(5,70);
//    this->ui->table_Message_21->setColumnWidth(6,90);
//    this->ui->table_Message_21->setColumnWidth(7,70);
//    this->ui->table_Message_21->setColumnWidth(8,70);
//    this->ui->table_Message_21->setColumnWidth(9,70);
//    this->ui->table_Message_21->setColumnWidth(10,70);
//}

////初始化输入框内容
//void  Select_Car::Select_Car_Clear_Message()
//{
//    this->ui->line_No_1->clear();
//    this->ui->line_Company_1->clear();
//    this->ui->line_Type_1->clear();
//    this->ui->line_Total_Km_1->clear();
//    this->ui->line_Oil_Consume_Km_1->clear();
//    this->ui->line_Road_Toll_1->clear();
//    this->ui->line_Buy_Time_1->clear();
//}

////将输入的信息在表格中显示
//void Select_Car::Select_Car_Message_Print(QString id,QString no,QString company,QString buy_Time,QString type,
//                                              QString total_Km,QString oil_Consume_Km,QString basic_Maintenance_Fee,
//                                              QString road_Toll,QString total_Cost,QString box_Num) const
//{
//    QString Manager_Message=id+"+"+no+"+"+company+"+"+buy_Time+"+"+type+"+"+total_Km+"+"+oil_Consume_Km+"+"+
//            basic_Maintenance_Fee+"+"+road_Toll+"+"+total_Cost +"+"+box_Num;
//    QStringList subs=Manager_Message.split("+");
//    for(int i=0;i<11;i++)
//    {
//    this->model_2->setItem(line_2,i,new QStandardItem(subs.at(i)));
//    }
//    line_2++;
//}


