#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_Connect_clicked();//连接按钮槽函数

    void on_pushButton_Send_clicked();//发送按钮槽函数

    void socket_Read_Data();//读服务器数据槽函数

    void socket_Disconnected();//断开与服务器连接槽函数

    void on_textEdit_Receive_textChanged();

private:
    Ui::MainWindow *ui;

    QTcpSocket *socket;//创建客户端对象
};

#endif // MAINWINDOW_H
