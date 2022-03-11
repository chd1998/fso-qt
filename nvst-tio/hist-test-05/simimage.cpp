#include "simimage.h"
#include "mainwindow.h"
#include <QCoreApplication>
#include <QRandomGenerator>
#include <QThread>

simImage::simImage()
{

}

void simImage::generateImg()
{
   int count=0;
   while(!stopped)
   {
           if ( QThread::currentThread()->isInterruptionRequested() )
             return;
           tlock.lock();
           //unsigned short **a;
           int row=imgY;
           int col=imgX;
           QImage grayimage;
           grayimage = QImage(imgX,imgY,QImage::Format_Grayscale16);
           /*a=new unsigned short*[row];
           for(int i = 0 ;i < row ;i++)
           {
               a[i] = new unsigned short[col];
           }*/
          //generate random data
           for(int i = 0 ;i < row ;i++)
           {
               for(int j=0;j< col;j++)
               {
                //a[i][j] = QRandomGenerator::global()->bounded(0,65535);
                grayimage.setPixelColor(i,j,QRandomGenerator::global()->bounded(0,65535));
                //qDebug()<<grayimage.pixel(i,j);
                //qDebug()<<a[i][j];
               }
           }
           //qDebug()<<"1";
           QImage grayimage_copy;
           grayimage_copy=grayimage.copy(QRect());
           emit imgReady(grayimage_copy);
           QElapsedTimer t;
           t.start();
           while(t.elapsed()<refreshrate)
               QCoreApplication::processEvents();
           //qDebug()<<"2";
           /*for(int i = 0 ;i < row ;i++)
           {
               delete[] a[i];
           }
           delete[] a;*/
           //qDebug()<<"3";
           //qDebug()<<"4";
           tlock.unlock();

           qDebug()<<"Round: "<<count;
           count++;

   }
}

