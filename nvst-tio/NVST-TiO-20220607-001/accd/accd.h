#ifndef ACCD_H
#define ACCD_H

#include <QThread>
#include <QString>
#include <QDateTime>
#include <QImage>
#include <QDir>
class aCCD:public QThread
{
    Q_OBJECT
public:
    aCCD(QObject *parent);
    ~aCCD()
    {
        requestInterruption();
        quit();
        wait();
    }
    void run();
    int init_aCCD();
    void getData();
    int saveData(QString,unsigned short*);
    //void buf2image(unsigned short*);

    /*unsigned char *buffer = nullptr;
    unsigned short *unpackedBuffer = nullptr;
    unsigned char *bufferback = nullptr;
    unsigned short *unpackedBufferback = nullptr;*/
    unsigned char *buffer = NULL;
    unsigned short *unpackedBuffer = NULL;
    unsigned char *bufferback = NULL;
    unsigned short *unpackedBufferback = NULL;
    unsigned short *unpackedBufferback01 = NULL;
    //QString saveDir;
    //QString current_date_d;
    //QString current_date_t;
    QByteArray tmpchar;
    char *fkword;
    QDateTime t1,t2;
    int lt1,lt2,dt;
    bool localsave;
    //QDir sdir;
    //=current_date_time.toString("hhmmss");

    //int serialNo=0;
signals:
    void buf_Ready(unsigned short*);
    void calcHist(QVector<unsigned short>);
    void stop_Acq();
};

#endif // ACCD_H
