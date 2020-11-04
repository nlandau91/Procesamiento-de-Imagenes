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
    //cargamos y mostramos la imagen original
    std::string src_path = "Hyacinthe_rigaud,_ritratto_di_luigi_xiv_di_francia,_1701,_01.jpg";
    cv::Mat src = cv::imread(src_path,cv::IMREAD_UNCHANGED);
    if (src.empty())
    {
        std::cout << "Error loading the image" << std::endl;
        return -1;
    }
    cv::namedWindow("src",cv::WINDOW_AUTOSIZE);

    //esperamos a que el usuario elija los pares de puntos
    //cv::setMouseCallback("src", srcPoints, NULL);
    cv::imshow("src",src);
    //cv::waitKey(0);
    
    //hardcodeamos los puntos
    pointsSrc = { cv::Point2f(86,120), cv::Point2f(360,120), cv::Point2f(380,540), cv::Point2f(60,540) };
    pointsDst = { cv::Point2f(86,120), cv::Point2f(360,120), cv::Point2f(360,540), cv::Point2f(86,540) };

    std::cout << "Points Src: " << std::endl;
    for (auto& i : pointsSrc) {
        std::cout << "( " << i.x << ", " << i.y << " )" << std::endl;
    }
    std::cout << "Points Dst: " << std::endl;
    for (auto& i : pointsDst) {
        std::cout << "( " << i.x << ", " << i.y << " )" << std::endl;
    }

    //calculamos la matriz de perspectiva a partir de los 4 pares de puntos 
    cv::Mat perspectiveMatrix = cv::getPerspectiveTransform(pointsSrc, pointsDst);
    //aplicamos la transformacion a la imagen
    cv::Mat perspective;
    cv::warpPerspective(src, perspective, perspectiveMatrix, src.size());

    //escalamos la imagen
    cv::Mat result;
    cv::resize(perspective,result,cv::Size(),0.75,0.75);

    //mostramos el resultado
    cv::namedWindow("perspective",cv::WINDOW_AUTOSIZE);
    cv::imshow("perspective", perspective);
    cv::namedWindow("result",cv::WINDOW_AUTOSIZE);
    cv::imshow("result", result);
    
    //guardamos las imagenes
    cv::imwrite("perspective.jpg",perspective);
    cv::imwrite("result.jpg", result);
    
    cv::waitKey(0);
}
