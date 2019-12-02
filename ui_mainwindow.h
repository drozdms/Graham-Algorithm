/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>
#include "convexhullwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *generatePoints;
    QPushButton *buildConvex;
    ConvexHullWidget *convexhullwidget;
    QSlider *debugSpeedSlider;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        generatePoints = new QPushButton(centralwidget);
        generatePoints->setObjectName(QString::fromUtf8("generatePoints"));
        generatePoints->setGeometry(QRect(0, 0, 141, 21));
        buildConvex = new QPushButton(centralwidget);
        buildConvex->setObjectName(QString::fromUtf8("buildConvex"));
        buildConvex->setGeometry(QRect(150, 0, 101, 21));
        convexhullwidget = new ConvexHullWidget(centralwidget);
        convexhullwidget->setObjectName(QString::fromUtf8("convexhullwidget"));
        convexhullwidget->setGeometry(QRect(0, 20, 801, 581));
        debugSpeedSlider = new QSlider(centralwidget);
        debugSpeedSlider->setObjectName(QString::fromUtf8("debugSpeedSlider"));
        debugSpeedSlider->setGeometry(QRect(260, 0, 101, 21));
        debugSpeedSlider->setMaximum(1000);
        debugSpeedSlider->setSingleStep(1);
        debugSpeedSlider->setValue(100);
        debugSpeedSlider->setOrientation(Qt::Horizontal);
        debugSpeedSlider->setInvertedAppearance(false);
        debugSpeedSlider->setInvertedControls(false);
        debugSpeedSlider->setTickPosition(QSlider::NoTicks);
        debugSpeedSlider->setTickInterval(100);
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        generatePoints->setText(QCoreApplication::translate("MainWindow", "Generate Random Points", nullptr));
        buildConvex->setText(QCoreApplication::translate("MainWindow", "BuildConvexHull", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
