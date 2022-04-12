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
        if(!stoppedB || !pausedB)
            emit fromB(src,countB);

        //calcHist();
        if(countB != 0)
            histfirst = false;
        countB++;

        lockB.unlock();
        QElapsedTimer t;
        t.start();
        while(t.elapsed()<histRate)
            QCoreApplication::processEvents();


        //if(countB==10 )
        //{
            //emit finished(src);
            //QThread::currentThread()->requestInterruption();
        //}
    }
}
void threadB::calcHist(QVector<unsigned short>myvecimg)
{
    if(syncAB)
    {
        QVector<unsigned short>vechistdata(65536,0);

        histmax=0;
        if(startedA)
        {
            for(int i=0;i != myvecimg.size();++i)
            {
                int idx=myvecimg.at(i);
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
        qDebug()<<"idx= "<<histindex<<" max= "<<histmax<<" vecimg size= "<<myvecimg.size();
        //std::sort(std::execution::par_unseq, vechistdata.begin(), vechistdata.end());
        //qDebug()<<"idx="<<histindex<<" Value="<<vechistdata.at(histindex);

        if(!histlocked)
            emit histReady(vechistdata,histmax,histindex);
    }
}
//void threadB::finished()
//{

//}
