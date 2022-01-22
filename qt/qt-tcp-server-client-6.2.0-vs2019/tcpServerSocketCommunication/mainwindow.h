#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
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
    void on_pushButton_Listen_clicked();//监听按钮槽函数

    void on_pushButton_Send_clicked();//发送信息按钮槽函数

    void server_New_Connect();//服务器接收到客户端消息信号，触发连接槽函数（连接客户端）

    void socket_Read_Data();//服务器读客户端传输过来的数据

    void socket_Disconnect();//断开服务器与客户端连接槽函数

    void on_textEdit_Receive_textChanged();

private:
    Ui::MainWindow *ui;

    QTcpServer *server;//创建TCP的服务器对象
    QTcpSocket *socket;//创建TCP的客户端对象
};

#endif // MAINWINDOW_H
