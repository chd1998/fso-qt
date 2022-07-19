#include "threadb.h"
#include "mainwindow.h"
#include <algorithm>
#include <execution>

threadB::threadB()
{
    countB=0;
    src="B";
    //if(destimg != NULL)
        //delete destimg;

}

void threadB::working()
{
    destimg=new unsigned short[imgX*imgY];
    while(!QThread::currentThread()->isInterruptionRequested())
    {
        QElapsedTimer t;
        t.start();
        //lockB.lock();
        //Blocked=true;

        uint imglen=imgX*imgY;
         if(!pausedB && !stoppedB && startedB && !pausedA && !stoppedA && startedA)
         {
            if(imgQueue.try_dequeue(destimg))
            {
                histmax=0;
                QVector<uint>vechistdata(65536,0);
                for(uint i=0;i<imglen;++i)
                {
                    int idx=destimg[i];
                    ++vechistdata[idx];
                    if(vechistdata[idx]>histmax)
                    {
                        histmax=vechistdata[idx];
                        histindex=idx;
                    }
               }


                emit fromB(src,countB,"Image dequeued in Hist Calc...");
                emit histReady(vechistdata,histmax,histindex);
                QVector<uint> nullvec;
                vechistdata.swap(nullvec);

             }
             else
             {
                    qDebug()<<countB1<<": Dequeue failed in Hist Calc, pls wait...";
                    emit fromB(src,countB1,"Image dequeue in hist calc failed...");
                    countB1++;
             }


            //countB++;
            //qDebug()<<countB1;
            //countB1++;


            //lockB.unlock();
            //Blocked=false;

            while(t.elapsed()<histRate)
                QCoreApplication::processEvents();


        }
    }
    delete[] destimg;
}

void threadB::calcHist()
{

}

