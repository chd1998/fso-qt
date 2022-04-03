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

class threadB : public QObject
{
    Q_OBJECT
public:
    threadB();
    QString src;
    int countB;
    void working();
    bool histfinished;

signals:
    void fromB(QString,int);
    void histReady();
    void finished(QString);

public slots:
    void calcHist();

};

#endif // THREADB_H
