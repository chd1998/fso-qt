#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>

class GraphicsScene : public QGraphicsScene {
    Q_OBJECT
public:
    GraphicsScene(QWidget *parent=nullptr);

signals:
    void positionMoved(double x, double y);
    void RectPoint1(double x1,double y1);
    void RectPoint2(double x2,double y2);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // GRAPHICSSCENE_H
