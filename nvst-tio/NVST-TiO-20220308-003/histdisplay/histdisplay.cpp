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
    if(imgready)
        emit hist_ready();
}


