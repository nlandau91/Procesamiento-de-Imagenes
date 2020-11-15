#include <iostream>
#include <opencv2/opencv.hpp>
#include "Cartoonify.hpp"
#include "Callbacks.hpp"




int main(int argc, char *argv[])
{
    char const *src_path = "car.jpg";
    if (argc > 1)
    {
        src_path = argv[1];
    }
    Userdata data;
    data.src = cv::imread(src_path, cv::IMREAD_UNCHANGED);
    if (data.src.empty())
    {
        std::cout << "No se pudo cargar la imagen" << std::endl;
        return -1;
    }

    data.cartoonify = Cartoonify();
    data.resultLaplacian = data.cartoonify.getCartoon(data.src, Cartoonify::LAPLACIAN);
    data.resultSobel = data.cartoonify.getCartoon(data.src, Cartoonify::SOBEL);
    data.resultCanny = data.cartoonify.getCartoon(data.src, Cartoonify::CANNY);
    data.resultLoG = data.cartoonify.getCartoon(data.src, Cartoonify::LOG1);
    data.resultLoG2 = data.cartoonify.getCartoon(data.src, Cartoonify::LOG2);
    //creamos las ventanas de las imagenes
    cv::namedWindow("original", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("resultLaplacian", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("resultSobel", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("resultCanny", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("resultLoG", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("resultLoG2", cv::WINDOW_AUTOSIZE);

    //creamos trackbars y las agregamos
    int kSizeLaplace = data.cartoonify.laplacianParameters.kSize;
    cv::createTrackbar("kSize","resultLaplacian",&kSizeLaplace,30,kSizeLaplacianCallback,&data);
    int scaleLaplace = data.cartoonify.laplacianParameters.scale;
    cv::createTrackbar("scale","resultLaplacian",&scaleLaplace,30,scaleLaplacianCallback,&data);
    int deltaLaplace = data.cartoonify.laplacianParameters.delta;
    cv::createTrackbar("delta","resultLaplacian",&deltaLaplace,30,deltaLaplacianCallback,&data);

    int kSizeSobel = data.cartoonify.sobelParameters.kSize;
    cv::createTrackbar("kSize","resultSobel",&kSizeSobel,30,kSizeSobelCallback,&data);
    int scaleSobel = data.cartoonify.sobelParameters.scale;
    cv::createTrackbar("scale","resultSobel",&scaleSobel,30,scaleSobelCallback,&data);
    int deltaSobel = data.cartoonify.sobelParameters.delta;
    cv::createTrackbar("delta","resultSobel",&deltaSobel,30,deltaSobelCallback,&data);

    int ratioCanny = data.cartoonify.cannyParameters.ratio;
    cv::createTrackbar("ratio","resultCanny",&ratioCanny,30,ratioCannyCallback,&data);
    int minThreshCanny = data.cartoonify.cannyParameters.minThresh;
    cv::createTrackbar("minThresh","resultCanny",&minThreshCanny,100,minThreshCannyCallback,&data);
    int kSizeCanny = data.cartoonify.cannyParameters.kSize;
    cv::createTrackbar("kSize","resultCanny",&kSizeCanny,7,kSizeCannyCallback,&data);

    int kSizeGauss = data.cartoonify.loGParameters.kSizeGauss;
    cv::createTrackbar("kSizeGauss","resultLoG",&kSizeGauss,30,kSizeGaussLoGCallback,&data);
    int deltaGauss = data.cartoonify.loGParameters.deltaGauss;
    cv::createTrackbar("deltaGauss","resultLoG",&deltaGauss,30,deltaGaussLoGCallback,&data);
    int kSizeLaplaceLoG = data.cartoonify.loGParameters.kSizeLaplace;
    cv::createTrackbar("kSizeLaplace","resultLoG",&kSizeLaplaceLoG,30,kSizeLaplaceLoGCallback,&data);

    int scaleLaplaceLoG = data.cartoonify.loGParameters.scaleLaplace;
    cv::createTrackbar("scaleLaplace","resultLoG",&scaleLaplaceLoG,30,scaleLaplaceLoGCallback,&data);
    int zeroCrossThresh = 100 * data.cartoonify.loGParameters.zeroCrossThreshold;
    cv::createTrackbar("zeroCrossThresh * 100","resultLoG",&zeroCrossThresh,100,zeroCrossThreshLoGCallback,&data);



    //mostramos las imagenes
    cv::imshow("original", data.src);
    cv::imshow("resultLaplacian", data.resultLaplacian);
    cv::imshow("resultSobel", data.resultSobel);
    cv::imshow("resultCanny", data.resultCanny);
    cv::imshow("resultLoG", data.resultLoG);
    cv::imshow("resultLoG2", data.resultLoG2);



    cv::waitKey(0);

    cv::destroyAllWindows();
}
