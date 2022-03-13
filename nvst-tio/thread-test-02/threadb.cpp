#include "threadb.h"
#include "mainwindow.h"

threadB::threadB()
{
    countB=0;
    src="B";
}

void threadB::working()
{
    //qDebug()<<"inside B";
    while(!QThread::currentThread()->isInterruptionRequested())
    //while(!stoppedB)
    {
        lockB.lock();
        //if(QThread::currentThread()->isInterruptionRequested())
            //break;
        emit fromB(src,countB);
        countB++;

        lockB.unlock();
        QElapsedTimer t;
        t.start();
        while(t.elapsed()<200)
            QCoreApplication::processEvents();


        //if(countB==10 )
        //{
            //emit finished(src);
            //QThread::currentThread()->requestInterruption();
        //}
    }
}

//void threadB::finished()
//{

//}
