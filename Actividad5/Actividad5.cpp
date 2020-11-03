#include <opencv2/opencv.hpp>
#include <iostream>

main()
{
    std::string src_path = "Hyacinthe_rigaud,_ritratto_di_luigi_xiv_di_francia,_1701,_01.jpg";
    cv::Mat src = cv::imread(src_path,cv::IMREAD_UNCHANGED);
    cv::namedWindow("src",cv::WINDOW_AUTOSIZE);
    cv::imshow("src",src);
    cv::waitKey(0);
}