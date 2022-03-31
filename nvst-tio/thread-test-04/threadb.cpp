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
    QElapsedTimer t;
    while(!QThread::currentThread()->isInterruptionRequested())
    //while(!stoppedB)
    {
        lockB.lock();
        //if(QThread::currentThread()->isInterruptionRequested())
            //break;
        emit fromB(src,countB);

        //calcHist();
        if(countB != 0)
            histfirst = false;

        countB++;

        lockB.unlock();
        t.start();
        while(t.elapsed()<100)
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
    //int yRange = 0;
    //int tmpdata[256]{0};
    //int max=0,idx=0;
    //histdata[256]{0};
    for(int i=0;i<256;i++)
        histdata[i]=0;
    //*histdata=*new int[256]{0};
    //int *histdataback=histdata;
    if(startedA)
    {
        histmax=0;
        for(int i = 0; i <grayimage.height(); i++){
                for(int j = 0; j < grayimage.width(); j++){
                    int index=grayimage.pixelIndex(i,j);
                    ++histdata[index];
                    if(histdata[index]>histmax)
                    {
                        histmax=histdata[index];
                        idx=index;
                    }
                }
            }
        if(!histlocked)
            emit histReady();
    }
    //delete histdataback;
}
//void threadB::finished()
//{

//}
