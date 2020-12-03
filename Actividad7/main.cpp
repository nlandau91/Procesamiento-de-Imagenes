#include <opencv2/opencv.hpp>
#include <iostream>

cv::Mat resize(cv::Mat &src, int h = 800)
{
    double ratio = (double)h / src.rows;
    cv::Mat dst;
    cv::resize(src, dst, cv::Size(), ratio, ratio);
    return dst;
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
    cv::imshow("thr", thr);
    cv::waitKey(0);
    //aplicamos un threshold para que nos quede una imagen binaria
    //cv::threshold(thr, thr, 170, 255, cv::THRESH_BINARY);
    cv::adaptiveThreshold(thr, thr, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 57, 4);
    cv::imshow("thr", thr);
    cv::waitKey(0);
    //filtro de la mediana para limpiar pequenios detalles
    cv::medianBlur(thr, thr, 5);
    cv::imshow("thr", thr);
    cv::waitKey(0);
    //agregamos un borde negro en caso de que la hoja este tocando un borde de la imagen
    cv::copyMakeBorder(thr, thr, 5, 5, 5, 5, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
    cv::Mat edges;
    cv::Canny(thr, edges, 200, 250);
    cv::imshow("edges", edges);
    cv::waitKey(0);
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

    cv::Rect boundRect = cv::boundingRect(pageContour);
    std::vector<cv::Point2f> squre_pts; //puntos destino
    squre_pts.push_back(cv::Point2f(boundRect.x, boundRect.y));
    squre_pts.push_back(cv::Point2f(boundRect.x, boundRect.y + boundRect.height));
    squre_pts.push_back(cv::Point2f(boundRect.x + boundRect.width, boundRect.y + boundRect.height));
    squre_pts.push_back(cv::Point2f(boundRect.x + boundRect.width, boundRect.y));
    cv::Mat transmtx = cv::getPerspectiveTransform(quad_pts, squre_pts);
    cv::Mat transformed = cv::Mat::zeros(src.rows, src.cols, CV_8UC3);
    cv::warpPerspective(src, transformed, transmtx, src.size());
    return transformed;
}

int main(int argc, char *argv[])
{
    //leemos la imagen
    char const *src_path = "img.png";
    if (argc > 1)
    {
        src_path = argv[1];
    }
    cv::Mat src = cv::imread(src_path, cv::IMREAD_UNCHANGED);
    if (src.empty())
    {
        std::cout << "No se pudo cargar la imagen" << std::endl;
        return -1;
    }
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
    int low_H_amarillo = 39 / 2, high_H_amarillo = 55 / 2 - 1;
    int low_H_rojo2 = 355 / 2, high_H_rojo2 = 360 / 2;

    //establecemos los rangos para el s y el v, esto nos sirve para que los blancos y negros no sean confundidos con algun color
    int low_S = 100, high_S = 220;
    int low_V = 100, high_V = 220;

    //Para cada rango, obtenemos una imagen con solamente los pixels que se encuentran en ese rango
    cv::Mat en_rango_rojo1, en_rango_rojo2, en_rango_rojo, en_rango_naranja, en_rango_amarillo;
    cv::inRange(hsv_image, cv::Scalar(low_H_rojo1, 40, low_V), cv::Scalar(high_H_rojo1, high_S, high_V), en_rango_rojo1);
    cv::inRange(hsv_image, cv::Scalar(low_H_rojo2, 40, low_V), cv::Scalar(high_H_rojo2, high_S, high_V), en_rango_rojo2);
    cv::bitwise_or(en_rango_rojo1, en_rango_rojo2, en_rango_rojo);
    cv::inRange(hsv_image, cv::Scalar(low_H_naranja, low_S, low_V), cv::Scalar(high_H_naranja, high_S, high_V), en_rango_naranja);
    cv::inRange(hsv_image, cv::Scalar(low_H_amarillo, low_S, low_V), cv::Scalar(high_H_amarillo, high_S, high_V), en_rango_amarillo);

    //aplicamos operaciones morfologicas (apertura + cierre) para eliminar imperfecciones
    cv::medianBlur(en_rango_rojo, en_rango_rojo, 5);
    cv::medianBlur(en_rango_naranja, en_rango_naranja, 5);
    cv::medianBlur(en_rango_amarillo, en_rango_amarillo, 5);
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    cv::morphologyEx(en_rango_rojo, en_rango_rojo, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(en_rango_rojo, en_rango_rojo, cv::MORPH_CLOSE, kernel);
    cv::morphologyEx(en_rango_naranja, en_rango_naranja, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(en_rango_naranja, en_rango_naranja, cv::MORPH_CLOSE, kernel);
    cv::morphologyEx(en_rango_amarillo, en_rango_amarillo, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(en_rango_amarillo, en_rango_amarillo, cv::MORPH_CLOSE, kernel);

    //usamos el metodo de hough para encontrar circulos
    std::vector<cv::Vec3f> red_circles;
    std::vector<cv::Vec3f> orange_circles;
    std::vector<cv::Vec3f> yellow_circles;
    cv::HoughCircles(en_rango_rojo, red_circles, cv::HOUGH_GRADIENT, 1, 
    en_rango_rojo.rows / 4, // change this value to detect circles with different distances to each other
    100, 20, 0, 0); // change the last two parameters (min_radius & max_radius) to detect larger circles
    cv::HoughCircles(en_rango_naranja, orange_circles, cv::HOUGH_GRADIENT, 1, en_rango_naranja.rows / 4, 100, 20, 0, 0);
    cv::HoughCircles(en_rango_amarillo, yellow_circles, cv::HOUGH_GRADIENT, 1, en_rango_amarillo.rows / 4, 100, 20, 0, 0);
    // Loop over all detected circles and outline them on the original image
    if (red_circles.size() > 0)
    {
        for (size_t current_circle = 0; current_circle < red_circles.size(); ++current_circle)
        {
            cv::Point center(std::round(red_circles[current_circle][0]), std::round(red_circles[current_circle][1]));
            int radius = std::round(red_circles[current_circle][2]);

            cv::circle(corrected, center, radius, cv::Scalar(181,113,255), 5);
        }
    }
    if (orange_circles.size() > 0)
    {
        for (size_t current_circle = 0; current_circle < orange_circles.size(); ++current_circle)
        {
            cv::Point center(std::round(orange_circles[current_circle][0]), std::round(orange_circles[current_circle][1]));
            int radius = std::round(orange_circles[current_circle][2]);

            cv::circle(corrected, center, radius, cv::Scalar(16,64,180), 5);
        }
    }
    if (yellow_circles.size() > 0)
    {
        for (size_t current_circle = 0; current_circle < yellow_circles.size(); ++current_circle)
        {
            cv::Point center(std::round(yellow_circles[current_circle][0]), std::round(yellow_circles[current_circle][1]));
            int radius = std::round(yellow_circles[current_circle][2]);

            cv::circle(corrected, center, radius, cv::Scalar(0,255,255), 5);
        }
    }

    cv::namedWindow("src", cv::WINDOW_NORMAL);
    cv::namedWindow("corrected", cv::WINDOW_NORMAL);
    cv::namedWindow("en_rango_rojo", cv::WINDOW_NORMAL);
    cv::namedWindow("en_rango_naranja", cv::WINDOW_NORMAL);
    cv::namedWindow("en_rango_amarillo", cv::WINDOW_NORMAL);
    cv::imshow("src", src);
    cv::imshow("corrected", corrected);
    cv::imshow("en_rango_rojo", en_rango_rojo);
    cv::imshow("en_rango_naranja", en_rango_naranja);
    cv::imshow("en_rango_amarillo", en_rango_amarillo);
    cv::waitKey(0);
}