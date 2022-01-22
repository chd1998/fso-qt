#ifndef SELECT_TRUCK_2_H
#define SELECT_TRUCK_2_H

#include <QDialog>
#include<QStandardItemModel>  //设置表格所用
namespace Ui {
class Select_Truck_2;
}

class Select_Truck_2 : public QDialog
{
    Q_OBJECT

public:
    explicit Select_Truck_2(QWidget *parent = nullptr);
    ~Select_Truck_2();
    //*************************卡车函数**********************************
   static int line;
   void Select_Truck_Select_Print(QString Line_Message);  //查询数据库中的数据
   void Select_Truck_New_Table();     //表格格式设置
   //输出信息到表格中
   void Select_Truck_Message_Print(QString id,QString no,QString company,QString buy_Time,QString type,
                                   QString total_Km,QString oil_Consume_Km,QString basic_Maintenance_Fee,
                                   QString road_Toll,QString total_Cost,QString load_Capacity) const ;//11个
   //**************************小轿车函数**********************************
   static int line_2;
   void Select_Car_Select_Print(QString Line_Message);  //查询数据库中的数据
   void Select_Car_New_Table();     //表格格式设置
   //输出信息到表格中
   void Select_Car_Message_Print(QString id,QString no,QString company,QString buy_Time,QString type,
                                   QString total_Km,QString oil_Consume_Km,QString basic_Maintenance_Fee,
                                   QString road_Toll,QString total_Cost,QString box_Num) const ;//11个



   //**************************大客车函数**********************************
   void Select_Bus_Select_Print(QString Line_Message);  //查询数据库中的数据
   void Select_Bus_New_Table();     //表格格式设置
   //输出信息到表格中
   void Select_Bus_Message_Print(QString id,QString no,QString company,QString buy_Time,QString type,
                                   QString total_Km,QString oil_Consume_Km,QString basic_Maintenance_Fee,
                                   QString road_Toll,QString total_Cost,QString max_Passengers) const ;//11个
   static int line_3;

private:
    Ui::Select_Truck_2 *ui;

    QStandardItemModel *model;
    QStandardItemModel *model_2;
    QStandardItemModel *model_3;
private slots:

     //*************************卡车函数**********************************
    void Select_Truck_Ok();//确认按钮后查询信息
    void Select_Truck_Input();//文本栏输入信息后查询

    //**************************小轿车函数**********************************

    void Select_Car_Ok();//确认按钮后查询信息
    void Select_Car_Input();//文本栏输入信息后查询

        //**************************大客车函数**********************************
    void Select_Bus_Ok();//确认按钮后查询信息
    void Select_Bus_Input();//文本栏输入信息后查询
};

#endif // SELECT_TRUCK_2_H
