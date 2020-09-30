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
    //establecemos los rangos para cada color, dividimos por dos porque el rango va de 0 a 180, no a 360
    int low_H_rojo1 = 0 / 2, high_H_rojo1 = 30 / 2;
    int low_H_amarillo = 30 / 2, high_H_amarillo = 90 / 2;
    int low_H_verde = 90 / 2, high_H_verde = 150 / 2;
    int low_H_cian = 150 / 2, high_H_cian = 210 / 2;
    int low_H_azul = 210 / 2, high_H_azul = 270 / 2;
    int low_H_magenta = 270 / 2, high_H_magenta = 330 / 2;
    int low_H_rojo2 = 330 / 2, high_H_rojo2 = 360 / 2;
    //obtenemos el canal h de la imagen
    vector<Mat> hsv_channels;
    split(img_hsv, hsv_channels);
    Mat channel_h = hsv_channels[0];
    //Para cada rango, obtenemos una imagen con solamente los pixels que se encuentran en ese rango
    Mat en_rango_rojo1, en_rango_amarillo, en_rango_verde, en_rango_cian, en_rango_azul, en_rango_magenta, en_rango_rojo2;
    inRange(channel_h,low_H_rojo1,high_H_rojo1,en_rango_rojo1);
    inRange(channel_h, low_H_, high_H_, en_rango_);
    inRange(channel_h, low_H_, high_H_, en_rango_);
    inRange(channel_h, low_H_, high_H_, en_rango_);
    inRange(channel_h, low_H_, high_H_, en_rango_);
    inRange(channel_h, low_H_, high_H_, en_rango_);
    inRange(channel_h, low_H_, high_H_, en_rango_);
    inRange(channel_h, low_H_rojo2, high_H_rojo2, en_rango_rojo2);


    system("pause");
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
