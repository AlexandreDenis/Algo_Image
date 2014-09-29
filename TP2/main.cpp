#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

#include "CImg.h"


using namespace cimg_library;
using namespace std;


/******************************************
Calcul de la phase du gradient de l'image imageIn
******************************************/
CImg<float> Phase (CImg<unsigned char> imageIn)
{
	CImg<double> res(imageIn.width(),imageIn.height(),1,1,0);
 
	// Calcul du gradient
	float prewitt_x[3*3] = { -1, 0, 1,
							 -1, 0, 1,
							 -1, 0, 1
							};
							
							
	float prewitt_y[3*3] = { -1, -1, -1,
							 0, 0, 0,
							 1, 1, 1
							};
							
	CImg<> img_prewitt_x(prewitt_x, 3, 3, 1, 1);
	CImg<> img_prewitt_y(prewitt_y, 3, 3, 1, 1);
							
	CImg<> gradient_x = imageIn.get_convolve(img_prewitt_x);
	CImg<> gradient_y = imageIn.get_convolve(img_prewitt_y);
	
	// Calcul de la phase du gradient
	res = gradient_y.get_atan2(gradient_x);

	return res;  
}

/******************************************
Calcul d'un sigma_filtre sur l'image imageIn
******************************************/
CImg<float> sigmaFiltre (CImg<unsigned char> imageIn)
{
	CImg<double> res(imageIn.width(),imageIn.height(),1,1,0);

	return res;  
}

/******************************************
Main
******************************************/
int main(int argc,char **argv) 
{ 
	cimg_usage("TP2 Imagerie - filtrage spatial");

	const string name_i   = cimg_option("-i","im.bmp","Image");

	CImg<unsigned char> image(name_i.c_str()); 
	
	CImg<float> resultPh = Phase(image);
	CImg<float> resultSg = sigmaFiltre(image);

	CImgDisplay main_disp(image,"Visualisation");
	CImgDisplay resP_disp(resultPh,"Phase du Gradient");
	//CImgDisplay resS_disp(resultSg,"Sigma_filtre de l'image");
	
	while ( !main_disp.is_closed() ) 
	{ 
		main_disp.wait(); 
	}

	return 0; 
}
