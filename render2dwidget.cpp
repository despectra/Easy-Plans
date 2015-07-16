#include "render2dwidget.h"

#include <QPainter>
#include <QDebug>
#include <QDateTime>
#include <QGraphicsView>

Render2DWidget::Render2DWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    backgroundBrush = QBrush(Qt::white);
    linePen = QPen(Qt::red);
    gridPen = QPen(QColor::fromRgbF(0.6, 0.6, 0.8));
    gridPen.setStyle(Qt::DotLine);

    cameraCenter = QPoint(0, 0);
    emit cameraDragged(cameraCenter);

    rectangles = new QVector<QRect>();
    rectangles->append(QRect(10, 10, 5, 5));

    gridSize = 50;

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
    drawBackground(painter);
    drawGrid(painter);
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
    window.setLeft(-w / 2 - cameraCenter.x());
    window.setTop(-h / 2 - cameraCenter.y());
    window.setWidth(w);
    window.setHeight(h);
    painter.setWindow(window);
}

void Render2DWidget::drawRectangles(QPainter &painter)
{
    painter.setPen(linePen);
    for(int i = 0; i < rectangles->size(); i++) {
        QRect rectangle = rectangles->at(i);
        painter.drawRect(rectangle);
    }
}

void Render2DWidget::drawBackground(QPainter &painter)
{
    painter.fillRect(painter.window(), backgroundBrush);
}

void Render2DWidget::drawGrid(QPainter &painter)
{
    painter.setBrush(Qt::NoBrush);
    painter.setPen(gridPen);
    drawVerticalGrid(painter);
    drawHorizontalGrid(painter);
    drawAxes(painter);
}

void Render2DWidget::drawVerticalGrid(QPainter &painter)
{
    int offsetX = window.left() % gridSize;
    for(int i = window.left() - offsetX; i < window.right() - offsetX; i += gridSize) {
        painter.drawLine(i, window.top(), i, window.bottom());
    }
}

void Render2DWidget::drawHorizontalGrid(QPainter &painter)
{
    int offsetY = window.top() % gridSize;
    for(int i = window.top() - offsetY; i < window.bottom() - offsetY; i += gridSize) {
        painter.drawLine(window.left(), i, window.right(), i);
    }
}

void Render2DWidget::drawAxes(QPainter &painter)
{
    painter.setPen(linePen);
    if(window.left() < 0 && 0 < window.right()) {
        painter.drawLine(0, window.top(), 0, window.bottom());
    }
    if(window.top() < 0 && 0 < window.bottom()) {
        painter.drawLine(window.left(), 0, window.right(), 0);
    }
}
