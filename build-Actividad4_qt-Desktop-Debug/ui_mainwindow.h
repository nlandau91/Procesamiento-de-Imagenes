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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
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
    QRadioButton *radioBtn_original;
    QRadioButton *radioBtn_rojo;
    QRadioButton *radioBtn_naranja;
    QRadioButton *radioBtn_amarillo;
    QLabel *lbl_img;
    QVBoxLayout *verticalLayout_3;
    QLabel *lbl_elemEstr;
    QComboBox *comboBox;
    QLabel *label_5;
    QGridLayout *gridLayout_3;
    QSlider *slider_kSize2;
    QSlider *slider_kSize1;
    QLabel *lbl_kSize1;
    QLabel *lbl_kSize2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QSlider *slider_pixels;
    QLabel *lbl_pixels;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLabel *lbl_cantRojo;
    QLabel *lbl_cantNaranja;
    QLabel *lbl_cantAmarillo;

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
        btn_load = new QPushButton(centralwidget);
        btn_load->setObjectName(QString::fromUtf8("btn_load"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btn_load->sizePolicy().hasHeightForWidth());
        btn_load->setSizePolicy(sizePolicy);
        btn_load->setMaximumSize(QSize(131, 28));

        verticalLayout->addWidget(btn_load);

        radioBtn_original = new QRadioButton(centralwidget);
        radioBtn_original->setObjectName(QString::fromUtf8("radioBtn_original"));
        radioBtn_original->setEnabled(false);
        radioBtn_original->setChecked(true);

        verticalLayout->addWidget(radioBtn_original);

        radioBtn_rojo = new QRadioButton(centralwidget);
        radioBtn_rojo->setObjectName(QString::fromUtf8("radioBtn_rojo"));
        radioBtn_rojo->setEnabled(false);
        sizePolicy.setHeightForWidth(radioBtn_rojo->sizePolicy().hasHeightForWidth());
        radioBtn_rojo->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(radioBtn_rojo);

        radioBtn_naranja = new QRadioButton(centralwidget);
        radioBtn_naranja->setObjectName(QString::fromUtf8("radioBtn_naranja"));
        radioBtn_naranja->setEnabled(false);
        sizePolicy.setHeightForWidth(radioBtn_naranja->sizePolicy().hasHeightForWidth());
        radioBtn_naranja->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(radioBtn_naranja);

        radioBtn_amarillo = new QRadioButton(centralwidget);
        radioBtn_amarillo->setObjectName(QString::fromUtf8("radioBtn_amarillo"));
        radioBtn_amarillo->setEnabled(false);
        sizePolicy.setHeightForWidth(radioBtn_amarillo->sizePolicy().hasHeightForWidth());
        radioBtn_amarillo->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(radioBtn_amarillo);


        horizontalLayout->addLayout(verticalLayout);

        lbl_img = new QLabel(centralwidget);
        lbl_img->setObjectName(QString::fromUtf8("lbl_img"));
        lbl_img->setScaledContents(true);

        horizontalLayout->addWidget(lbl_img);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        lbl_elemEstr = new QLabel(centralwidget);
        lbl_elemEstr->setObjectName(QString::fromUtf8("lbl_elemEstr"));

        verticalLayout_3->addWidget(lbl_elemEstr);

        comboBox = new QComboBox(centralwidget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setEnabled(false);

        verticalLayout_3->addWidget(comboBox);

        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        verticalLayout_3->addWidget(label_5);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        slider_kSize2 = new QSlider(centralwidget);
        slider_kSize2->setObjectName(QString::fromUtf8("slider_kSize2"));
        slider_kSize2->setEnabled(false);
        slider_kSize2->setMinimum(1);
        slider_kSize2->setOrientation(Qt::Vertical);

        gridLayout_3->addWidget(slider_kSize2, 0, 1, 1, 1);

        slider_kSize1 = new QSlider(centralwidget);
        slider_kSize1->setObjectName(QString::fromUtf8("slider_kSize1"));
        slider_kSize1->setEnabled(false);
        slider_kSize1->setMinimum(1);
        slider_kSize1->setOrientation(Qt::Vertical);

        gridLayout_3->addWidget(slider_kSize1, 0, 0, 1, 1);

        lbl_kSize1 = new QLabel(centralwidget);
        lbl_kSize1->setObjectName(QString::fromUtf8("lbl_kSize1"));

        gridLayout_3->addWidget(lbl_kSize1, 1, 0, 1, 1);

        lbl_kSize2 = new QLabel(centralwidget);
        lbl_kSize2->setObjectName(QString::fromUtf8("lbl_kSize2"));

        gridLayout_3->addWidget(lbl_kSize2, 1, 1, 1, 1);


        verticalLayout_3->addLayout(gridLayout_3);


        horizontalLayout->addLayout(verticalLayout_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_2->addWidget(label_2);

        slider_pixels = new QSlider(centralwidget);
        slider_pixels->setObjectName(QString::fromUtf8("slider_pixels"));
        slider_pixels->setEnabled(false);
        slider_pixels->setMinimum(1);
        slider_pixels->setMaximum(200000);
        slider_pixels->setValue(10000);
        slider_pixels->setOrientation(Qt::Vertical);

        verticalLayout_2->addWidget(slider_pixels);

        lbl_pixels = new QLabel(centralwidget);
        lbl_pixels->setObjectName(QString::fromUtf8("lbl_pixels"));

        verticalLayout_2->addWidget(lbl_pixels);


        horizontalLayout->addLayout(verticalLayout_2);


        gridLayout_2->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        lbl_cantRojo = new QLabel(centralwidget);
        lbl_cantRojo->setObjectName(QString::fromUtf8("lbl_cantRojo"));

        horizontalLayout_2->addWidget(lbl_cantRojo);

        lbl_cantNaranja = new QLabel(centralwidget);
        lbl_cantNaranja->setObjectName(QString::fromUtf8("lbl_cantNaranja"));

        horizontalLayout_2->addWidget(lbl_cantNaranja);

        lbl_cantAmarillo = new QLabel(centralwidget);
        lbl_cantAmarillo->setObjectName(QString::fromUtf8("lbl_cantAmarillo"));

        horizontalLayout_2->addWidget(lbl_cantAmarillo);


        gridLayout_2->addLayout(horizontalLayout_2, 2, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        btn_load->setText(QApplication::translate("MainWindow", "Load", nullptr));
        radioBtn_original->setText(QApplication::translate("MainWindow", "Original", nullptr));
        radioBtn_rojo->setText(QApplication::translate("MainWindow", "Rojo", nullptr));
        radioBtn_naranja->setText(QApplication::translate("MainWindow", "Naranja", nullptr));
        radioBtn_amarillo->setText(QApplication::translate("MainWindow", "Amarillo", nullptr));
        lbl_img->setText(QApplication::translate("MainWindow", "lbl_img", nullptr));
        lbl_elemEstr->setText(QApplication::translate("MainWindow", "<html><head/><body><p>Elemento</p><p>Estructural</p></body></html>", nullptr));
        comboBox->setItemText(0, QApplication::translate("MainWindow", "Rectangulo", nullptr));
        comboBox->setItemText(1, QApplication::translate("MainWindow", "Cruz", nullptr));
        comboBox->setItemText(2, QApplication::translate("MainWindow", "Elipse", nullptr));

        label_5->setText(QApplication::translate("MainWindow", "kernel sizes", nullptr));
        lbl_kSize1->setText(QApplication::translate("MainWindow", "0", nullptr));
        lbl_kSize2->setText(QApplication::translate("MainWindow", "0", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "pixels", nullptr));
        lbl_pixels->setText(QApplication::translate("MainWindow", "0", nullptr));
        label->setText(QApplication::translate("MainWindow", "Galletas detectadas:", nullptr));
        lbl_cantRojo->setText(QApplication::translate("MainWindow", "Rojo: ", nullptr));
        lbl_cantNaranja->setText(QApplication::translate("MainWindow", "Naranja:", nullptr));
        lbl_cantAmarillo->setText(QApplication::translate("MainWindow", "Amarillo:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
