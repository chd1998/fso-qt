#ifndef TIME_SYS_H
#define TIME_SYS_H

#include <QThread>
#include <QObject>

class time_Sys:public QThread
{
    Q_OBJECT
public:
    time_Sys(QObject *parent);
    void run();
    void getTime();

signals:
    void onesecPassed();

//slots:

};

#endif // TIME_SYS_H
