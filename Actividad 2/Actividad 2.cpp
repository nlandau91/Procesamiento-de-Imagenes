#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    //cargamos la imagen y la mostramos
    Mat img_orig = imread("img.jpg", IMREAD_COLOR);
    imshow("img_orig", img_orig);
    waitKey(1);

    //la convertimos a hsv, ya que es mas facil diferenciar colores
    Mat img_hsv;
    cvtColor(img_orig, img_hsv, COLOR_BGR2HSV);

    //establecemos los rangos para cada color, dividimos por dos porque el rango va de 0 a 180, no a 360, restamos 1  en el tope superior porque los rangos son cerrados
    int low_H_rojo1 = 0 / 2, high_H_rojo1 = 30 / 2 -1;
    int low_H_amarillo = 30 / 2, high_H_amarillo = 90 / 2 -1;
    int low_H_verde = 90 / 2, high_H_verde = 150 / 2 - 1;
    int low_H_cian = 150 / 2, high_H_cian = 210 / 2 - 1;
    int low_H_azul = 210 / 2, high_H_azul = 270 / 2 - 1;
    int low_H_magenta = 270 / 2, high_H_magenta = 330 / 2 - 1;
    int low_H_rojo2 = 330 / 2, high_H_rojo2 = 360 / 2 - 1;

    //establecemos los rangos para el s y el v, esto nos sirve para que los blancos y negros no sean confundidos con algun color
    int low_S = 20, high_S = 255;
    int low_V = 20, high_V = 255;

    //Para cada rango, obtenemos una imagen con solamente los pixels que se encuentran en ese rango
    Mat en_rango_rojo1, en_rango_amarillo, en_rango_verde, en_rango_cian, en_rango_azul, en_rango_magenta, en_rango_rojo2;
    inRange(img_hsv, Scalar(low_H_rojo1,low_S,low_V), Scalar(high_H_rojo1,high_S,high_V), en_rango_rojo1);
    inRange(img_hsv, Scalar(low_H_amarillo, low_S, low_V), Scalar(high_H_amarillo, high_S, high_V), en_rango_amarillo);
    inRange(img_hsv, Scalar(low_H_verde, low_S, low_V), Scalar(high_H_verde, high_S, high_V), en_rango_verde);
    inRange(img_hsv, Scalar(low_H_cian, low_S, low_V), Scalar(high_H_cian, high_S, high_V), en_rango_cian);
    inRange(img_hsv, Scalar(low_H_azul, low_S, low_V), Scalar(high_H_azul, high_S, high_V), en_rango_azul);
    inRange(img_hsv, Scalar(low_H_magenta,low_S,low_V), Scalar(high_H_magenta, high_S, high_V), en_rango_magenta);
    inRange(img_hsv, Scalar(low_H_rojo2, low_S, low_V), Scalar(high_H_rojo2, high_S, high_V), en_rango_rojo2);

    //obtenemos la cantidad de pixeles que se encuentran dentro de cada rango
    int cant_rojo = countNonZero(en_rango_rojo1) + countNonZero(en_rango_rojo2);
    int cant_amarillo = countNonZero(en_rango_amarillo);
    int cant_verde = countNonZero(en_rango_verde);
    int cant_cian = countNonZero(en_rango_cian);
    int cant_azul = countNonZero(en_rango_azul);
    int cant_magenta = countNonZero(en_rango_magenta);

    //establecemos cuantos pixels ocupa un objeto
    int pixels_objeto;
    cout << "cuantos pixels ocupa un objeto?" << endl;
    cin >> pixels_objeto;

    //calculamos cuantos objetos de cada color hay
    int objetos_rojo = cant_rojo / pixels_objeto;
    int objetos_amarillo = cant_amarillo / pixels_objeto;
    int objetos_verde = cant_verde / pixels_objeto;
    int objetos_cian = cant_cian / pixels_objeto;
    int objetos_azul = cant_azul / pixels_objeto;
    int objetos_magenta = cant_magenta / pixels_objeto;

    //imprimimos por pantalla los resultados
    cout << "Rojo: " << objetos_rojo << endl;
    cout << "Amarillo: " << objetos_amarillo << endl;
    cout << "Verde: " << objetos_verde << endl;
    cout << "Cian: " << objetos_cian << endl;
    cout << "Azul: " << objetos_azul << endl;
    cout << "Magenta: " << objetos_magenta << endl;

    system("pause");
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