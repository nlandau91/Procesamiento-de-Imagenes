#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>
#include "../include/tinyfiledialogs/tinyfiledialogs.h"
#include <vector>

cv::Mat medianFilter(cv::Mat &src, int kSize)
{
    //(1) aplicamos filtro de la mediana
    cv::Mat afterMedianFilter;
    cv::medianBlur(src,afterMedianFilter,kSize);
    return afterMedianFilter;
}

cv::Mat gaussianFilter(cv::Mat &src,int kSize)
{
    cv::Mat afterGaussianFilter;
    cv::GaussianBlur(src,afterGaussianFilter,cv::Size(kSize,kSize),0);
    return afterGaussianFilter;
}

cv::Mat edgesLaplace(cv::Mat &src,int ddepth, int kSize, int scale, int delta)
{
    //(2) detectamos bordes de la imagen en escala de grises usando Laplacian
    //convertimos la imagen a escala de gris
    cv::Mat grayscale;
    cv::cvtColor(src,grayscale,cv::COLOR_BGR2GRAY);
    //aplicamos el filtro laplaciano
    cv::Mat afterLaplace;
    cv::Laplacian(grayscale,afterLaplace,ddepth,kSize,scale,delta,cv::BORDER_DEFAULT);
    //volvemos a convertir la imagen a escala de gris
    cv::Mat absAfterLaplace;
    cv::convertScaleAbs(afterLaplace,absAfterLaplace);
    //invertimos la imagen para que los bordes sean negros
    cv::Mat edges_grayscale;
    cv::bitwise_not(absAfterLaplace, edges_grayscale);
    //aplicamos la funcion de umbral para que los bordes esten mejor definidos
    cv::Mat edges;
    cv::threshold(edges_grayscale, edges, 150, 255, cv::THRESH_BINARY);
    return edges;
}

