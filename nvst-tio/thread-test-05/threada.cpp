#include "threada.h"
#include "mainwindow.h"
#include <complex>

threadA::threadA()
{
    countA=0;
    src="A";

}

void threadA::working()
{
    //qDebug()<<"1: "<<imgX<<" "<<imgY<<" "<<vecimg.size();
    //grayimage16 = new QImage(imgX,imgY,QImage::Format_Grayscale16);
    //grayimage = new QImage(imgX,imgY,QImage::Format_Indexed8);



    while(!QThread::currentThread()->isInterruptionRequested())
    {

        lockA.lock();
        vecimg.clear();
        //vecimg.resize(imgX*imgY,0);
       //generate random data
        syncAB=false;
        for(int i = 0 ;i < imgY*imgX ;i++)
        {

                //myImage[j+i*row]=(unsigned short)QRandomGenerator::global()->bounded(0,65535);
                int randnum=QRandomGenerator::global()->bounded(low,high);
                //myImage[i]=randnum;
                vecimg.append(randnum);

        }
        syncAB=true;
        if(!stoppedA || !pausedA)
            emit fromA(src,countA);
        if(!imglocked || !histlocked)
        {
            emit imgReady(vecimg);
        }
        QVector<unsigned short> nullvec;
        vecimg.swap(nullvec);
        countA++;
        lockA.unlock();
        //qDebug()<<countA<<":"<<imgX<<" "<<imgY<<" "<<vecimg.size()<<" "<<sizeof(* myImage)/sizeof(myImage[0]);
        QElapsedTimer t;
        t.start();
        while(t.elapsed()<frameRate)
            QCoreApplication::processEvents();
        //delete grayimage16;
        //delete grayimage;
        //delete[] myImageBack;

    }
    //delete grayimage16;
    //delete grayimage;
    //delete[] myImageBack;
}

//void threadA::finished()
//{

//}
