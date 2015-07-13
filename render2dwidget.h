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

    qint64 lastPaintAt;

    QBrush backgroundBrush;
    QPen linePen;
    bool isMousePressed;
    QPoint lastPressedPoint;
    QPoint cameraCenter;
    QVector<QRect>* rectangles;
};

#endif // RENDERWIDGET_H
