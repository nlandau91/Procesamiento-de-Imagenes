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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QPushButton *btn_load;
    QRadioButton *radioBtn_result;
    QRadioButton *radioBtn_rojo;
    QRadioButton *radioBtn_naranja;
    QRadioButton *radioBtn_amarillo;
    QLabel *lbl_img;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_5;
    QSlider *verticalSlider;
    QLabel *label_4;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QSlider *verticalSlider_2;
    QLabel *label_3;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(597, 311);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        btn_load = new QPushButton(centralwidget);
        btn_load->setObjectName(QString::fromUtf8("btn_load"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btn_load->sizePolicy().hasHeightForWidth());
        btn_load->setSizePolicy(sizePolicy);
        btn_load->setMaximumSize(QSize(131, 28));

        verticalLayout->addWidget(btn_load);

        radioBtn_result = new QRadioButton(centralwidget);
        radioBtn_result->setObjectName(QString::fromUtf8("radioBtn_result"));
        sizePolicy.setHeightForWidth(radioBtn_result->sizePolicy().hasHeightForWidth());
        radioBtn_result->setSizePolicy(sizePolicy);
        radioBtn_result->setChecked(true);

        verticalLayout->addWidget(radioBtn_result);

        radioBtn_rojo = new QRadioButton(centralwidget);
        radioBtn_rojo->setObjectName(QString::fromUtf8("radioBtn_rojo"));
        sizePolicy.setHeightForWidth(radioBtn_rojo->sizePolicy().hasHeightForWidth());
        radioBtn_rojo->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(radioBtn_rojo);

        radioBtn_naranja = new QRadioButton(centralwidget);
        radioBtn_naranja->setObjectName(QString::fromUtf8("radioBtn_naranja"));
        sizePolicy.setHeightForWidth(radioBtn_naranja->sizePolicy().hasHeightForWidth());
        radioBtn_naranja->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(radioBtn_naranja);

        radioBtn_amarillo = new QRadioButton(centralwidget);
        radioBtn_amarillo->setObjectName(QString::fromUtf8("radioBtn_amarillo"));
        sizePolicy.setHeightForWidth(radioBtn_amarillo->sizePolicy().hasHeightForWidth());
        radioBtn_amarillo->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(radioBtn_amarillo);


        horizontalLayout->addLayout(verticalLayout);

        lbl_img = new QLabel(centralwidget);
        lbl_img->setObjectName(QString::fromUtf8("lbl_img"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lbl_img->sizePolicy().hasHeightForWidth());
        lbl_img->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(lbl_img);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        verticalLayout_3->addWidget(label_5);

        verticalSlider = new QSlider(centralwidget);
        verticalSlider->setObjectName(QString::fromUtf8("verticalSlider"));
        verticalSlider->setOrientation(Qt::Vertical);

        verticalLayout_3->addWidget(verticalSlider);

        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout_3->addWidget(label_4);


        horizontalLayout->addLayout(verticalLayout_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_2->addWidget(label_2);

        verticalSlider_2 = new QSlider(centralwidget);
        verticalSlider_2->setObjectName(QString::fromUtf8("verticalSlider_2"));
        verticalSlider_2->setOrientation(Qt::Vertical);

        verticalLayout_2->addWidget(verticalSlider_2);

        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_2->addWidget(label_3);


        horizontalLayout->addLayout(verticalLayout_2);


        gridLayout_2->addLayout(horizontalLayout, 0, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 597, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        btn_load->setText(QApplication::translate("MainWindow", "Load", nullptr));
        radioBtn_result->setText(QApplication::translate("MainWindow", "Result", nullptr));
        radioBtn_rojo->setText(QApplication::translate("MainWindow", "Rojo", nullptr));
        radioBtn_naranja->setText(QApplication::translate("MainWindow", "Naranja", nullptr));
        radioBtn_amarillo->setText(QApplication::translate("MainWindow", "Amarillo", nullptr));
        lbl_img->setText(QApplication::translate("MainWindow", "lbl_img", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "kSize", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "pixels", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
