#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main()
{
	//cargamos la imagen en bgr
	Mat3b const src_color = imread("src2.jpg", IMREAD_COLOR);
	//mostramos la imagen
	imshow("parte 2 Imagen Original", src_color);
	waitKey(500);
	//convertimos la imagen a hsv y cielab
	Mat3b src_hsv;
	Mat3b src_lab;
	cvtColor(src_color,src_hsv,COLOR_BGR2HSV);
	cvtColor(src_color, src_lab, COLOR_BGR2Lab);
	//armamos los arreglos con la decomposicion de la imagen
	vector<Mat> splits[3];
	split(src_color, splits[0]);
	split(src_hsv, splits[1]);
	split(src_lab, splits[2]);
	//entrada del usuario
	int opc;
	char c;
	cout << " Elija una opcion" << endl;
	cout << "1) Mostrar canales bgr" << endl;
	cout << "2) Mostrar canales hsv" << endl;
	cout << "3) Mostrar canales lab" << endl;
	cout << "4) Recomponer imagen a partir de canales individuales" << endl;
	cout << "5) Salir" << endl;
	cin >> opc;
	while (opc != 5)
	{
		//seleccionamos que espacio de color mostrar a partir de la entrada del usuario
		if (opc > 0 && opc < 4)
		{	
			//mostramos los canales de cada conversion
			imshow("channel 0", splits[opc - 1][0]);
			imshow("channel 1", splits[opc - 1][1]);
			imshow("channel 2", splits[opc - 1][2]);
			waitKey(500);
			cout << "Presione enter para continuar" << endl;
			std::cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cin.get();
			destroyWindow("channel 0");
			destroyWindow("channel 1");
			destroyWindow("channel 2");
		}
		// armamos una imagen a partir de los canales que quiera el usuario
		if (opc == 4)
		{
			int espacio;
			int canal0;
			int canal1;
			int canal2;
			cout << "Elija que espacio de color utilizar" << endl;
			cout << "1) bgr" << endl;
			cout << "2) hsv" << endl;
			cout << "3) lab" << endl;
			cin >> espacio;
			cout << "Elija que canal poner en el canal 0 de la imagen a componer" << endl;
			cout << "1) Canal 0 bgr" << endl;
			cout << "2) Canal 1 bgr" << endl;
			cout << "3) Canal 2 bgr" << endl;
			cout << "4) Canal 0 hsv" << endl;
			cout << "5) Canal 1 hsv" << endl;
			cout << "6) Canal 2 hsv" << endl;
			cout << "7) Canal 0 lab" << endl;
			cout << "8) Canal 1 lab" << endl;
			cout << "9) Canal 2 lab" << endl;
			cin >> canal0;
			cout << "Elija que canal poner en el canal 1 de la imagen a componer" << endl;
			cout << "1) Canal 0 bgr" << endl;
			cout << "2) Canal 1 bgr" << endl;
			cout << "3) Canal 2 bgr" << endl;
			cout << "4) Canal 0 hsv" << endl;
			cout << "5) Canal 1 hsv" << endl;
			cout << "6) Canal 2 hsv" << endl;
			cout << "7) Canal 0 lab" << endl;
			cout << "8) Canal 1 lab" << endl;
			cout << "9) Canal 2 lab" << endl;
			cin >> canal1;
			cout << "Elija que canal poner en el canal 2 de la imagen a componer" << endl;
			cout << "1) Canal 0 bgr" << endl;
			cout << "2) Canal 1 bgr" << endl;
			cout << "3) Canal 2 bgr" << endl;
			cout << "4) Canal 0 hsv" << endl;
			cout << "5) Canal 1 hsv" << endl;
			cout << "6) Canal 2 hsv" << endl;
			cout << "7) Canal 0 lab" << endl;
			cout << "8) Canal 1 lab" << endl;
			cout << "9) Canal 2 lab" << endl;
			cin >> canal2;

			//armamos la imagen a partir de las opciones del usuario
			Mat channel0 = (splits[(canal0 - 1) / 3][(canal0 - 1) % 3]);
			Mat channel1 = (splits[(canal1 - 1) / 3][(canal1 - 1) % 3]);
			Mat channel2 = (splits[(canal2 - 1) / 3][(canal2 - 1) % 3]);
			Mat compuesta;
			Mat channels[3] = {channel0,channel1,channel2};
			Mat bgr;
			merge(channels, 3, bgr);
			
			if (espacio == 1)
			{
				compuesta = bgr;
			}
			if (espacio == 2)
			{
				cvtColor(bgr, compuesta, COLOR_HSV2BGR);
			}
			if (espacio == 3)
			{
				cvtColor(bgr, compuesta, COLOR_Lab2BGR);
			}
			imshow("Compuesta", compuesta);
			waitKey(500);
			cout << "Presione enter para continuar" << endl;
			std::cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cin.get();
			destroyWindow("Compuesta");
		}

		cout << " Elija una opcion" << endl;
		cout << "1) Mostrar canales bgr" << endl;
		cout << "2) Mostrar canales hsv" << endl;
		cout << "3) Mostrar canales lab" << endl;
		cout << "4) Recomponer imagen a partir de canales individuales" << endl;
		cout << "5) Salir" << endl;
		cin >> opc;
	}
	destroyAllWindows();
	
	//esperamos a que se presione una tecla
	//waitKey();
	return 0;
}
