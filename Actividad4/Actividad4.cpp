#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>
#include "../include/tinyfiledialogs/tinyfiledialogs.h"

//usamos este struct para pasar variables entre metodos sin utilizar variables globales
typedef struct userdata
{
    cv::Mat src;
    char const *src_path = NULL;
    cv::Mat kernel;
    cv::Mat thresholds[3];
    int pixPerCookie = 0;
    bool changes = false;
} userdata;

//Realiza una apertura y luego un cierre a fin de eliminar imperfecciones
void procesar(cv::Mat src, cv::Mat &dst, cv::Mat kernel);

//contamos la cantidad de pixeles de algunos colores en la imagen
void contarPixels(cv::Mat src, cv::Mat dst[3]);

//paramos cuando se ingresa q
//lo hacemos en un thread para no bloquear la ejecucion
void checkDone(bool *done)
{
    char c = 0;
    while (!(*done))
    {
        *done = 'q' == (c = std::cin.get());
    }
}

//callbacks de las trackbars
static void trackbar_kSize(int value, void *data)
{
    if (value > 0)
    {
        int kernelSize = 2 * floor(value / 2) + 1; //tiene que ser impar y positivo
        ((userdata *)data)->kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(value, value));
        ((userdata *)data)->changes = true;
    }
}
static void trackbar_pixPerCookie(int value, void *data)
{
    if (value > 0)
    {
        ((userdata *)data)->pixPerCookie = value;
        ((userdata *)data)->changes = true;
    }
}

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
void obtenerThresholds(cv::Mat src, cv::Mat dst[3])
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
    cv::inRange(img_hsv, cv::Scalar(low_H_rojo1, 40, low_V), cv::Scalar(high_H_rojo1, high_S, high_V), en_rango_rojo1);
    cv::inRange(img_hsv, cv::Scalar(low_H_rojo2, 40, low_V), cv::Scalar(high_H_rojo2, high_S, high_V), en_rango_rojo2);
    cv::bitwise_or(en_rango_rojo1,en_rango_rojo2,en_rango_rojo);
    cv::inRange(img_hsv, cv::Scalar(low_H_naranja, low_S, low_V), cv::Scalar(high_H_naranja, high_S, high_V), en_rango_naranja);
    cv::inRange(img_hsv, cv::Scalar(low_H_amarillo, low_S, low_V), cv::Scalar(high_H_amarillo, high_S, high_V), en_rango_amarillo);

    //obtenemos la cantidad de pixeles que se encuentran dentro de cada rango
    dst[0] = en_rango_rojo;
    dst[1] = en_rango_naranja;
    dst[2] = en_rango_amarillo;
}

int main(int argc, char *argv[])
{
    userdata data = userdata();
    //creamos la ventana
    cv::namedWindow("Actividad4", cv::WINDOW_AUTOSIZE);
    //intentamos cargar la imagen img.png por defecto
    data.src = cv::imread("img.png",cv::IMREAD_UNCHANGED);
    //no se encontro la imagen, abrimos un dialogo para buscar una imagen
    if(data.src.empty())
    {
		//cargamos la imagen
		while (!data.src_path)
		{
			data.src_path = tinyfd_openFileDialog("Ingrese una imagen", "", 0, NULL, NULL, 0);
			if (!data.src_path)
			{
				tinyfd_messageBox("Error", "Open file name is NULL", "ok", "error", 0);
			}
			else
			{
				data.src = cv::imread(data.src_path,cv::IMREAD_UNCHANGED);
			}
		}
    }

    //agregamos las trackbars
    int kSize = 1;
    int kSizeSlider = kSize;
    const int kSizeSliderMax = 99;
    cv::createTrackbar("kSize", "Actividad4", &kSizeSlider, kSizeSliderMax, trackbar_kSize, &data);

    data.pixPerCookie = 30000;
    int pixPerCookieSlider = data.pixPerCookie;
    const int pixPerCookieSliderMax = 300000;
    cv::createTrackbar("pixPerCookie", "Actividad4", &pixPerCookieSlider, pixPerCookieSliderMax, trackbar_pixPerCookie, &data);

    //calculamos los thresholds
    obtenerThresholds(data.src, data.thresholds);
    //Procesamos los thresholds utilizando transformaciones morfologicas
    data.kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kSize, kSize));
    procesar(data.thresholds[0],data.thresholds[0],data.kernel);
    procesar(data.thresholds[1],data.thresholds[1],data.kernel);
    procesar(data.thresholds[2],data.thresholds[2],data.kernel);


    //mostramos la imagen original
    cv::imshow("Actividad4",data.src);

    data.changes = true;

    //continuamos la ejecucion hasta que se ingrese 'q'
    std::cout << "Ingrese 'q' para terminar." << std::endl;
    bool done = false;
    std::thread t2(checkDone, &done);
    while (!done)
    {
        if(data.changes)
        {
            //procesamos la imagen y calculamos los colores nuevamente
            obtenerThresholds(data.src,data.thresholds);
            procesar(data.thresholds[0],data.thresholds[0],data.kernel);
            procesar(data.thresholds[1],data.thresholds[1],data.kernel);
            procesar(data.thresholds[2],data.thresholds[2],data.kernel);

            //imprimimos cuantas galletas de cada color hay en la imagen
            //a partir de los pixeles de color contados y de la cantidad de pixeles de una galleta
            std::cout << "Rojo: " << cv::countNonZero(data.thresholds[0]) / data.pixPerCookie << std::endl;
            std::cout << "Naranja: " << cv::countNonZero(data.thresholds[1]) / data.pixPerCookie << std::endl;
            std::cout << "Amarillo: " << cv::countNonZero(data.thresholds[2]) / data.pixPerCookie << std::endl;
            std::cout << "Ingrese 'q' para terminar." << std::endl;
            cv::imshow("Rojo", data.thresholds[0]);
            cv::imshow("Naranja", data.thresholds[1]);
            cv::imshow("Amarillo", data.thresholds[2]);
            data.changes = false;
        }
        cv::waitKey(200);
        //checkeamos si se cierra la ventana
        try
        {
            cv::Rect rect = cv::getWindowImageRect("Actividad4");
        }
        catch (cv::Exception e)
        {
            done == true;
            t2.detach();
            t2.~thread();
            cv::destroyAllWindows();
            return 0;
        }
    }
    t2.join();
    cv::destroyAllWindows();
}
