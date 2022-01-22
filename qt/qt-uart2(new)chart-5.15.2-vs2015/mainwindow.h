#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>

/*
#include <QtCharts/QChartView>//显示图表
#include <QtCharts/QLineSeries>//线系列
#include <QDoubleSpinBox>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QAreaSeries>
#include <QChart>
#include <QChartView>
*/

#include <QtCharts>//线系列
#include <QDoubleSpinBox>
#include <QChart>
#include <QChartView>

using namespace QtCharts;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool isHexSend = false;
    bool isHexDisplay = false;
    int maxCount = 50;               //最大显示行数
    int currentCount = 0;           //当前显示行数
    int rec_data=0;

     QString portname;            //串口号
     int baudrate;                //波特率
     int databit;                 //数据位
     QString parity;              //校验位
     double stopbit;              //停止位

private slots:

    void group1_clicked();

    void group2_clicked();

    void initForm();

    void saveConfig();

    void on_openBtn_clicked();

    void readCom();

    void on_receiveCleanBtn_clicked();

    void on_saveReceiveBtn_clicked();

    void on_fileBtn_clicked();

    void on_sendBtn_clicked();

    void on_sendCleanBtn_clicked();

    void on_saveSendBtn_clicked();

    void on_zidong_toggled(bool checked);

    void selectAutoSend();

    void initChart();

    void RealtimeDataSlot();

    void RealtimeDataSlot(int comdate);

private:
    Ui::MainWindow *ui;
    QButtonGroup *group1;
    QButtonGroup *group2;
    QSerialPort *myPort;
    int times;//调试用的
    QTimer *timer;

    QTimer *m_timer;
    QChart chart;            //图表
    QSplineSeries m_series;  //创建平滑曲线
    QPen spline;             //折线
    QStringList m_titles;    //字符串列表
    QValueAxis X_axis;       //QChart X坐标
    QValueAxis Y_axis;       //QChart Y坐标
    qreal m_step;           //在Arm平台上，qreal相当于float，其他情况下qreal和double是一样的
    qreal dx;              //X轴每次x步进
    qreal chart_dx;        //X轴每次增加长度
    qreal x;
    qreal y;

    qreal com_dx;
    qreal comchart_dx;
    int com_x;
    int com_y;
};

#endif // MAINWINDOW_H
