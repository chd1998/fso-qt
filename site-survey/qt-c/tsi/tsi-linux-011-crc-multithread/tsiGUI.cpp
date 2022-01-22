#include "tsiGUI.h"
#include "myserver.h"
#include "QDebug"
#include "ui_mainwindow.h"
#include "mythread.h"
#include "Crc16Class.h"

using std::cout;
using namespace Platform::Communication;

QString myIP="*",commandTSI;
uint myPort=7071;
QString SPName="/dev/ttyUSB1",SPParityBits="none",devID="TSI-01",tsiIP="*",SP485Addr="01",recvData="";
int SPBaudRate=4800,SPDataBits=8,SPStopBits=1,SPTimeInterval=1,tsiIPPort=7071;
bool cfgStatus=false,dataok=false;
QString recvTSI_str,current_date,current_time;
QDateTime current_date_time;
int recvTSI=0;

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
        ui->lineEdit_sp485addr->setText(SP485Addr);
        ui->lineEditRefreshTime->setText(QString::number(SPTimeInterval));
        ui->lineEditIP->setText(tsiIP);
        ui->lineEditPort->setText(QString::number(tsiIPPort));
        ui->labelDev->setText(" "+devID+" Ready");
    }
    else
    {
        qDebug()<<"Using default settings...";
        ui->lineEditPortName->setText(SPName);
        ui->lineEditBaudRate->setText(QString::number(SPBaudRate));
        ui->lineEditDataBits->setText(QString::number(SPDataBits));
        ui->lineEditParityBits->setText(SPParityBits);
        ui->lineEditStopBits->setText(QString::number(SPStopBits));
        ui->lineEditRefreshTime->setText(QString::number(SPTimeInterval));
        ui->lineEditIP->setText(tsiIP);
        ui->lineEditPort->setText(QString::number(tsiIPPort));
        ui->labelDev->setText(devID);
        ui->lineEdit_sp485addr->setText(SP485Addr);
        bool wstatus=writeCfg();
        if(wstatus)
            qDebug()<<"cfg file generated...";
        else
            qDebug()<<"generating cfg file failed...";
    }
    myPort=tsiIPPort;
    time_interval=SPTimeInterval*1000;
    readTimer = new QTimer(this);
    //    connect(ui->searchButton,SIGNAL(clicked()),this,SLOT(on_searchButton_clicked()));     //ui上使用槽后会自动connect,不用再connect，否则会链接两遍
    //    connect(ui->openComButton,SIGNAL(clicked()),this,SLOT(on_openComButton_clicked()));
    connect(readTimer,SIGNAL(timeout()),this,SLOT(readSPData()));
    setFixedSize(this->width(), this->height());
    this->setWindowTitle("TSI for Site Survey (Ver. 0.1.1 - MultiThread)");
    qDebug()<<"open serial port...";
    openSerialPort();
    qDebug()<<"open server...";
    MyServer* tsi01=new MyServer(this);
    connect(tsi01,&MyServer::sendMsg2UI,this,&MainWindow::displayMsg);
    tsi01->startServer();

}

MainWindow::~MainWindow()
{
    if(opened)
        closeSerialPort();
    delete ui;
}

void MainWindow::displayMsg(QString myMsg)
{
    ui->tsiPlainTextEdit->appendPlainText(myMsg);
}

void MainWindow::displayMsg01()
{
    ui->tsiPlainTextEdit->appendPlainText("New Client Connected...");
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

//生成配置文件
bool MainWindow::writeCfg()
{
    //QFile cfgfile(QDir::homePath() + "\\gps_site.json");
    QFile cfgfile(QDir::currentPath() + "//tsi-"+devID+".json");
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

//打开配置文件
bool MainWindow::openCfg()
{
    //打开Json文件并用QByteArray读取全部内容
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
            ui->comInformationTextEdit->append(ui->lineEditPortName->text()+" Ready...");
            serialPortset->setBaudRate(ui->lineEditBaudRate->text().toInt());//将当前波特率以整形显示在boteComboBox

            if(ui->lineEditDataBits->text()=="8")
                serialPortset->setDataBits(QSerialPort::Data8); //setDataBits选择Data8

            if(ui->lineEditParityBits->text()=="none")    //校验位选择
                serialPortset->setParity(QSerialPort::NoParity);

            if(ui->lineEditStopBits->text()=="1") //停止位选择
                serialPortset->setStopBits(QSerialPort::OneStop);

            readTimer->start(time_interval);
            opened=true;
            ui->comStatusLabel->setText(ui->lineEditPortName->text()+" Ready...");
        }
        else{
            ui->comInformationTextEdit->append("Open "+ui->lineEditPortName->text()+" Failed...");
            ui->comStatusLabel->setText(ui->lineEditPortName->text()+" Failed...");
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
//读取串口数据类型为$GPRMC，可以得到经纬度，日期时间，速度等信息
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
        //if (serialPortset->waitForReadyRead(200)) {
        int num = static_cast<int>(serialPortset->bytesAvailable());
        qDebug()<<num;
        if(num==7)
        {
            reception_buffer = serialPortset->readAll();
            current_date_time =QDateTime::currentDateTime();
            current_date =current_date_time.toString("yyyy-MM-dd");
            current_time =current_date_time.toString(" hh:mm:ss");
            //while (serialPortset.waitForReadyRead(10)) {
            //    reception_buffer += serialPortset->readAll();
            //}
            Platform::Communication::Crc16Class crc16;
            dataok=crc16.crc_Checking(reception_buffer.toHex());
            if(dataok)
            {
                qDebug() << "reception_buffer ready";
                bool ok;
                QByteArray tsi;
                tsi.resize(2);
                tsi=reception_buffer.mid(3,2);
                recvData=tsi.toHex();
                recvTSI=recvData.toInt(&ok,16);
                //recvTSI=recvData.toInt(&ok,10);
                if(recvTSI<0 || recvTSI>1800)
                    recvTSI=0;
                qDebug()<<current_date<<" "<<current_time<<" "<<recvData<<" "<<recvTSI<<" ";
                recvTSI_str=QString::number(recvTSI);
                tsiDisplay();
            }else{
                recvTSI=0;
                recvTSI_str="NaN";
                tsiDisplay();
                qDebug()<<"reception_buffer data Error...";
            }
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
    if(opened && dataok)
    {
        ui->comInformationTextEdit->append(current_date+" "+current_time+" : "+devID+" "+QString::number(recvTSI)+" w/m^2");
    }
    else
    {
        if(!dataok)
          ui->comInformationTextEdit->append(current_date+" "+current_time+" : "+devID+" Data Error, pls check...");
        if(!opened)
          ui->comInformationTextEdit->append(current_date+" "+current_time+" : open "+devID+" "+SPName+" failed...");
        //ui->textEdit_tsiData->append(current_date+": "+devID+" Data Error...");
    }
}
//纬度模块

void MainWindow::on_tsiPlainTextEdit_textChanged()
{
    QTextCursor cursor=ui->tsiPlainTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->tsiPlainTextEdit->setTextCursor(cursor);
}
