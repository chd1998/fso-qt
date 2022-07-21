#ifndef THREADA_H
#define THREADA_H

#include <QObject>
#include <QDebug>
#include <QMutex>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QThread>
#include <complex>
#include <random>
//#include "concurrentqueue.h"

class threadA : public QObject
{
    Q_OBJECT
public:
    threadA();
    ~threadA();
    //int countA;
    QString src;
    std::random_device rd{};
    std::mt19937 gen{};
    std::normal_distribution<> d;
    unsigned short* srcimg=nullptr;

    void working();



signals:
    void fromA(QString,int,QString);
    //void finished();
    //void imgReady();


};

#endif // THREADA_H
