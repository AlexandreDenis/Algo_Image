#include "CImg.h"

using namespace cimg_library;

int main() {
	CImg<> image("lena.jpg");
	CImg<> image2 = image.get_channel(2);
	
	CImgList<> g = image2.get_gradient("xy",4);
	
	CImg<> c = g[0].mul(g[0]) + g[1].mul(g[1]);
	
	c.display("ex2");
	
	return 0;
}
