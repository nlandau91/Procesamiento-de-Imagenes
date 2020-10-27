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
    pixPerCookie = 30000;
    kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(3,3));

}

void MainWindow::update_image(cv::Mat src)
{
    ui->lbl_img->setPixmap(cvMatToQPixmap(src));
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_btn_load_clicked()
{
    //path hardcoded
    std::string src_path = "img.png";
    original = cv::imread(src_path,cv::IMREAD_UNCHANGED);
    update_image(original);
}

void MainWindow::on_radioBtn_result_clicked()
{

}

void MainWindow::on_radioBtn_rojo_clicked()
{

}

void MainWindow::on_radioBtn_naranja_clicked()
{

}

void MainWindow::on_radioBtn_amarillo_clicked()
{

}
