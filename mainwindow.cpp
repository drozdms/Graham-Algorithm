#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setMouseTracking(true);
    setLayout(new QHBoxLayout(this));
    setupUi(this);
    connect(generatePoints, SIGNAL(clicked()), convexhullwidget, SLOT(generatePoints()));
    connect(buildConvex, SIGNAL(clicked()), convexhullwidget, SLOT(buildConvex()));
    connect(debugSpeedSlider, SIGNAL(valueChanged(int)), convexhullwidget, SLOT(changeDebugSpeed(int)));
    debugSpeedSlider->valueChanged(100);

}

MainWindow::~MainWindow()
{
    //
}

