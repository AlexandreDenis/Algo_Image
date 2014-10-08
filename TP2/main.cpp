#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

#include "CImg.h"


using namespace cimg_library;
using namespace std;

/******************************************
Calcul du gradient d'une image imageIn
******************************************/
void getGradientWithPrewitt(const CImg<unsigned char> &imageIn, CImg<> &grad_x, CImg<> &grad_y)
{
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
							
	grad_x = imageIn.get_convolve(img_prewitt_x);
	grad_y = imageIn.get_convolve(img_prewitt_y);
}

/******************************************
Calcul de la phase du gradient de l'image imageIn
******************************************/
CImg<float> Phase (CImg<unsigned char> imageIn)
{
	CImg<double> res(imageIn.width(),imageIn.height(),1,1,0);

	CImg<> gradient_x, gradient_y;

	getGradientWithPrewitt(imageIn, gradient_x, gradient_y);
	
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

	CImg<> gradient_x, gradient_y;

	getGradientWithPrewitt(imageIn, gradient_x, gradient_y);
	
	// calcul de la norme du gradient
	CImg<> norme0 = gradient_x.mul(gradient_x);
	CImg<> norme1 = gradient_y.mul(gradient_y);
	CImg<> norme  = norme0 + norme1;
	norme = norme.sqrt();
	
	// calcul inverse du gradient
	CImg<> invGrad(imageIn.width(),imageIn.height(),1,1,0);
	
	cimg_forXY(norme,x,y)
	{
		if ( norme(x,y) == 0 ) 
		{
			invGrad(x,y) = 255;
		}
		else 
		{
			invGrad(x,y) = 1 / norme(x,y);
		}				
	}
	
	
	// Voisinage 3x3 I
	CImg_3x3(I,float);
	
	double sum;
	int ximg, yimg, xi, yi;
	double width = imageIn.width();
	double height = imageIn.height();
	
	// Boucle sur les dimensions en utilisant le voisinage I
	cimg_for3x3(imageIn,x,y,0,0,I, float)
	{	
		sum = 0;
		for(int i = 0; i < 9; ++i)
		{
			xi = i % 3;
			yi = i / 3;
			ximg = x + xi - 1;
			yimg = y + yi - 1;
			
			if(ximg >= 0 && ximg < width && yimg >= 0 && yimg < height)
			{
				sum += imageIn(ximg, yimg) * invGrad(ximg, yimg);
			}
		}
		
		res(x,y) = sum;
	}

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
	CImgDisplay resS_disp(resultSg,"Sigma_filtre de l'image");
	
	while ( !main_disp.is_closed() ) 
	{ 
		main_disp.wait(); 
	}

	return 0; 
}
