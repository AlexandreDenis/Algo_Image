#include "CImg.h"
using namespace cimg_library;

int main()
{
	int i = -1;
	int j;
	
	CImg<> img(256,256,1,1,0);
	
	cimg_forXY(img,x,y)
	{
		if(x == 0)
		{
			i++;
			j = i;
		}
		
		img(x,y) = j;
		
		j++;
	}
	
	img.display("ex1");
	
	return 0;
}