cv::Mat edgesSobel(cv::Mat &src,int ddepth, int ksize, int scale, int delta)
{
    // Convert the image to grayscale
    cv::Mat grayscale;
    cv::cvtColor(src,grayscale,cv::COLOR_BGR2GRAY);
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

cv::Mat edgesCanny(cv::Mat &src,double t1, int ratio, int kSize)
{
    // Convert the image to grayscale
    cv::Mat grayscale;
    cv::cvtColor(src,grayscale,cv::COLOR_BGR2GRAY);
    cv::Mat edges;
    cv::Canny(grayscale,edges, t1, t1*ratio, kSize);
    cv::bitwise_not(edges,edges);
    return edges;
}

cv::Mat requantize(cv::Mat &src, int quantScale)
{
    //(3) reducimos la cantidad de colores en (1)
    cv::Mat requant = src.clone();
    for(int i = 0; i < requant.rows; i++)
    {
        for(int j = 0; j < requant.cols; j++)
        {
           requant.at<cv::Vec3b>(i,j)[0] = floor(requant.at<cv::Vec3b>(i,j)[0] / quantScale) * quantScale;
           requant.at<cv::Vec3b>(i,j)[1] = floor(requant.at<cv::Vec3b>(i,j)[1] / quantScale) * quantScale;
           requant.at<cv::Vec3b>(i,j)[2] = floor(requant.at<cv::Vec3b>(i,j)[2] / quantScale) * quantScale;
        }
    }
    return requant;
}

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

cv::Mat zeroCross(cv::Mat &src)
{
    cv::Mat edgeMap = cv::Mat::zeros(src.rows,src.cols,CV_8UC1);
    int rows = src.rows;  
    int cols = src.cols;

    for (int x = 0; x < rows; x++)      
          for (int y = 0; y < cols; y++)        
          {             
               // Ignore borders            
          if (x + 1 >= src.rows || x - 1 < 0 || y + 1 >= src.cols || y - 1 < 0) continue;

          if ((sgn(src.at<int>(x, y)) != sgn(src.at<int>(x + 1, y)))) 
                       edgeMap.at<uchar>(x, y) = 255;                         
          if ((sgn(src.at<int>(x, y)) != sgn(src.at<int>(x, y + 1)))) 
                       edgeMap.at<uchar>(x, y) = 255;       
          }

    return edgeMap;
}

cv::Mat addEdges(cv::Mat &src, cv::Mat &edges)
{
    //(4) agregamos a (3) los bordes obtenidos en (2)
    cv::Mat result;
    cv::bitwise_and(src,src,result,edges);
    return result;
}

int main(int argc, char * argv[])
{
    char const * src_path = "car.jpg";
    cv::Mat src = cv::imread(src_path,cv::IMREAD_UNCHANGED);
    if(src.empty())
    {
        std::cout << "No se pudo cargar la imagen" << std::endl;
        return -1;
    }

    //(1.a) aplicamos filtro de la mediana
    int kSizeMedian = 7;
    cv::Mat afterMediantFilter = medianFilter(src,kSizeMedian);

    //(1.b) aplicamos el filtro gaussiano
    int kSizeGaussian = 7;
    cv::Mat afterGaussianFilter = gaussianFilter(src,kSizeGaussian);
    
    //(2.a) detectamos bordes de la imagen en escala de grises usando Laplacian
    int ddepth = CV_16S;
    int kSizeLaplacian = 3;
    int scaleLaplacian = 5;
    int deltaLaplacian = 0;
    cv::Mat edgesAfterLaplace = edgesLaplace(afterMediantFilter,ddepth,kSizeLaplacian,scaleLaplacian,deltaLaplacian);
    cv::Mat LoG = edgesLaplace(afterGaussianFilter,ddepth,kSizeLaplacian,scaleLaplacian,deltaLaplacian);
    cv::Mat edgesLoG = zeroCross(LoG);


    //(2.b) detectamos bordes de la imagen en escala de grises usando sobel
    int kSizeSobel = 3;
    int scaleSobel = 5;
    int deltaSobel = 0;
    cv::Mat edgesAfterSobel = edgesSobel(afterMediantFilter,ddepth,kSizeSobel,scaleSobel,deltaSobel);

    //(2.c) detectamos bordes de la imagen en escala de grises usando canny
    double cannyMinThresh = 10.0f;
    int cannyRatio = 3;
    int kSizeCanny = 3;
    cv::Mat edgesAfterCanny = edgesCanny(afterMediantFilter,cannyMinThresh,cannyRatio,kSizeCanny);
    
    //(3) reducimos la cantidad de colores en (1)
    int quantScale = 24;
    cv::Mat afterRequant = requantize(afterMediantFilter,quantScale);
    
    //(4) agregamos a (3) los bordes obtenidos en (2)
    cv::Mat resultLaplacian = addEdges(afterRequant, edgesAfterLaplace);
    cv::Mat resultSobel = addEdges(afterRequant, edgesAfterSobel);
    cv::Mat resultCanny = addEdges(afterRequant, edgesAfterCanny);
    cv::Mat resultLoG = addEdges(afterRequant,edgesLoG);
    
    //creamos las ventanas de las imagenes
    cv::namedWindow("original",cv::WINDOW_AUTOSIZE);
    cv::namedWindow("resultLaplacian",cv::WINDOW_AUTOSIZE);
    cv::namedWindow("resultSobel",cv::WINDOW_AUTOSIZE);
    cv::namedWindow("resultCanny",cv::WINDOW_AUTOSIZE);
    cv::namedWindow("resultLoG",cv::WINDOW_AUTOSIZE);

    //mostramos las imagenes
    cv::imshow("original",src);
    cv::imshow("resultLaplacian",resultLaplacian);
    cv::imshow("resultSobel",resultSobel);
    cv::imshow("resultCanny",resultCanny);
    cv::imshow("resultLoG",resultLoG);
    cv::waitKey(0);
    
    cv::destroyAllWindows();
}
