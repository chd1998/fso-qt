#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QIODevice>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <QFileDialog>
#include <fstream>
#include <QFile>

#define X_range  100
#define X_tickCount  10


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initForm();
    initChart();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//初始化界面
void MainWindow::initForm()
{
    setWindowTitle(tr("Serial Port Assistant"));

    //将QRadioButton分组
    //将QRadioButton分组
    group1 = new QButtonGroup(this);
    group2 = new QButtonGroup(this);

    group1->addButton(ui->sendTextMode,1);
    group1->addButton(ui->sendHexMode,2);
    group2->addButton(ui->recTextMode,1);
    group2->addButton(ui->recHexMode,2);
    connect(ui->sendTextMode,SIGNAL(clicked()),this,SLOT(group1_clicked()));
    connect(ui->sendHexMode,SIGNAL(clicked()),this,SLOT(group1_clicked()));
    connect(ui->recTextMode,SIGNAL(clicked()),this,SLOT(group2_clicked()));
    connect(ui->recHexMode,SIGNAL(clicked()),this,SLOT(group2_clicked()));

    //改变串口的各种选项时，重新启动串口
    connect(ui->chuankou,SIGNAL(currentIndexChanged(int)),this,SLOT(saveConfig()));
    connect(ui->botelv,SIGNAL(currentIndexChanged(int)),this,SLOT(saveConfig()));
    connect(ui->shujuwei,SIGNAL(currentIndexChanged(int)),this,SLOT(saveConfig()));
    connect(ui->jiaoyanwei,SIGNAL(currentIndexChanged(int)),this,SLOT(saveConfig()));
    connect(ui->tingzhiwei,SIGNAL(currentIndexChanged(int)),this,SLOT(saveConfig()));

    //初始化串口信息(在.PRO文件中加入QT+=serialport)
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->chuankou->addItem(info.portName());
            serial.close();
        }
    }

    //选中自动发送，每隔一段时间就发送一次
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(selectAutoSend()));

    times = 0;  //调试用
}

//每次选项改变的时候重新启动串口,各种参数好像能使用枚举类型搞定
void MainWindow::saveConfig()
{
    qDebug()<<"Options";
    MainWindow::portname = ui->chuankou->currentText();
    MainWindow::baudrate = ui->botelv->currentText().toInt();
    MainWindow::databit = ui->shujuwei->currentText().toInt();
    MainWindow::parity = ui->jiaoyanwei->currentText();
    MainWindow::stopbit = ui->tingzhiwei->currentText().toDouble();

    qDebug() <<"Port ID/Baud Rate/Data Bits/Parity Bit/Stop Bit"  <<portname <<baudrate <<databit <<parity <<stopbit;
}

//打开串口bool setDataBits(DataBits dataBits);

void MainWindow::on_openBtn_clicked()
{
    if(ui->openBtn->text() == "Open")
    //if(ui->openBtn-isEnabled())
    {
        myPort = new QSerialPort(this);
        myPort->setPortName(portname);
        if(myPort->open(QIODevice::ReadWrite))
        {
              myPort->setBaudRate(baudrate);
              myPort->setDataBits(QSerialPort::DataBits(ui->shujuwei->currentText().toInt()));
              myPort->setParity(QSerialPort::Parity(ui->tingzhiwei->currentIndex()));
              myPort->setStopBits(QSerialPort::StopBits(ui->tingzhiwei->currentIndex()));

              //QMessageBox::information(this,tr("提示框"),tr("串口已经打开"));
              statusBar()->showMessage(tr("Serial Port Opened..."));//状态栏显示的内容
              connect(myPort, SIGNAL(readyRead()), this, SLOT(readCom()));

        }
        else
        {
            //QMessageBox::information(this,tr("提示框"),tr("串口无法打开"));
            statusBar()->showMessage(tr("Open Serial Port Failed..."));//状态栏显示的内容
        }
        ui->openBtn->setText(tr("Close"));
    }
    else
    {
        //自动发送按钮没有关闭，直接按关闭串口按钮，定时器关闭，因为设备已经关了，再发送数据没有意义
//        if(timer->isActive())
//            timer->stop();
       myPort->close();                             //初始化控件不可用
       statusBar()->showMessage(tr("Serial Port Closed...."));  //状态栏显示的内容
       ui->openBtn->setText(tr("Open"));
       return;
    }
}

