#include "time_sys.h"
#include <QDateTime>
#include <QString>
#include <QThread>

time_Sys::time_Sys(QObject *parent):QThread(parent)
{

}
void time_Sys::run()
{
    while(!isInterruptionRequested()){
        msleep(1000);
        emit onesecPassed();
    }
}

void time_Sys::getTime()
{
    //while(1){

    //}
}

//void time_Sys::sendTime()
//{
//}

