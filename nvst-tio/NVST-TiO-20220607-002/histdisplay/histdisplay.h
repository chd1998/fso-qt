#ifndef HISTDISPLAY_H
#define HISTDISPLAY_H

#include <QObject>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class histdisplay : public QObject
{
    Q_OBJECT
public:
    histdisplay();
    //QVector<unsigned short>localvecimg;
    QVector<unsigned short>localvechistdata;

signals:
    void draw_hist(QVector<uint>,uint,uint);
    void finished();
public slots:
    void buf2hist(QVector<unsigned short>);
    void deleteLater();
    void working();
public:

};

#endif // HISTDISPLAY_H
