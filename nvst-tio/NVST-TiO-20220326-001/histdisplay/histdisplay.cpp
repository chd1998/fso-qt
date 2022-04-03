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
    QImage *grayimage;
    cv::Mat currentImage = cv::Mat(static_cast<int>(imgH), static_cast<int>(imgW), CV_16UC1, imgbuf);
    cv::Mat lossyImage;
    cv::normalize(currentImage, lossyImage, 0, 255, cv::NORM_MINMAX, CV_8U);
    grayimage= new QImage(static_cast<unsigned char *>(lossyImage.data), lossyImage.cols, lossyImage.rows, static_cast<int>(lossyImage.step), QImage::Format_Indexed8);

    //int arrlen=sizeof(*imgbuf)/sizeof(imgbuf);
    //int arrlen=std::end(*imgbuf)-std::begin(*imgbuf);
    histmax=0;
    for(int i = 0; i <grayimage->height(); i++){
            for(int j = 0; j < grayimage->width(); j++){
                int index=grayimage->pixelIndex(i,j);
                ++histdata[index];
                if(histdata[index]>histmax)
                {
                    histmax=histdata[index];
                    histidx=index;
                }
            }
      }
    currentImage.release();
    lossyImage.release();
    delete grayimage;
    if(imgready)
        emit hist_ready();
    histlock.unlock();
}


