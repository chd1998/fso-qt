#ifndef SELECT_TRUCK_H
#define SELECT_TRUCK_H


#include <QDialog>
#include<QStandardItemModel>  //设置表格所用
#include"add_driver.h" //添加司机信息

namespace Ui {
class Select_Truck;
}

class Select_Truck : public QDialog
{
    Q_OBJECT

public:
    explicit Select_Truck(QWidget *parent = nullptr);
    ~Select_Truck();



     //*************************卡车函数**********************************
    static int line;
    void Select_Truck_Select_Print(QString Line_Message);  //查询数据库中的数据
    void Select_Truck_New_Table();     //表格格式设置
    void Select_Truck_Clear_Message(); //清除表格信息

    void Update_Truck_Message_Judge(QString put_Message,QString message);//判断需要改变什么
    //输出信息到表格中
    void Select_Truck_Message_Print(QString id,QString no,QString company,QString buy_Time,QString type,
                                    QString total_Km,QString oil_Consume_Km,QString basic_Maintenance_Fee,
                                    QString road_Toll,QString total_Cost,QString load_Capacity) const ;//11个


    //**************************小轿车函数**********************************
    static int line_2;
    void Select_Car_Select_Print(QString Line_Message);  //查询数据库中的数据
    void Select_Car_New_Table();     //表格格式设置
    void Select_Car_Clear_Message(); //清除表格信息

    void Update_Car_Message_Judge(QString put_Message,QString message);//判断需要改变什么
    //输出信息到表格中
    void Select_Car_Message_Print(QString id,QString no,QString company,QString buy_Time,QString type,
                                    QString total_Km,QString oil_Consume_Km,QString basic_Maintenance_Fee,
                                    QString road_Toll,QString total_Cost,QString box_Num) const ;//11个



    //**************************大客车函数**********************************
    void Select_Bus_Select_Print(QString Line_Message);  //查询数据库中的数据
    void Select_Bus_New_Table();     //表格格式设置
    void Select_Bus_Clear_Message(); //清除表格信息

    void Update_Bus_Message_Judge(QString put_Message,QString message);//判断需要改变什么
    //输出信息到表格中
    void Select_Bus_Message_Print(QString id,QString no,QString company,QString buy_Time,QString type,
                                    QString total_Km,QString oil_Consume_Km,QString basic_Maintenance_Fee,
                                    QString road_Toll,QString total_Cost,QString max_Passengers) const ;//11个
    static int line_3;


private:
    Ui::Select_Truck *ui;

    QStandardItemModel *model;
    QStandardItemModel *model_2;
    QStandardItemModel *model_3;
    Add_Driver *add;

private slots:

     //*************************卡车函数**********************************
    void Select_Truck_Ok();//确认按钮后查询信息
    void Select_Truck_Input();//文本栏输入信息后查询
    void Update_Truck_Message_Ok();//确认按钮后更新按钮
    void Delete_Truck_Message_Ok();//确认按钮后删除信息
    void Select_Truck_Select_Update(); //更新输入时查询信息
    void Click_On_Table_Message(const QModelIndex &index); //选中表格一行数据

    //**************************小轿车函数**********************************

    void Select_Car_Ok();//确认按钮后查询信息
    void Select_Car_Input();//文本栏输入信息后查询
    void Update_Car_Message_Ok();//确认按钮后更新按钮
    void Delete_Car_Message_Ok();//确认按钮后删除信息
    void Select_Car_Select_Update(); //更新输入时查询信息

        //**************************大客车函数**********************************
    void Select_Bus_Ok();//确认按钮后查询信息
    void Select_Bus_Input();//文本栏输入信息后查询
    void Update_Bus_Message_Ok();//确认按钮后更新按钮
    void Delete_Bus_Message_Ok();//确认按钮后删除信息
    void Select_Bus_Select_Update(); //更新输入时查询信息

signals:
    void Send_Message(QString,QString,QString );  //向Add_Truck发射信息
};
#endif // SELECT_TRUCK_H
