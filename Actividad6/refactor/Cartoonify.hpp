#ifndef CARTOONIFY_H
#define CARTOONIFY_H
#include <opencv2/opencv.hpp>

class Cartoonify
{
public:
    enum EdgeMode
    {
        LAPLACIAN,
        SOBEL,
        CANNY,
        LOG1,
        LOG2
    };

    struct LaplacianParameters
    {
        int kSize;
        int scale;
        int delta;
    } laplacianParameters;

    struct SobelParameters
    {
        int kSize;
        int scale;
        int delta;
    } sobelParameters;

    struct CannyParameters
    {
        double minThresh;
        int ratio;
        int kSize;
    } cannyParameters;

    struct LoGParameters
    {
        int kSizeGauss;
        int deltaGauss;
        int kSizeLaplace;
        int scaleLaplace;
        float zeroCrossThreshold;
    } loGParameters;

    int kSizeMedian;
    int quantScale;

    Cartoonify();
    cv::Mat getCartoon(cv::Mat &src, EdgeMode edgemode);

private:
    
    cv::Mat edgesLaplace(cv::Mat &src, int ddepth, int kSize, int scale, int delta);
    cv::Mat edgesSobel(cv::Mat &src, int ddepth, int ksize, int scale, int delta);
    cv::Mat edgesCanny(cv::Mat &src, double t1, int ratio, int kSize);
    cv::Mat edgesLoG(cv::Mat src, int zeroCrossMode, int gaussKSize, int gaussDelta, int laplaceKSize, int laplaceScaleSize, float zeroCrossThreshold);
};

#endif