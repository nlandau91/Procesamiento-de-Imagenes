#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>
#include "../include/tinyfiledialogs/tinyfiledialogs.h"

int main()
{
    char const * lTheOpenFileName;
    lTheOpenFileName = tinyfd_openFileDialog(
		"Ingrese una imagen",
		"",
		0,
		NULL,
		NULL,
		0);
    if (! lTheOpenFileName)
	{
		tinyfd_messageBox(
			"Error",
			"Open file name is NULL",
			"ok",
			"error",
			0);
		return 1 ;
	}
	std::cout << "testing" << std::endl;
	
	std::cout << lTheOpenFileName << std::endl;
    
    //FILE * lIn;
    
    
    
    return 0;
}
