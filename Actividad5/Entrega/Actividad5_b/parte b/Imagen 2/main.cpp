#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>


std::vector<cv::Point2f> pointsSrc;
std::vector<cv::Point2f> pointsDst;

//callback para guardar las posiciones de los clicks sobre la imagen
//los primeros 4 clicks corresponden a la fuente, los ultimos 4 clicks al destino
void srcPoints(int event, int x, int y, int flags, void* userdata)
{
    if (event == cv::EVENT_LBUTTONDOWN and pointsSrc.size() < 4)
    {
        std::cout << "click - position (" << x << ", " << y << ")" << std::endl;
        pointsSrc.push_back(cv::Point2f(x, y));
    }
    else if (event == cv::EVENT_LBUTTONDOWN) {
        std::cout << "click - destination (" << x << ", " << y << ")" << std::endl;
        pointsDst.push_back(cv::Point2f(x, y));
    }
}
  

main()
{   
    //cargamos y mostramos la imagen
    std::string src_path = "result.jpg";
    cv::Mat src = cv::imread(src_path,cv::IMREAD_UNCHANGED);
    if (src.empty())
    {
        std::cout << "Error loading the image" << std::endl;
        return -1;
    }
    
    //escalamos la imagen
    cv::Mat scaled;
    double scaleFactor = 1/0.75;
    cv::resize(src,scaled,cv::Size(435, 600),scaleFactor,scaleFactor);
    
    //hardcodeamos los puntos
    pointsDst = { cv::Point2f(86,120), cv::Point2f(360,120), cv::Point2f(380,540), cv::Point2f(60,540) };
    pointsSrc = { cv::Point2f(86,120), cv::Point2f(360,120), cv::Point2f(360,540), cv::Point2f(86,540) };


    //calculamos la matriz de perspectiva a partir de los 4 pares de puntos 
    cv::Mat perspectiveMatrix = cv::getPerspectiveTransform(pointsSrc, pointsDst);
    //aplicamos la transformacion a la imagen
    cv::Mat restored;
    cv::warpPerspective(scaled, restored, perspectiveMatrix, cv::Size(435, 600));

    //mostramos el resultado
    cv::namedWindow("src",cv::WINDOW_FREERATIO);
    cv::imshow("src",src);
    cv::namedWindow("restored",cv::WINDOW_FREERATIO);
    cv::imshow("restored", restored);
    
    //guardamos las imagenes);
    cv::imwrite("restored.jpg", restored);
    
    cv::waitKey(0);
}
