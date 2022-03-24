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
    histlock.lock();
    cv::Mat tmpimg = cv::Mat(static_cast<int>(imgH), static_cast<int>(imgW), CV_8U, imgbuf);
    //cv::Mat lossyImage;
    //if(tmpimage.data)
    //{
        //cv::normalize(tmpimage, lossyImage, 0, 255, cv::NORM_MINMAX, CV_8U);
    //}

    histimg = new QImage(static_cast<unsigned char *>(tmpimg.data), tmpimg.rows, tmpimg.cols, static_cast<int>(tmpimg.step), QImage::Format_Indexed8);
    //int yRange = 0;
    int data[256]{0};
    int max=0,idx=0;

    qDebug()<<histimg->height()<<","<<histimg->width();
    /*for(int i = 0; i <lossyImage.rows; i++){
            for(int j = 0; j < lossyImage.cols; j++){
                //int index=cv::saturate_cast<int>(grayimage.ptr<cv::Vec3b>(i)[j][0]);
                int index=lossyImage.at<uchar>(i,j);
                ++data[index];
                if(data[index]>max)
                {
                    max=data[index];
                    idx=index;
                }
        }
    }*/
    for(int i = 0; i <tmpimg.rows; i++){
        uchar* rdata = tmpimg.ptr<uchar>(i);
        for(int j = 0; j < tmpimg.cols; j++){
            //int index=cv::saturate_cast<int>(grayimage.ptr<cv::Vec3b>(i)[j][0]);
            int index=*rdata++;
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
    tmpimg.release();
    histlock.unlock();
}


