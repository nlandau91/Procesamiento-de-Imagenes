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
    cvMats[CVMAT_ACTUAL] = newMat;
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
        update_image(cvMats[CVMAT_ACTUAL]);
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
    //int maxidx = 0;
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
                   // maxidx = idx;
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

void MainWindow::obtener_thresholds(cv::Mat &src, std::vector<cv::Mat> &dst)
{
    //la convertimos a hsv, ya que es mas facil diferenciar colores
    cv::Mat img_hsv;
    cv::cvtColor(src,img_hsv,cv::COLOR_BGR2HSV);

    //establecemos los rangos para los thresholds
    //H va de 0 a 180
    //S va de 0 a 255
    //V va de 0 a 255

    cv::Scalar rosa_low_1 = cv::Scalar(0/2, 0.15*255, 0.55*255);
    cv::Scalar rosa_high_1 = cv::Scalar(25/2,0.45*255, 0.9*255);

    cv::Scalar rosa_low_2 = cv::Scalar(290/2, 0.15*255, 0.55*255);
    cv::Scalar rosa_high_2 = cv::Scalar(360/2, 0.45*255, 0.9*255);

    cv::Scalar naranja_low = cv::Scalar(20/2, 0.35*255, 0.5*255);
    cv::Scalar naranja_high = cv::Scalar(36/2, 0.60*255, 0.80*255);

    cv::Scalar amarillo_low = cv::Scalar(40/2, 0.25*255, 0.50*255);
    cv::Scalar amarillo_high = cv::Scalar(70/2, 0.99*255, 0.99*255);

    cv::Scalar chocolate_low = cv::Scalar(10/2, 0.30*255, 0.35*255);
    cv::Scalar chocolate_high = cv::Scalar(25/2, 0.50*255, 0.55*255);



    //Para cada rango, obtenemos una imagen con solamente los pixels que se encuentran en ese rango
    cv::Mat en_rango_rosa1, en_rango_rosa2, thresh_rosa, thresh_naranja, thresh_amarillo, thresh_chocolate;
    cv::inRange(img_hsv, rosa_low_1, rosa_high_1, en_rango_rosa1);
    cv::inRange(img_hsv, rosa_low_2, rosa_high_2, en_rango_rosa2);
    cv::bitwise_or(en_rango_rosa1, en_rango_rosa2, thresh_rosa);
    cv::inRange(img_hsv, naranja_low, naranja_high, thresh_naranja);
    cv::inRange(img_hsv, amarillo_low, amarillo_high, thresh_amarillo);
    cv::inRange(img_hsv, chocolate_low, chocolate_high, thresh_chocolate);

    dst.push_back(thresh_rosa);
    dst.push_back(thresh_naranja);
    dst.push_back(thresh_amarillo);
    dst.push_back(thresh_chocolate);
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
    std::vector<cv::Mat> masks;
    obtener_thresholds(corrected, masks);
    cvMats[CVMAT_ROSA] = masks[0];
    cvMats[CVMAT_NARANJA] = masks[1];
    cvMats[CVMAT_AMARILLO] = masks[2];
    cvMats[CVMAT_CHOCOLATE] = masks[3];

    //aplicamos operaciones morfologicas (apertura + cierre) para eliminar imperfecciones

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(9, 9));
    cv::morphologyEx(cvMats[CVMAT_ROSA], cvMats[CVMAT_ROSA], cv::MORPH_OPEN, kernel);
    cv::morphologyEx(cvMats[CVMAT_ROSA], cvMats[CVMAT_ROSA], cv::MORPH_CLOSE, kernel);
    cv::morphologyEx(cvMats[CVMAT_NARANJA], cvMats[CVMAT_NARANJA], cv::MORPH_OPEN, kernel);
    cv::morphologyEx(cvMats[CVMAT_NARANJA], cvMats[CVMAT_NARANJA], cv::MORPH_CLOSE, kernel);
    cv::morphologyEx(cvMats[CVMAT_AMARILLO], cvMats[CVMAT_AMARILLO], cv::MORPH_OPEN, kernel);
    cv::morphologyEx(cvMats[CVMAT_AMARILLO], cvMats[CVMAT_AMARILLO], cv::MORPH_CLOSE, kernel);
    cv::morphologyEx(cvMats[CVMAT_CHOCOLATE], cvMats[CVMAT_CHOCOLATE], cv::MORPH_OPEN, kernel);
    cv::morphologyEx(cvMats[CVMAT_CHOCOLATE], cvMats[CVMAT_CHOCOLATE], cv::MORPH_CLOSE, kernel);
    cv::GaussianBlur(cvMats[CVMAT_ROSA],cvMats[CVMAT_ROSA],cv::Size(5,5),0);
    cv::GaussianBlur(cvMats[CVMAT_NARANJA],cvMats[CVMAT_NARANJA],cv::Size(5,5),0);
    cv::GaussianBlur(cvMats[CVMAT_AMARILLO],cvMats[CVMAT_AMARILLO],cv::Size(5,5),0);
    cv::GaussianBlur(cvMats[CVMAT_CHOCOLATE],cvMats[CVMAT_CHOCOLATE],cv::Size(5,5),0);

    //aplicamos los thresholds como mascara a la imagen en escala de gris
    cv::Mat filtered_gray;
    cv::cvtColor(filtered,filtered_gray,cv::COLOR_BGR2GRAY);
    cv::bitwise_and(filtered_gray,cvMats[CVMAT_ROSA],cvMats[CVMAT_ROSA]);
    cv::bitwise_and(filtered_gray,cvMats[CVMAT_NARANJA],cvMats[CVMAT_NARANJA]);
    cv::bitwise_and(filtered_gray,cvMats[CVMAT_AMARILLO],cvMats[CVMAT_AMARILLO]);
    cv::bitwise_and(filtered_gray,cvMats[CVMAT_CHOCOLATE],cvMats[CVMAT_CHOCOLATE]);

    //usamos el metodo de hough para encontrar circulos
    std::vector<cv::Vec3f> circulos_rosa;
    std::vector<cv::Vec3f> circulos_naranja;
    std::vector<cv::Vec3f> circulos_amarillo;
    std::vector<cv::Vec3f> circulos_chocolate;
    int param1 = (corrected.rows * ui->lbl_minDist->text().toInt())/100; //minDist
    int param2 = ui->lbl_thresh->text().toInt(); //high thresh of the edge detector
    int param3 = ui->lbl_acc->text().toInt(); //accumulator
    int param4 = ui->lbl_minRad->text().toInt(); //minradius
    int param5 = ui->lbl_maxRad->text().toInt(); //maxradius
    cv::HoughCircles(cvMats[CVMAT_ROSA], circulos_rosa, cv::HOUGH_GRADIENT, 1,
                     param1, // change this value to detect circles with different distances to each other
                     param2,param3,param4,param5); // change the last two parameters (min_radius & max_radius) to detect larger circles
    cv::HoughCircles(cvMats[CVMAT_NARANJA], circulos_naranja, cv::HOUGH_GRADIENT, 1, param1,param2,param3,param4,param5);
    cv::HoughCircles(cvMats[CVMAT_AMARILLO], circulos_amarillo, cv::HOUGH_GRADIENT, 1, param1,param2,param3,param4,param5);
    cv::HoughCircles(cvMats[CVMAT_CHOCOLATE], circulos_chocolate, cv::HOUGH_GRADIENT, 1, param1,param2,param3,param4,param5);
    //iteramos sobre los circulos encontrados para dibujar un circulo en la imagen original
    if (circulos_rosa.size() > 0)
    {
        for (size_t current_circle = 0; current_circle < circulos_rosa.size(); ++current_circle)
        {
            cv::Point center(std::round(circulos_rosa[current_circle][0]), std::round(circulos_rosa[current_circle][1]));
            int radius = std::round(circulos_rosa[current_circle][2]);

            cv::circle(corrected, center, radius, cv::Scalar(181,113,255,255), 5);
        }
    }
    if (circulos_naranja.size() > 0)
    {
        for (size_t current_circle = 0; current_circle < circulos_naranja.size(); ++current_circle)
        {
            cv::Point center(std::round(circulos_naranja[current_circle][0]), std::round(circulos_naranja[current_circle][1]));
            int radius = std::round(circulos_naranja[current_circle][2]);

            cv::circle(corrected, center, radius, cv::Scalar(16,64,180,255), 5);
        }
    }
    if (circulos_amarillo.size() > 0)
    {
        for (size_t current_circle = 0; current_circle < circulos_amarillo.size(); ++current_circle)
        {
            cv::Point center(std::round(circulos_amarillo[current_circle][0]), std::round(circulos_amarillo[current_circle][1]));
            int radius = std::round(circulos_amarillo[current_circle][2]);

            cv::circle(corrected, center, radius, cv::Scalar(0,255,255,255), 5);
        }
    }
    if (circulos_chocolate.size() > 0)
    {
        for (size_t current_circle = 0; current_circle < circulos_chocolate.size(); ++current_circle)
        {
            cv::Point center(std::round(circulos_chocolate[current_circle][0]), std::round(circulos_chocolate[current_circle][1]));
            int radius = std::round(circulos_chocolate[current_circle][2]);

            cv::circle(corrected, center, radius, cv::Scalar(18,52,98,255),5);
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
    ui->radioBtn_rosa->setDisabled(true);
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
        cvMats[CVMAT_ORIGINAL] = cv::imread(fileName.toStdString(),cv::IMREAD_UNCHANGED);
        if(!cvMats[CVMAT_ORIGINAL].empty())
        {
            cvMats[CVMAT_RESULT] = cvMats[CVMAT_ORIGINAL].clone();
            update_image(cvMats[CVMAT_ORIGINAL]);
            ui->btn_procesar->setEnabled(true);
        }
    }

}