//是一个槽函数，不断读取从机器返回来的数据
void MainWindow::readCom()
{
    times++;   
    QByteArray byte_data = myPort->readAll();    //读取串口数据
    QString strDisplay;

    if (currentCount >= maxCount)    //当前显示行数
    {
        ui->receiveText->clear();      //清屏
        currentCount = 0;
    }

    if (byte_data.isEmpty())
    {
       return;
    }

    //将读取到的数据进行转化,是否是按16进制显示
    if(isHexDisplay)
    {
        QString str = byte_data.toHex();  //返回16进制编码的副本
        str = str.toUpper ();             //把小字符串转化为大写字符串
        for(int i = 0;i<str.length ();i+=2)
        {
            QString st = str.mid (i,2);//从i这个位置开始，返回2个字符串
            strDisplay += st;
            strDisplay += " ";
        }
        qDebug()<<"Recv in HEX"<<times;
        ui->receiveText->insertPlainText(strDisplay);
    }
    else
    {
        ui->receiveText->insertPlainText(byte_data);
       if(byte_data[0] == 'A' && byte_data[1] == 'D')
            rec_data = (byte_data[11]-0x30)*1000 + (byte_data[13]-0x30) * 100 + (byte_data[14]-0x30) * 10;
       RealtimeDataSlot(rec_data);
       qDebug()<<"TXT Times："<<times<<"TXT Contents："<<byte_data<<"Recorded Data："<<rec_data;

    }
    byte_data.clear();
    currentCount++;
 }

//清空接收框
void MainWindow::on_receiveCleanBtn_clicked()
{
    ui->receiveText->clear();
}

//保存接收数据
void MainWindow::on_saveReceiveBtn_clicked()
{
    QString text = ui->receiveText->toPlainText();
    if(text == "")
    {
        QMessageBox::warning(this,tr("Warnning"),tr("No Message to Save..."));
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),"./receieve.txt",tr("Text files (*.txt)"));
    QFile file(fileName);
    if(file.open((QIODevice::WriteOnly)))
    {
        QTextStream in(&file);
        in<<text;
    }
}

//发送文件,带有缓冲区，防止阻塞等操作
void MainWindow::on_fileBtn_clicked()
{

    QString f;
    QString line;
    QFile file1("/savefile.txt");
    if(file1.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file1);
        line = in.readLine();
        if(line == "")
            f = "/";
        else
            f = line;
    }
    file1.close();
    QString s = QFileDialog::getOpenFileName(this,"Pls select file to upload",line,tr("Text files(*.bin)"));
    //上面的这一部分操作保证的是打开的是上一次的上传的文件
    if(file1.open((QIODevice::WriteOnly)))
    {
        QTextStream in(&file1);
        in<<s;
    }
    file1.close();
    if(s.isEmpty())//用户没选中文件，接着进行下面的操作就会崩溃,因为s是一个空字符串
        return;
    QFile file(s);
    if(file.open(QIODevice::ReadOnly))
    {
        char rdBytes[20];
        qint64 rdNum;
        qint64 rdTotalNum = 0;
        rdNum = file.read(rdBytes,20);
        rdTotalNum = rdNum;
        while (rdNum > 0)
        {
            myPort->write(rdBytes,rdNum);
            rdNum = file.read(rdBytes,20);
            rdTotalNum += rdNum;
        }
        file.close();
        QString str = s.setNum(rdTotalNum);
        //往发送文本框中显示发送的大小
        ui->sendText->append("Sent " + str + "Bytes");
        qDebug()<<"Sent "<<rdTotalNum;
        QMessageBox::information(this,tr("Hints"),tr("Upload OK..."));
    }
    else
    {
        QMessageBox::information(this,tr("Hints"),tr("Open File Failed..."));
    }
}

//发送数据
void MainWindow::on_sendBtn_clicked()
{
    times++;
    QString sdata;
    sdata = ui->sendText->toPlainText();
    if(sdata.isEmpty())
        return;
    QByteArray sendBuf = "";
    if(isHexSend)
    {
        sendBuf = QByteArray::fromHex (sdata.toLatin1());  //返回16进制编码的副本
        qDebug()<<"Send in HEX"<<times;
    }
    else
    {
        sendBuf = sdata.toLatin1();   //转化为ISO-8859-1
        qDebug()<<"Send "<<times;
    }
    myPort->write(sendBuf);
}

//清空发送框
void MainWindow::on_sendCleanBtn_clicked()
{
    ui->sendText->clear();
}

