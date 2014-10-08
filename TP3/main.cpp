////////////////////////////////////////////////////////////////////////////////
//                          TP 3 : Filtrage Fréquentiel                       //
////////////////////////////////////////////////////////////////////////////////

#include "CImg.h"
#include <math.h>

using namespace cimg_library;

/*******************************************************************************

    Calcul d'une image composee du module de image1 et de la phase de image2

*******************************************************************************/
CImg<> Melange (CImg<> imageIn1,CImg<> imageIn2)
{
	 CImg<> imgOut(imageIn1.width(),imageIn1.height(),1,1,0);
	 
	 CImgList<> tmpIm1 = imageIn1.get_FFT();
	 CImgList<> tmpIm2 = imageIn2.get_FFT();
	 
	 CImg<> moduleTF1 = (tmpIm1[0].mul(tmpIm1[0])+tmpIm1[1].mul(tmpIm1[1])).sqrt();
	 CImg<> phaseTF2  =  tmpIm2[1].get_atan2(tmpIm2[0]);
	 	 
	 CImg<> realPart = moduleTF1.get_mul(phaseTF2.get_cos());
	 CImg<> imPart = moduleTF1.get_mul(phaseTF2.get_sin());
	 
	 CImg<>::FFT(realPart, imPart, true);
	 
	 imgOut = realPart;
	 

	 return imgOut;
}

/*******************************************************************************

                        Filtrage d'une image tramee

*******************************************************************************/
CImg<unsigned char> Detramage (CImg<unsigned char> imageIn)
{
	float P = 6;	// Periode de la trame
	int Freq_c;		// frequence de coupure du filtre
	
	// Calcul de la transformée de Fourier
	CImgList<float> F_Img = imageIn.get_FFT();
	
	// Calcul de la fréquence de coupure
	Freq_c = imageIn.height()/2./P;
	
	// Filtrage sequentiel pour separer l'image de la trame
	cimg_forXY(F_Img[0],x,y)
	{
		if(y > Freq_c && y < F_Img[0].height() - Freq_c)
		{
			F_Img[0](x,y) = F_Img[1](x,y) = 0;
		}
	}
	
	// TF inverse et recuperation de la partie reelle (symetrie hermitienne)
	CImg<unsigned char> imgOut = F_Img.get_FFT(true)[0].normalize(0,255);
	
	return imgOut;
}

/*******************************************************************************

      Filtrage Gaussien d'une image (avec un ecart-type spatial de 'sigma')

*******************************************************************************/
CImg<> FiltrageGaussien (CImg<> imageIn, float sigma)
{
	 CImg<> filtre_gaussien(imageIn.width(),imageIn.height(),1,1,0);
	 
	 // recuperation de la transformee de Fourier de l'image en entree
	 CImgList<> F_Img = imageIn.get_FFT();
	 
	 // creation du filtre gaussien
	 cimg_forXY(filtre_gaussien,x,y)
	 {
		 filtre_gaussien(x,y) = exp(-(x*x + y*y)/(2. * sigma * sigma)) / (2. * M_PI * sigma * sigma);
	 }
	 
	 // calcul de la fonction de transfert du filtre gaussien
	 CImgList<> H = filtre_gaussien.get_FFT();
	 
	 CImgList<> res(2);
	 
	 res[0] = H[0].get_mul(F_Img[0]);
	 res[1] = H[1].get_mul(F_Img[1]);

	 CImg<unsigned char> imgOut = res.get_FFT(true)[0].normalize(0,255);

	 return imgOut;
}

/*******************************************************************************

                                    Main

*******************************************************************************/
int main()
{
	 ///////////////////////////////////////////////////////////////////////////////
	 //                        Question 1: Melange d'image                        //
	 ///////////////////////////////////////////////////////////////////////////////

	 // Ouverture des deux images
	 CImg<> imgClown("clown.bmp");
	 CImg<> imgGatlin("gatlin.bmp");

	 // Resultat du melange
	 CImg<> imgRes1 = Melange (imgClown.channel(0),imgGatlin.channel(0));

	 // Affichage du resultat
	 //CImgDisplay dispClown(imgClown,"Question 1 : Image de Clown (1/3)");
	 //CImgDisplay dispGatlin(imgGatlin,"Question 1 : Image de Gatlin (2/3)");
	 //CImgDisplay dispRes1(imgRes1,"Question 1 : Image Melangee (3/3)");

	 ///////////////////////////////////////////////////////////////////////////////
	 //                        Question 2: Détramage                              //
	 ///////////////////////////////////////////////////////////////////////////////

	 // Ouverture de l'image "lena_trame.bmp"
	 CImg<> imgLenaTramee("lena_trame.bmp");

	 // Resultat du détramage
	 CImg<> imgRes2 = Detramage (imgLenaTramee.channel(0));

	 // Affichage du resultat
	 //CImgDisplay dispTramee(imgLenaTramee,"Question 2 : Image de Lena Tramee (1/2)");
	 //CImgDisplay dispDetramee(imgRes2,"Question 2 : Image de Lena Detramee (2/2)");


	 ///////////////////////////////////////////////////////////////////////////////
	 //                        Question 3: Filtrage Gaussien                      //
	 ///////////////////////////////////////////////////////////////////////////////

	 // Ouverture de l'image "lena.bmp"
	 CImg<> imgLena("lena.bmp");

	 // Ajout d'un bruit additif Gaussien
	 imgLena.noise(-5);

	 // Résultat du détramage
	 float sigma = 1;
	 CImg<> imgRes3 = FiltrageGaussien (imgLena.channel(0),sigma);

	 // Affichage du resultat
	 CImgDisplay dispBruitee(imgLena,"Question 3 : Image de Lena Bruitee (1/2)");
	 CImgDisplay dispDebruitee(imgRes3,"Question 3 : Image de Lena Debruitee (2/2)");


	 while (!dispBruitee.is_closed())
	 {
	  dispBruitee.wait();
	 }
	 return 0;
}
