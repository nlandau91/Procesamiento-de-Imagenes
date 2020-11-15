#include "Cartoonify.hpp"
#include <iostream>
Cartoonify::Cartoonify()
{
    //parametros de los distintos modos
    kSizeMedian = 7;
    quantScale = 24;

    laplacianParameters.kSize = 3;
    laplacianParameters.scale = 5;
    laplacianParameters.delta = 0;
    sobelParameters.kSize = 3;
    sobelParameters.scale = 5;
    sobelParameters.delta = 0;
    cannyParameters.minThresh = 30.0f;
    cannyParameters.ratio = 3;
    cannyParameters.kSize = 3;
    loGParameters.kSizeGauss = 13;
    loGParameters.deltaGauss = 2;
    loGParameters.kSizeLaplace = 3;
    loGParameters.scaleLaplace = 1;
    loGParameters.zeroCrossThreshold = 0.01;
}

cv::Mat Cartoonify::getCartoon(cv::Mat &src, EdgeMode edgemode)
{
    cv::Mat result;
    cv::Mat afterMediantFilter;
    cv::medianBlur(src, afterMediantFilter, kSizeMedian);
    cv::Mat edges;
    if (edgemode == LAPLACIAN)
    {
        edges = edgesLaplace(afterMediantFilter, CV_16S, laplacianParameters.kSize, laplacianParameters.scale, laplacianParameters.delta);
    }
    if (edgemode == SOBEL)
    {
        edges = edgesSobel(afterMediantFilter, CV_16S, sobelParameters.kSize, sobelParameters.scale, sobelParameters.delta);
    }
    if (edgemode == CANNY)
    {
        edges = edgesCanny(afterMediantFilter, cannyParameters.minThresh, cannyParameters.ratio, cannyParameters.kSize);
    }
    if (edgemode == LOG1)
    {
        edges = edgesLoG(afterMediantFilter, 0, loGParameters.kSizeGauss, loGParameters.deltaGauss, loGParameters.kSizeLaplace, loGParameters.scaleLaplace, loGParameters.zeroCrossThreshold);
    }
    if (edgemode == LOG2)
    {
        edges = edgesLoG(afterMediantFilter, 1, loGParameters.kSizeGauss, loGParameters.deltaGauss, loGParameters.kSizeLaplace, loGParameters.scaleLaplace, loGParameters.zeroCrossThreshold);
    }

    //reducimos la cantidad de colores
    cv::Mat requant = afterMediantFilter.clone();
    for (int i = 0; i < requant.rows; i++)
    {
        for (int j = 0; j < requant.cols; j++)
        {
            requant.at<cv::Vec3b>(i, j)[0] = floor(requant.at<cv::Vec3b>(i, j)[0] / quantScale) * quantScale;
            requant.at<cv::Vec3b>(i, j)[1] = floor(requant.at<cv::Vec3b>(i, j)[1] / quantScale) * quantScale;
            requant.at<cv::Vec3b>(i, j)[2] = floor(requant.at<cv::Vec3b>(i, j)[2] / quantScale) * quantScale;
        }
    }

    //sumamos los bordes a la imagen recuantizada
    cv::bitwise_and(requant, requant, result, edges);

    return result;
}

cv::Mat Cartoonify::edgesLaplace(cv::Mat &src, int ddepth, int kSize, int scale, int delta)
{
    //detectamos bordes de la imagen en escala de grises usando Laplacian
    //convertimos la imagen a escala de gris
    cv::Mat grayscale;
    cv::cvtColor(src, grayscale, cv::COLOR_BGR2GRAY);
    //aplicamos el filtro laplaciano
    cv::Mat afterLaplace;
    cv::Laplacian(grayscale, afterLaplace, ddepth, kSize, scale, delta, cv::BORDER_DEFAULT);
    //volvemos a convertir la imagen a escala de gris
    cv::Mat absAfterLaplace;
    cv::convertScaleAbs(afterLaplace, absAfterLaplace);
    //invertimos la imagen para que los bordes sean negros
    cv::Mat edges_grayscale;
    cv::bitwise_not(absAfterLaplace, edges_grayscale);
    //aplicamos la funcion de umbral para que los bordes esten mejor definidos
    cv::Mat edges;
    cv::threshold(edges_grayscale, edges, 150, 255, cv::THRESH_BINARY);
    return edges;
}

cv::Mat Cartoonify::edgesSobel(cv::Mat &src, int ddepth, int ksize, int scale, int delta)
{
    // Convert the image to grayscale
    cv::Mat grayscale;
    cv::cvtColor(src, grayscale, cv::COLOR_BGR2GRAY);
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;
    cv::Sobel(grayscale, grad_x, ddepth, 1, 0, ksize, scale, delta, cv::BORDER_DEFAULT);
    cv::Sobel(grayscale, grad_y, ddepth, 0, 1, ksize, scale, delta, cv::BORDER_DEFAULT);
    // converting back to CV_8U
    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);
    cv::Mat grad;
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
    //invertimos la imagen para que los bordes sean negros
    cv::Mat edges;
    cv::bitwise_not(grad, edges);
    //aplicamos la funcion de umbral para que los bordes esten mejor definidos
    cv::threshold(edges, edges, 150, 255, cv::THRESH_BINARY);
    return edges;
}

