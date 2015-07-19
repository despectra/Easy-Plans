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
    objectPen = QPen(Qt::black);

    cameraCenter = QPoint(0, 0);
    prevCameraCenter = cameraCenter;
    emit cameraDragged(cameraCenter);
    setDraggingMode();
    prevWindow = QRect(0, 0, 0, 0);
    updateWindow();
    isMousePressed = false;

    rectangles = new QVector<QRect>();
    gridSize = 50;
    wallSize = 10;
    snapToGrid = false;
    currentDrawingShape = NULL;
}

Render2DWidget::~Render2DWidget()
{
    delete rectangles;
    if(currentDrawingShape != NULL) {
        delete currentDrawingShape;
    }
}

void Render2DWidget::setDraggingMode()
{
    currentMode = Dragging;
}

void Render2DWidget::setSelectionMode()
{
    currentMode = Selection;
}

void Render2DWidget::setDrawingMode()
{
    currentMode = Drawing;
}

void Render2DWidget::setSnapToGrid(bool enabled)
{
    snapToGrid = enabled;
}

void Render2DWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter;
    painter.begin(this);
    centerToCamera(painter);
    drawBackground(painter);
    drawGrid(painter);
    drawRectangles(painter);
    drawCurrentDrawingShape(painter);
    painter.end();
}

void Render2DWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->type() == QMouseEvent::MouseButtonPress) {
        QPoint cursor = mapCursorPositionToCanvas(e);
        lastPressedPoint = cursor;
        isMousePressed = true;
        if(currentMode == Drawing) {
            if(currentDrawingShape == NULL) {
                currentDrawingShape = new QRect();
            }
            currentDrawingShape->setTopLeft(lastPressedPoint);
            currentDrawingShape->setWidth(0);
            currentDrawingShape->setHeight(0);
        }
        updateCursor();
        update();
    }
}

void Render2DWidget::mouseMoveEvent(QMouseEvent *e)
{
    QPoint cursor = mapCursorPositionToCanvas(e);
    int xOffset = cursor.x() - lastPressedPoint.x();
    int yOffset = cursor.y() - lastPressedPoint.y();
    QPoint offset(xOffset, yOffset);
    switch(currentMode) {
    case Dragging:
        moveCamera(offset);
        break;
    case Drawing:
        MovementDirection direction = getCursorMovementDirection(offset);
        resizeDrawingShape(direction, cursor);
        break;
    }
    update();
}

void Render2DWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->type() == QMouseEvent::MouseButtonRelease) {
        isMousePressed = false;
        prevWindow = window;
        prevCameraCenter = cameraCenter;
        updateCursor();
        update();
    }
}

QPoint Render2DWidget::mapCursorPositionToCanvas(QMouseEvent *event)
{
    int xNew = event->x() + prevWindow.left();
    int yNew = event->y() + prevWindow.top();
    QPoint result(xNew, yNew);
    return result;
}

Render2DWidget::MovementDirection Render2DWidget::getCursorMovementDirection(QPoint offset)
{
    bool signX = offset.x() > 0;
    bool signY = offset.y() > 0;
    if(signX && signY) {
        return BottomRight;
    }
    if(!signX && signY) {
        return BottomLeft;
    }
    if(!signX && !signY) {
        return TopLeft;
    }
    if(signX && !signY) {
        return TopRight;
    }
}

bool Render2DWidget::isCameraDragging()
{
    return currentMode == Dragging && isMousePressed;
}

void Render2DWidget::moveCamera(QPoint offset)
{
    cameraCenter = prevCameraCenter + offset;
}

void Render2DWidget::resizeDrawingShape(Render2DWidget::MovementDirection direction, QPoint offset)
{
    QPoint from, to;
    if(snapToGrid) {
        from = (lastPressedPoint / gridSize) * gridSize;
        to = (offset / gridSize) * gridSize;
    } else {
        from = lastPressedPoint;
        to = offset;
    }
    switch(direction) {
    case TopRight:
        currentDrawingShape->setBottomLeft(from);
        currentDrawingShape->setTopRight(to);
        break;
    case TopLeft:
        currentDrawingShape->setBottomRight(from);
        currentDrawingShape->setTopLeft(to);
        break;
    case BottomLeft:
        currentDrawingShape->setTopRight(from);
        currentDrawingShape->setBottomLeft(to);
        break;
    case BottomRight:
        currentDrawingShape->setTopLeft(from);
        currentDrawingShape->setBottomRight(to);
        break;
    }
}

void Render2DWidget::updateCursor()
{
    QCursor newCursor;
    if(isMousePressed) {
        switch(currentMode) {
        case Dragging:
            newCursor = Qt::ClosedHandCursor;
            break;
        case Selection:
            newCursor = Qt::ArrowCursor;
            break;
        case Drawing:
            newCursor = Qt::CrossCursor;
            break;
        }
    } else {
        newCursor = Qt::ArrowCursor;
    }
    setCursor(newCursor);
}

void Render2DWidget::updateWindow()
{
    int w = width();
    int h = height();
    window.setLeft(-w / 2 - cameraCenter.x());
    window.setTop(-h / 2 - cameraCenter.y());
    window.setWidth(w);
    window.setHeight(h);
}

void Render2DWidget::centerToCamera(QPainter &painter)
{
    updateWindow();
    if(prevWindow.size().isNull() || !isCameraDragging()) {
        prevWindow = window;
    }
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

void Render2DWidget::drawCurrentDrawingShape(QPainter &painter)
{
    if(currentDrawingShape == NULL || currentDrawingShape->size().isEmpty()) {
        return;
    }
    int halfWallSize = wallSize / 2;
    painter.setPen(objectPen);
    QMargins wallMargin(halfWallSize, halfWallSize, halfWallSize, halfWallSize);
    QRect outerRect = currentDrawingShape->marginsAdded(wallMargin);
    painter.drawRect(outerRect);
    if(currentDrawingShape->width() > wallSize && currentDrawingShape->height() > wallSize) {
        QRect innerRect = currentDrawingShape->marginsRemoved(wallMargin);
        painter.drawRect(innerRect);
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
