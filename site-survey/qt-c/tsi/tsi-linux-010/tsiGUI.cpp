#include "tsiGUI.h"
#include "QDebug"
#include "ui_mainwindow.h"
#include "sp2tcpserver.h"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>

#include <QSysInfo>

#include <cstdlib>
#include <iostream>

using std::cout;

QString SPName="/dev/ttyUSB1",SPParityBits="none",devID="TSI-01",tsiIP="*",SP485Addr="01",commandTSI="",recvData="";
int recvTSI=0;
int SPBaudRate=4800,SPDataBits=8,SPStopBits=1,SPTimeInterval=1,tsiIPPort=7071;
bool cfgStatus=false;
QDateTime current_date_time;
QString current_date="",recvTSI_str="",current_time="";


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    opened=false;
    /*myPort=ui->lineEditPort->text().toUInt();
    time_interval=ui->lineEditRefreshTime->text().toUInt();
    if(time_interval<=0)
        time_interval=1;
    time_interval=time_interval*1000;*/
    cfgStatus=openCfg();
    if(cfgStatus)
    {
        ui->lineEditPortName->setText(SPName);
        ui->lineEditBaudRate->setText(QString::number(SPBaudRate));
        ui->lineEditDataBits->setText(QString::number(SPDataBits));
        ui->lineEditParityBits->setText(SPParityBits);
        ui->lineEditStopBits->setText(QString::number(SPStopBits));
        ui->lineEdit_485addr->setText(SP485Addr);
        ui->lineEditRefreshTime->setText(QString::number(SPTimeInterval));
        ui->lineEditIP->setText(tsiIP);
        ui->lineEditPort->setText(QString::number(tsiIPPort));
        ui->labelSite->setText(devID);
    }
    else
    {
        qDebug()<<"Using default settings and generating cfg file...";
        ui->lineEditPortName->setText(SPName);
        ui->lineEditBaudRate->setText(QString::number(SPBaudRate));
        ui->lineEditDataBits->setText(QString::number(SPDataBits));
        ui->lineEditParityBits->setText(SPParityBits);
        ui->lineEditStopBits->setText(QString::number(SPStopBits));
        ui->lineEdit_485addr->setText(SP485Addr);
        ui->lineEditRefreshTime->setText(QString::number(SPTimeInterval));
        ui->lineEditIP->setText(tsiIP);
        ui->lineEditPort->setText(QString::number(tsiIPPort));
        ui->labelSite->setText(devID);
        bool wstatus=writeCfg();
        if(wstatus)
            qDebug()<<"cfg file generated...";
        else
            qDebug()<<"generating cfg file failed...";
    }
    //myPort=tsiIPPort;
    time_interval=SPTimeInterval*1000;
    readTimer = new QTimer(this);
    //    connect(ui->searchButton,SIGNAL(clicked()),this,SLOT(on_searchButton_clicked()));     //ui上使用槽后会自动connect,不用再connect，否则会链接两遍
    //    connect(ui->openComButton,SIGNAL(clicked()),this,SLOT(on_openComButton_clicked()));
    connect(readTimer,SIGNAL(timeout()),this,SLOT(readSPData()));
    setFixedSize(this->width(), this->height());
    this->setWindowTitle("TSI for Site Survey (Ver. 0.1.0)");
    SP2TCPServer *myserver1=new SP2TCPServer(this);
    connect(myserver1,&SP2TCPServer::sendMsg2UI,this,&MainWindow::displayMsg);
    ui->gpsPlainTextEdit->appendPlainText(devID+": Listening on "+QString::number(tsiIPPort)+"...");
    //myserver1->start();
    openSerialPort();
}

MainWindow::~MainWindow()
{
    closeSerialPort();
    delete ui;
}

void MainWindow::StringToHex(QString str, QByteArray &senddata) //字符串转换为十六进制数据0-F
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;

    for(int i=0; i<len; )
    {
        //char lstr,
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
}

char MainWindow::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return ch-ch;//不在0-f范围内的会发送成0
}
//crc16校验



void MainWindow::displayMsg(QString myMsg)
{
    ui->gpsPlainTextEdit->appendPlainText(myMsg);
}

