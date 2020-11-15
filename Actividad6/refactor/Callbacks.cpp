#include "Callbacks.hpp"
void kSizeLaplacianCallback(int value, void *userdata)
{
    if ((value % 2) == 1)
    {
        ((Userdata *)userdata)->cartoonify.laplacianParameters.kSize = value;
        ((Userdata *)userdata)->resultLaplacian = ((Userdata *)userdata)->cartoonify.getCartoon(((Userdata *)userdata)->src, Cartoonify::LAPLACIAN);
        cv::imshow("resultLaplacian", ((Userdata *)userdata)->resultLaplacian);
    }
}

void scaleLaplacianCallback(int value, void *userdata)
{

    ((Userdata *)userdata)->cartoonify.laplacianParameters.scale = value;
    ((Userdata *)userdata)->resultLaplacian = ((Userdata *)userdata)->cartoonify.getCartoon(((Userdata *)userdata)->src, Cartoonify::LAPLACIAN);
    cv::imshow("resultLaplacian", ((Userdata *)userdata)->resultLaplacian);
}
void deltaLaplacianCallback(int value, void *userdata)
{

    ((Userdata *)userdata)->cartoonify.laplacianParameters.delta = value;
    ((Userdata *)userdata)->resultLaplacian = ((Userdata *)userdata)->cartoonify.getCartoon(((Userdata *)userdata)->src, Cartoonify::LAPLACIAN);
    cv::imshow("resultLaplacian", ((Userdata *)userdata)->resultLaplacian);
}
void kSizeSobelCallback(int value, void *userdata)
{
    if ((value % 2) == 1)
    {
        ((Userdata *)userdata)->cartoonify.sobelParameters.kSize = value;
        ((Userdata *)userdata)->resultSobel = ((Userdata *)userdata)->cartoonify.getCartoon(((Userdata *)userdata)->src, Cartoonify::SOBEL);
        cv::imshow("resultSobel", ((Userdata *)userdata)->resultSobel);
    }
}
void scaleSobelCallback(int value, void *userdata)
{
    ((Userdata *)userdata)->cartoonify.sobelParameters.scale = value;
    ((Userdata *)userdata)->resultSobel = ((Userdata *)userdata)->cartoonify.getCartoon(((Userdata *)userdata)->src, Cartoonify::SOBEL);
    cv::imshow("resultSobel", ((Userdata *)userdata)->resultSobel);
}
void deltaSobelCallback(int value, void *userdata)
{
    ((Userdata *)userdata)->cartoonify.sobelParameters.delta = value;
    ((Userdata *)userdata)->resultSobel = ((Userdata *)userdata)->cartoonify.getCartoon(((Userdata *)userdata)->src, Cartoonify::SOBEL);
    cv::imshow("resultSobel", ((Userdata *)userdata)->resultSobel);
}

void kSizeCannyCallback(int value, void *userdata)
{
    if (value >= 3 && value <= 7 && (value % 2) == 1)
    {
        ((Userdata *)userdata)->cartoonify.cannyParameters.kSize = value;
        ((Userdata *)userdata)->resultCanny = ((Userdata *)userdata)->cartoonify.getCartoon(((Userdata *)userdata)->src, Cartoonify::CANNY);
        cv::imshow("resultCanny", ((Userdata *)userdata)->resultCanny);
    }
}
void minThreshCannyCallback(int value, void *userdata)
{
    ((Userdata *)userdata)->cartoonify.cannyParameters.minThresh = value;
    ((Userdata *)userdata)->resultCanny = ((Userdata *)userdata)->cartoonify.getCartoon(((Userdata *)userdata)->src, Cartoonify::CANNY);
    cv::imshow("resultCanny", ((Userdata *)userdata)->resultCanny);
}
void ratioCannyCallback(int value, void *userdata)
{
    ((Userdata *)userdata)->cartoonify.cannyParameters.ratio = value;
    ((Userdata *)userdata)->resultCanny = ((Userdata *)userdata)->cartoonify.getCartoon(((Userdata *)userdata)->src, Cartoonify::CANNY);
    cv::imshow("resultCanny", ((Userdata *)userdata)->resultCanny);
}

