#include "threada.h"
#include "mainwindow.h"
#include <complex>

threadA::threadA()
{
    countA=0;
    countA1=0;
    src="A";
    //unsigned short *srcimg;
    //srcimg=new unsigned short[imgX*imgY]();
    //std::default_random_engine gen;
    //double mu{32768}, sigma{9000};
    //std::normal_distribution<> d{mu, sigma};
    //if(srcimg != NULL)
        //delete srcimg;


}

threadA::~threadA()
{
    //delete[] srcimg;
}

void threadA::working()
{

    qDebug()<<"A-started";
    unsigned short *srcimg;
    srcimg=new unsigned short[imgX*imgY]();
    while(startedA)
    {

        QElapsedTimer t;
        t.start();

        lockA.lock();
        if(pausedA)
        {
            qDebug()<<"A-paused";
            pauseCondA.wait(&lockA);
        }
        lockA.unlock();

        for(uint i = 0 ;i < imgX*imgY ;i++)
        {
                //unsigned short randnum=QRandomGenerator::global()->bounded(low,high);
                srcimg[i]=(unsigned short)QRandomGenerator::global()->bounded(low,high);
        }
        if(imgQueue.enqueue(srcimg))
        {
            countA++;
            //emit imgReady();
            emit fromA(src,countA,"Image enqueued...");
            qDebug()<<"Enqueue count: "<<countA<<" imgQueue size: "<<imgQueue.size_approx();
            //countA1++;
        }else
        {
            emit fromA(src,countA1,"Image enqueue failed, pls wait...");
            //qDebug()<<countA1<<": Enqueue faild ... ";
            countA1++;
        }

        if(imgQueue.size_approx() > MAXQUEUE){
            //imgQueueFull=true;
            imgQueue=moodycamel::ConcurrentQueue<unsigned short*>();
        }

        while(t.elapsed()<frameRate )
        {
            QCoreApplication::processEvents();
        }
        //if(!startedA)
            //break;
        //delete[] srcimg;
    }

    delete[] srcimg;
    qDebug()<<"ThreadA finished";
    //emit finished();
}

//void threadA::finished()
//{
//    stoppedA=true;
//}
