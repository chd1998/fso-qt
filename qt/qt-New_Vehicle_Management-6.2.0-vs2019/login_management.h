#ifndef LOGIN_MANAGEMENT_H
#define LOGIN_MANAGEMENT_H

#include <QMainWindow>
#include"mainwindow.h"
#include "mainwindow_2.h"
namespace Ui {
class login_Management;
}

class login_Management : public QMainWindow
{
    Q_OBJECT

public:
    explicit login_Management(QWidget *parent = nullptr);
    ~login_Management();

    //连接数据库
    bool Open_Database();

private slots:
    void Login();//登录
    void on_btn_Forget_clicked(); //申请账号

private:
    Ui::login_Management *ui;
    MainWindow w;
    MainWindow_2 w_2;
};

#endif // LOGIN_MANAGEMENT_H
