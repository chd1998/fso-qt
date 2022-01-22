#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_recvNum = 0;
    m_sendNum = 0;
    sPortThread = new TA_SerialThread;
    sPortThread->SearchPort();

    for(uint8_t i = 0;i < sPortThread->portCnt;i++)
        ui->comboBox_SPortName->addItem(sPortThread->m_arrSerial[i].portName());

    //0:1200 1:2400 2:4800 3:9600 4:19200 5:38400 6:57600 7:115200
    ui->comboBox_SPortBaud->setCurrentIndex(3);
    //0:5 1:6 2:7 3:8
    ui->comboBox_SPortDataBit->setCurrentIndex(3);
    //0:no p 2:e p 3:o p 4:s p 5:m p
    ui->comboBox_SPortOEBit->setCurrentIndex(0);
    //0:1 1:2 2:1.5
    ui->comboBox_SPortStopBit->setCurrentIndex(0);

    ui->pushButton_SendCmd->setEnabled(false);
    ui->graphicsView_ComStatus->setStyleSheet("background-color: rgb(255, 0, 0);");
    ui->label_ComStatus->setText(tr("串口状态： 断开"));

    connect(ui->pushButton_OpenCom, SIGNAL(clicked(bool)), this, SLOT(MWOnoffPort()));
    connect(ui->pushButton_SendCmd, SIGNAL(clicked(bool)), this, SLOT(MWSendCmd()));
    connect(ui->pushButton_ClearRecv,SIGNAL(clicked(bool)),this, SLOT(MWClearRecv()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::MWOnoffPort(void)
{
    uint8_t comIndex = ui->comboBox_SPortName->currentIndex();

    if(ui->pushButton_OpenCom->text()==tr("打开串口"))
    {
        sPortThread->InitPortName(comIndex,ui->comboBox_SPortName->currentText());
        sPortThread->InitPortBaudRate(comIndex,ui->comboBox_SPortBaud->currentText().toInt());

        //qDebug()<<ui->comboBox_SPortDataBit->currentIndex();
        switch(ui->comboBox_SPortDataBit->currentIndex())
        {
            case 0: sPortThread->InitPortDataBits(comIndex,QSerialPort::Data5); break;//0:5
            case 1: sPortThread->InitPortDataBits(comIndex,QSerialPort::Data6); break;//1:6
            case 2: sPortThread->InitPortDataBits(comIndex,QSerialPort::Data7); break;//2:7
            case 3: sPortThread->InitPortDataBits(comIndex,QSerialPort::Data8); break;//3:8
            default: break;
        }

        switch(ui->comboBox_SPortStopBit->currentIndex())
        {
            case 0:sPortThread->InitPortStopBits(comIndex,QSerialPort::OneStop);break;
            case 1:sPortThread->InitPortStopBits(comIndex,QSerialPort::TwoStop);break;
            case 2:sPortThread->InitPortStopBits(comIndex,QSerialPort::OneAndHalfStop);break;
            default: break;
        }

        switch(ui->comboBox_SPortOEBit->currentIndex())
        {
            case 0:sPortThread->InitPortParity(comIndex,QSerialPort::NoParity);break;
            case 1:sPortThread->InitPortParity(comIndex,QSerialPort::EvenParity);break;
            case 2:sPortThread->InitPortParity(comIndex,QSerialPort::OddParity);break;
            case 3:sPortThread->InitPortParity(comIndex,QSerialPort::SpaceParity);break;
            case 4:sPortThread->InitPortParity(comIndex,QSerialPort::MarkParity);break;
            default: break;
        }
        sPortThread->OpenPort(comIndex);

        connect(&(sPortThread->m_arrSerial[comIndex]), SIGNAL(readyRead()), this, SLOT(MWReadData()));

        //参数设置禁止
        ui->comboBox_SPortName->setEnabled(false);
        ui->comboBox_SPortBaud->setEnabled(false);
        ui->comboBox_SPortDataBit->setEnabled(false);
        ui->comboBox_SPortOEBit->setEnabled(false);
        ui->comboBox_SPortStopBit->setEnabled(false);
        ui->pushButton_OpenCom->setText(tr("关闭串口"));
        //发送设置使能
        ui->pushButton_SendCmd->setEnabled(true);
        ui->graphicsView_ComStatus->setStyleSheet("background-color: rgb(0, 255, 0);");
        ui->label_ComStatus->setText(tr("串口状态： 连接"));
    }
    else
    {
        sPortThread->ClosePort(comIndex);
        //参数设置使能
        ui->comboBox_SPortName->setEnabled(true);
        ui->comboBox_SPortBaud->setEnabled(true);
        ui->comboBox_SPortDataBit->setEnabled(true);
        ui->comboBox_SPortOEBit->setEnabled(true);
        ui->comboBox_SPortStopBit->setEnabled(true);
        ui->pushButton_OpenCom->setText(tr("打开串口"));
        //发送设置禁止
        ui->pushButton_SendCmd->setEnabled(false);
        ui->graphicsView_ComStatus->setStyleSheet("background-color: rgb(255, 0, 0);");
        ui->label_ComStatus->setText(tr("串口状态： 断开"));
    }
}

void MainWindow::MWSendCmd(void)
{
   sPortThread->SendDataBuf(ui->comboBox_SPortName->currentIndex(),ui->textEdit->toPlainText().toLatin1());
   m_sendNum +=  (ui->textEdit->toPlainText().toLatin1()).length();
   MWRSNumDisplay();
}

void MainWindow::MWReadData(void)
{
    QByteArray buf;

    buf = sPortThread->GetDataBuf(ui->comboBox_SPortName->currentIndex());

    if(!buf.isEmpty())
    {
        QString str = ui->textBrowser->toPlainText();
        str += tr(buf);
        ui->textBrowser->clear();
        ui->textBrowser->append(str);
        m_recvNum += tr(buf).length();
        MWRSNumDisplay();
    }

    buf.clear();
}

void MainWindow::MWClearRecv(void)
{
    ui->textBrowser->clear();
    m_recvNum = 0;
    m_sendNum = 0;
    MWRSNumDisplay();
}

void MainWindow::MWRSNumDisplay(void)
{
    ui->label_RecvNum->setText(tr("已接收：") + QString::number(m_recvNum,10));
    ui->label_SendNum->setText(tr("已发送：") + QString::number(m_sendNum,10));
}
