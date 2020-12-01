#include <opencv2/opencv.hpp>
#include <iostream>

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
    //la convertimos de bgr a hsv
    cv::Mat src_hsv;
    cv::cvtColor(src, src_hsv, cv::COLOR_BGR2HSV);
    //hacemos threshold en el canal s
    cv::Mat split[3];
    cv::split(src_hsv, split);
    cv::Mat src_hsv_s_threshold;
    cv::threshold(split[1], src_hsv_s_threshold, 50, 255, cv::THRESH_BINARY_INV);

    //encontramos el contorno exterior maximo
    std::vector<cv::Mat> contornos;
    cv::findContours(src_hsv_s_threshold, contornos, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    cv::Mat cont_max;
    double cont_size = 0;
    for (cv::Mat cont : contornos)
    {
        if (cv::contourArea(cont) > cont_size)
        {
            cont_size = cv::contourArea(cont);
            cont_max = cont;
        }
    }
    //corregimos la perspectiva
    //https://stackoverflow.com/questions/22519545/automatic-perspective-correction-opencv/22550609
    std::vector<std::vector<cv::Point>> contours_poly(1);
    cv::Mat aprox;
    cv::approxPolyDP(cont_max, contours_poly[0], 5, true);
    cv::Rect boundRect = cv::boundingRect(cont_max);
    if (contours_poly[0].size() == 4)
    {
        std::vector<cv::Point2f> quad_pts;
        std::vector<cv::Point2f> squre_pts;
        quad_pts.push_back(cv::Point2f(contours_poly[0][0].x, contours_poly[0][0].y));
        quad_pts.push_back(cv::Point2f(contours_poly[0][1].x, contours_poly[0][1].y));
        quad_pts.push_back(cv::Point2f(contours_poly[0][3].x, contours_poly[0][3].y));
        quad_pts.push_back(cv::Point2f(contours_poly[0][2].x, contours_poly[0][2].y));
        squre_pts.push_back(cv::Point2f(boundRect.x, boundRect.y));
        squre_pts.push_back(cv::Point2f(boundRect.x, boundRect.y + boundRect.height));
        squre_pts.push_back(cv::Point2f(boundRect.x + boundRect.width, boundRect.y));
        squre_pts.push_back(cv::Point2f(boundRect.x + boundRect.width, boundRect.y + boundRect.height));

        cv::Mat transmtx = cv::getPerspectiveTransform(quad_pts, squre_pts);
        cv::Mat transformed = cv::Mat::zeros(src.rows, src.cols, CV_8UC3);
        cv::warpPerspective(src, transformed, transmtx, src.size());
        cv::Point P1 = contours_poly[0][0];
        cv::Point P2 = contours_poly[0][1];
        cv::Point P3 = contours_poly[0][2];
        cv::Point P4 = contours_poly[0][3];

        cv::line(src, P1, P2, cv::Scalar(0, 0, 255), 1, cv::LINE_AA, 0);
        cv::line(src, P2, P3, cv::Scalar(0, 0, 255), 1, cv::LINE_AA, 0);
        cv::line(src, P3, P4, cv::Scalar(0, 0, 255), 1, cv::LINE_AA, 0);
        cv::line(src, P4, P1, cv::Scalar(0, 0, 255), 1, cv::LINE_AA, 0);
        cv::rectangle(src, boundRect, cv::Scalar(0, 255, 0), 1, 8, 0);
        cv::rectangle(transformed, boundRect, cv::Scalar(0, 255, 0), 1, 8, 0);

        cv::imshow("quadrilateral", transformed);
        cv::imshow("src", src);
        cv::waitKey();
    }
    else
    {

        std::cout << "Make sure that your are getting 4 corner using approxPolyDP..." <<  std::endl;
    }
}