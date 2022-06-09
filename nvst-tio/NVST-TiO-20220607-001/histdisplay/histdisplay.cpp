#include "histdisplay.h"
#include "mainwindow.h"

using namespace std;

histdisplay::histdisplay()
{
    //QVector<unsigned short>localvecimg(imgH*imgW,0);
    //QVector<unsigned short>localvechistdata(65536,0);
}


void histdisplay::deleteLater()
{

}

void histdisplay::working()
{
    while(!QThread::currentThread()->isInterruptionRequested())
    {
        //qDebug()<<"Working in histdisplay...";
        QElapsedTimer t1;
        t1.start();
        while(t1.elapsed()<1000)
        {
            QCoreApplication::processEvents();
        }
    }

}


void histdisplay::buf2hist(QVector<unsigned short>localvecimg)
{
    histlock.lock();
    histcalc_locked=true;
    if(!histshow_locked)
    {       
        uint localhistmax=0;
        uint localhistidx=0;
        QVector<unsigned short>localvechistdata(65536,0);
        //QVector<unsigned short>localvecimg(imgbuf,imgbuf+imglen);
        //QVector<unsigned short>localvecimg(imglen,0);
        //std::copy(imgbuf, imgbuf+imglen, std::back_inserter(localvecimg));
        //uint imglen=_msize(imgbuf)/(sizeof(*imgbuf));
        for(uint i=0;i<localvecimg.size();++i)
        {
            //int idx=imgbuf[i];
            int idx=localvecimg[i];
            //qDebug()<<"i: "<<i<<" Value: "<<vecimg.at(i);
            ++localvechistdata[idx];
            //()<<"idx="<<idx<<" Value="<<vechistdata.at(idx);
            if(localvechistdata[idx]>localhistmax)
            {
                localhistmax=localvechistdata[idx];
                localhistidx=idx;
            }
            //std::cout<<idx<<"@"<<vechistdata[idx]<<std::endl;
        }
        //if(!histshow_locked)
        emit draw_hist(localvechistdata,localhistmax,localhistidx);

        QVector<unsigned short> nullvec;
        localvechistdata.swap(nullvec);
        //localvecimg.swap(nullvec);
        //QElapsedTimer t1;
        //t1.start();
        //while(t1.elapsed()<50)
        //{
        //    QCoreApplication::processEvents();
        //}
    }
    histlock.unlock();
    histcalc_locked=false;
}


