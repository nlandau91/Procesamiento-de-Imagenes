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
	//(2,4) -> (166,48)
 	//(504,5) -> (400,83)
	//(500,331) -> (323,256)
    //(2,329) -> (91,214)
    std::vector<cv::Point2f> pointsSrc = { cv::Point2f(3860,1417), cv::Point2f(1800,1200), cv::Point2f(0,0), cv::Point2f(2060,220) };
    std::vector<cv::Point2f> pointsDst = { cv::Point2f(1000,1086), cv::Point2f(1000,0), cv::Point2f(0,0), cv::Point2f(0,1086) };
    cv::Mat warp_mat = cv::getPerspectiveTransform(pointsSrc,pointsDst);
    cv::Mat perspectiveImage;
    cv::warpPerspective(src,perspectiveImage,warp_mat,cv::Size(1000,1086));
    
    //Escalado simetrico con factor 1/1.1
    double scaleFactor = 1/1.1f;
    cv::Mat restoredImage;
    cv::resize(perspectiveImage,restoredImage,cv::Size(),scaleFactor,scaleFactor);

    //mostramos las imagenes de los pasos
    cv::namedWindow("src",cv::WINDOW_FREERATIO);
    cv::namedWindow("perspectiveImage",cv::WINDOW_FREERATIO);
    cv::namedWindow("restored",cv::WINDOW_FREERATIO);
    cv::imshow("src",src);
    cv::imshow("perspectiveImage",scaledImage);
    cv::imshow("restored",restoredImage);
    cv::imwrite("perspectiveImage.jpg",scaledImage);
    cv::imwrite("restored.jpg",restoredImage);
    cv::waitKey(0);
    

}
