#ifndef THREADA_H
#define THREADA_H

#include <QObject>
#include <QDebug>
#include <QMutex>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QThread>
#include <complex>

class threadA : public QObject
{
    Q_OBJECT
public:
    threadA();
    int countA;
    QString src;
    void working();


signals:
    void fromA(QString,int);
    void imgReady();
    void finished(QString);

};

#endif // THREADA_H
