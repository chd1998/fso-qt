#ifndef SIMIMAGE_H
#define SIMIMAGE_H

#include <QObject>
#include <QDebug>
#include <QMutex>
#include <QImage>

class simImage : public QObject
{
    Q_OBJECT
public:
    simImage();
    void generateImg();
    QImage grayimage;


signals:
    void imgReady(QImage);
    void finished();
    //void simImageFinished();



};

#endif // SIMIMAGE_H
