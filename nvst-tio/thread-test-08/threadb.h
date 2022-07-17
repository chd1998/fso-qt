#ifndef THREADB_H
#define THREADB_H

#include <QObject>
#include <QDebug>
#include <QMutex>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QThread>
#include <iostream>
#include <vector>
#include <algorithm>
#include <execution>
//#include "concurrentqueue.h"

class threadB : public QObject
{
    Q_OBJECT
public:
    threadB();
    QString src;
    int countB;
    QVector<unsigned short>vechistdata;
    void working();
    void calcHist();

    //bool histfinished;

signals:
    void fromB(QString,int);
    void histReady(QVector<unsigned short>,int,int);
    void finished(QString);


//public slots:
    //void calcHist();

};

#endif // THREADB_H
