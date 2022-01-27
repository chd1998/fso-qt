#include "gpsGUI.h"
#include "myserver.h"
#include "QDebug"
#include "ui_mainwindow.h"
#include "sp2tcpserver.h"

using std::cout;

QString myIP="Any";
uint myPort=7070;
QString lat_str="";      //纬度
QString lon_str="";     //经度
QString time_str="";          //时间
QString date_str="";          //日期
QString alt_str="";         //高度
QString lat_str_ip="";      //纬度 for ip
QString lon_str_ip="";     //经度 for ip
QString NetTxt="";
QString SPName="COM1",SPParityBits="none",siteID="Site-01",gpsIP="*";
int SPBaudRate=9600,SPDataBits=8,SPStopBits=1,SPTimeInterval=1,gpsIPPort=7070;
bool cfgStatus=false;

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
        ui->lineEditRefreshTime->setText(QString::number(SPTimeInterval));
        ui->lineEditIP->setText(gpsIP);
        ui->lineEditPort->setText(QString::number(gpsIPPort));
        ui->labelSite->setText(siteID);
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
        ui->lineEditIP->setText(gpsIP);
        ui->lineEditPort->setText(QString::number(gpsIPPort));
        ui->labelSite->setText(siteID);
        bool wstatus=writeCfg();
        if(wstatus)
            qDebug()<<"cfg file generated...";
        else
            qDebug()<<"generating cfg file failed...";
    }
    myPort=gpsIPPort;
    time_interval=SPTimeInterval*1000;
    readTimer = new QTimer(this);
    //    connect(ui->searchButton,SIGNAL(clicked()),this,SLOT(on_searchButton_clicked()));     //ui上使用槽后会自动connect,不用再connect，否则会链接两遍
    //    connect(ui->openComButton,SIGNAL(clicked()),this,SLOT(on_openComButton_clicked()));
    connect(readTimer,SIGNAL(timeout()),this,SLOT(readSPData()));
    setFixedSize(this->width(), this->height());
    this->setWindowTitle("GPS for Site Survey (Ver. 0.1.1 - MultiThread)");
    qDebug()<<"open serial port...";
    openSerialPort();
    qDebug()<<"open server...";
    MyServer* gps01=new MyServer(this);
    gps01->startServer();
    connect(gps01,&MyServer::sendMsg2UI,this,&MainWindow::displayMsg);
    //connect(gps01,&MyServer::sendMsg2UI,this,&MainWindow::displayMsg);
    //MyServer gps01;
    //gps01.startServer();
    //gps01.listen(QHostAddress::Any, myPort);
    //connect(&gps01,&MyServer::sendMsg2UI,this,&MainWindow::displayMsg);
    //connect(&gps01,&MyServer::newConnection,this,&MainWindow::displayMsg01);
    //qDebug()<<"1";
}

MainWindow::~MainWindow()
{
    closeSerialPort();
    delete ui;
}

void MainWindow::displayMsg(QString myMsg)
{
    ui->gpsPlainTextEdit->appendPlainText(myMsg);
}

void MainWindow::displayMsg01()
{
    ui->gpsPlainTextEdit->appendPlainText("New Client Connected...");
}

//生成配置文件
bool MainWindow::writeCfg()
{
    //QFile cfgfile(QDir::homePath() + "\\gps_site.json");
    QFile cfgfile("f:\\gps\\gps-site-01.json");
    if(!cfgfile.open(QIODevice::WriteOnly))
    {
        qDebug()<<"cfg file open error...";
        return false;
    }else{
        qDebug()<<"cfg file wait for write...";
    }
    cfgfile.resize(0);

    QJsonArray gps_site;
    QJsonObject siteidobj,sitedesobj,spobj,spobj_site,sptimeobj,updateobj,gpsipobj,gpsipobj_site;
    QJsonObject site;

    //rootobjobj.insert("Site ID","site_01");
    //branchobjobj.insert("GPS Site",leafobjobj);
    //gps_site.append(rootobjobj);
    //clearJsonObject(rootobjobj);
    //clearJsonObject(branchobjobj);
    siteidobj.insert("Site ID",siteID);
    sitedesobj.insert("Site Desc.",siteidobj);
    gps_site.append(sitedesobj);
    spobj.insert("SPName",SPName);
    spobj.insert("SPBaudRate",SPBaudRate);
    spobj.insert("SPDataBits",SPDataBits);
    spobj.insert("SPParityBits",SPParityBits);
    spobj.insert("SPStopBits",SPStopBits);
    //spobj.insert("SPStopBits",1);
    spobj_site.insert("GPS Serial Port",spobj);
    gps_site.append(spobj_site);
    sptimeobj.insert("SPTimeInterval",SPTimeInterval);
    updateobj.insert("Update Freq.", sptimeobj);
    gps_site.append(updateobj);
    gpsipobj.insert("gpsIP",gpsIP);
    gpsipobj.insert("gpsIPPort",gpsIPPort);
    gpsipobj_site.insert("GPS IP Info",gpsipobj);
    gps_site.append(gpsipobj_site);

    site.insert("GPS Site",gps_site);
    QJsonDocument jsonDocument;
    jsonDocument.setObject(site);
    //QByteArray byteArray=jsonDocument.toJson(QJsonDocument::Indented);

    cfgfile.write(jsonDocument.toJson(QJsonDocument::Indented));

    cfgfile.close();
    return true;
}

