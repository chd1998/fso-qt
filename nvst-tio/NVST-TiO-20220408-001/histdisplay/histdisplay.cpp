#include "histdisplay.h"
#include "mainwindow.h"

using namespace std;

histdisplay::histdisplay()
{


}


void histdisplay::deleteLater()
{

}


void histdisplay::buf2img(unsigned short* imgbuf)
{
    histlock.lock();
    int localhistmax=0;
    int localhistidx=0;
    QVector<unsigned short>localvechistdata(65536,0);
    QVector<unsigned short>localvecimg(imgbuf,imgbuf+sizeof(*imgbuf)/sizeof(imgbuf[0]));
    for(uint i=0;i != localvecimg.size();++i)
    {
        int idx=localvecimg.at(i);
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
    if(imgready)
        emit hist_ready(localvechistdata,localhistmax,localhistidx);
    histlock.unlock();
}


