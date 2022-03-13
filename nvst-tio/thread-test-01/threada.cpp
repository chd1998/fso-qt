#include "threada.h"
#include "mainwindow.h"

threadA::threadA()
{
    countA=0;
    src="A";
}

void threadA::working()
{
    //while(!QThread::currentThread()->isInterruptionRequested())
    while(!stoppedA)
    {
        //if(QThread::currentThread()->isInterruptionRequested())
        //{
        //    break;
        //}
        lockA.lock();
        emit fromA(src,countA);
        countA++;

        lockA.unlock();
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
