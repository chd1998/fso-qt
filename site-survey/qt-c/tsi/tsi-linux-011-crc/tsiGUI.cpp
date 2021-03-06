#include "tsiGUI.h"
#include "QDebug"
#include "ui_mainwindow.h"
#include "sp2tcpserver.h"
#include "Crc16Class.h"
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
using namespace Platform::Communication;

QString SPName="COM3",SPParityBits="none",devID="TSI-01",tsiIP="*",SP485Addr="01",commandTSI="",recvData="";
int recvTSI=0;
int SPBaudRate=4800,SPDataBits=8,SPStopBits=1,SPTimeInterval=1,tsiIPPort=7071;
bool cfgStatus=false,dataok=false;
QDateTime current_date_time;
QString current_date="",recvTSI_str="";


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
    //    connect(ui->searchButton,SIGNAL(clicked()),this,SLOT(on_searchButton_clicked()));     //ui????????????????????????connect,?????????connect????????????????????????
    //    connect(ui->openComButton,SIGNAL(clicked()),this,SLOT(on_openComButton_clicked()));
    connect(readTimer,SIGNAL(timeout()),this,SLOT(readSPData()));
    setFixedSize(this->width(), this->height());
    this->setWindowTitle("TSI for Site Survey (Ver. 0.1.1 with CRC16-MODBUS)");
    SP2TCPServer *myserver1=new SP2TCPServer(this);
    connect(myserver1,&SP2TCPServer::sendMsg2UI,this,&MainWindow::displayMsg);
    //myserver1->start();
    openSerialPort();
}

MainWindow::~MainWindow()
{
    if(opened)
        closeSerialPort();
    delete ui;
}

void MainWindow::StringToHex(QString str, QByteArray &senddata) //????????????????????????????????????0-F
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
    else return ch-ch;//??????0-f????????????????????????0
}
//crc16??????



void MainWindow::displayMsg(QString myMsg)
{
    ui->gpsPlainTextEdit->appendPlainText(myMsg);
}

//??????????????????
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
    QJsonObject devidobj,devdesobj,spobj,spobj_dev,sptimeobj,updateobj,tsiipobj,tsiipobj_dev;
    QJsonObject dev;

    //rootobjobj.insert("Site ID","site_01");
    //branchobjobj.insert("GPS Site",leafobjobj);
    //gps_site.append(rootobjobj);
    //clearJsonObject(rootobjobj);
    //clearJsonObject(branchobjobj);
    devidobj.insert("Dev ID",devID);
    devdesobj.insert("Site Desc.",devidobj);
    tsi_dev.append(devdesobj);
    spobj.insert("SPName",SPName);
    spobj.insert("SPBaudRate",SPBaudRate);
    spobj.insert("SPDataBits",SPDataBits);
    spobj.insert("SPParityBits",SPParityBits);
    spobj.insert("SPStopBits",SPStopBits);
    spobj.insert("SP485Addr",SP485Addr);
    //spobj.insert("SPStopBits",1);
    spobj_dev.insert("TSI Serial Port",spobj);
    tsi_dev.append(spobj_dev);
    sptimeobj.insert("SPTimeInterval",SPTimeInterval);
    updateobj.insert("Update Freq.", sptimeobj);
    tsi_dev.append(updateobj);
    tsiipobj.insert("tsiIP",tsiIP);
    tsiipobj.insert("tsiIPPort",tsiIPPort);
    tsiipobj_dev.insert("TSI IP Info",tsiipobj);
    tsi_dev.append(tsiipobj_dev);

    dev.insert("TSI Dev",tsi_dev);
    QJsonDocument jsonDocument;
    jsonDocument.setObject(dev);
    //QByteArray byteArray=jsonDocument.toJson(QJsonDocument::Indented);

    cfgfile.write(jsonDocument.toJson(QJsonDocument::Indented));

    cfgfile.close();
    return true;
}

