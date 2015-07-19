#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QOpenGLWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QBrush>
#include <QPen>
#include <QTimer>

#include <QVector>
#include <QSet>

class Render2DWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    enum Mode {
        Dragging = 0,
        Selection = 1,
        Drawing = 2
    };
    enum MovementDirection {
        TopRight = 1,
        TopLeft = 2,
        BottomLeft = 3,
        BottomRight = 4
    };

    Render2DWidget(QWidget* parent);
    ~Render2DWidget();
public slots:
    void setDraggingMode();
    void setSelectionMode();
    void setDrawingMode();
signals:
    void cameraDragged(const QPoint& position);
protected:
    void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
private:
    QPoint mapCursorPositionToCanvas(QMouseEvent* event);
    MovementDirection getCursorMovementDirection(QPoint offset);
    bool isCameraDragging();
    void moveCamera(QPoint offset);
    void resizeDrawingShape(MovementDirection direction, QPoint offset);
    void updateCursor();
    void updateWindow();
    void centerToCamera(QPainter& painter);
    void drawRectangles(QPainter& painter);
    void drawCurrentDrawingShape(QPainter& painter);
    void drawBackground(QPainter& painter);
    void drawGrid(QPainter& painter);
    void drawVerticalGrid(QPainter& painter);
    void drawHorizontalGrid(QPainter& painter);
    void drawAxes(QPainter& painter);

    qint64 lastPaintAt;

    QBrush backgroundBrush;
    QPen gridPen;
    QPen linePen;
    QPen objectPen;

    bool isMousePressed;
    QPoint lastPressedPoint;
    QPoint cameraCenter;
    QPoint prevCameraCenter;
    QRect window;
    QRect prevWindow;
    int gridSize;
    QVector<QRect>* rectangles;
    Mode currentMode;
    QRect* currentDrawingShape;
    int wallSize;
};

#endif // RENDERWIDGET_H
