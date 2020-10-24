#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>
#include "../include/tinyfiledialogs/tinyfiledialogs.h"

//algunas variables globales, para el control de los sliders
int kSize = 5;
const int kSizeSliderMax = 99;
int kSizeSlider;
cv::Mat src;
cv::Mat threshold;
cv::Mat result;
char const * src_img = NULL;
bool newLoad = false;

void showImages()
{
    cv::imshow("src",src);
    cv::imshow("result",result);
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

void procesar(cv::Mat src, cv::Mat dst, cv::Mat kernel)
{
    //Primero vamos a hacer una apertura (erosion y dilatacion)
    erode(src,result,kernel);
    dilate(result,result,kernel);
    //Luego, un cierre (dilatacion y erocion)
    dilate(result,result,kernel);
    erode(result,result,kernel);
}

//callbacks de las trackbars
static void trackbar_kSize( int, void* )
{
    kSize = 2*floor(kSizeSlider/2)+1; //tiene que ser impar y positivo
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(kSize,kSize));
    procesar(src,result,kernel);
    showImages();
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
    kSizeSlider = kSize;
    cv::createTrackbar( "kSize", "trackbars", &kSizeSlider, kSizeSliderMax, trackbar_kSize);
   
    //creamos las ventanas de las imagenes
    cv::namedWindow("src",cv::WINDOW_NORMAL);
    cv::namedWindow("result",cv::WINDOW_NORMAL);
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(kSize,kSize));
    procesar(src,result,kernel);

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
