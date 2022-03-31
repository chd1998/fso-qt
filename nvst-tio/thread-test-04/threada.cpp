#include "threada.h"
#include "mainwindow.h"

threadA::threadA()
{
    countA=0;
    src="A";


}

void threadA::working()
{
    QElapsedTimer t;
    while(!QThread::currentThread()->isInterruptionRequested())
    {
        QImage grayimage16 = QImage(imgX,imgY,QImage::Format_Grayscale16);
        grayimage = QImage(imgX,imgY,QImage::Format_Indexed8);
        myImage= new unsigned short[imgX*imgY];

        lockA.lock();
        int row=imgY;
        int col=imgX;
       //generate random data
        for(int i = 0 ;i < row ;i++)
        {
            for(int j=0;j< col;j++)
            {
                myImage[j+i*row]=QRandomGenerator::global()->bounded(0,65535);
            }
        }
        grayimage16 = QImage((unsigned char *)myImage,imgX,imgY,QImage::Format_Grayscale16);
        grayimage=grayimage16.convertToFormat(QImage::Format_Indexed8);
        emit fromA(src,countA);
        if(!imglocked || !histlocked)
            emit imgReady();
        countA++;
        lockA.unlock();
        //qDebug()<<"inside A-03";
        t.start();
        while(t.elapsed()<100)
            QCoreApplication::processEvents();

        //grayimage16=NULL;
        //grayimage=NULL;
        //myImage=NULL;
        //delete grayimage16;
        //delete grayimage;
        delete myImage;

    }


}

//void threadA::finished()
//{

//}
