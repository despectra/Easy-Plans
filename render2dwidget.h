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

#define PARENT_WIDGET_CLASS QOpenGLWidget

#ifdef GPU_DRIVERS_ARE_FUCKED==1
#define PARENT_WIDGET_CLASS QWidget
#endif

class Render2DWidget : public PARENT_WIDGET_CLASS
{
    Q_OBJECT
public:
    Render2DWidget(QWidget* parent);
    ~Render2DWidget();
signals:
    void cameraDragged(const QPoint& position);
protected:
    void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
private:
    void updateCursor();
    void centerToCamera(QPainter& painter);
    void drawRectangles(QPainter& painter);
    void drawBackground(QPainter& painter);
    void drawGrid(QPainter& painter);
    void drawVerticalGrid(QPainter& painter);
    void drawHorizontalGrid(QPainter& painter);
    void drawAxes(QPainter& painter);

    qint64 lastPaintAt;

    QBrush backgroundBrush;
    QPen gridPen;
    QPen linePen;

    bool isMousePressed;
    QPoint lastPressedPoint;
    QPoint cameraCenter;
    QRect window;
    int gridSize;
    QVector<QRect>* rectangles;
};

#endif // RENDERWIDGET_H
