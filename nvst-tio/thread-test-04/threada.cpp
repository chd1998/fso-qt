#include "threada.h"
#include "mainwindow.h"

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
             //a[i][j] = QRandomGenerator::global()->bounded(0,65535);
             //grayimage16->setPixelColor(i,j,QRandomGenerator::global()->bounded(0,65535));
             myImage[j+i*row]=QRandomGenerator::global()->bounded(0,65535);
             //qDebug()<<grayimage.pixel(i,j);
             //qDebug()<<a[i][j];
            }
        }
        *grayimage16 = QImage((unsigned char *)myImage,imgX,imgY,QImage::Format_Grayscale16);
        *grayimage=grayimage16->convertToFormat(QImage::Format_Indexed8);
        emit fromA(src,countA);
        if(!imglocked)
            emit imgReady();
        countA++;
        lockA.unlock();
        //qDebug()<<"inside A-03";
        QElapsedTimer t;
        t.start();
        while(t.elapsed()<100)
            QCoreApplication::processEvents();

    }
    delete grayimage16;
    delete grayimage;
    delete myImage;
}

//void threadA::finished()
//{

//}
