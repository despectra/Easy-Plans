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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateCameraPositionMessage(const QPoint& position)
{
    this->statusBar()->showMessage(QString("Camera position: X = %1, Y = %2")
                                        .arg(QString::number(position.x()), QString::number(position.y())));
}
