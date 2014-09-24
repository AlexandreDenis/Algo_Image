#include "CImg.h"

using namespace cimg_library;

int main() {
	CImg<> image("lena.jpg");
	CImg<> image2 = image.get_channel(2);
	
	CImgList<> g = image2.get_gradient("xy",4);
	
	CImg<> norme0 = g[0].mul(g[0]);
	CImg<> norme1 = g[1].mul(g[1]);
	CImg<> norme  = norme0 + norme1;
	norme = norme.sqrt();
	
	CImg<> phase0 = cos(norme0.sqrt().div(norme));
		
	phase0.display("ex2_phase1");
	norme.display("ex2_norme");
	
	return 0;
}
