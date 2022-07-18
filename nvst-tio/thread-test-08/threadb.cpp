#include "threadb.h"
#include "mainwindow.h"

threadB::threadB()
{
    countB=0;
    src="B";   
    //QVector<unsigned short>vechistdata(65536,0);
}

void threadB::working()
{
    //qDebug()<<"inside B";
    while(!QThread::currentThread()->isInterruptionRequested())
    //while(!stoppedB)
    {
        lockB.lock();
        //Blocked=true;
        if(!stoppedB || !pausedB )
        {
            emit fromB(src,countB);
        //if(!calchist_locked)
           calcHist();
        }

        /*if(imgQueue.size_approx() > MAXQUEUE){
            //imgQueueFull=true;
            imgQueue=moodycamel::ConcurrentQueue<unsigned short*>();
        }*/

        if(countB != 0)
            histfirst = false;
        //countB++;

        lockB.unlock();
        //Blocked=false;
        QElapsedTimer t;
        t.start();
        while(t.elapsed()<histRate || Alocked)
            QCoreApplication::processEvents();


        //if(countB==10 )
        //{
            //emit finished(src);
            //QThread::currentThread()->requestInterruption();
        //}
    }
}
void threadB::calcHist()
{
    //calchistlock.lock();
    //calchist_locked=true;
    if(imgQueue.try_dequeue(srcimg))
    {
        QVector<uint>vechistdata(65536,0);
        //vechistdata.clear();
        histmax=0;
        if(startedA)
        {
            for(int i=0;i<imgX*imgY;++i)
            {
                int idx=srcimg[i];
                //qDebug()<<"i: "<<i<<" Value: "<<vecimg.at(i);
                ++vechistdata[idx];
                //()<<"idx="<<idx<<" Value="<<vechistdata.at(idx);
                if(vechistdata[idx]>histmax)
                {
                    histmax=vechistdata[idx];
                    histindex=idx;
                }
                //std::cout<<idx<<"@"<<vechistdata[idx]<<std::endl;
           }
        }
        //qDebug()<<"Count "<<countB<<":"<<"idx= "<<histindex<<" max= "<<histmax<<" vecimg size= "<<sizeof(myvecimg);
        //std::sort(std::execution::par_unseq, vechistdata.begin(), vechistdata.end());
        //qDebug()<<"idx="<<histindex<<" Value="<<vechistdata.at(histindex);

        //if( !Alocked && !histlocked)
        //if( !histlocked )
        emit histReady(vechistdata,histmax,histindex);
        QVector<uint> nullvec;
        vechistdata.swap(nullvec);
    }
    //calchistlock.unlock();
    //calchist_locked=false;

}
//void threadB::finished()
//{

//}