void MainWindow::on_btn_procesar_clicked()
{
    cvMats[CVMAT_RESULT] = procesar(cvMats[CVMAT_ORIGINAL]);
    update_image(cvMats[CVMAT_RESULT]);
    ui->radioBtn_result->setEnabled(true);
    ui->radioBtn_rosa->setEnabled(true);
    ui->radioBtn_naranja->setEnabled(true);
    ui->radioBtn_amarillo->setEnabled(true);
    ui->btn_save->setEnabled(true);
}

void MainWindow::on_btn_save_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("Save Image"),"",
                                                    tr("jpg (*.jpg);;png (*.png);;All Files (*)"));
    cv::imwrite(filename.toStdString(), cvMats[CVMAT_RESULT]);
}

void MainWindow::on_radioBtn_rosa_clicked()
{
    update_image(cvMats[CVMAT_ROSA]);
}

void MainWindow::on_radioBtn_naranja_clicked()
{
    update_image(cvMats[CVMAT_NARANJA]);
}

void MainWindow::on_radioBtn_amarillo_clicked()
{
    update_image(cvMats[CVMAT_AMARILLO]);
}

void MainWindow::on_radioBtn_result_clicked()
{
    update_image(cvMats[CVMAT_RESULT]);
}

void MainWindow::on_verticalSlider_minDist_valueChanged(int value)
{
    ui->lbl_minDist->setText(QString::number(value));
}