//??????????????????
bool MainWindow::openCfg()
{
    //??????Json????????????QByteArray??????????????????
        //QFile file("f:\\gps\\gps-site-01.json");
        QFile file(QDir::currentPath() + "//tsi-"+devID+".json");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << QString("fail to open the json file...");
            return false;
        }
        QByteArray array = file.readAll();
        file.close();

        //???????????????????????????QJsonDocument????????????????????????????????????
        QJsonParseError jsonParseError;
        QJsonDocument jsonDocument(QJsonDocument::fromJson(array, &jsonParseError));
        if (QJsonParseError::NoError != jsonParseError.error) {
            qDebug() << QString("JsonParseError: %1").arg(jsonParseError.errorString());
            return false;
        }
        //QJsonDocument??????QJsonObject
        QJsonObject rootObject = jsonDocument.object();
        //traversJson(rootObject);
        //???????????????
        QStringList keys = rootObject.keys();
        for (int i = 0; i < keys.size(); i++) {
            qDebug() << "key" << i << " is:" << keys.at(i);
        }

        //???????????????????????????????????????????????????
        for (int i = 0; i < keys.size(); i++) {
            if (rootObject.contains(keys.at(i)) && rootObject[keys.at(i)].isArray()) {
                QJsonArray subArray = rootObject.value(keys.at(i)).toArray();
                for (int i = 0; i < subArray.size(); i++) {
                    if (subArray[i].isObject()) {
                        QStringList subKeys = subArray[i].toObject().keys();
                        for (int m = 0; m < subKeys.size(); m++) {
                            qDebug() << "Subkey is:" << subKeys.at(m);
                            //??????????????????????????????????????????????????????
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

//????????????????????????
void MainWindow::openSerialPort()
{

        serialPortset = new QSerialPort;
        serialPortset->setPortName(ui->lineEditPortName->text());//QSerialPort??????????????????serialPortset???????????????comComboBox??????????????????????????????????????????
        //????????????????????????
        if(serialPortset->open(QIODevice::ReadWrite))
        {
            ui->comInformationTextEdit->append(ui->lineEditPortName->text()+" Ready...");
            ui->comStatusLabel->setText(ui->lineEditPortName->text()+" Ready...");
            serialPortset->setBaudRate(ui->lineEditBaudRate->text().toInt());//????????????????????????????????????boteComboBox

            if(ui->lineEditDataBits->text()=="8")
                serialPortset->setDataBits(QSerialPort::Data8); //setDataBits??????Data8

            if(ui->lineEditParityBits->text()=="none")    //???????????????
                serialPortset->setParity(QSerialPort::NoParity);

            if(ui->lineEditStopBits->text()=="1") //???????????????
                serialPortset->setStopBits(QSerialPort::OneStop);

            readTimer->start(time_interval);
            opened=true;
        }
        else{
            ui->comInformationTextEdit->append("Open "+ui->lineEditPortName->text()+" Failed...");
            ui->comStatusLabel->setText(ui->lineEditPortName->text()+": Failed...");
            opened=false;
        }
        //qDebug() << " Open ???yes\r\n";
    }

void MainWindow::closeSerialPort()
{
        serialPortset->clear();       //serialPortset??????????????????
        serialPortset->close();       //??????
        serialPortset->deleteLater(); //????????????
        readTimer->stop();
        //        qDebug() << " Close ???yes\r\n";

}
//???????????????????????????$GPRMC?????????????????????????????????????????????????????????
void MainWindow::readSPData()//????????????readData??????
{
    if(opened)
    {
        commandTSI=SP485Addr+"0300000001840A"; //?????????01-SP485Addr ????????????03 ????????????0000 ???16???????????? crc16??????-0A84
        QByteArray senddata;
        StringToHex(commandTSI,senddata);//???commandTSI??????????????????16???????????????
        serialPortset->write(senddata);//???????????????
        serialPortset->waitForBytesWritten(100);
        QByteArray reception_buffer;
        if (serialPortset->waitForReadyRead(100)) {
            reception_buffer = serialPortset->readAll();
            //while (serialPortset.waitForReadyRead(10)) {
            //    reception_buffer += serialPortset->readAll();
            //}
            Platform::Communication::Crc16Class crc16;
            dataok=crc16.crc_Checking(reception_buffer.toHex());
            if(dataok)
            {
                qDebug() << "reception_buffer ready";
                current_date_time =QDateTime::currentDateTime();
                current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss");
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
            }else{
                tsiDisplay();
                qDebug()<<"Data Error, pls check...";
            }
            //ui->comInformationTextEdit->append()
        }
        else {
            qDebug() << "Timeout";
        }

    }

}
//tsi??????
void MainWindow::tsiDisplay()
{
    if(opened && dataok)
    {
        ui->comInformationTextEdit->append(current_date+": "+devID+" "+QString::number(recvTSI)+" w/m^2");
    }
    else
    {
        ui->comInformationTextEdit->append(current_date+": "+devID+" Data Error...");
    }
}


void MainWindow::on_gpsPlainTextEdit_textChanged()
{
    QTextCursor cursor=ui->gpsPlainTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->gpsPlainTextEdit->setTextCursor(cursor);
}