//保存发送数据
void MainWindow::on_saveSendBtn_clicked()
{
    QString text = ui->sendText->toPlainText();
    if(text == "")
    {
        QMessageBox::warning(this,tr("Warnning"),tr("No message to save..."));
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),"./send.txt",tr("Text files (*.txt)"));
    QFile file(fileName);
    if(file.open((QIODevice::WriteOnly)))
    {
        QTextStream in(&file);
        in<<text;
    }
}

//是否选中自动发送
void MainWindow::on_zidong_toggled(bool checked)
{
    if(checked)
    {
        int time = ui->time->text().toInt();
        timer->start(time);
        ui->sendBtn->setEnabled(false);
    }
    else
    {
        timer->stop();
        ui->sendBtn->setEnabled(true);
    }
}

//选中自动发送后选中的槽函数，不断向机器发送数据
void MainWindow::selectAutoSend()
{
    on_sendBtn_clicked();
}


//发送文本框单选按钮绑定的槽函数
void MainWindow::group1_clicked()
{
    switch (group1->checkedId()) {
    case 1:
        isHexSend = false;
        qDebug()<<"Send in TXT";
        break;
    case 2:
        isHexSend = true;
        qDebug()<<"Send in HEX";
        break;
    default:break;
    }
}

//接收文本框单选按钮绑定的槽函数
void MainWindow::group2_clicked()
{
    switch (group2->checkedId()) {
    case 1:
        isHexDisplay = false;
        qDebug()<<"Recv in TXT";
        break;
    case 2:
        isHexDisplay = true;
        qDebug()<<"Recv in HEX";
        break;
    default:break;
    }
}

void MainWindow::initChart()
{
    //setWindowTitle("动态正弦波形图");
    m_timer = new QTimer();
    m_timer->setInterval(500);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(RealtimeDataSlot()));
    x=0;
    y=0;
    com_x=0;
    com_y=0;

    spline.setColor(Qt::red);     //设置折线颜色
    spline.setWidth(1);           //设置折线宽度
    m_series.setPen(spline);
    m_series.append(x, y);

    chart.setTheme(QChart::ChartThemeBlueCerulean);//设置系统主题
    //chart.setTitle("动态正弦波形图");//设置标题
    //chart.setTitleFont(QFont("微软雅黑",10));
    chart.legend()->hide();  //设置图例

    chart.addSeries(&m_series);
    chart.createDefaultAxes();   // 基于已添加到图表的 series 来创轴
    chart.setAxisX(&X_axis,&m_series);
    chart.setAxisY(&Y_axis,&m_series);

    X_axis.setRange(0,X_range);            //设置范围
    //X_axis.setLabelFormat("%g");    //设置刻度的格式
    X_axis.setTickCount(X_tickCount);          //设置X轴刻度线数目
    //X_axis.setTitleText("X");         //设置X轴的标题
    X_axis.setGridLineVisible(true);  //设置是否显示网格线
    X_axis.setLabelsVisible(true);     //设置刻度是否显示

    Y_axis.setRange(0,3300);
    Y_axis.setTickCount(5);          //设置Y轴刻度线数目
    //Y_axis.setTitleText("Y");

    QChartView *chartView = new QChartView(&chart);   //创建图表显示
    ui->widgetWaveForm->addWidget(chartView);//将图表显示添加到layout控件
    //QGridLayout *baseLayout = new QGridLayout();      //便于显示，创建网格布局
    //baseLayout->addWidget(chartView, 0, 0);
    //ui->widgetWaveForm->setLayout(baseLayout);         //显示到QWidget控件
    chartView->setRenderHint(QPainter::Antialiasing);   //启动反走样 Antialiasing反混

    //m_timer->start();
}

void MainWindow::RealtimeDataSlot()
{
    dx = X_range / X_axis.tickCount();
    chart_dx = chart.plotArea().width() / X_axis.tickCount();
    x += dx;
    y=sin(x);
    qDebug() << chart.plotArea().width() << X_axis.tickCount() << dx << x << y;
    m_series.append(x, y);
    if(x > X_range)
    chart.scroll(chart_dx, 0);    //实现滚动
}

void MainWindow::RealtimeDataSlot(int comdate)
{
    com_y = comdate;
    m_series.append(com_x, com_y);
    com_x += 1;
    comchart_dx = chart.plotArea().width() / X_range;
    qDebug() << com_x << com_y << comchart_dx;
    if(com_x > X_range)
    chart.scroll(comchart_dx, 0);    //实现滚动
}
