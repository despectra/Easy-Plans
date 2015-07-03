#include "render2dwidget.h"

#include <QPainter>
#include <QDebug>

Render2DWidget::Render2DWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    //setFixedSize(300, 300);
    backgroundBrush = QBrush(Qt::white);
    linePen = QPen(Qt::red);

    cameraCenter = QPoint(0, 0);
    emit cameraDragged(cameraCenter);

    rectangles = new QVector<QRect>();
    rectangles->append(QRect(10, 10, 20, 30));
    rectangles->append(QRect(50, 50, 40, 40));
}

Render2DWidget::~Render2DWidget()
{
    delete rectangles;
}

void Render2DWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(e->rect(), backgroundBrush);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(linePen);

    drawRectangles(painter);

    painter.end();
}

void Render2DWidget::mousePressEvent(QMouseEvent *e)
{
    qDebug() << "mousePressEvent";
    if(e->type() == QMouseEvent::MouseButtonPress) {
        isMousePressed = true;
        lastPressedPoint.setX(e->x() - cameraCenter.x());
        lastPressedPoint.setY(e->y() - cameraCenter.y());
        updateCursor();
        update();
    }
}

void Render2DWidget::mouseMoveEvent(QMouseEvent *e)
{
    qDebug() << "mouseMoveEvent";
    int xOffset = e->x() - lastPressedPoint.x();
    int yOffset = e->y() - lastPressedPoint.y();
    cameraCenter.setX(xOffset);
    cameraCenter.setY(yOffset);
    emit cameraDragged(cameraCenter);
    update();
}


void Render2DWidget::mouseReleaseEvent(QMouseEvent *e)
{
    qDebug() << "mouseReleaseEvent";
    if(e->type() == QMouseEvent::MouseButtonRelease) {
        isMousePressed = false;
        updateCursor();
        update();
    }
}

void Render2DWidget::updateCursor()
{
    setCursor(isMousePressed ? Qt::ClosedHandCursor : Qt::ArrowCursor);
}

void Render2DWidget::drawRectangles(QPainter &painter)
{
    for(int i = 0; i < rectangles->size(); i++) {
        QRect rectangle = rectangles->at(i);
        rectangle.moveLeft(rectangle.left() + cameraCenter.x());
        rectangle.moveTop(rectangle.top() + cameraCenter.y());
        painter.drawRect(rectangle);
    }
}
