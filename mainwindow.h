#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "render2dwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Render2DWidget* renderWidget;

private slots:
    void updateCameraPositionMessage(const QPoint &position);
};

#endif // MAINWINDOW_H
