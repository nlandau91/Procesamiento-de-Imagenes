/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tab_result;
    QLabel *img_result;
    QWidget *tab_rojo;
    QLabel *img_rojo;
    QWidget *tab_naranja;
    QLabel *img_naranja;
    QWidget *tab_amarillo;
    QLabel *img_amarillo;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 801, 581));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
        tab_result = new QWidget();
        tab_result->setObjectName(QString::fromUtf8("tab_result"));
        img_result = new QLabel(tab_result);
        img_result->setObjectName(QString::fromUtf8("img_result"));
        img_result->setGeometry(QRect(0, 0, 800, 550));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(img_result->sizePolicy().hasHeightForWidth());
        img_result->setSizePolicy(sizePolicy1);
        tabWidget->addTab(tab_result, QString());
        tab_rojo = new QWidget();
        tab_rojo->setObjectName(QString::fromUtf8("tab_rojo"));
        img_rojo = new QLabel(tab_rojo);
        img_rojo->setObjectName(QString::fromUtf8("img_rojo"));
        img_rojo->setGeometry(QRect(0, 0, 800, 550));
        sizePolicy1.setHeightForWidth(img_rojo->sizePolicy().hasHeightForWidth());
        img_rojo->setSizePolicy(sizePolicy1);
        tabWidget->addTab(tab_rojo, QString());
        tab_naranja = new QWidget();
        tab_naranja->setObjectName(QString::fromUtf8("tab_naranja"));
        img_naranja = new QLabel(tab_naranja);
        img_naranja->setObjectName(QString::fromUtf8("img_naranja"));
        img_naranja->setGeometry(QRect(0, 0, 800, 550));
        sizePolicy1.setHeightForWidth(img_naranja->sizePolicy().hasHeightForWidth());
        img_naranja->setSizePolicy(sizePolicy1);
        tabWidget->addTab(tab_naranja, QString());
        tab_amarillo = new QWidget();
        tab_amarillo->setObjectName(QString::fromUtf8("tab_amarillo"));
        img_amarillo = new QLabel(tab_amarillo);
        img_amarillo->setObjectName(QString::fromUtf8("img_amarillo"));
        img_amarillo->setGeometry(QRect(0, 0, 800, 550));
        sizePolicy1.setHeightForWidth(img_amarillo->sizePolicy().hasHeightForWidth());
        img_amarillo->setSizePolicy(sizePolicy1);
        tabWidget->addTab(tab_amarillo, QString());
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        img_result->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_result), QApplication::translate("MainWindow", "Result", nullptr));
        img_rojo->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_rojo), QApplication::translate("MainWindow", "Rojo", nullptr));
        img_naranja->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_naranja), QApplication::translate("MainWindow", "Naranja", nullptr));
        img_amarillo->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_amarillo), QApplication::translate("MainWindow", "Amarillo", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
