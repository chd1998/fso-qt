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
    grayimage16 = new QImage(imgX,imgY,QImage::Format_Grayscale16);
    grayimage = new QImage(imgX,imgY,QImage::Format_Indexed8);
    myImage= new unsigned short[imgX*imgY];
    myImageBack=myImage;

    while(!QThread::currentThread()->isInterruptionRequested())
    {

        lockA.lock();
        int row=imgY;
        int col=imgX;
       //generate random data
        for(int i = 0 ;i < row ;i++)
        {
            for(int j=0;j< col;j++)
            {
                //myImage[j+i*row]=(unsigned short)QRandomGenerator::global()->bounded(0,65535);
                int randnum=QRandomGenerator::global()->bounded(5000,42000);
                myImage[j+i*row]=randnum;
                vecimg[j+i*row]=myImage[j+i*row];
            }
        }
        emit fromA(src,countA);
        if(!imglocked || !histlocked)
        {
            //std::vector<unsigned short> vecimg(myImage,myImage+sizeof(*myImage)/sizeof(myImage[0]));
            //std::vector<unsigned short> vecimg(sizeof(*myImage)/sizeof(myImage[0]));
            //memcpy(&vecimg[0], myImage, sizeof(*myImage));
            //qDebug()<<"img size: "<<vecimg.size()<<" Value: "<<vecimg.at(32768);
            emit imgReady();
        }
        countA++;
        lockA.unlock();
        //qDebug()<<"inside A-03";
        QElapsedTimer t;
        t.start();
        while(t.elapsed()<frameRate)
            QCoreApplication::processEvents();

    }
    delete grayimage16;
    delete grayimage;
    delete[] myImageBack;
}

//void threadA::finished()
//{

//}
