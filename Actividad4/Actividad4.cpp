#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>
#include "../include/tinyfiledialogs/tinyfiledialogs.h"

//usamos este struct para pasar variables entre metodos sin utilizar variables globales
typedef struct userdata
{
    cv::Mat src;
    cv::Mat result;
    char const * src_path = NULL;
    bool newLoad = false;
    int colores[3] = {0};
} userdata;

//paramos cuando se ingresa q
//lo hacemos en un thread para no bloquear la ejecucion
void checkDone(bool * done)
{
    char c = 0;
    while(!done)
    {
        *done = 'q' == (c = std::cin.get());
    }
}

void btn_callback(int event, int x, int y, int flags, void* data)
{
    if(event == cv::EVENT_LBUTTONDOWN)
    {
        (*userdata)data
        src_img = tinyfd_openFileDialog("Ingrese una imagen","",0,NULL,NULL,0);
        if (! src_img)
	    {
		    tinyfd_messageBox("Error","Open file name is NULL","ok","error",0);
	    }
	    else
	    {
            src = cv::imread(src_img,cv::IMREAD_UNCHANGED);
            newLoad = true;
	    }
    }
}

//Realiza una apertura y luego un cierre a fin de eliminar imperfecciones
void procesar(cv::Mat src, cv::Mat dst, cv::Mat kernel)
{
    //Primero vamos a hacer una apertura (erosion y dilatacion)
    erode(src,result,kernel);
    dilate(result,result,kernel);
    //Luego, un cierre (dilatacion y erocion)
    dilate(result,result,kernel);
    erode(result,result,kernel);
}

//contamos la cantidad de pixeles de algunos colores en la imagen
void contarPixels(cv::Mat src, int dst[3])
{
    //la convertimos a hsv, ya que es mas facil diferenciar colores
	cv::Mat img_hsv;
	cv::cvtColor(src, img_hsv, cv::COLOR_BGR2HSV);

	//establecemos los rangos para cada color, dividimos por dos porque el rango va de 0 a 180, no a 360, restamos 1  en el tope superior porque los rangos son cerrados
	int low_H_rojo1 = 0 / 2, high_H_rojo1 = 15 / 2 - 1;
	int low_H_naranja = 25 / 2, high_H_naranja = 36 / 2 - 1;
	int low_H_amarillo = 39 / 2, high_H_amarillo = 55 / 2 - 1;
	int low_H_rojo2 = 355 / 2, high_H_rojo2 = 360 / 2 - 1;

	//establecemos los rangos para el s y el v, esto nos sirve para que los blancos y negros no sean confundidos con algun color
	int low_S = 100, high_S = 200;
	int low_V = 100, high_V = 200;

	//Para cada rango, obtenemos una imagen con solamente los pixels que se encuentran en ese rango
	cv::Mat en_rango_rojo1, en_rango_naranja, en_rango_amarillo, en_rango_rojo2;
	cv::inRange(img_hsv, cv::Scalar(low_H_rojo1, 40, low_V), cv::Scalar(high_H_rojo1, high_S, high_V), en_rango_rojo1);
	cv::inRange(img_hsv, cv::Scalar(low_H_naranja, low_S, low_V), cv::Scalar(high_H_naranja, high_S, high_V), en_rango_naranja);
	cv::inRange(img_hsv, cv::Scalar(low_H_amarillo, low_S, low_V), cv::Scalar(high_H_amarillo, high_S, high_V), en_rango_amarillo);
	cv::inRange(img_hsv, cv::Scalar(low_H_rojo2, 40, low_V), cv::Scalar(high_H_rojo2, high_S, high_V), en_rango_rojo2);

	//obtenemos la cantidad de pixeles que se encuentran dentro de cada rango
	dst[0] = cv::countNonZero(en_rango_rojo1) + countNonZero(en_rango_rojo2);
	dst[1] = cv::countNonZero(en_rango_naranja);
	dst[2] = cv::countNonZero(en_rango_amarillo);
}

//callbacks de las trackbars
static void trackbar_kSize( int value, void* )
{
    if(value > 0)
    {
        int kernelSize = 2*floor(value/2)+1; //tiene que ser impar y positivo
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(value,value));
        procesar(src,result,kernel);
        contarPixels(result, colores);
        showImages();
    }
}
static void trackbar_pixPerCookie( int value, void* userdata)
{
    if(value > 0)
    {
        //pixPerCookie
        *((int*)(userdata)) = value;
    }

}

int main(int argc, char * argv[])
{
    userdata Data = userdata();
    //creamos la ventana de las trackbars y el boton de carga
    cv::namedWindow("trackbars", cv::WINDOW_FREERATIO);   
    
    //creamos el boton de carga
    cv::Mat button;
    button = cv::imread("../res/btn_load.png",cv::IMREAD_UNCHANGED);
    cv::setMouseCallback("trackbars",btn_callback);
    cv::imshow("trackbars",button);
    
    while(!src_img)
    {
        cv::waitKey(200);
    }

    //agregamos las trackbars
    int kSize = 5;
    int kSizeSlider = kSize;
    const int kSizeSliderMax = 99;
    cv::createTrackbar( "kSize", "trackbars", &kSizeSlider, kSizeSliderMax, trackbar_kSize);

    int pixPerCookie = 30000;
    int pixPerCookieSlider = pixPerCookie;
    const int pixPerCookieSliderMax = 300000;
    cv::createTrackbar( "pixPerCookie", "trackbars", &pixPerCookieSlider, pixPerCookieSliderMax,trackbar_pixPerCookie, &pixPerCookie);
   
    //creamos las ventanas de las imagenes
    cv::namedWindow("src",cv::WINDOW_NORMAL);
    cv::namedWindow("result",cv::WINDOW_NORMAL);

    //Procesamos la imagen utilizando transformaciones morfologicas
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(kSize,kSize));
    procesar(src,result,kernel);

    //contamos los pixeles de color en la imagen
    contarPixels(result, colores);

    showImages();
    //continuamos la ejecucion hasta que se ingrese 'q'
    std::cout << "Ingrese 'q' para terminar." << std::endl;
    bool done = false;
    std::thread t2(checkDone, &done);
    while(!done)
    {
        if(newLoad)
        {
            showImages();
            newLoad = false;
        }
        std::cout << "Rojo: " << colores[0]/pixPerCookie << std::endl;
        std::cout << "Naranja: " << colores[1]/pixPerCookie << std::endl;
        std::cout << "Amarillo: " << colores[2]/pixPerCookie << std::endl;

        cv::waitKey(200);
        //checkeamos si se cierra la ventana de las trackbars
        try
        {
            cv::Rect rect = cv::getWindowImageRect("trackbars");
        } catch ( cv::Exception e )
        {
            done == true;
            t2.join();
            cv::destroyAllWindows();
            return 0;
        }
        

    }
    t2.join();
    cv::destroyAllWindows();
}
