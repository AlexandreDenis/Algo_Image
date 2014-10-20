////////////////////////////////////////////////////////////////////////////////
//                          TP 6 : Flot optique                     //
////////////////////////////////////////////////////////////////////////////////

#include "CImg.h"
#include <math.h>
#include <iostream>

using namespace cimg_library;


CImg<> HornSchunck(CImg<> seq)
{
	CImg<> field(seq.width(),seq.height(),1,2);
	CImgList<> grad = seq.get_gradient("xyz"); // 0 -> x      1 -> y      2 -> z
	CImg<> u(seq.width(),seq.height(),1,2,0);
	CImg<> v(seq.width(),seq.height(),1,2,0);
	
	double mask[3*3] = { 1./9., 1./9., 1./9.,
							1./9., 1./9., 1./9.,
							1./9., 1./9., 1./9.
						};
	CImg<> matmask(mask, 3, 3, 1, 1);
	
	CImg<> umoy;
	CImg<> vmoy;
	
	double lambda = 10; // jusqu'a quel niveau on tiendra compte du gradient, varie entre 0 et (2*256)²

	for ( int i = 0 ; i < 50 ; ++i ) {
		//valeur moyenne u et v avec filtre moyenneur -> use get_convolve
		umoy = u.get_convolve(matmask);
		vmoy = v.get_convolve(matmask);
		
		cimg_forXY(u,x,y) {
			u(x,y) = umoy(x,y) - (grad[0](x,y) * (grad[0](x,y) * umoy(x,y) + grad[1](x,y) * vmoy(x,y) + grad[2](x,y)) 
								/ (lambda * lambda + grad[0](x,y) * grad[0](x,y) + grad[1](x,y) * grad[1](x,y)));
		
			v(x,y) = vmoy(x,y) - (grad[1](x,y) * (grad[0](x,y) * umoy(x,y) + grad[1](x,y) * vmoy(x,y) + grad[2](x,y)) 
								/ (lambda * lambda + grad[0](x,y) * grad[0](x,y) + grad[1](x,y) * grad[1](x,y))); 
		}
	}
	
	cimg_forXY(field,x,y) {
		field(x,y,0) = u(x,y);
		field(x,y,1) = v(x,y);
	}

	return field;
}

CImg<> LucasKanade(CImg<> seq)
{
 CImg<> field(seq.width(),seq.height(),1,2);
 CImgList<> grad = seq.get_gradient("xyz");
 CImg<> A(2, seq.width() * seq.height());
 CImg<> b(1, seq.width() * seq.height());
 
 // initialisation de la matrice A et du vecteur b
 cimg_forXY(seq,x,y)
 {
	A(0,x + 2*y) = grad[0](x,y);
	A(1,x + 2*y) =  grad[1](x,y);
	b(0,x + 2*y) = -1 * grad[2](x,y);
 };
 
 CImg<> res = b.solve(A);
 
 std::cout << "width = " << res.width() << std::endl;
 std::cout << "height = " << res.height() << std::endl;
 //field = b.solve(A);

 return field;
}

CImg<> LucasKanade2(CImg<> seq)
{
 CImg<> field(seq.width(),seq.height(),1,2);

 cimg_forXYC(field,x,y,v)
  field(x,y,v) = 0.01;

 return field;
}

/*******************************************************************************

                                    Main

*******************************************************************************/
int main()
{
 // Ouverture de l'image "rubic.bmp"
 CImg<> seq = CImg<>("rubic1.bmp").channel(0);
 seq.append(CImg<>("rubic2.bmp").channel(0),'z');

 CImg<> displacementHS  = HornSchunck(seq);
 CImg<> displacementLK  = LucasKanade(seq);
 CImg<> displacementLK2 = LucasKanade2(seq);

 // Affichage du champ résultat
 float color=500; unsigned int  sampling = 8; float  factor = 40; int  quiver_type = 0; float  opacity = 0.5;

 CImg<> imageHS = seq.get_slice(0).draw_quiver(displacementHS,&color,opacity,sampling,factor,quiver_type);
 CImgDisplay resHS_disp(imageHS,"Horn et Schunck");

 CImg<> imageLK = seq.get_slice(0).draw_quiver(displacementLK,&color,opacity,sampling,factor,quiver_type);
 CImgDisplay resLK_disp(imageLK,"Lucas et Kanade");

 CImg<> imageLK2 = seq.get_slice(0).draw_quiver(displacementLK2,&color,opacity,sampling,factor,quiver_type);
 CImgDisplay resLK2_disp(imageLK2,"Lucas et Kanade avec gestion des valeurs propres");

 while (!resLK_disp.is_closed())
 {

  resLK_disp.wait();
 }
  return 0;
}
