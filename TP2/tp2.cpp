#include "CImg.h"

using namespace cimg_library;

int main() 
{
	CImg<> image("im.bmp");
	
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
							
	CImg<> gradient_x = image.get_convolve(img_prewitt_x);
	CImg<> gradient_y = image.get_convolve(img_prewitt_y);
	
	// Calcul de la phase du gradient
	CImg<> phase_gradient = gradient_y.get_div(gradient_x).get_atan();
	
	gradient_x.display("grad x");
	gradient_y.display("grad y");	
	
	phase_gradient.display("Phase du gradient");
	//image.display();
	
	return 0;
}
