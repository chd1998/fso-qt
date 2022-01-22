#include <QCoreApplication>
#include <QByteArray>
#include "Crc16Class.h"

using namespace Platform::Communication;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QByteArray in = QByteArray::fromHex("0103020064b9af").toHex();
    /*QByteArray in;
    in.resize(7);
    in[0]=0x01;
    in[1]=0x03;
    in[2]=0x02;
    in[3]=0x00;
    in[4]=0x64;
    in[5]=0xb9;
    in[6]=0xaf;*/
    qDebug()<<in;
    //QString input="0103020064b9af";
    QString input=in;
    //QString input1="0103020064";
    Platform::Communication::Crc16Class crc16M;
    //crc16M.crc16("0103020064",5);
    if(crc16M.crc_Checking(input))
        qDebug()<<"check passed";
    else
        qDebug()<<"check failed";

    return a.exec();
}
