#include "threadb.h"
#include "mainwindow.h"
#include <algorithm>
#include <execution>

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
        //lockB.lock();
        //Blocked=true;
        if(startedA && startedB)
        {
            calcHist();
            emit fromB(src,countB);
        //if(!calchist_locked)

        }

        /*if(imgQueue.size_approx() > MAXQUEUE){
            //imgQueueFull=true;
            imgQueue=moodycamel::ConcurrentQueue<unsigned short*>();
        }*/

        if(countB != 0)
            histfirst = false;
        //countB++;

        //lockB.unlock();
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
    uint imglen=imgX*imgY;
    if(imgQueue.try_dequeue(srcimg))
    {
        histmax=0;
        QVector<uint>vechistdata(65536,0);
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
        /*QVector<unsigned short>localvecimg(srcimg,srcimg+imglen);
        //vechistdata.clear();
        histmax=0;

        std::for_each(std::execution::par_unseq,localvecimg.begin(),localvecimg.end(),[&](uint v)
        {
            int idx=v;
            //qDebug()<<"i: "<<i<<" Value: "<<vecimg.at(i);
            ++vechistdata[idx];
            //()<<"idx="<<idx<<" Value="<<vechistdata.at(idx);
            if(vechistdata[idx]>histmax)
            {
                histmax=vechistdata[idx];
                histindex=idx;
            }
            //std::cout<<idx<<"@"<<vechistdata[idx]<<std::endl;
       });*/

        //qDebug()<<"Count "<<countB<<":"<<"idx= "<<histindex<<" max= "<<histmax<<" vecimg size= "<<sizeof(myvecimg);
        //std::sort(std::execution::par_unseq, vechistdata.begin(), vechistdata.end());
        //qDebug()<<"idx="<<histindex<<" Value="<<vechistdata.at(histindex);

        //if( !Alocked && !histlocked)
        //if( !histlocked )
        emit histReady(vechistdata,histmax,histindex);
        QVector<uint> nullvec;
        vechistdata.swap(nullvec);
        //QVector<unsigned short> nullvec1;
        //localvecimg.swap(nullvec1);
    }
    //calchistlock.unlock();
    //calchist_locked=false;

}
//void threadB::finished()
//{

//}
