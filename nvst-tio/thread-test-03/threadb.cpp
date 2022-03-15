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

        //calcHist();
        if(countB != 0)
            histfirst = false;
        if(!histlocked)
            emit histReady();
        countB++;

        lockB.unlock();
        QElapsedTimer t;
        t.start();
        while(t.elapsed()<150)
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
    if(grayimage != nullptr && startedA)
    {
        histmax=0;
        for(int i = 0; i <grayimage->height(); i++){
                for(int j = 0; j < grayimage->width(); j++){
                    //int index=cv::saturate_cast<int>(grayimage.ptr<cv::Vec3b>(i)[j][0]);
                    //int index=grayimage.ptr<cv::Vec3b>(i)[j][0];
                    int index=grayimage->pixelIndex(i,j);
                    ++histdata[index];
                    if(histdata[index]>histmax)
                    {
                        histmax=histdata[index];
                        idx=index;
                    }
                    //oldhistdata[index]=histdata[index];
                }
            }
    }
}
//void threadB::finished()
//{

//}