void kSizeGaussLoGCallback(int value, void* userdata)
{
    if ((value % 2) == 1)
    {
        ((Userdata *)userdata)->cartoonify.loGParameters.kSizeGauss = value;
        ((Userdata *)userdata)->resultLoG = ((Userdata *)userdata)->cartoonify.getCartoon(((Userdata *)userdata)->src, Cartoonify::LOG1);
        ((Userdata *)userdata)->resultLoG2 = ((Userdata *)userdata)->cartoonify.getCartoon(((Userdata *)userdata)->src, Cartoonify::LOG2);
        cv::imshow("resultLoG", ((Userdata *)userdata)->resultLoG);
        cv::imshow("resultLoG2", ((Userdata *)userdata)->resultLoG2);
    }
}
void deltaGaussLoGCallback(int value, void* userdata)
{       
        ((Userdata *)userdata)->cartoonify.loGParameters.deltaGauss = value;
        ((Userdata *)userdata)->resultLoG = ((Userdata *)userdata)->cartoonify.getCartoon(((Userdata *)userdata)->src, Cartoonify::LOG1);
        ((Userdata *)userdata)->resultLoG2 = ((Userdata *)userdata)->cartoonify.getCartoon(((Userdata *)userdata)->src, Cartoonify::LOG2);
        cv::imshow("resultLoG", ((Userdata *)userdata)->resultLoG);
        cv::imshow("resultLoG2", ((Userdata *)userdata)->resultLoG2); 
}
void kSizeLaplaceLoGCallback(int value, void* userdata)
{
    if ((value % 2) == 1)
    {
        ((Userdata *)userdata)->cartoonify.loGParameters.kSizeLaplace = value;
        ((Userdata *)userdata)->resultLoG = ((Userdata *)userdata)->cartoonify.getCartoon(((Userdata *)userdata)->src, Cartoonify::LOG1);
        ((Userdata *)userdata)->resultLoG2 = ((Userdata *)userdata)->cartoonify.getCartoon(((Userdata *)userdata)->src, Cartoonify::LOG2);
        cv::imshow("resultLoG", ((Userdata *)userdata)->resultLoG);
        cv::imshow("resultLoG2", ((Userdata *)userdata)->resultLoG2);
    }
}
void scaleLaplaceLoGCallback(int value, void* userdata)
{       
        ((Userdata *)userdata)->cartoonify.loGParameters.scaleLaplace = value;
        ((Userdata *)userdata)->resultLoG = ((Userdata *)userdata)->cartoonify.getCartoon(((Userdata *)userdata)->src, Cartoonify::LOG1);
        ((Userdata *)userdata)->resultLoG2 = ((Userdata *)userdata)->cartoonify.getCartoon(((Userdata *)userdata)->src, Cartoonify::LOG2);
        cv::imshow("resultLoG", ((Userdata *)userdata)->resultLoG);
        cv::imshow("resultLoG2", ((Userdata *)userdata)->resultLoG2); 
}
void zeroCrossThreshLoGCallback(int value, void* userdata)
{       
        ((Userdata *)userdata)->cartoonify.loGParameters.zeroCrossThreshold = value/100.0f;
        ((Userdata *)userdata)->resultLoG = ((Userdata *)userdata)->cartoonify.getCartoon(((Userdata *)userdata)->src, Cartoonify::LOG1);
        ((Userdata *)userdata)->resultLoG2 = ((Userdata *)userdata)->cartoonify.getCartoon(((Userdata *)userdata)->src, Cartoonify::LOG2);
        cv::imshow("resultLoG", ((Userdata *)userdata)->resultLoG);
        cv::imshow("resultLoG2", ((Userdata *)userdata)->resultLoG2); 
}