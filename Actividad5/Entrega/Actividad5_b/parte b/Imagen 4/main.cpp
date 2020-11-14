#include <opencv2/opencv.hpp>
#include <iostream>

main()
{
    //cargamos y mostramos la imagen modificada
    std::string src_path = "destino.jpg";
    cv::Mat src = cv::imread(src_path,cv::IMREAD_UNCHANGED);
    if (src.empty())
    {
        std::cout << "Error loading the image" << std::endl;
        return -1;
    }
    
    //empleamos los pasos inversos para restaurar la imagen

    //Perspectiva con los siguientes mapeos de puntos:
	//(0,0) -> (166,48)
 	//(256,0) -> (400,83)
	//(256,170) -> (325,256)
    //(0,170) -> (85,220)
    std::vector<cv::Point2f> pointsSrc = { cv::Point2f(0,0), cv::Point2f(256,0), cv::Point2f(256,170), cv::Point2f(0,170) };
    std::vector<cv::Point2f> pointsDst = { cv::Point2f(166,48), cv::Point2f(400,83), cv::Point2f(325,256), cv::Point2f(85,220) };
    cv::Mat warp_mat = cv::getPerspectiveTransform(pointsSrc,pointsDst);
    cv::Mat restoredImage;
    cv::warpPerspective(src,restoredImage,warp_mat,cv::Size(512,341));


    //mostramos las imagenes de los pasos
    cv::namedWindow("src",cv::WINDOW_FREERATIO);;
    cv::namedWindow("restored",cv::WINDOW_FREERATIO);
    cv::imshow("src",src);
    cv::imshow("restored",restoredImage);
    cv::imwrite("restored.jpg",restoredImage);
    cv::waitKey(0);
    

}
