#include "graphicsscene.h"
#include "mainwindow.h"

GraphicsScene::GraphicsScene(QWidget * parent): QGraphicsScene(parent) {}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if(event->buttons()&Qt::LeftButton){
        QPointF p = event->scenePos();
        double x = p.x();
        double y = p.y();
        ex=x;
        ey=y;
        drawing=true;
        emit this->positionMoved(x, y);
    }else{
        QPointF p = event->scenePos();
        double x = p.x();
        double y = p.y();
        emit this->positionMoved(x, y);
    }
    //std::cout << x << ":" << y << std::endl;
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if(event->button()==Qt::LeftButton)//left button pressed
    {        
        QPointF p = event->scenePos();
        double x = p.x();
        double y = p.y();
        sx=x;
        sy=y;
        ex=x;
        ex=y;
        crossX=static_cast<int>(x);
        crossY=static_cast<int>(y);
        drawing=false;

        emit this->RectPoint1(x, y);
    }

}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    QPointF p = event->scenePos();
    double x = p.x();
    double y = p.y();
    if(event->button()==Qt::LeftButton)  //left button released
    {
        /*QPointF p = event->scenePos();
        double x = p.x();
        double y = p.y();*/

        drawing=true;

        emit this->RectPoint2(x, y);
    }
    if(event->button()==Qt::RightButton) //right button released
        drawing=false;
}
