#include <iostream>
#include <opencv2/opencv.hpp>
#include "Cartoonify.hpp"

int main(int argc, char *argv[])
{
    char const *src_path = "car.jpg";
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

    Cartoonify cartoonify = Cartoonify();
    cv::Mat resultLaplacian = cartoonify.getCartoon(src, Cartoonify::LAPLACIAN);
    cv::Mat resultSobel = cartoonify.getCartoon(src, Cartoonify::SOBEL);
    cv::Mat resultCanny = cartoonify.getCartoon(src, Cartoonify::CANNY);
    cv::Mat resultLoG = cartoonify.getCartoon(src, Cartoonify::LOG1);
    cv::Mat resultLoG2 = cartoonify.getCartoon(src, Cartoonify::LOG2);
    //creamos las ventanas de las imagenes
    cv::namedWindow("original", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("resultLaplacian", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("resultSobel", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("resultCanny", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("resultLoG", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("resultLoG2", cv::WINDOW_AUTOSIZE);

    //mostramos las imagenes
    cv::imshow("original", src);
    cv::imshow("resultLaplacian", resultLaplacian);
    cv::imshow("resultSobel", resultSobel);
    cv::imshow("resultCanny", resultCanny);
    cv::imshow("resultLoG", resultLoG);
    cv::imshow("resultLoG2", resultLoG2);

    cv::waitKey(0);

    cv::destroyAllWindows();
}
