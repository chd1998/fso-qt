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
signals:
    void hist_ready();
    void finished();
public slots:
    void buf2img(unsigned short*);
    void deleteLater();
public:

};

#endif // HISTDISPLAY_H
