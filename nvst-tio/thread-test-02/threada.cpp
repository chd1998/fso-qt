#include "threada.h"
#include "mainwindow.h"

threadA::threadA()
{
    countA=0;
    src="A";
}

void threadA::working()
{
    qDebug()<<"inside A-01";
    while(!QThread::currentThread()->isInterruptionRequested())
    //while(!stoppedA)
    {
        //if(QThread::currentThread()->isInterruptionRequested())
        //{
        //    break;
        //}
        qDebug()<<"inside A-02";
        lockA.lock();
        emit fromA(src,countA);
        countA++;
        lockA.unlock();
        qDebug()<<"inside A-03";
        QElapsedTimer t;
        t.start();
        while(t.elapsed()<100)
            QCoreApplication::processEvents();

        /*if(countA==10 )
        {
            emit finished(src);
            QThread::currentThread()->requestInterruption();
        }*/
    }
}

//void threadA::finished()
//{

//}
