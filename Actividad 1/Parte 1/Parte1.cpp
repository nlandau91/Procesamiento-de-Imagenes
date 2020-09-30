#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

//calcula y muestra el histograma de la imagen en escala de gris
void show_histogram(string const& name, Mat1b const& image)
{
	// Set histogram bins count
	int bins = 256;
	int histSize[] = { bins };
	// Set ranges for histogram bins
	float lranges[] = { 0, 256 };
	const float* ranges[] = { lranges };
	// create matrix for histogram
	Mat hist;
	int channels[] = { 0 };

	// create matrix for histogram visualization
	int const hist_height = 256;
	Mat3b hist_image = cv::Mat3b::zeros(hist_height, bins);

	//usamos la funcion de libreria calcHist para calcular el histograma
	calcHist(&image, 1, channels, Mat(), hist, 1, histSize, ranges, true, false);

	double max_val = 0;
	minMaxLoc(hist, 0, &max_val);

	// visualize each bin
	for (int b = 0; b < bins; b++)
	{
		float const binVal = hist.at<float>(b);
		int   const height = cvRound((long long)binVal * hist_height / max_val);
		cv::line
		(hist_image
			, Point(b, hist_height - height), Point(b, hist_height)
			, Scalar::all(255)
		);
	}
	imshow(name, hist_image);
}

//calcula el histograma a mano y lo muestra
void show_histogram2(string const& name, Mat1b const& image)
{
	int bins = 256;
	int rows = image.rows;
	int cols = image.cols;
	//vamos a tener 256 separaciones, para cada una guardamos un int que representa cuantas veces se encuentra en la imagen
	int hist[256] = { 0 };
	double max_val = 0;
	//recorremos la imagen y calculamos la intensidad en cada pixel
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			int val = (int)image.at<uchar>(i, j);
			hist[val]++;
			//buscamos el valor mayor, para graficar luego
			if (hist[val] > max_val)
			{
				max_val = hist[val];
			}
		}	
	}

	int const hist_height = 256;
	Mat3b hist_image = cv::Mat3b::zeros(hist_height, bins);

	// visualize each bin
	for (int b = 0; b < bins; b++)
	{
		float const binVal = hist[b];
		//casteamos a 8 bytes para no causar overflow
		int   const height = cvRound((long long)binVal * hist_height / max_val);
		cv::line
		(hist_image
			, Point(b, hist_height - height), Point(b, hist_height)
			, Scalar::all(255)
		);
	}
	imshow(name, hist_image);
}
int main()
{
	//cargamos la imagen
	Mat3b const src_color = imread("src1.png", IMREAD_COLOR);
	//la pasamos a grayscale
	Mat1b src_gray;
	cvtColor(src_color, src_gray, cv::COLOR_BGR2GRAY);
	//mostramos la imagen
	imshow("src_gray", src_gray);
	//mostramos el histograma
	show_histogram("src_gray hist", src_gray);
	show_histogram2("src_gray hist2", src_gray);
	waitKey(500);
	cout << "done" << endl;
	//esperamos a que se presione una tecla
	cout << "Presione enter para continuar" << endl;
	std::cin.clear();
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	cin.get();
	destroyAllWindows();
	return 0;
}
