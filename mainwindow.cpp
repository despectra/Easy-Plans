#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    renderWidget = new Render2DWidget(this);
    setCentralWidget(renderWidget);
    connect(renderWidget, SIGNAL(cameraDragged(const QPoint&)), this, SLOT(updateCameraPositionMessage(const QPoint&)));
    connect(this->ui->actionWallPaintMode, SIGNAL(toggled(bool)), this, SLOT(wallPaintModeToggled(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::WindowStateChange) {
        renderWidget->update();
    }
}

void MainWindow::updateCameraPositionMessage(const QPoint& position)
{

}

void MainWindow::wallPaintModeToggled(bool checked)
{
    if(checked) {
        renderWidget->setDrawingMode();
    } else {
        renderWidget->setDraggingMode();
    }
}
