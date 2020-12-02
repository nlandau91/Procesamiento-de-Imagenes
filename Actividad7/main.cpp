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
    cv::bilateralFilter(src_gray, thr, 9, 75, 75);
    //aplicamos un threshold para que nos quede una imagen binaria
    //cv::threshold(thr, thr, 70, 255, cv::THRESH_BINARY);
    cv::adaptiveThreshold(thr, thr, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 115, 4);
    //filtro de la mediana para limpiar pequenios detalles
    cv::medianBlur(thr, thr, 11);
    //agregamos un borde negro en caso de que la hoja este tocando un borde de la imagen
    cv::copyMakeBorder(thr, thr, 5, 5, 5, 5, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
    cv::Mat edges;
    cv::Canny(thr, edges, 200, 250);
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
    //suponemos que la hoja ocupa al menos la mitad de la imagen
    //int maxAreaFound = max_contour_area * 0.5;
    int maxAreaFound = 0;
    //guardamos el contorno de la pagina
    std::vector<cv::Point> pageContour = {cv::Point(5, 5), cv::Point(5, height - 5), cv::Point(width - 5, height - 5), cv::Point(width - 5, 5)};
    //cv::Mat pageContour = cv::Mat(pc);
    //iteramos sobre todos los contornos
    int idx = 0;
    int maxidx = 0;
    for (std::vector<cv::Point> cnt : contours)
    {
        //sinplificamos el contorno
        double perimeter = cv::arcLength(cnt, true);
        //cv::Mat approx;
        std::vector<cv::Point> approx;
        cv::approxPolyDP(cnt, approx, 0.03 * perimeter, true);

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
    std::vector<cv::Point2f> quad_pts(4);  //puntos origen

    for(cv::Point p : pageContour)
    {

        if(p.x < midX && p.y < midY)
        {
            quad_pts[0] = p;
        }
        if(p.x < midX && p.y >= midY)
        {
            quad_pts[1] = p;
        }
        if(p.x >= midX && p.y >= midY)
        {
            quad_pts[2] = p;
        }
        if(p.x >= midX && p.y < midY)
        {
            quad_pts[3] = p;
        }
    }
    //arreglamos el offset de 5 del contorno
    for(cv::Point p : quad_pts)
    {
        p.x -= 5;
        if(p.x < 0)
        {
            p.x = 0;
        }
        p.y -= 5;
        if(p.y < 0)
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
    cv::namedWindow("src",cv::WINDOW_NORMAL);
    cv::namedWindow("corrected",cv::WINDOW_NORMAL);
    cv::imshow("src", src);
    cv::imshow("corrected", corrected);
    cv::waitKey(0);

    //ahora que tenemos la perspectiva corregida, procedemos a identificar circulos

}