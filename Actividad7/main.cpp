#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
    //leemos la imagen
    char const *src_path = "img2.png";
    if(argc > 1)
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
    cv::cvtColor(src,src_hsv,cv::COLOR_BGR2HSV);
    //hacemos threshold en el canal s
    cv::Mat split[3]; 
    cv::split(src_hsv, split);
    cv::Mat src_hsv_s_threshold;
    cv::threshold(split[1],src_hsv_s_threshold,50,255,cv::THRESH_BINARY_INV);

    //encontramos el contorno exterior maximo
    std::vector<cv::Mat> contornos;
    cv::findContours(src_hsv_s_threshold, contornos, cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
    cv::Mat cont_max;
    double cont_size = 0;
    for(cv::Mat cont : contornos)
    {
        if(cv::contourArea(cont) > cont_size)
        {
            cont_size = cv::contourArea(cont);
            cont_max = cont;
        }
    }
    //corregimos la perspectiva
    //https://stackoverflow.com/questions/22519545/automatic-perspective-correction-opencv/22550609
    std::vector<std::vector<cv::Point> > contours_poly(1);
    cv::Mat aprox;
    cv::approxPolyDP(cont_max,contours_poly[0],5,true);

}