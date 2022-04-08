#include "threadb.h"
#include "mainwindow.h"

threadB::threadB()
{
    countB=0;
    src="B";
    histfinished=false;
}

void threadB::working()
{
    //qDebug()<<"inside B";
    while(!QThread::currentThread()->isInterruptionRequested())
    //while(!stoppedB)
    {
        lockB.lock();
        emit fromB(src,countB);

        //calcHist();
        if(countB != 0)
            histfirst = false;
        countB++;
        histfinished=false;

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
void threadB::calcHist()
{
    QVector<unsigned short>vechistdata(65536,0);

    histmax=0;
    if(startedA)
    {
        //std::vector<unsigned short> vecimg(myImageBack,myImageBack+sizeof(*myImageBack)/sizeof(myImageBack[0]));
        //std::vector<int>::iterator it;
        //int len=sizeof(*myImageBack)/sizeof(myImageBack[0]);
        //qDebug()<<"Len: "<<vecimg.size();
        //for(int i=0;i < len; ++i)
        for(uint i=0;i != vecimg.size();++i)
        {
            int idx=vecimg.at(i);
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
    //qDebug()<<"idx= "<<histindex<<" max= "<<histmax<<" value= "<<vecimg.at(32768);
    //std::sort(std::execution::par_unseq, vechistdata.begin(), vechistdata.end());
    //qDebug()<<"idx="<<histindex<<" Value="<<vechistdata.at(histindex);
    histfinished=true;
    if(!histlocked)
        emit histReady(vechistdata,histmax,histindex);
}
//void threadB::finished()
//{

//}
