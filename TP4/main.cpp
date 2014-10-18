////////////////////////////////////////////////////////////////////////////////
//                          TP 4 : Filtrage de Diffusion                      //
////////////////////////////////////////////////////////////////////////////////

#include "CImg.h"

using namespace cimg_library;

// option 1
// g(s) = 1 / (1 + (lambda * s)²)
CImg<> get_option1(CImg<> s, float lambda)
{
	CImg<> one = s;
	CImg<> res;
	CImg<> tmp = (lambda * s).get_mul(lambda * s);
	
	cimg_forXY(one,x,y) {
		one(x,y) = 1.;
	}
	
	res = one.get_div(1. + tmp);
	
	return res;
}

// option 2
// g(s) = exp(-(lambda * s)²)
CImg<> get_option2(CImg<> s, float lambda)
{
	CImg<> res;
	CImg<> tmp = (lambda * s).get_mul(lambda * s);
	
	res = (-1. * tmp).get_exp();
	
	return res;
}
/*
CImg<> get_norme(CImg<> imageIn)
{
	CImgList<> grad = imageIn.get_gradient();
	
	CImg<> normex = grad[0].get_mul(grad[0]);
	CImg<> normey = grad[1].get_mul(grad[1]);
	CImg<> norme  = normex + normey;
	norme = norme.sqrt();
	
	return norme;
}
*/
/*******************************************************************************

                             Filtrage de diffusion

*******************************************************************************/
CImg<> PeronaMalik2D(CImg<float> imageIn, int nbiter, float delta_t, float lambda, int option)
{
	CImg<> imgOut = imageIn;
	
	CImg<> cstE;
	CImg<> cstO;
	CImg<> cstN;
	CImg<> cstS;
	
	CImgList<> forward_grad;
	CImgList<> backward_grad;
	
	CImg<> grad_E;
	CImg<> grad_O;
	CImg<> grad_N;
	CImg<> grad_S;
	
	CImg<> abs_E;
	CImg<> abs_O;
	CImg<> abs_N;
	CImg<> abs_S;
	
	CImg<> imgTmp;
	
	for ( int i = 0 ; i < nbiter ; ++i) {
	
		forward_grad = imgOut.get_gradient("xy", 1);
		backward_grad = imgOut.get_gradient("xy", -1);
		
		grad_E = forward_grad[0];
		grad_O = -1. * backward_grad[0];
		grad_N = -1. * backward_grad[1];
		grad_S = forward_grad[1];
		
		abs_E = grad_E.get_abs();
		abs_O = grad_O.get_abs();
		abs_N = grad_N.get_abs();
		abs_S = grad_S.get_abs();
		
		switch(option)
		{
			case 1:
				cstE = get_option1(abs_E, lambda);
				cstO = get_option1(abs_O, lambda);
				cstN = get_option1(abs_N, lambda);
				cstS = get_option1(abs_S, lambda);
				break;
			default:
				cstE = get_option2(abs_E, lambda);
				cstO = get_option2(abs_O, lambda);
				cstN = get_option2(abs_N, lambda);
				cstS = get_option2(abs_S, lambda);
				break;
		}
		
		imgTmp = cstE.get_mul(grad_E) + cstN.get_mul(grad_N) + cstS.get_mul(grad_S) + cstO.get_mul(grad_O);
		imgTmp = delta_t * imgTmp;
		
		imgOut = imgTmp + imgOut;	
		
	}
	
	return imgOut;
}
/*******************************************************************************

                   Filtrage de diffusion spatio-temporel

*******************************************************************************/
CImg<> PeronaMalik2D_T(CImg<> imageIn, int nbiter, float delta_t, float lambda, int option)
{
 CImg<> imgOut = imageIn;
	
	CImg<> cstE;
	CImg<> cstO;
	CImg<> cstN;
	CImg<> cstS;
	CImg<> cstF;
	CImg<> cstB;
	
	CImgList<> forward_grad;
	CImgList<> backward_grad;
	
	CImg<> grad_E;
	CImg<> grad_O;
	CImg<> grad_N;
	CImg<> grad_S;
	CImg<> grad_F;
	CImg<> grad_B;
	
	CImg<> abs_E;
	CImg<> abs_O;
	CImg<> abs_N;
	CImg<> abs_S;
	CImg<> abs_F;
	CImg<> abs_B;
	
	CImg<> imgTmp;
	
	for ( int i = 0 ; i < nbiter ; ++i) {
	
		forward_grad = imgOut.get_gradient("xyz", 1);
		backward_grad = imgOut.get_gradient("xyz", -1);
		
		grad_E = forward_grad[0];
		grad_O = -1. * backward_grad[0];
		grad_N = -1. * backward_grad[1];
		grad_B = -1 * backward_grad[2];
		grad_S = forward_grad[1];
		grad_F = forward_grad[2];
		
		abs_E = grad_E.get_abs();
		abs_O = grad_O.get_abs();
		abs_N = grad_N.get_abs();
		abs_S = grad_S.get_abs();
		abs_F = grad_F.get_abs();
		abs_B = grad_B.get_abs();
		
		switch(option)
		{
			case 1:
				cstE = get_option1(abs_E, lambda);
				cstO = get_option1(abs_O, lambda);
				cstN = get_option1(abs_N, lambda);
				cstS = get_option1(abs_S, lambda);
				cstF = get_option1(abs_F, lambda);
				cstB = get_option1(abs_B, lambda);
				break;
			default:
				cstE = get_option2(abs_E, lambda);
				cstO = get_option2(abs_O, lambda);
				cstN = get_option2(abs_N, lambda);
				cstS = get_option2(abs_S, lambda);
				cstF = get_option2(abs_F, lambda);
				cstB = get_option2(abs_B, lambda);
				break;
		}
		
		imgTmp = cstE.get_mul(grad_E) + cstN.get_mul(grad_N) 
				+ cstS.get_mul(grad_S) + cstO.get_mul(grad_O)
				+ cstF.get_mul(grad_F) + cstB.get_mul(grad_B);
		imgTmp = delta_t * imgTmp;
		
		imgOut = imgTmp + imgOut;	
		
	}
	
	return imgOut;
}
/*******************************************************************************

                                    Main

*******************************************************************************/
int main()
{
 // Ouverture de l'image "Lena.bmp"
 CImg<> img = CImg<>("TwoLeaveShop2front0330.bmp").channel(0);
 img.append(CImg<>("TwoLeaveShop2front0335.bmp").channel(0),'z');
 img.append(CImg<>("TwoLeaveShop2front0340.bmp").channel(0),'z');
 img.append(CImg<>("TwoLeaveShop2front0345.bmp").channel(0),'z');
 img.append(CImg<>("TwoLeaveShop2front0350.bmp").channel(0),'z');
 img.append(CImg<>("TwoLeaveShop2front0355.bmp").channel(0),'z');
 img.append(CImg<>("TwoLeaveShop2front0360.bmp").channel(0),'z');
 img.append(CImg<>("TwoLeaveShop2front0365.bmp").channel(0),'z');

 // Bruitage de l'image
 img.noise(-5);

 // Paramètres du filtre
 int   nbiter  = 30;
 float delta_t = 0.3;
 float lambda  = 1./5;
 int   model   = 1;

 //Filtrage 2D de la quatrième image
 CImg<> imgout2D = PeronaMalik2D(img.get_slice(3),nbiter,delta_t,lambda,model);

 // Affichage
 CImgDisplay dispSpatial(img.get_slice(3),"Image Originale");
 CImgDisplay dispFilter2D(imgout2D,"Image Filtree en 2D");

 //Filtrage 2D+T
 CImg<> imgout2DT = PeronaMalik2D_T(img,nbiter,delta_t,lambda,model);

 // Affichage
 CImgDisplay dispFilter2DT(imgout2DT.get_slice(3),"Image Filtree en 2D+T");

  while (!dispFilter2DT.is_closed())
  {
	dispFilter2DT.wait();
  }
  return 0;
}
