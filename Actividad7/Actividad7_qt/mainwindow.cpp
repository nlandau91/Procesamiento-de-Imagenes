#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

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

void MainWindow::update_image(cv::Mat &newMat)
{
    int w =  ui->lbl_img->width();
    int h =  ui->lbl_img->height();
    ui->lbl_img->setPixmap(cvMatToQPixmap(newMat).scaled(w,h,Qt::KeepAspectRatio));
    //ui->lbl_img->setScaledContents(true);
    //ui->lbl_img->setSizePolicy((QSizePolicy::Ignored,QSizePolicy::Ignored));
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    //int h = event->size().height();
    //int w = event->size().width();
    //std::cout << "h: " << h << "w: " << w <<std::endl;
    if(ui->radioBtn_result->isEnabled())
    {
        //update_image();
    }
}

//corrige la perspectiva de una imagen
cv::Mat corregirPerspectiva(cv::Mat &src)
{
    //buscamos los bordes de la hoja
    //para eso, la convertimos a escala de grises
    cv::Mat src_gray;
    cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);
    //aplicamos un filtro bilateral que preserva bordes
    cv::Mat thr;
    cv::bilateralFilter(src_gray, thr, 5, 75, 75);
    //cv::imshow("thr", thr);
    //cv::waitKey(0);
    //aplicamos un threshold para que nos quede una imagen binaria
    //cv::threshold(thr, thr, 170, 255, cv::THRESH_BINARY);
    cv::adaptiveThreshold(thr, thr, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 57, 4);
    //cv::imshow("thr", thr);
    //cv::waitKey(0);
    //filtro de la mediana para limpiar pequenios detalles
    cv::medianBlur(thr, thr, 5);
    //cv::imshow("thr", thr);
    //cv::waitKey(0);
    //agregamos un borde negro en caso de que la hoja este tocando un borde de la imagen
    cv::copyMakeBorder(thr, thr, 5, 5, 5, 5, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
    cv::Mat edges;
    cv::Canny(thr, edges, 200, 250);
    //cv::imshow("edges", edges);
    //cv::waitKey(0);
    //ahora que tenemos los bordes, encontramos el contorno
    //cv::Mat contours;
    std::vector<std::vector<cv::Point>> contours;
    cv::Mat hierarchy;
    cv::findContours(edges, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    //buscamos el contorno del rectangulo mas grande
    //si no lo encontramos, devolvemos las esquinas de la imagen original
    int height = edges.rows;
    int width = edges.cols;
    int max_contour_area = (height - 10) * (height - 10);
    //suponemos que la hoja ocupa al menos un cuarto de la imagen
    int maxAreaFound = max_contour_area * 0.25;
    //guardamos el contorno de la pagina
    std::vector<cv::Point> pageContour = {cv::Point(5, 5), cv::Point(5, height - 5), cv::Point(width - 5, height - 5), cv::Point(width - 5, 5)};
    //iteramos sobre todos los contornos
    int idx = 0;
    int maxidx = 0;
    for (std::vector<cv::Point> cnt : contours)
    {
        //simplificamos el contorno
        double perimeter = cv::arcLength(cnt, true);
        //cv::Mat approx;
        std::vector<cv::Point> approx;
        cv::approxPolyDP(cnt, approx, 0.02 * perimeter, true);

        //la pagina tiene 4 esquinas y es convexa
        //el area debe ser mayor que maxAreaFound
        if (approx.size() == 4)
        {
            if (cv::isContourConvex(approx))
            {

                double area = cv::contourArea(approx);
                if (maxAreaFound < area && area < max_contour_area)
                {
                    maxAreaFound = area;
                    pageContour = approx;
                    maxidx = idx;
                }
            }
        }
        idx++;
    }
    //cv::drawContours(src,contours,maxidx,cv::Scalar(255,255,255),1,cv::LINE_8,hierarchy,0);
    //ahora tenemos los 4 puntos que definen el contorno
    //los ordenamos en sentido antihorario comenzando por la esquina superior izquierda
    double midX = pageContour[0].x + pageContour[1].x + pageContour[2].x + pageContour[3].x;
    midX = (double)midX / 4;
    double midY = pageContour[0].y + pageContour[1].y + pageContour[2].y + pageContour[3].y;
    midY = (double)midY / 4;
    std::vector<cv::Point2f> quad_pts(4); //puntos origen

    for (cv::Point p : pageContour)
    {

        if (p.x < midX && p.y < midY)
        {
            quad_pts[0] = p;
        }
        if (p.x < midX && p.y >= midY)
        {
            quad_pts[1] = p;
        }
        if (p.x >= midX && p.y >= midY)
        {
            quad_pts[2] = p;
        }
        if (p.x >= midX && p.y < midY)
        {
            quad_pts[3] = p;
        }
    }
    //arreglamos el offset de 5 del contorno
    for (cv::Point p : quad_pts)
    {
        p.x -= 5;
        if (p.x < 0)
        {
            p.x = 0;
        }
        p.y -= 5;
        if (p.y < 0)
        {
            p.y = 0;
        }
    }

    std::vector<cv::Point2f> squre_pts; //puntos destino
    //cv::Rect boundRect = cv::boundingRect(pageContour);
    //squre_pts.push_back(cv::Point2f(boundRect.x, boundRect.y));
    //squre_pts.push_back(cv::Point2f(boundRect.x, boundRect.y + boundRect.height));
    //squre_pts.push_back(cv::Point2f(boundRect.x + boundRect.width, boundRect.y + boundRect.height));
    //squre_pts.push_back(cv::Point2f(boundRect.x + boundRect.width, boundRect.y));
    int cols = src.cols;
    int rows = src.rows;
    squre_pts.push_back(cv::Point2f(0,0));
    squre_pts.push_back(cv::Point2f(0,rows));
    squre_pts.push_back(cv::Point2f(cols,rows));
    squre_pts.push_back(cv::Point2f(cols,0));
    cv::Mat transmtx = cv::getPerspectiveTransform(quad_pts, squre_pts);
    cv::Mat transformed = cv::Mat::zeros(src.rows, src.cols, CV_8UC3);
    cv::warpPerspective(src, transformed, transmtx, src.size());
    return transformed;
}

cv::Mat MainWindow::procesar(cv::Mat &src)
{
    //empezamos por corregir la perspectiva de la hoja
    cv::Mat corrected = corregirPerspectiva(src);
    //ahora que tenemos la perspectiva corregida, procedemos a identificar circulos
    //filtramos la imagen para elimiar ruido
    cv::Mat filtered;
    cv::medianBlur(corrected,filtered,5);
    //aplicamos thresholds a la imagen para separar los colores que nos interesan
    //esto es mas sencillo si utilizamos hsv
    cv::Mat hsv_image;
    cv::cvtColor(filtered, hsv_image, cv::COLOR_BGR2HSV);
    //establecemos los rangos para cada color, dividimos por dos porque el rango va de 0 a 180, no a 360
    int low_H_rojo1 = 0 / 2, high_H_rojo1 = 15 / 2 - 1;
    int low_H_naranja = 28 / 2, high_H_naranja = 36 / 2 - 1;
    int low_H_amarillo = 39 / 2, high_H_amarillo = 75 / 2 - 1;
    int low_H_rojo2 = 290 / 2, high_H_rojo2 = 360 / 2;

    //establecemos los rangos para el s y el v, esto nos sirve para que los blancos y negros no sean confundidos con algun color
    int low_S = 60, high_S = 220; //max=255
    int low_V = 60, high_V = 220; //max=255

    //Para cada rango, obtenemos una imagen con solamente los pixels que se encuentran en ese rango
    cv::Mat en_rango_rojo1, en_rango_rojo2, en_rango_rojo, en_rango_naranja, en_rango_amarillo;
    cv::inRange(hsv_image, cv::Scalar(low_H_rojo1, 40, low_V), cv::Scalar(high_H_rojo1, high_S, high_V), en_rango_rojo1);
    cv::inRange(hsv_image, cv::Scalar(low_H_rojo2, 40, low_V), cv::Scalar(high_H_rojo2, high_S, high_V), en_rango_rojo2);
    cv::bitwise_or(en_rango_rojo1, en_rango_rojo2, en_rango_rojo);
    cv::inRange(hsv_image, cv::Scalar(low_H_naranja, low_S, low_V), cv::Scalar(high_H_naranja, high_S, high_V), en_rango_naranja);
    cv::inRange(hsv_image, cv::Scalar(low_H_amarillo, low_S, low_V), cv::Scalar(high_H_amarillo, high_S, high_V), en_rango_amarillo);

    //aplicamos operaciones morfologicas (apertura + cierre) para eliminar imperfecciones

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    cv::morphologyEx(en_rango_rojo, en_rango_rojo, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(en_rango_rojo, en_rango_rojo, cv::MORPH_CLOSE, kernel);
    cv::morphologyEx(en_rango_naranja, en_rango_naranja, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(en_rango_naranja, en_rango_naranja, cv::MORPH_CLOSE, kernel);
    cv::morphologyEx(en_rango_amarillo, en_rango_amarillo, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(en_rango_amarillo, en_rango_amarillo, cv::MORPH_CLOSE, kernel);


    //aplicamos los thresholds como mascara a la imagen en escala de gris
    cv::Mat filtered_gray;
    cv::cvtColor(filtered,filtered_gray,cv::COLOR_BGR2GRAY);
    cv::bitwise_and(filtered_gray,en_rango_rojo,thresholds[0]);
    cv::bitwise_and(filtered_gray,en_rango_naranja,thresholds[1]);
    cv::bitwise_and(filtered_gray,en_rango_amarillo,thresholds[2]);

    //usamos el metodo de hough para encontrar circulos
    std::vector<cv::Vec3f> red_circles;
    std::vector<cv::Vec3f> orange_circles;
    std::vector<cv::Vec3f> yellow_circles;
    int param1 = corrected.rows/8;
    int param2 = 100;
    int param3 = 20;
    int param4 = 0; //minradius
    int param5 = corrected.cols / 8; //maxradius
    cv::HoughCircles(thresholds[0], red_circles, cv::HOUGH_GRADIENT, 1,
                     param1, // change this value to detect circles with different distances to each other
                     param2,param3,param4,param5); // change the last two parameters (min_radius & max_radius) to detect larger circles
    cv::HoughCircles(thresholds[1], orange_circles, cv::HOUGH_GRADIENT, 1, param1,param2,param3,param4,param5);
    cv::HoughCircles(thresholds[2], yellow_circles, cv::HOUGH_GRADIENT, 1, param1,param2,param3,param4,param5);
    //iteramos sobre los circulos encontrados para dibujar un circulo en la imagen original
    if (red_circles.size() > 0)
    {
        for (size_t current_circle = 0; current_circle < red_circles.size(); ++current_circle)
        {
            cv::Point center(std::round(red_circles[current_circle][0]), std::round(red_circles[current_circle][1]));
            int radius = std::round(red_circles[current_circle][2]);

            cv::circle(corrected, center, radius, cv::Scalar(181,113,255,255), 5);
        }
    }
    if (orange_circles.size() > 0)
    {
        for (size_t current_circle = 0; current_circle < orange_circles.size(); ++current_circle)
        {
            cv::Point center(std::round(orange_circles[current_circle][0]), std::round(orange_circles[current_circle][1]));
            int radius = std::round(orange_circles[current_circle][2]);

            cv::circle(corrected, center, radius, cv::Scalar(16,64,180,255), 5);
        }
    }
    if (yellow_circles.size() > 0)
    {
        for (size_t current_circle = 0; current_circle < yellow_circles.size(); ++current_circle)
        {
            cv::Point center(std::round(yellow_circles[current_circle][0]), std::round(yellow_circles[current_circle][1]));
            int radius = std::round(yellow_circles[current_circle][2]);

            cv::circle(corrected, center, radius, cv::Scalar(0,255,255,255), 5);
        }
    }
    return corrected;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resetUi()
{
    ui->lbl_img->clear();
    ui->btn_procesar->setDisabled(true);
    ui->btn_save->setDisabled(true);
    ui->radioBtn_amarillo->setDisabled(true);
    ui->radioBtn_naranja->setDisabled(true);
    ui->radioBtn_rojo->setDisabled(true);
    ui->radioBtn_result->setDisabled(true);
    ui->radioBtn_result->setChecked(true);
}

void MainWindow::on_btn_load_clicked()
{
    resetUi();
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Image"), "",
                                                    tr("Image (*.jpg);;Image (*.png);;All Files (*)"));
    if(!fileName.isEmpty())
    {
        src = cv::imread(fileName.toStdString(),cv::IMREAD_UNCHANGED);
        if(!src.empty())
        {
            result = src.clone();
            update_image(src);
            ui->btn_procesar->setEnabled(true);
        }
    }

}

void MainWindow::on_btn_procesar_clicked()
{
    result = procesar(src);
    update_image(result);
    ui->radioBtn_result->setEnabled(true);
    ui->radioBtn_rojo->setEnabled(true);
    ui->radioBtn_naranja->setEnabled(true);
    ui->radioBtn_amarillo->setEnabled(true);
    ui->btn_save->setEnabled(true);
}

void MainWindow::on_btn_save_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("Save Image"),"",
                                                    tr("jpg (*.jpg);;png (*.png);;All Files (*)"));
    cv::imwrite(filename.toStdString(),result);
}

void MainWindow::on_radioBtn_rojo_clicked()
{
    update_image(thresholds[0]);
}

void MainWindow::on_radioBtn_naranja_clicked()
{
    update_image(thresholds[1]);
}

void MainWindow::on_radioBtn_amarillo_clicked()
{
    update_image(thresholds[2]);
}

void MainWindow::on_radioBtn_result_clicked()
{
    update_image(result);
}