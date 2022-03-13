#ifndef THREADB_H
#define THREADB_H

#include <QObject>
#include <QDebug>
#include <QMutex>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QThread>

class threadB : public QObject
{
    Q_OBJECT
public:
    threadB();
    QString src;
    int countB;
    void working();

signals:
    void fromB(QString,int);
    void finished(QString);

};

#endif // THREADB_H
