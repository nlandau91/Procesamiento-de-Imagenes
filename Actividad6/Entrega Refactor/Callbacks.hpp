#ifndef CALLBACKS_H
#define CALLBACKS_H
#include <opencv2/opencv.hpp>
#include "Cartoonify.hpp"
struct Userdata
{
    cv::Mat src, resultLaplacian, resultSobel, resultCanny, resultLoG, resultLoG2;
    Cartoonify cartoonify;
};

void kSizeLaplacianCallback(int value, void* userdata);
void scaleLaplacianCallback(int value, void* userdata);
void deltaLaplacianCallback(int value, void* userdata);

void kSizeSobelCallback(int value, void* userdata);
void scaleSobelCallback(int value, void* userdata);
void deltaSobelCallback(int value, void* userdata);

void kSizeCannyCallback(int value, void* userdata);
void minThreshCannyCallback(int value, void* userdata);
void ratioCannyCallback(int value, void* userdata);

void kSizeGaussLoGCallback(int value, void* userdata);
void deltaGaussLoGCallback(int value, void* userdata);
void kSizeLaplaceLoGCallback(int value, void* userdata);
void scaleLaplaceLoGCallback(int value, void* userdata);
void zeroCrossThreshLoGCallback(int value, void* userdata);
#endif