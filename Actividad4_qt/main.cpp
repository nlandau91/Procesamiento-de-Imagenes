#include "mainwindow.h"
#include <opencv2/opencv.hpp>

#include <QApplication>
#include <QTimer>
#include <QDebug>

//usamos este struct para pasar variables entre metodos sin utilizar variables globales
typedef struct Userdata
{
    cv::Mat src;
    cv::Mat result;
    char const *src_path = NULL;
    cv::Mat kernel;
    int kSize = 1;
    int colores[3] = {0};
    cv::Mat thresholds[3];
    int pixPerCookie = 0;
    bool changes = false;
    MainWindow w;
} Userdata;

//Realiza una apertura y luego un cierre a fin de eliminar imperfecciones
void procesar(cv::Mat src, cv::Mat &dst, cv::Mat kernel);

//contamos la cantidad de pixeles de algunos colores en la imagen
void contarPixels(cv::Mat src, int dst[3]);

//Realiza una apertura y luego un cierre a fin de eliminar imperfecciones
void procesar(cv::Mat src, cv::Mat &dst, cv::Mat kernel)
{
    //Primero vamos a hacer una apertura (erosion y dilatacion)
    erode(src, dst, kernel);
    dilate(dst, dst, kernel);
    //Luego, un cierre (dilatacion y erocion)
    dilate(dst, dst, kernel);
    erode(dst, dst, kernel);
}

//contamos la cantidad de pixeles de algunos colores en la imagen
void contarPixels(cv::Mat src, int dst_colores[3], cv::Mat dst_thresholds[3])
{
    //la convertimos a hsv, ya que es mas facil diferenciar colores
    cv::Mat img_hsv;
    cv::cvtColor(src, img_hsv, cv::COLOR_BGR2HSV);

    //establecemos los rangos para cada color, dividimos por dos porque el rango va de 0 a 180, no a 360, restamos 1  en el tope superior porque los rangos son cerrados
    int low_H_rojo1 = 0 / 2, high_H_rojo1 = 15 / 2 - 1;
    int low_H_naranja = 28 / 2, high_H_naranja = 36 / 2 - 1;
    int low_H_amarillo = 39 / 2, high_H_amarillo = 55 / 2 - 1;
    int low_H_rojo2 = 355 / 2, high_H_rojo2 = 360 / 2 - 1;

    //establecemos los rangos para el s y el v, esto nos sirve para que los blancos y negros no sean confundidos con algun color
    int low_S = 100, high_S = 220;
    int low_V = 100, high_V = 220;

    //Para cada rango, obtenemos una imagen con solamente los pixels que se encuentran en ese rango
    cv::Mat en_rango_rojo1, en_rango_rojo2, en_rango_rojo, en_rango_naranja, en_rango_amarillo;
    //rojo
    cv::inRange(img_hsv, cv::Scalar(low_H_rojo1, 40, low_V), cv::Scalar(high_H_rojo1, high_S, high_V), en_rango_rojo1);
    cv::inRange(img_hsv, cv::Scalar(low_H_rojo2, 40, low_V), cv::Scalar(high_H_rojo2, high_S, high_V), en_rango_rojo2);
    cv::bitwise_or(en_rango_rojo1,en_rango_rojo2,en_rango_rojo);
    //naranja
    cv::inRange(img_hsv, cv::Scalar(low_H_naranja, low_S, low_V), cv::Scalar(high_H_naranja, high_S, high_V), en_rango_naranja);
    //amarillo
    cv::inRange(img_hsv, cv::Scalar(low_H_amarillo, low_S, low_V), cv::Scalar(high_H_amarillo, high_S, high_V), en_rango_amarillo);

    //obtenemos la cantidad de pixeles que se encuentran dentro de cada rango
    dst_colores[0] = cv::countNonZero(en_rango_rojo);
    dst_colores[1] = cv::countNonZero(en_rango_naranja);
    dst_colores[2] = cv::countNonZero(en_rango_amarillo);

    dst_thresholds[0] = en_rango_rojo;
    dst_thresholds[1] = en_rango_naranja;
    dst_thresholds[2] = en_rango_amarillo;
}

QImage Mat2QImage(cv::Mat const& src)
{
     cv::Mat temp; // make the same cv::Mat
     cv::cvtColor(src, temp,cv::COLOR_BGR2RGB); // cvtColor Makes a copt, that what i need
     QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
     dest.bits(); // enforce deep copy, see documentation
     // of QImage::QImage ( const uchar * data, int width, int height, Format format )
     return dest;
}

void work(Userdata &data, MainWindow &w)
{
    std::cout<< data.kSize << std::endl;
    data.kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(data.kSize,data.kSize));
    procesar(data.src,data.result,data.kernel);
    w.set_amarillo_qimage(Mat2QImage(data.result));
    if(data.kSize < 100)
    {
        data.kSize += 2;
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Userdata data;
    data.src_path = "img.png";
    data.src = cv::imread(data.src_path,cv::IMREAD_UNCHANGED);
    if(data.src.empty())
    {
        std::cout << "No se pudo cargar la imagen en " << data.src_path << std::endl;
        return 0;
    }
    QTimer timer;
        QObject::connect(&timer, &QTimer::timeout, [&data, &w]() { work(data, w); });
        timer.start(500);


    return a.exec();
}
