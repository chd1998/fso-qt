#ifndef TIME_SYS_H
#define TIME_SYS_H

#include <QThread>
#include <QString>
#include <QDateTime>
class time_Sys:public QThread
{
    Q_OBJECT
public:
    time_Sys(QObject *parent);
    void run();
signals:
    void onesecPassed();

//slots:

};

#endif // TIME_SYS_H
