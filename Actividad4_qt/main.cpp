#include "mainwindow.h"
#include <opencv2/opencv.hpp>

#include <QApplication>

//usamos este struct para pasar variables entre metodos sin utilizar variables globales
typedef struct userdata
{
    cv::Mat src;
    cv::Mat result;
    char const *src_path = NULL;
    cv::Mat kernel;
    int colores[3] = {0};
    cv::Mat thresholds[3];
    int pixPerCookie = 0;
    bool changes = false;
} userdata;

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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    userdata data = userdata();
    data.pixPerCookie = 30000;
    int kSize = 3;
    data.kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kSize,kSize));

    //intentamos cargar la imagen img.png por defecto
    data.src = cv::imread("img.png",cv::IMREAD_UNCHANGED);
    //no se encontro la imagen, abrimos un dialogo para buscar una imagen
    if(data.src.empty())
    {
        std::cout << "Error al cargar la imagen" << std::endl;
        return 0;
    }
    procesar(data.src,data.result,data.kernel);
    //contamos los pixeles de color en la imagen
    contarPixels(data.result, data.colores, data.thresholds);

    data.changes = true;

    //continuamos la ejecucion hasta que se ingrese 'q'
    std::cout << "Ingrese 'q' para terminar." << std::endl;
    bool done = false;
    while (!done)
    {
        if(data.changes)
        {
            //procesamos la imagen y calculamos los colores nuevamente
            procesar(data.src,data.result,data.kernel);
            contarPixels(data.result,data.colores,data.thresholds);

            //imprimimos cuantas galletas de cada color hay en la imagen
            //a partir de los pixeles de color contados y de la cantidad de pixeles de una galleta
            std::cout << "Rojo: " << data.colores[0] / data.pixPerCookie << std::endl;
            std::cout << "Naranja: " << data.colores[1] / data.pixPerCookie << std::endl;
            std::cout << "Amarillo: " << data.colores[2] / data.pixPerCookie << std::endl;
            std::cout << "Ingrese 'q' para terminar." << std::endl;
            cv::Mat converted;
            cv::cvtColor(data.result,converted,cv::COLOR_BGR2RGB);
            w.set_result_pixmap(QPixmap::fromImage(QImage(converted.data, converted.cols, converted.rows, converted.step, QImage::Format_RGB888)));
            //data.changes = false;
        }
        //cv::waitKey(200);
        w.show();
    }

    return a.exec();
}
