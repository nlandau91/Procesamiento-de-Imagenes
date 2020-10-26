#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

void MainWindow::funcionActivacionTimer()
{

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    img_result = ui->img_result;
    img_rojo = ui->img_rojo;
    img_naranja = ui->img_naranja;
    img_amarillo = ui->img_amarillo;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_result_pixmap(QPixmap qpixmap)
{
    const int w = img_result->width();
    const int h = img_result->height();
    img_result->setPixmap(qpixmap.scaled(w,h,Qt::IgnoreAspectRatio));
}
void MainWindow::set_rojo_pixmap(QPixmap qpixmap)
{
    const int w = img_rojo->width();
    const int h = img_rojo->height();
    img_rojo->setPixmap(qpixmap.scaled(w,h,Qt::IgnoreAspectRatio));
}
void MainWindow::set_naranja_pixmap(QPixmap qpixmap)
{
    const int w = img_naranja->width();
    const int h = img_naranja->height();
    img_naranja->setPixmap(qpixmap.scaled(w,h,Qt::IgnoreAspectRatio));
}
void MainWindow::set_amarillo_pixmap(QPixmap qpixmap)
{
    const int w = img_amarillo->width();
    const int h = img_amarillo->height();
    img_amarillo->setPixmap(qpixmap.scaled(w,h,Qt::IgnoreAspectRatio));
}

void MainWindow::set_result_qimage(QImage qimage)
{
    const int w = img_result->width();
    const int h = img_result->height();
    img_result->setPixmap(QPixmap::fromImage(qimage).scaled(w,h,Qt::IgnoreAspectRatio));
}
void MainWindow::set_rojo_qimage(QImage qimage)
{
    const int w = img_rojo->width();
    const int h = img_rojo->height();
    img_rojo->setPixmap(QPixmap::fromImage(qimage).scaled(w,h,Qt::IgnoreAspectRatio));
}
void MainWindow::set_naranja_qimage(QImage qimage)

{
    const int w = img_naranja->width();
    const int h = img_naranja->height();
    img_naranja->setPixmap(QPixmap::fromImage(qimage).scaled(w,h,Qt::IgnoreAspectRatio));
}
void MainWindow::set_amarillo_qimage(QImage qimage)

{
    const int w = img_amarillo->width();
    const int h = img_amarillo->height();
    img_amarillo->setPixmap(QPixmap::fromImage(qimage).scaled(w,h,Qt::IgnoreAspectRatio));
}