cv::Mat Cartoonify::edgesCanny(cv::Mat &src, double t1, int ratio, int kSize)
{
    // Convert the image to grayscale
    cv::Mat grayscale;
    cv::cvtColor(src, grayscale, cv::COLOR_BGR2GRAY);
    cv::Mat edges;
    cv::Canny(grayscale, edges, t1, t1 * ratio, kSize);
    cv::bitwise_not(edges, edges);
    return edges;
}

// Pasamos el rango dinamico de la imagen a [0,1]
void ImageAdjust(cv::Mat &src, cv::Mat &dst)
{
    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            int g = src.at<uchar>(i, j);
            dst.at<float>(i, j) = g / 255.;
        }
    }
}

//encuentra los cruces por cero de una imagen.
//Si la suma de un cuadrante es positiva y la suma de otro cuadrante es negativa, la posicion es un cero
void zeroCross(cv::Mat &src, cv::Mat &result, double threshold)
{
    for (int y = 1; y < src.rows - 1; ++y)
    {
        for (int x = 1; x < src.cols; ++x)
        {
            float q1, q2, q3, q4;
            q1 = src.at<float>(y, x) + src.at<float>(y - 1, x) + src.at<float>(y - 1, x - 1) + src.at<float>(y, x - 1);
            q2 = src.at<float>(y, x) + src.at<float>(y, x - 1) + src.at<float>(y + 1, x - 1) + src.at<float>(y + 1, x);
            q3 = src.at<float>(y, x) + src.at<float>(y + 1, x) + src.at<float>(y + 1, x + 1) + src.at<float>(y, x + 1);
            q4 = src.at<float>(y, x) + src.at<float>(y, x + 1) + src.at<float>(y - 1, x + 1) + src.at<float>(y - 1, x);
            float max = std::max({q1, q2, q3, q4});
            float min = std::min({q1, q2, q3, q4});
            if (max > 0 && min < 0 && (std::abs(max - min) > threshold))
            {
                result.at<uchar>(y, x) = 255;
            }
        }
    }
}

//encuentra los cruces por cero de una imagen. Version 2
//Si al menos un vecino es positivo y un vecino es negativo, el pixel es un cero
void zeroCross2(cv::Mat &src, cv::Mat &result, double threshold)
{
    for (int y = 1; y < src.rows - 1; ++y)
    {
        for (int x = 1; x < src.cols; ++x)
        {
            float p1, p2, p3, p4, p5, p6, p7, p8;
            p1 = src.at<float>(y - 1, x - 1);
            p2 = src.at<float>(y - 1, x);
            p3 = src.at<float>(y - 1, x + 1);
            p4 = src.at<float>(y, x - 1);
            p5 = src.at<float>(y, x + 1);
            p6 = src.at<float>(y + 1, x - 1);
            p7 = src.at<float>(y + 1, x);
            p8 = src.at<float>(y + 1, x + 1);
            float max = std::max({p1, p2, p3, p4, p5, p6, p7, p8});
            float min = std::min({p1, p2, p3, p4, p5, p6, p7, p8});
            if (max > 0 && min < 0 && (std::abs(max - min) > threshold))
            {
                result.at<uchar>(y, x) = 255;
            }
        }
    }
}

cv::Mat Cartoonify::edgesLoG(cv::Mat src, int zeroCrossMode, int gaussKSize, int gaussDelta, int laplaceKSize, int laplaceScaleSize, float zeroCrossThreshold)
{
    //Image is converted to grayscale
    cv::Mat gray(src.size(), CV_32FC1);
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    cv::Mat gray01;
    gray01 = cv::Mat::zeros(gray.size(), CV_32FC1);
    ImageAdjust(gray, gray01);
    //Generate a Gaussian kernel matrix and smooth the image
    cv::Mat gaussianblur;
    cv::GaussianBlur(gray01, gaussianblur, cv::Size(gaussKSize, gaussKSize), gaussDelta);

    // Laplacian transformation of the image after smoothing cv::Mat Laplace;
    cv::Mat Laplace = cv::Mat::zeros(gaussianblur.size(), CV_32FC1);
    cv::Laplacian(gaussianblur, Laplace, CV_32FC1, laplaceKSize, laplaceScaleSize);

    //Neighborhood decision, zero cross search
    cv::Mat result;
    result = cv::Mat::zeros(Laplace.size(), CV_8U);
    if (zeroCrossMode == 0)
    {
        zeroCross(Laplace, result, zeroCrossThreshold);
    }
    if (zeroCrossMode == 1)
    {
        zeroCross2(Laplace, result, zeroCrossThreshold);
    }

    //invertimos la imagen para que los bordes sean negros
    cv::bitwise_not(result, result);

    return result;
}
