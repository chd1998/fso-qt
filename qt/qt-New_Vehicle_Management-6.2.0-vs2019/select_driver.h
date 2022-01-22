#ifndef SELECT_DRIVER_H
#define SELECT_DRIVER_H

#include <QDialog>
#include<QStandardItemModel>  //设置表格所用


namespace Ui {
class Select__Driver;
}

class Select__Driver : public QDialog
{
    Q_OBJECT

public:
    explicit Select__Driver(QWidget *parent = nullptr);
    ~Select__Driver();
    static int line;
    void Select__Driver_Select_Print(QString Line_Message);  //查询数据库中的数据
    void Select__Driver_New_Table();     //表格格式设置
    //输出信息到表格中
    void Select__Driver_Message_Print(QString id,QString no,QString type,QString name,QString gender,
                                    QString km,QString fee_Km,QString base_Salary, QString salary) const ;//9个

private:
    Ui::Select__Driver *ui;
    QStandardItemModel *model;

private slots:
    void Select__Driver_Ok();//确认按钮后查询信息
    void Select__Driver_Input();//文本栏输入信息后查询
    void Delete_Driver_Message_Ok();//确认按钮后删除信息
};

#endif // SELECT_DRIVER_H
