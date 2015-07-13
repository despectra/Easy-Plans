#include "render2dwidget.h"

#include <QPainter>
#include <QDebug>
#include <QDateTime>

Render2DWidget::Render2DWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    backgroundBrush = QBrush(Qt::white);
    linePen = QPen(Qt::red);

    cameraCenter = QPoint(0, 0);
    emit cameraDragged(cameraCenter);

    rectangles = new QVector<QRect>();
    rectangles->append(QRect(10, 10, 5, 5));

    lastPaintAt = 0;
}

Render2DWidget::~Render2DWidget()
{
    delete rectangles;
}

void Render2DWidget::paintEvent(QPaintEvent *e)
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    qint64 timeBetweenPaints = now - lastPaintAt;
    lastPaintAt = now;
    qDebug() << timeBetweenPaints;

    QPainter painter;
    painter.begin(this);
    centerToCamera(painter);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(painter.window(), backgroundBrush);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(linePen);
    drawRectangles(painter);
    painter.end();
}

void Render2DWidget::mousePressEvent(QMouseEvent *e)
{
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
    int xOffset = e->x() - lastPressedPoint.x();
    int yOffset = e->y() - lastPressedPoint.y();
    cameraCenter.setX(xOffset);
    cameraCenter.setY(yOffset);
    emit cameraDragged(cameraCenter);
    update();
}

void Render2DWidget::mouseReleaseEvent(QMouseEvent *e)
{
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

void Render2DWidget::centerToCamera(QPainter &painter)
{
    int w = width();
    int h = height();
    painter.setWindow(-w / 2 - cameraCenter.x(), -h / 2 - cameraCenter.y(), w, h);
}

void Render2DWidget::drawRectangles(QPainter &painter)
{
    for(int i = 0; i < rectangles->size(); i++) {
        QRect rectangle = rectangles->at(i);
        painter.drawRect(rectangle);
    }
}
