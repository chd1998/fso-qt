#include "histdisplay.h"
#include "mainwindow.h"

histdisplay::histdisplay()
{


}


void histdisplay::deleteLater()
{

}


void histdisplay::buf2img(unsigned short* imgbuf)
{
    cv::Mat tmpimage = cv::Mat(static_cast<int>(imgH), static_cast<int>(imgW), CV_16UC1, imgbuf);
    cv::Mat lossyImage;
    if(tmpimage.data)
    {
        cv::normalize(tmpimage, lossyImage, 0, 255, cv::NORM_MINMAX, CV_8U);
    }

    histimg = new QImage(static_cast<unsigned char *>(lossyImage.data), lossyImage.rows, lossyImage.cols, static_cast<int>(lossyImage.step), QImage::Format_Indexed8);
    //int yRange = 0;
    int data[256]{0};
    int max=0,idx=0;

    qDebug()<<histimg->height()<<","<<histimg->width();
    for(int i = 0; i <histimg->height(); i++){
            for(int j = 0; j < histimg->width();j++){
                //int index=cv::saturate_cast<int>(grayimage.ptr<cv::Vec3b>(i)[j][0]);
                //int index=grayimage.ptr<cv::Vec3b>(i)[j][0];
                int index=histimg->pixelIndex(i,j);
                //int index=(int)grayimage.at<uchar>(i,j);
                ++data[index];

                if(data[index]>max)
                {
                    max=data[index];
                    idx=index;
                }
            }
        }
    if(imgready)
        emit hist_ready(data,max,idx);
}