//打开配置文件
bool MainWindow::openCfg()
{
    //打开Json文件并用QByteArray读取全部内容
        QFile file("f:\\gps\\gps-site-01.json");
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
                                if(subObj.contains("Site ID") ) {
                                    qDebug() << "Site ID : " << subObj["Site ID"].toString();
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
                                if (subObj.contains("SPTimeInterval") ) {
                                    qDebug() << "SPTimeInterval : " << subObj["SPTimeInterval"].toInt();
                                }
                                if (subObj.contains("gpsIP") ) {
                                    qDebug() << "gpsIP : " << subObj["gpsIP"].toString();
                                }
                                if (subObj.contains("gpsIPPort") ) {
                                    qDebug() << "gpsIPPort : " << subObj["gpsIPPort"].toInt();
                                }
                          }
                        }
                    }
                }
            }
        }
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
        QByteArray GPS_Data = serialPortset->readAll();//读取所有数据存入QByteArray类的对象GPS_Data里
        QByteArray GPS_Data_2 = GPS_Data;
        if(!GPS_Data.isEmpty())
        {
            //ui->gpsPlainTextEdit->appendPlainText(GPS_Data);//大框里显示所有接收数据
            if(GPS_Data.contains("$GNRMC"))                //如果这串数据中包含$GNRMC（具体百度GPS帧格式，了解接收数据的帧格式）
            {

                GPS_Data.remove(0,GPS_Data.indexOf("$GNRMC")); //删除数据起始到$GNRMC之间的数据
                if(GPS_Data.contains("*"))                     //如果剩下的字符中包含*
                {

                    GPS_RMC = GPS_Data.left(GPS_Data.indexOf("*"));//则储存*之前的数据到GPS_RMC，（具体参阅帧格式）。此部分数据即为所需的数据
                    GPS_list<<GPS_RMC.split(',');  //注意为单引号
                    gpsDisplay();
                }

            }
            if(GPS_Data_2.contains("$GNGGA"))
            {
                GPS_Data_2.remove(0,GPS_Data_2.indexOf("$GNGGA"));
                if(GPS_Data_2.contains("*"))
                {
                    GPS_RMC_2 = GPS_Data_2.left(GPS_Data_2.indexOf("*"));
                    GPS_list_2 << GPS_RMC_2.split(',');
                    gpsDisplay_2();
                }
            }

        }
        ui->comInformationTextEdit->append(lat_str+","+lon_str+","+alt_str+","+time_str+","+date_str);
        GPS_list.clear();
        GPS_list_2.clear();
        GPS_Data.clear();
        GPS_Data_2.clear();
    }

}
//gps显示
void MainWindow::gpsDisplay()
{
    if(opened)
    {
        //qDebug() << "Send: " << ui->gpsPlainTextEdit->toPlainText();
        /*QString lat_str;      //纬度
        QString lon_str;     //经度
        QString time_str;          //时间
        //    QString direction_str;
        //    QString speed;         //速度
        //    QString status;        //状态
        QString date_str;          //日期*/
        ui->latitudeLineEdit->setText(latitude(lat_str));//显示纬度
        ui->longitudeLineEdit->setText(longitude(lon_str));//显示经度
        ui->timeLineEdit->setText(time(time_str));//显示时间
        ui->speedLineEdit->setText(GPS_list[7]);//显示速度
        if(GPS_list[2].contains("A"))
            ui->statusLineEdit->setText("GPS Valid");//显示状态
        else
            ui->statusLineEdit->setText("GPS inValid");
        ui->dateLineEdit->setText(date(date_str)); //显示日期
        //qDebug() << "$GNRMC";
        //    ui->directionLineEdit->setText(direction(direction_str));
    }
}
//纬度模块
QString& MainWindow::latitude(QString& lat_str)
{
    lat_str.clear();
    QByteArray lat_data = GPS_list[3];      //详见帧格式
    //qDebug()<<lat_data;
    float second = lat_data.mid(5,5).toFloat()/10000*60;  //lat_data.mid(5,4)为末尾小数，toInt后变为四位的整数，除以10000再变回小数，乘以60换算为秒
    //qDebug()<<second;
    QString str = QString::number(second);
    if(GPS_list[4]=="N")
    {
        lat_str = ("N")+lat_data.mid(0,2)+(u8"°")+lat_data.mid(2,2)+("\'")+str.mid(0,2)+("\"");
        lat_str_ip = ("N")+lat_data.mid(0,2)+("-")+lat_data.mid(2,2)+("-")+str.mid(0,2);
    }
    else
    {
        lat_str = ("S")+lat_data.mid(0,2)+(u8"°")+lat_data.mid(2,2)+("\'")+str.mid(0,2)+("\"");
        lat_str_ip = ("S")+lat_data.mid(0,2)+("-")+lat_data.mid(2,2)+("-")+str.mid(0,2);
    }
    return lat_str;
}
//经度模块
QString& MainWindow::longitude(QString& lon_str)
{
    lon_str.clear();
    QByteArray lon_data = GPS_list[5];
    //qDebug()<<lon_data;
    float second2 = lon_data.mid(6,5).toFloat()/10000*60;
    QString str2 = QString::number(second2);
    if(GPS_list[6]=="E")
    {
        lon_str = ("E")+lon_data.mid(0,3)+(u8"°")+lon_data.mid(2,2)+("\'")+str2.mid(0,2)+("\"");
        lon_str_ip = ("E")+lon_data.mid(0,3)+("-")+lon_data.mid(2,2)+("-")+str2.mid(0,2);
    }
    else
    {
        lon_str = ("W")+lon_data.mid(0,3)+(u8"°")+lon_data.mid(2,2)+("\'")+str2.mid(0,2)+("\"");
        lon_str_ip = ("W")+lon_data.mid(0,3)+("-")+lon_data.mid(2,2)+("-")+str2.mid(0,2);
    }
    return lon_str;
}
//时间模块
QString& MainWindow::time(QString& time_str)
{
    time_str.clear();
    QByteArray time_data = GPS_list[1];
    QString htmp=QString("%1").arg(time_data.mid(0,2).toInt()+8, 2, 10, QLatin1Char('0'));
    time_str = htmp+(":")+time_data.mid(2,2)+(":")+time_data.mid(4,2);//东八区
    //time_str = QString::number(time_data.mid(0,2).toInt()+8)+(":")+time_data.mid(2,2)+(":")+time_data.mid(4,2);//东八区
//    qDebug() << "$GNRMC";
    return time_str;
}
//日期模块
QString& MainWindow::date(QString& date_str)
{
    date_str.clear();
    QByteArray date_data = GPS_list[9];
    date_str = ("20")+date_data.mid(4,2)+("-")+date_data.mid(2,2)+("-")+date_data.mid(0,2);
    return date_str;
}

//QString& MainWindow::direction(QString& direction_str)//角度模块
//{
//    direction_str.clear();
//    QByteArray direction_data = GPS_list[8];
//    direction_str = direction_data+("度");
//    return direction_str;
//}
//显示当前可用卫星数
void MainWindow::gpsDisplay_2()
{
    //alt_str;
    ui->lineEditAlt->setText(getAlt(alt_str)+"m");
    QString satellite_str;
    ui->satelliteLineEdit->setText(satellite(satellite_str));

}

//高度模块
QString& MainWindow::getAlt(QString& alt_str)
{
    alt_str.clear();
    QByteArray alt_data = GPS_list_2[9];
    alt_str = alt_data.mid(0,6);
    //qDebug()<<alt_str;
    return alt_str;
}
//卫星模块
QString& MainWindow::satellite(QString& satellite_str)
{
    satellite_str.clear();
    QByteArray satellite_data = GPS_list_2[7];
    satellite_str = satellite_data.mid(0,2);
    return satellite_str;
}

void MainWindow::on_gpsPlainTextEdit_textChanged()
{
    QTextCursor cursor=ui->gpsPlainTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->gpsPlainTextEdit->setTextCursor(cursor);
}