//生成配置文件
bool MainWindow::writeCfg()
{
    QFile cfgfile(QDir::currentPath() + "//tsi-"+devID+".json");
    //QFile cfgfile("f:\\gps\\gps-site-01.json");
    if(!cfgfile.open(QIODevice::WriteOnly))
    {
        qDebug()<<"cfg file open error...";
        return false;
    }else{
        qDebug()<<"cfg file wait for write...";
    }
    cfgfile.resize(0);

    QJsonArray tsi_dev;
    QJsonObject devidobj,devdesobj,spobj,spobj_site,sptimeobj,updateobj,tsiipobj,tsiipobj_dev;
    QJsonObject dev;

    //rootobjobj.insert("Site ID","site_01");
    //branchobjobj.insert("GPS Site",leafobjobj);
    //gps_site.append(rootobjobj);
    //clearJsonObject(rootobjobj);
    //clearJsonObject(branchobjobj);
    devidobj.insert("Dev ID",devID);
    devdesobj.insert("Dev Desc.",devidobj);
    tsi_dev.append(devdesobj);
    spobj.insert("SPName",SPName);
    spobj.insert("SPBaudRate",SPBaudRate);
    spobj.insert("SPDataBits",SPDataBits);
    spobj.insert("SPParityBits",SPParityBits);
    spobj.insert("SPStopBits",SPStopBits);
    spobj.insert("SP485Addr",SP485Addr);
    //spobj.insert("SPStopBits",1);
    spobj_site.insert("TSI Dev Serial Port",spobj);
    tsi_dev.append(spobj_site);
    sptimeobj.insert("SPTimeInterval",SPTimeInterval);
    updateobj.insert("Update Freq.", sptimeobj);
    tsi_dev.append(updateobj);
    tsiipobj.insert("tsiIP",tsiIP);
    tsiipobj.insert("tsiIPPort",tsiIPPort);
    tsiipobj_dev.insert("tsi Dev IP Info",tsiipobj);
    tsi_dev.append(tsiipobj_dev);

    dev.insert("GPS Site",tsi_dev);
    QJsonDocument jsonDocument;
    jsonDocument.setObject(dev);
    //QByteArray byteArray=jsonDocument.toJson(QJsonDocument::Indented);

    cfgfile.write(jsonDocument.toJson(QJsonDocument::Indented));

    cfgfile.close();
    return true;
}

//打开配置文件
bool MainWindow::openCfg()
{
    //打开Json文件并用QByteArray读取全部内容
        //QFile file("f:\\gps\\gps-site-01.json");
        QFile file(QDir::currentPath() + "//tsi-"+devID+".json");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << QString("fail to open the json file...");
            return false;
        }
        QByteArray array = file.readAll();
        file.close();

        //读取的全部内容转为QJsonDocument，如果报错则输出错误信息
        QJsonParseError jsonParseError;
        QJsonDocument jsonDocument(QJsonDocument::fromJson(array, &jsonParseError));
        if (QJsonParseError::NoError != jsonParseError.error) {
            qDebug() << QString("JsonParseError: %1").arg(jsonParseError.errorString());
            return false;
        }
        //QJsonDocument转为QJsonObject
        QJsonObject rootObject = jsonDocument.object();
        //traversJson(rootObject);
        //获取根结点
        QStringList keys = rootObject.keys();
        for (int i = 0; i < keys.size(); i++) {
            qDebug() << "key" << i << " is:" << keys.at(i);
        }

        //根据获取到的根结点继续获取二级结点
        for (int i = 0; i < keys.size(); i++) {
            if (rootObject.contains(keys.at(i)) && rootObject[keys.at(i)].isArray()) {
                QJsonArray subArray = rootObject.value(keys.at(i)).toArray();
                for (int i = 0; i < subArray.size(); i++) {
                    if (subArray[i].isObject()) {
                        QStringList subKeys = subArray[i].toObject().keys();
                        for (int m = 0; m < subKeys.size(); m++) {
                            qDebug() << "Subkey is:" << subKeys.at(m);
                            //根据获取到的二级结点继续获取三级结点
                            QJsonObject subObj = subArray[i].toObject()[subKeys.at(m)].toObject();
                            //traversJson(subObj);
                            //qDebug()<<subObj.value("Site ID").toString();
                            //QStringList subObjKeys = subObj.keys();
                            if (subArray[i].toObject().contains(subKeys.at(m)) && subArray[i].toObject()[subKeys.at(m)].isObject()) {
                                if(subObj.contains("Dev ID") ) {
                                    qDebug() << "Dev ID : " << subObj["Dev ID"].toString();
                                    //qDebug() <<  subObj.value("Site ID").toString();
                                }
                                if (subObj.contains("SPName") ) {
                                    qDebug() << "SPName : " << subObj["SPName"].toString();
                                }
                                if (subObj.contains("SPBaudRate") ) {
                                    qDebug() << "SPBaudRate : " << subObj["SPBaudRate"].toInt();
                                }
                                if (subObj.contains("SPDataBits") ) {
                                    qDebug() << "SPDataBits : " << subObj["SPDataBits"].toInt();
                                }
                                if (subObj.contains("SPParityBits") ) {
                                    qDebug() << "SPParityBits : " << subObj["SPParityBits"].toString();
                                }
                                if (subObj.contains("SPStopBits") ) {
                                    qDebug() << "SPStopBits : " << subObj["SPStopBits"].toInt();
                                }
                                if (subObj.contains("SP485Addr") ) {
                                    qDebug() << "SP485Addr : " << subObj["SP485Addr"].toString();
                                }
                                if (subObj.contains("SPTimeInterval") ) {
                                    qDebug() << "SPTimeInterval : " << subObj["SPTimeInterval"].toInt();
                                }
                                if (subObj.contains("tsiIP") ) {
                                    qDebug() << "tsiIP : " << subObj["tsiIP"].toString();
                                }
                                if (subObj.contains("tsiIPPort") ) {
                                    qDebug() << "tsiIPPort : " << subObj["tsiIPPort"].toInt();
                                }
                          }
                        }
                    }
                }
            }
        }
        return true;
}

