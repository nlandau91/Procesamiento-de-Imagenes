#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
    string src_img = "car.jpg";
	// Check the number of parameters
	if (argc < 2) {
		std::cout << "No se ingreso un parametro, usando nombre de imagen por defecto 'car.jpg'" << endl;
	}
	else
	{
		src_img = argv[1];
	}
    //cargamos la imagen
    Mat src = imread(src_img, IMREAD_COLOR);
    imshow("orig",src);
    waitKey(200);
    
    //(1) aplicamos filtro de la mediana
    Mat afterMedianFilter;
    int kSize = 5;
    medianBlur(src,afterMedianFilter,kSize);
    imshow("afterMedianFilter",afterMedianFilter);
    waitKey(200);
    
    //(2) detectamos bordes de la imagen en escala de grises usando Laplacian
    Mat grayscale;
    cvtColor(src,grayscale,COLOR_BGR2GRAY);
    Mat afterLaplace;
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;
    Laplacian(grayscale,afterLaplace,ddepth,kSize,scale,delta,BORDER_DEFAULT);
    Mat absAfterLaplace;
    convertScaleAbs(afterLaplace,absAfterLaplace);
    Mat edges;
    bitwise_not(absAfterLaplace, edges);
    imshow("edges",edges);
    waitKey(200);
    
    //(3) reducimos la cantidad de colores en (1)
    Mat requant = afterMedianFilter.clone();
    int quantScale = 24;
    for(int i = 0; i < requant.rows; i++)
    {
        for(int j = 0; j < requant.cols; j++)
        {
           requant.at<Vec3b>(i,j)[0] = floor(requant.at<Vec3b>(i,j)[0] / quantScale) * quantScale;
           requant.at<Vec3b>(i,j)[1] = floor(requant.at<Vec3b>(i,j)[1] / quantScale) * quantScale;
           requant.at<Vec3b>(i,j)[2] = floor(requant.at<Vec3b>(i,j)[2] / quantScale) * quantScale;
        }
    }
    imshow("requant",requant);
    waitKey(200);
    
    //(4) agregamos a (3) los bordes obtenidos en (2)
    Mat result;
    bitwise_and(requant,requant,result,edges);
    imshow("result",result);
    waitKey(200);
    
    //done
    cout << "Presione enter para continuar" << endl;
	std::cin.clear();
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	cin.get();
}