void MainWindow::on_verticalSlider_minRad_valueChanged(int value)
{
    ui->lbl_minRad->setText(QString::number(value));
}

void MainWindow::on_verticalSlider_maxRad_valueChanged(int value)
{
    ui->lbl_maxRad->setText(QString::number(value));
}

void MainWindow::on_verticalSlider_minDist_sliderReleased()
{
    if(ui->radioBtn_result->isEnabled())
    {
        cvMats[CVMAT_RESULT] = procesar(cvMats[CVMAT_ORIGINAL]);
        update_image(cvMats[CVMAT_RESULT]);
    }
}

void MainWindow::on_verticalSlider_minRad_sliderReleased()
{
    if(ui->radioBtn_result->isEnabled())
    {
        cvMats[CVMAT_RESULT] = procesar(cvMats[CVMAT_ORIGINAL]);
        update_image(cvMats[CVMAT_RESULT]);
    }
}

void MainWindow::on_verticalSlider_maxRad_sliderReleased()
{
    if(ui->radioBtn_result->isEnabled())
    {
        cvMats[CVMAT_RESULT] = procesar(cvMats[CVMAT_ORIGINAL]);
        update_image(cvMats[CVMAT_RESULT]);
    }
}

void MainWindow::on_verticalSlider_thresh_valueChanged(int value)
{
    ui->lbl_thresh->setText(QString::number(value));
}

void MainWindow::on_verticalSilder_acc_valueChanged(int value)
{
    ui->lbl_acc->setText(QString::number(value));
}

void MainWindow::on_verticalSlider_thresh_sliderReleased()
{
    if(ui->radioBtn_result->isEnabled())
    {
        cvMats[CVMAT_RESULT] = procesar(cvMats[CVMAT_ORIGINAL]);
        update_image(cvMats[CVMAT_RESULT]);
    }
}

void MainWindow::on_verticalSilder_acc_sliderReleased()
{
    if(ui->radioBtn_result->isEnabled())
    {
        cvMats[CVMAT_RESULT] = procesar(cvMats[CVMAT_ORIGINAL]);
        update_image(cvMats[CVMAT_RESULT]);
    }
}

void MainWindow::on_radioBtn_chocolate_clicked()
{
    update_image(cvMats[CVMAT_CHOCOLATE]);
}
