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
    QVector<unsigned short>vechistdata(65536,0);
    QVector<unsigned short>vecimg(imgbuf,imgbuf+sizeof(*imgbuf)/sizeof(imgbuf[0]));
    /*for(int i = 0 ;i < imgH ;i++)
    {
        for(int j=0;j< imgW;j++)
        {
           vecimg[j+i*imgH]=imgbuf[j+i*imgH];
        }
    }*/
    for(uint i=0;i != vecimg.size();++i)
    {
        int idx=vecimg.at(i);
        //qDebug()<<"i: "<<i<<" Value: "<<vecimg.at(i);
        ++vechistdata[idx];
        //()<<"idx="<<idx<<" Value="<<vechistdata.at(idx);
        if(vechistdata[idx]>histmax)
        {
            histmax=vechistdata[idx];
            histidx=idx;
        }
        //std::cout<<idx<<"@"<<vechistdata[idx]<<std::endl;
   }
    if(imgready)
        emit hist_ready();
    histlock.unlock();
}


