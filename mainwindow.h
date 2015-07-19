#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEvent>

#include "render2dwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    QString CAMERA_POSITION_STRING = QString("Camera position: X = %1, Y = %2");
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent* event);

private:
    Ui::MainWindow *ui;
    Render2DWidget* renderWidget;

private slots:
    void updateCameraPositionMessage(const QPoint &position);
    void wallPaintModeToggled(bool checked);
};

#endif // MAINWINDOW_H
