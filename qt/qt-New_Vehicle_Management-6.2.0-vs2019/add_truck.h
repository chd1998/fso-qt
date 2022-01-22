#ifndef ADD_TRUCK_H
#define ADD_TRUCK_H

#include <QDialog>
#include<QStandardItemModel>//设置表格所用
namespace Ui {
class Add_Vehicle;
}

class Add_Truck : public QDialog
{
    Q_OBJECT

public:
    static int line;
    static int line_2;
    static int line_3;
    explicit Add_Truck(QWidget *parent = nullptr);
    ~Add_Truck();

     //*********************卡车函数*************************
    void Add_Truck_Message();  //表格信息
    void Add_Truck_Clear_Message(); //清楚输入栏的内容
    //将查询信息输入到表格上
    void Add_Truck_Manage_Print(QString id,QString no,QString company,QString buy_Time,QString type,
                                QString total_Km,QString oil_Consume_Km,QString road_Toll,QString load_Capacity) const ;

    bool Add_Truck_Insert_Table(QString id,QString no,QString company,QString buy_Time,QString type,
                                QString total_Km,QString oil_Consume_Km,QString road_Toll,QString load_Capacity) const ;


     //*********************小轿车函数*************************

    void Add_Car_Message();  //表格信息
    void Add_Car_Clear_Message(); //清楚输入栏的内容
    //将查询信息输入到表格上
    void Add_Car_Manage_Print(QString id,QString no,QString company,QString buy_Time,QString type,
                              QString total_Km,QString oil_Consume_Km,QString road_Toll,QString box_Num) const ;
    bool Add_Car_Insert_Table(QString id,QString no,QString company,QString buy_Time,QString type,
                              QString total_Km,QString oil_Consume_Km,QString road_Toll,QString box_Num) const ;


    //*********************大客车函数*************************
    void Add_Bus_Message();  //表格信息
    void Add_Bus_Clear_Message(); //清楚输入栏的内容
    //将查询信息输入到表格上
    void Add_Bus_Manage_Print(QString id,QString no,QString company,QString buy_Time,QString type,
                              QString total_Km,QString oil_Consume_Km,QString road_Toll,QString man_Passengers) const ;
    bool Add_Bus_Insert_Table(QString id,QString no,QString company,QString buy_Time,QString type,
                              QString total_Km,QString oil_Consume_Km,QString road_Toll,QString man_Passengers) const ;

private slots:
    //*********************卡车函数*************************
    void Add_Truck_Ok();  //确认输入
    void Add_Truck_Close();  //关闭窗口

     //*********************小轿车函数*************************
    void Add_Car_Ok();  //确认输入
    void Add_Car_Close();  //关闭窗口


    //*********************大客车函数*************************
    void Add_Bus_Ok();  //确认输入
    void Add_Bus_Close();  //关闭窗口


private:
    Ui::Add_Vehicle *ui;
    QStandardItemModel *model;   //卡车
    QStandardItemModel *model_2; //小轿车
    QStandardItemModel *model_3; //大客车





};

#endif // ADD_TRUCK_H
