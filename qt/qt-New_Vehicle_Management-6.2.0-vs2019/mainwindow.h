#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"add_truck.h" //添加车辆
#include"select_truck.h" //查询、修改、删除车辆
#include"select_driver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_triggered();
    void Main_Add_Truck();//添加车辆
    void Main_Select_Truck();//查询、删除、修改车辆
    void Select_My_Driver();


private:
    Ui::MainWindow *ui;
    Add_Truck add;
    Select_Truck select;
    Select__Driver select_Driver;


};

#endif // MAINWINDOW_H
