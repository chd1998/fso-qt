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
        Alocked=true;

       //generate random data
        //syncAB=false;
        for(int i = 0 ;i < imgX*imgY ;i++)
        {

                //myImage[j+i*row]=(unsigned short)QRandomGenerator::global()->bounded(0,65535);
                int randnum=QRandomGenerator::global()->bounded(low,high);
                //myImage[i]=randnum;
                //vecimg.append(randnum);
                srcimg[i]=randnum;

        }
        //syncAB=true;
        if(!stoppedA || !pausedA)
        {
            emit fromA(src,countA);
            imgQueue.enqueue(srcimg);
        }
        /*if(!imglocked || !histlocked)
        {
            emit imgReady(srcimg);
        }*/
        //if(!imglocked || !histlocked)

        countA++;
        lockA.unlock();
        Alocked=false;
        //qDebug()<<countA<<":"<<imgX<<" "<<imgY<<" "<<vecimg.size()<<" "<<sizeof(* myImage)/sizeof(myImage[0]);
        QElapsedTimer t;
        t.start();
        //while(t.elapsed()<frameRate || Blocked)
        while(t.elapsed()<frameRate )
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
