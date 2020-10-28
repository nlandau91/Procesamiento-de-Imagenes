#include "mainwindow.h"
#include "ui_mainwindow.h"

inline QImage  cvMatToQImage( const cv::Mat &inMat )
{
    switch ( inMat.type() )
    {
    // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB32 );

        return image;
    }

        // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB888 );

        return image.rgbSwapped();
    }

        // 8-bit, 1 channel
    case CV_8UC1:
    {
        static QVector<QRgb>  sColorTable;

        // only create our color table once

        QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_Indexed8 );
        return image;
    }

    default:
    {
        break;
    }

    }

    return QImage();
}

inline QPixmap cvMatToQPixmap( const cv::Mat &inMat )
{
    return QPixmap::fromImage( cvMatToQImage( inMat ) );
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init_values();
}

void MainWindow::init_values()
{
    pixPerCookie = 10000;
    ui->lbl_pixels->setText(QString::number(pixPerCookie));
    kSize = 1;
    shape = cv::MORPH_RECT;
    ui->lbl_kSize->setText(QString::number(kSize));
}

void MainWindow::update_image()
{
    ui->lbl_img->setPixmap(cvMatToQPixmap(cvMats[CVMAT_ACTUAL]));
}

void MainWindow::procesar(cv::Mat &src, cv::Mat &dst)
{
    cv::Mat kernel = cv::getStructuringElement(shape,cv::Size(kSize,kSize));
    cv::erode(src,dst,kernel);
    cv::dilate(dst,dst,kernel);
    cv::dilate(dst,dst,kernel);
    cv::erode(dst,dst,kernel);
}

void MainWindow::obtener_thresholds(cv::Mat &src)
{
    //la convertimos a hsv, ya que es mas facil diferenciar colores
    cv::Mat img_hsv;
    cv::cvtColor(src,img_hsv,cv::COLOR_BGR2HSV);

    //establecemos los rangos para cada color, dividimos por dos porque el rango va de 0 a 180, no a 360, restamos 1  en el tope superior porque los rangos son cerrados
    int low_H_rojo1 = 0 / 2, high_H_rojo1 = 15 / 2 - 1;
    int low_H_naranja = 28 / 2, high_H_naranja = 36 / 2 - 1;
    int low_H_amarillo = 39 / 2, high_H_amarillo = 55 / 2 - 1;
    int low_H_rojo2 = 355 / 2, high_H_rojo2 = 360 / 2 - 1;

    //establecemos los rangos para el s y el v, esto nos sirve para que los blancos y negros no sean confundidos con algun color
    int low_S = 100, high_S = 220;
    int low_V = 100, high_V = 220;

    //Para cada rango, obtenemos una imagen con solamente los pixels que se encuentran en ese rango
    cv::Mat en_rango_rojo1, en_rango_rojo2;
    cv::inRange(img_hsv, cv::Scalar(low_H_rojo1, 40, low_V), cv::Scalar(high_H_rojo1, high_S, high_V), en_rango_rojo1);
    cv::inRange(img_hsv, cv::Scalar(low_H_rojo2, 40, low_V), cv::Scalar(high_H_rojo2, high_S, high_V), en_rango_rojo2);
    cv::bitwise_or(en_rango_rojo1,en_rango_rojo2,cvMats[CVMAT_ROJO]);
    cv::inRange(img_hsv, cv::Scalar(low_H_naranja, low_S, low_V), cv::Scalar(high_H_naranja, high_S, high_V), cvMats[CVMAT_NARANJA]);
    cv::inRange(img_hsv, cv::Scalar(low_H_amarillo, low_S, low_V), cv::Scalar(high_H_amarillo, high_S, high_V), cvMats[CVMAT_AMARILLO]);
}

void MainWindow::calcular_cookies()
{
    int cantRojo = cv::countNonZero(cvMats[CVMAT_ROJO]) / pixPerCookie;
    int cantNaranja = cv::countNonZero(cvMats[CVMAT_NARANJA]) / pixPerCookie;
    int cantAmarillo = cv::countNonZero(cvMats[CVMAT_AMARILLO]) / pixPerCookie;
    ui->lbl_cantRojo->setText("Rojo: " + QString::number(cantRojo));
    ui->lbl_cantNaranja->setText("Naranja: " + QString::number(cantNaranja));
    ui->lbl_cantAmarillo->setText("Amarillo: " + QString::number(cantAmarillo));
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_btn_load_clicked()
{
    //path hardcodeado
    std::string src_path = "img.png";
    cvMats[CVMAT_ORIGINAL] = cv::imread(src_path,cv::IMREAD_UNCHANGED);
    cvMats[CVMAT_ACTUAL] = cvMats[CVMAT_ORIGINAL];
    obtener_thresholds(cvMats[CVMAT_ORIGINAL]);
    procesar(cvMats[CVMAT_ROJO],cvMats[CVMAT_ROJO]);
    procesar(cvMats[CVMAT_NARANJA],cvMats[CVMAT_NARANJA]);
    procesar(cvMats[CVMAT_AMARILLO],cvMats[CVMAT_AMARILLO]);
    calcular_cookies();
    update_image();
    ui->radioBtn_original->setEnabled(true);
    ui->radioBtn_original->setChecked(true);
    ui->radioBtn_rojo->setEnabled(true);
    ui->radioBtn_naranja->setEnabled(true);
    ui->radioBtn_amarillo->setEnabled(true);
    ui->slider_kSize->setEnabled(true);
    ui->slider_pixels->setEnabled(true);
    ui->btn_load->setEnabled(false);
}

void MainWindow::on_radioBtn_rojo_clicked()
{
    cvMats[CVMAT_ACTUAL] = cvMats[CVMAT_ROJO];
    update_image();
}

void MainWindow::on_radioBtn_naranja_clicked()
{
    cvMats[CVMAT_ACTUAL] = cvMats[CVMAT_NARANJA];
    update_image();
}

void MainWindow::on_radioBtn_amarillo_clicked()
{
    cvMats[CVMAT_ACTUAL] = cvMats[CVMAT_AMARILLO];
    update_image();
}

void MainWindow::on_radioBtn_original_clicked()
{
    cvMats[CVMAT_ACTUAL] = cvMats[CVMAT_ORIGINAL];
    update_image();
}

void MainWindow::on_slider_kSize_sliderMoved(int position)
{
    if(position % 2 == 1)
    {
        kSize = position;
        ui->lbl_kSize->setText(QString::number(position));
        obtener_thresholds(cvMats[CVMAT_ORIGINAL]);
        procesar(cvMats[CVMAT_ROJO],cvMats[CVMAT_ROJO]);
        procesar(cvMats[CVMAT_NARANJA],cvMats[CVMAT_NARANJA]);
        procesar(cvMats[CVMAT_AMARILLO],cvMats[CVMAT_AMARILLO]);
        calcular_cookies();
        update_image();
    }
}

void MainWindow::on_slider_pixels_sliderMoved(int position)
{
    pixPerCookie = position;
    calcular_cookies();
    ui->lbl_pixels->setText(QString::number(position));
}
