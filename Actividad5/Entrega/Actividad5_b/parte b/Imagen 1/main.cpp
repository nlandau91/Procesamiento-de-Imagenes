#include <opencv2/opencv.hpp>
#include <iostream>

main()
{
    //cargamos y mostramos la imagen modificada
    std::string src_path = "modificada.jpg";
    cv::Mat src = cv::imread(src_path,cv::IMREAD_UNCHANGED);
    if (src.empty())
    {
        std::cout << "Error loading the image" << std::endl;
        return -1;
    }
    
    //empleamos los pasos inversos para restaurar la imagen
    //Traslación 50 píxeles a la izquierda, 20 píxeles arriba.
    int tx = -50;
    int ty = -20;
    cv::Mat translationMatrix = (cv::Mat_<double>(2, 3) << 1, 0, tx, 0, 1, ty);
    cv::Mat translatedImage;
    cv::warpAffine(src,translatedImage,translationMatrix,src.size());

    //Rotación 90° en sentido antihorario.
    cv::Mat rotatedImage;
    cv::rotate(translatedImage,rotatedImage,cv::ROTATE_90_COUNTERCLOCKWISE);

    //Afín con los siguientes mapeos de puntos:
	//- (1,3)   → (0,0)
 	//- (0,10)  → (0,10)
	//- (11,13) → (10,10)
    std::vector<cv::Point2f> pointsSrc = { cv::Point2f(1,3), cv::Point2f(0,10), cv::Point2f(11,13) };
    std::vector<cv::Point2f> pointsDst = { cv::Point2f(0,0), cv::Point2f(0,10), cv::Point2f(10,10) };
    cv::Mat warp_mat = cv::getAffineTransform(pointsSrc,pointsDst);
    cv::Mat restoredImage;
    cv::warpAffine(rotatedImage,restoredImage,warp_mat,rotatedImage.size());

    //mostramos las imagenes de los pasos
    cv::namedWindow("src",cv::WINDOW_AUTOSIZE);
    cv::namedWindow("restored",cv::WINDOW_AUTOSIZE);
    cv::imshow("src",src);;
    cv::imshow("restored",restoredImage);
    cv::imwrite("restored.jpg",restoredImage);
    cv::waitKey(0);

    

}