//打开串口并初始化
void MainWindow::openSerialPort()
{

        serialPortset = new QSerialPort;
        serialPortset->setPortName(ui->lineEditPortName->text());//QSerialPort读取串口信息serialPortset，并将串口comComboBox的名称设置为当前找到的串口名
        //打开串口读写功能
        if(serialPortset->open(QIODevice::ReadWrite))
        {
            ui->comInformationTextEdit->append(ui->lineEditPortName->text()+": Ready...");
            ui->comStatusLabel->setText(ui->lineEditPortName->text()+" Ready...");
            serialPortset->setBaudRate(ui->lineEditBaudRate->text().toInt());//将当前波特率以整形显示在boteComboBox

            if(ui->lineEditDataBits->text()=="8")
                serialPortset->setDataBits(QSerialPort::Data8); //setDataBits选择Data8

            if(ui->lineEditParityBits->text()=="none")    //校验位选择
                serialPortset->setParity(QSerialPort::NoParity);

            if(ui->lineEditStopBits->text()=="1") //停止位选择
                serialPortset->setStopBits(QSerialPort::OneStop);

            readTimer->start(time_interval);
            opened=true;
        }
        else{
            ui->comInformationTextEdit->append("Open "+ui->lineEditPortName->text()+" Failed...");
            ui->comStatusLabel->setText(ui->lineEditPortName->text()+": Failed...");
            opened=false;
        }
        //qDebug() << " Open ：yes\r\n";
    }

void MainWindow::closeSerialPort()
{
        serialPortset->clear();       //serialPortset对象数据清空
        serialPortset->close();       //关闭
        serialPortset->deleteLater(); //删除缓存
        readTimer->stop();
        //        qDebug() << " Close ：yes\r\n";

}
//读取串口数据,获取太阳辐射强度
void MainWindow::readSPData()//创建一个readData对象
{
    if(opened)
    {
        commandTSI=SP485Addr+"0300000001840A"; //从地址01 按功能码03 起始地址0000 读16位的数据 crc16校验
        QByteArray senddata;
        StringToHex(commandTSI,senddata);//将commandTSI字符串转换为16进制的形式
        serialPortset->write(senddata);//发送到串口
        serialPortset->flush();
        QByteArray reception_buffer;
        int num = static_cast<int>(serialPortset->bytesAvailable());
        qDebug()<<num;
        if(num==7)
        {
        //if (serialPortset->waitForReadyRead(500)) {
            reception_buffer = serialPortset->readAll();
            //while (serialPortset.waitForReadyRead(10)) {
            //    reception_buffer += serialPortset->readAll();
            //}
            qDebug() << "reception_buffer ready";
            current_date_time =QDateTime::currentDateTime();
            current_date =current_date_time.toString("yyyy-MM-dd");
            current_time =current_date_time.toString("hh:mm:ss");
            bool ok;
            QByteArray tsi;
            tsi.resize(2);
            tsi=reception_buffer.mid(3,2);
            recvData=tsi.toHex();
            recvTSI=recvData.toInt(&ok,16);

            //recvTSI=recvData.toInt(&ok,10);
            if(recvTSI<0 || recvTSI>1800)
                recvTSI=0;
            qDebug()<<current_date<<" "<<recvData<<" "<<recvTSI<<" ";
            recvTSI_str=QString::number(recvTSI);
            tsiDisplay();
            //ui->comInformationTextEdit->append()
        }
        else {
            qDebug() << "Timeout";
        }

    }

}
//tsi显示
void MainWindow::tsiDisplay()
{
    if(opened)
    {
        ui->comInformationTextEdit->append(current_date+" "+current_time+": "+QString::number(recvTSI)+" w/m^2");
    }
}


void MainWindow::on_gpsPlainTextEdit_textChanged()
{
    QTextCursor cursor=ui->gpsPlainTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->gpsPlainTextEdit->setTextCursor(cursor);
}
