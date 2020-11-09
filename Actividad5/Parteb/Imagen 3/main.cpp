#include <opencv2/opencv.hpp>
#include <iostream>

main()
{
    //cargamos y mostramos la imagen modificada
    std::string src_path = "perspective.jpg";
    cv::Mat src = cv::imread(src_path,cv::IMREAD_UNCHANGED);
    if (src.empty())
    {
        std::cout << "Error loading the image" << std::endl;
        return -1;
    }
    
    //empleamos los pasos inversos para restaurar la imagen
    //Escalado simetrico con factor 1/1.1
    double scaleFactor = 1/1.1f;
    std::cout << scaleFactor << std::endl;
    cv::Mat scaledImage;
    cv::resize(src,scaledImage,cv::Size(),scaleFactor,scaleFactor);

    //Perspectiva con los siguientes mapeos de puntos:
	//- (3860,1417) -> (1000,1086)
 	//- (1800,1200)  → (1000,0)
	//- (0,0) → (0,0)
    //- (2060,220) -> (0,1086)
    std::vector<cv::Point2f> pointsSrc = { cv::Point2f(3860,1417), cv::Point2f(1800,1200), cv::Point2f(0,0), cv::Point2f(2060,220) };
    std::vector<cv::Point2f> pointsDst = { cv::Point2f(1000,1086), cv::Point2f(1000,0), cv::Point2f(0,0), cv::Point2f(0,1086) };
    cv::Mat warp_mat = cv::getPerspectiveTransform(pointsSrc,pointsDst);
    cv::Mat restoredImage;
    cv::warpPerspective(scaledImage,restoredImage,warp_mat,cv::Size(1000,1086));

    //mostramos las imagenes de los pasos
    cv::namedWindow("src",cv::WINDOW_FREERATIO);
    cv::namedWindow("scaled",cv::WINDOW_FREERATIO);
    cv::namedWindow("restored",cv::WINDOW_FREERATIO);
    cv::imshow("src",src);
    cv::imshow("scaled",scaledImage);
    cv::imshow("restored",restoredImage);
    cv::imwrite("scaled.jpg",scaledImage);
    cv::imwrite("restored.jpg",restoredImage);
    cv::waitKey(0);
    

}