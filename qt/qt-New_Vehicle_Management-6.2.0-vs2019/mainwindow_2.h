#ifndef MAINWINDOW_2_H
#define MAINWINDOW_2_H

#include <QMainWindow>
#include"apply_account.h"
#include"select_driver_2.h"
#include"select_truck_2.h"
namespace Ui {
class MainWindow_2;
}

class MainWindow_2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow_2(QWidget *parent = nullptr);
    ~MainWindow_2();

private slots:

    void on_btn_Select_Driver_2_clicked();

    void on_btn_Select_Vehicle_2_clicked();

private:
    Ui::MainWindow_2 *ui;
    Apply_Account apply;
    Select_Driver_2 select_Driver_2;
    Select_Truck_2 select_Truck_2;
};

#endif // MAINWINDOW_2_H
