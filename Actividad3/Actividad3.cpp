#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>
#include "../include/tinyfiledialogs/tinyfiledialogs.h"

//algunas variables globales, para el control de los sliders
const int kSizeMedian_slider_max = 100;
const int kSizeLaplacian_slider_max = 31;
const int quantScale_slider_max = 255;
const int scaleLaplacian_slider_max = 100;
const int deltaLaplacian_slider_max = 100;
int kSizeMedian_slider;
int kSizeLaplacian_slider;
int scaleLaplacian_slider;
int deltaLaplacian_slider;
int quantScale_slider;
cv::Mat src;
cv::Mat afterMedianFilter;
cv::Mat edges;
cv::Mat requant;
cv::Mat result;
int kSizeMedian = 7;
int kSizeLaplacian = 3;
int ddepth = CV_16S;
int scaleLaplacian = 5;
int deltaLaplacian = 0;
int quantScale = 24;
char const * src_img = NULL;
bool newLoad = false;

void showImages()
{
    cv::imshow("original",src);
    cv::imshow("afterMedianFilter", afterMedianFilter);
    cv::imshow("edges",edges);
    cv::imshow("requant",requant);
    cv::imshow("result",result);
}

void paso1()
{
    //(1) aplicamos filtro de la mediana
    cv::medianBlur(src,afterMedianFilter,kSizeMedian); 
}

void paso2()
{
    //(2) detectamos bordes de la imagen en escala de grises usando Laplacian
    //convertimos la imagen a escala de gris
    cv::Mat grayscale;
    cv::cvtColor(afterMedianFilter,grayscale,cv::COLOR_BGR2GRAY);
    //aplicamos el filtro laplaciano
    cv::Mat afterLaplace;
    cv::Laplacian(grayscale,afterLaplace,ddepth,kSizeLaplacian,scaleLaplacian,deltaLaplacian,cv::BORDER_DEFAULT);
    //volvemos a convertir la imagen a escala de gris
    cv::Mat absAfterLaplace;
    cv::convertScaleAbs(afterLaplace,absAfterLaplace);
    //invertimos la imagen para que los bordes sean negros
    cv::Mat edges_grayscale;
    cv::bitwise_not(absAfterLaplace, edges_grayscale);
    //aplicamos la funcion de umbral para que los bordes esten mejor definidos
    cv::threshold(edges_grayscale, edges, 150, 255, cv::THRESH_BINARY);
}

void paso3()
{
    //(3) reducimos la cantidad de colores en (1)
    requant = afterMedianFilter.clone();
    for(int i = 0; i < requant.rows; i++)
    {
        for(int j = 0; j < requant.cols; j++)
        {
           requant.at<cv::Vec3b>(i,j)[0] = floor(requant.at<cv::Vec3b>(i,j)[0] / quantScale) * quantScale;
           requant.at<cv::Vec3b>(i,j)[1] = floor(requant.at<cv::Vec3b>(i,j)[1] / quantScale) * quantScale;
           requant.at<cv::Vec3b>(i,j)[2] = floor(requant.at<cv::Vec3b>(i,j)[2] / quantScale) * quantScale;
        }
    }
}

void paso4()
{
    //(4) agregamos a (3) los bordes obtenidos en (2)
    result = cv::Mat::zeros(result.size(),result.type());
    cv::bitwise_and(requant,requant,result,edges);
}


//callbacks de las trackbars
//en todas las callbacks lo que se hace es actualizar el parametro y volver a llamar a los pasos
static void trackbar_kSizeLaplacian( int, void* )
{
    kSizeLaplacian = 2*floor(kSizeLaplacian_slider/2)+1; //tiene que ser impar y positivo
    paso2();
    paso4();
    //showImages();
}

static void trackbar_scaleLaplacian( int, void* )
{
    scaleLaplacian = scaleLaplacian_slider; //tiene que ser impar y positivo
    paso2();
    paso4();
    //showImages();
}

static void trackbar_deltaLaplacian( int, void* )
{
    deltaLaplacian = deltaLaplacian_slider; //tiene que ser impar y positivo
    paso2();
    paso4();
    //showImages();
}

static void trackbar_quantScale( int, void* )
{
    if(quantScale_slider > 0)
    {
        quantScale = quantScale_slider;
        paso3();
        paso4();
        //showImages();
    }
}

static void trackbar_kSizeMedian( int, void* )
{
    kSizeMedian = 2*floor(kSizeMedian_slider/2)+1; //tiene que ser impar y >1
    paso1();
    paso2();
    paso3();
    paso4();
    //showImages();
}

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

void btn_callback(int event, int x, int y, int flags, void* userdata)
{
    if(event == cv::EVENT_LBUTTONDOWN)
    {
        src_img = tinyfd_openFileDialog(
		"Ingrese una imagen",
		"",
		0,
		NULL,
		NULL,
		0);
        if (! src_img)
	    {
		    tinyfd_messageBox(
			    "Error",
			    "Open file name is NULL",
			    "ok",
			    "error",
			    0);
	    }
	    else
	    {
	    src = cv::imread(src_img,cv::IMREAD_UNCHANGED);
	    newLoad = true;
	    }
    }
}

int main(int argc, char * argv[])
{

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
    kSizeMedian_slider = kSizeMedian;
    cv::createTrackbar( "kSizeMedian", "trackbars", &kSizeMedian_slider, kSizeMedian_slider_max, trackbar_kSizeMedian);
    kSizeLaplacian_slider = kSizeLaplacian;
    cv::createTrackbar( "kSizeLaplace", "trackbars", &kSizeLaplacian_slider, kSizeLaplacian_slider_max, trackbar_kSizeLaplacian);
    scaleLaplacian_slider = scaleLaplacian;
    cv::createTrackbar( "scaleLaplace", "trackbars", &scaleLaplacian_slider, scaleLaplacian_slider_max, trackbar_scaleLaplacian);
    deltaLaplacian_slider = deltaLaplacian;
    cv::createTrackbar( "deltaLaplace", "trackbars", &deltaLaplacian_slider, deltaLaplacian_slider_max, trackbar_deltaLaplacian);
    quantScale_slider = quantScale;
    cv::createTrackbar( "quantScale", "trackbars", &quantScale_slider, quantScale_slider_max, trackbar_quantScale);
    //(1) aplicamos filtro de la mediana
    paso1();    
    
    //(2) detectamos bordes de la imagen en escala de grises usando Laplacian
    paso2();
    
    //(3) reducimos la cantidad de colores en (1)
    paso3();
    
    //(4) agregamos a (3) los bordes obtenidos en (2)
    paso4();
    
    //creamos las ventanas de las imagenes
    cv::namedWindow("original",cv::WINDOW_NORMAL);
    cv::namedWindow("afterMedianFilter",cv::WINDOW_NORMAL);
    cv::namedWindow("edges",cv::WINDOW_NORMAL);
    cv::namedWindow("requant",cv::WINDOW_NORMAL);
    cv::namedWindow("result",cv::WINDOW_NORMAL);
   
    //mostramos las imagenes
    showImages();

    //continuamos la ejecucion hasta que se ingrese 'q'
    std::cout << "Ingrese 'q' para terminar." << std::endl;
    bool done = false;
    std::thread t2(checkDone, &done);
    while(!done)
    {
        if(newLoad)
        {
            paso1();
            paso2();
            paso3();
            paso4();
            newLoad = false;
        }
        showImages();
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
