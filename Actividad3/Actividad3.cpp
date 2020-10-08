#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    //cargamos la imagen
    Mat src = imread("buzz.jpg", IMREAD_COLOR);
    imshow("orig",src);
    waitKey(200);
    //(1) aplicamos filtro de la mediana
    
    //(2) detectamos bordes
    
    //(3) reducimos la cantidad de colores en (1)
    
    //(4) agregamos a (3) los bordes obtenidos en (2)
    
    //done
    cout << "Presione enter para continuar" << endl;
	std::cin.clear();
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	cin.get();
}
