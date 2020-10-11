#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>

using namespace std;
using namespace cv;

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
Mat src;
Mat afterMedianFilter;
Mat edges;
Mat requant;
Mat result;
int kSizeMedian = 7;
int kSizeLaplacian = 3;
int ddepth = CV_16S;
int scaleLaplacian = 5;
int deltaLaplacian = 0;
int quantScale = 24;

void showImages()
{
    imshow("original",src);
    imshow("afterMedianFilter", afterMedianFilter);
    imshow("edges",edges);
    imshow("requant",requant);
    imshow("result",result);
}

void paso1()
{
    //(1) aplicamos filtro de la mediana
    medianBlur(src,afterMedianFilter,kSizeMedian); 
}

void paso2()
{
    //(2) detectamos bordes de la imagen en escala de grises usando Laplacian
    //convertimos la imagen a escala de gris
    Mat grayscale;
    cvtColor(afterMedianFilter,grayscale,COLOR_BGR2GRAY);
    //aplicamos el filtro laplaciano
    Mat afterLaplace;
    Laplacian(grayscale,afterLaplace,ddepth,kSizeLaplacian,scaleLaplacian,deltaLaplacian,BORDER_DEFAULT);
    //volvemos a convertir la imagen a escala de gris
    Mat absAfterLaplace;
    convertScaleAbs(afterLaplace,absAfterLaplace);
    //invertimos la imagen para que los bordes sean negros
    Mat edges_grayscale;
    bitwise_not(absAfterLaplace, edges_grayscale);
    //aplicamos la funcion de umbral para que los bordes esten mejor definidos
    threshold(edges_grayscale, edges, 150, 255, THRESH_BINARY);
}

void paso3()
{
    //(3) reducimos la cantidad de colores en (1)
    requant = afterMedianFilter.clone();
    for(int i = 0; i < requant.rows; i++)
    {
        for(int j = 0; j < requant.cols; j++)
        {
           requant.at<Vec3b>(i,j)[0] = floor(requant.at<Vec3b>(i,j)[0] / quantScale) * quantScale;
           requant.at<Vec3b>(i,j)[1] = floor(requant.at<Vec3b>(i,j)[1] / quantScale) * quantScale;
           requant.at<Vec3b>(i,j)[2] = floor(requant.at<Vec3b>(i,j)[2] / quantScale) * quantScale;
        }
    }
}

void paso4()
{
    //(4) agregamos a (3) los bordes obtenidos en (2)
    result = Mat::zeros(result.size(),result.type());
    bitwise_and(requant,requant,result,edges);
}

//callbacks de las trackbars
//en todas las callbacks lo que se hace es actualizar el parametro y volver a llamar a los pasos
static void trackbar_kSizeLaplacian( int, void* )
{
    kSizeLaplacian = 2*floor(kSizeLaplacian_slider/2)+1; //tiene que ser impar y positivo
    paso2();
    paso4();
    showImages();
}

static void trackbar_scaleLaplacian( int, void* )
{
    scaleLaplacian = scaleLaplacian_slider; //tiene que ser impar y positivo
    paso2();
    paso4();
    showImages();
}

static void trackbar_deltaLaplacian( int, void* )
{
    deltaLaplacian = deltaLaplacian_slider; //tiene que ser impar y positivo
    paso2();
    paso4();
    showImages();
}

static void trackbar_quantScale( int, void* )
{
    if(quantScale_slider > 0)
    {
        quantScale = quantScale_slider;
        paso3();
        paso4();
        showImages();
    }
}

static void trackbar_kSizeMedian( int, void* )
{
    kSizeMedian = 2*floor(kSizeMedian_slider/2)+1; //tiene que ser impar y >1
    paso1();
    paso2();
    paso3();
    paso4();
    showImages();
}

//paramos cuando se ingresa q
//lo hacemos en un thread para no bloquear la ejecucion
void checkDone(bool * done)
{
    char c = 0;
    while( c != 'q' )
    {
        c = cin.get();
    }
    *done = true;
}

int main(int argc, char * argv[])
{
    
    string src_img = "car.jpg";
    // Check the number of parameters
    if (argc < 2) {
        std::cout << "No se ingreso un parametro, usando nombre de imagen por defecto 'car.jpg'" << endl;
    }
    else
    {
        src_img = argv[1];
    }
    //cargamos la imagen
    src = imread(src_img, IMREAD_COLOR);
    if( src.empty() ) { cout << "Error loading src \n"; return -1; }
    
    //(1) aplicamos filtro de la mediana
    paso1();    
    
    //(2) detectamos bordes de la imagen en escala de grises usando Laplacian
    paso2();
    
    //(3) reducimos la cantidad de colores en (1)
    paso3();
    
    //(4) agregamos a (3) los bordes obtenidos en (2)
    paso4();
    
    //creamos las ventanas
    namedWindow("original",WINDOW_AUTOSIZE);
    namedWindow("afterMedianFilter",WINDOW_AUTOSIZE);
    namedWindow("edges",WINDOW_AUTOSIZE);
    namedWindow("requant",WINDOW_AUTOSIZE);
    namedWindow("result",WINDOW_AUTOSIZE);
    namedWindow("trackbars", WINDOW_FREERATIO);
    
    //agregamos las trackbars
    kSizeMedian_slider = kSizeMedian;
    createTrackbar( "kSizeMedian", "trackbars", &kSizeMedian_slider, kSizeMedian_slider_max, trackbar_kSizeMedian);
    kSizeLaplacian_slider = kSizeLaplacian;
    createTrackbar( "kSizeLaplace", "trackbars", &kSizeLaplacian_slider, kSizeLaplacian_slider_max, trackbar_kSizeLaplacian);
    scaleLaplacian_slider = scaleLaplacian;
    createTrackbar( "scaleLaplace", "trackbars", &scaleLaplacian_slider, scaleLaplacian_slider_max, trackbar_scaleLaplacian);
    deltaLaplacian_slider = deltaLaplacian;
    createTrackbar( "deltaLaplace", "trackbars", &deltaLaplacian_slider, deltaLaplacian_slider_max, trackbar_deltaLaplacian);
    quantScale_slider = quantScale;
    createTrackbar( "quantScale", "trackbars", &quantScale_slider, quantScale_slider_max, trackbar_quantScale);

    //mostramos las imagenes
    showImages();

    //continuamos la ejecucion hasta que se ingrese 'q'
    cout << "Ingrese 'q' para terminar." << endl;
    bool done = false;
    thread t1(checkDone, &done);
    while(!done)
    {
        waitKey(200);
    }
    t1.join();
    destroyAllWindows();
}
