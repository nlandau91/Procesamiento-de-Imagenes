#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    //cargamos la imagen y la mostramos
    Mat img_orig = imread("4.png", IMREAD_COLOR);
    imshow("img_orig", img_orig);
    waitKey(500);

    //la convertimos a hsv, ya que es mas facil diferenciar colores
    Mat img_hsv;
    cvtColor(img_orig, img_hsv, COLOR_BGR2HSV);

    //establecemos los rangos para cada color, dividimos por dos porque el rango va de 0 a 180, no a 360, restamos 1  en el tope superior porque los rangos son cerrados
    int low_H_rojo1 = 0 / 2, high_H_rojo1 = 25 / 2 - 1;
    int low_H_naranja = 25 / 2, high_H_naranja = 36 / 2 - 1;
    int low_H_amarillo = 36 / 2, high_H_amarillo = 60 / 2 - 1;
    int low_H_rojo2 = 345 / 2, high_H_rojo2 = 360 / 2 - 1;

    //establecemos los rangos para el s y el v, esto nos sirve para que los blancos y negros no sean confundidos con algun color
    int low_S = 60, high_S = 255;
    int low_V = 60, high_V = 255;

    //Para cada rango, obtenemos una imagen con solamente los pixels que se encuentran en ese rango
    Mat en_rango_rojo1, en_rango_naranja, en_rango_amarillo, en_rango_rojo2;
    inRange(img_hsv, Scalar(low_H_rojo1, low_S, low_V), Scalar(high_H_rojo1, high_S, high_V), en_rango_rojo1);
    inRange(img_hsv, Scalar(low_H_naranja, low_S, low_V), Scalar(high_H_naranja, high_S, high_V), en_rango_naranja);
    inRange(img_hsv, Scalar(low_H_amarillo, low_S, low_V), Scalar(high_H_amarillo, high_S, high_V), en_rango_amarillo);
    inRange(img_hsv, Scalar(low_H_rojo2, low_S, low_V), Scalar(high_H_rojo2, high_S, high_V), en_rango_rojo2);

    //obtenemos la cantidad de pixeles que se encuentran dentro de cada rango
    int cant_rojo = countNonZero(en_rango_rojo1) + countNonZero(en_rango_rojo2);
    int cant_naranja = countNonZero(en_rango_naranja);
    int cant_amarillo = countNonZero(en_rango_amarillo);

    //establecemos cuantos pixels ocupa un objeto
    int pixels_objeto;
    cout << "cuantos pixels ocupa un objeto?" << endl;
    cin >> pixels_objeto;

    //calculamos cuantos objetos de cada color hay
    int objetos_rojo = (cant_rojo*1.1) / pixels_objeto;
    int objetos_naranja = (cant_naranja*0.9) / pixels_objeto;
    int objetos_amarillo = (cant_amarillo*1.1) / pixels_objeto;

    //imprimimos por pantalla los resultados
    cout << "Rojo: " << objetos_rojo << endl;
    cout << "Naranja: " << objetos_naranja << endl;
    cout << "Amarillo: " << objetos_amarillo << endl;

    //system("pause");
    cout << "Presione enter para continuar" << endl;
    std::cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.get();
    destroyAllWindows();
}

//rangos de hue
//h0 rojo
//
//h60 amarillo
//
//h120 verde
//
//h180 cian
//
//h240 azul
//
//h300 magenta