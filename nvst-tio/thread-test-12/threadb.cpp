#include "threadb.h"
#include "mainwindow.h"
#include <algorithm>
#include <execution>

threadB::threadB()
{
    countB=0;
    countB1=0;
    src="B";
    //if(destimg != NULL)
        //delete destimg;

}

threadB::~threadB()
{

}

void threadB::working()
{
    //destimg=new unsigned short[imgX*imgY];
    while(startedB)
    {
        QElapsedTimer t;
        t.start();

        lockB.lock();
        if(pausedB || pausedA)
            pauseCondB.wait(&lockB);
        lockB.unlock();

        uint imglen=imgX*imgY;

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
            if(!stoppedA || !pausedA)
                emit histReady(vechistdata,histmax,histindex);
            QVector<uint> nullvec;
            vechistdata.swap(nullvec);
            //qDebug()<<src<<" "<<countB<<" "<<imgQueue.size_approx();
         }
         else
         {
            //qDebug()<<countB1<<": Dequeue failed in Hist Calc, pls wait...";
            emit fromB(src,countB1,"Image dequeue in hist calc failed...");
            countB1++;
         }

         while(t.elapsed()<histRate)
             QCoreApplication::processEvents();

    }
    qDebug()<<"ThreadB finished...";
    //delete[] destimg;
    //emit finished();
}

//void threadB::finished()
//{
//    stoppedB=true;
//}

