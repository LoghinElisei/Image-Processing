#include <QApplication>
#include "ImageGrid.h"
#include "operatii.h"

/* Shortcuts:

Scroll wheel - zoom in/out

Left button drag - deplasare in imagine cand aceasta este mai mare decat fereastra

Ctrl+A - activeaza / dezactiveaza ajustarea automata a dimensiunii imaginii
		 pe care se afla cursorul mouse-ului

Shift+A - activeaza / dezactiveaza ajustarea automata a dimensiunii
		  tuturor imaginilor

Ctrl+R - reseteaza imaginea curenta la dimensiunile sale initiale

Shift+R - reseteaza toate imaginile la dimensiunile lor initiale

*/

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	ImageGrid *grid = new ImageGrid("Prelucrarea imaginilor");
	int w, h;
	int status = 0;

	QString imageDir = "Images/";

	std::cout << "Choose one photo\n\t[1]-CT [2]-Lena [3]-Circle [4]-Harvest \n";
	int option ;
	std::cin >> option;
	char file_name[64]="\0"; 
	switch(option)
	{
		case 1:
			strcpy(file_name,"ct.jpg");
			break;
		case 2:
			strcpy(file_name,"lena512.bmp");
			break;
		case 3:
			strcpy(file_name,"Fig5.03.jpg");
			break;
		default:
			strcpy(file_name,"photo.jpg");
			break;
	}

	QString imageFile(imageDir + file_name);
	grid->addImage(imageFile, 0, 0);

	unsigned char *img = Tools::readImageGray8(imageFile, w, h);

	// std::cout << "\t[1]-Filtru Sobel [2]-Filtru Prewitt\n";

	// int option;
	// std::cin >> option;

	

	unsigned char *sobel_on_X_dir;
	unsigned char *sobel_on_Y_dir;
	unsigned char *prewitt_on_X_dir;
	unsigned char *prewitt_on_Y_dir;
	unsigned char *sobel_on_XY;
	unsigned char *prewitt_on_XY;

	sobel_on_X_dir = applyFilterOnDirection(img, w, h, 1, 1);
	grid->addImage(sobel_on_X_dir, w, h, 0, 1, "sobel X dir");

	sobel_on_Y_dir = applyFilterOnDirection(img, w, h, 1, 2);
	grid->addImage(sobel_on_Y_dir, w, h, 0, 2, "sobel Y dir");

	prewitt_on_X_dir = applyFilterOnDirection(img, w, h, 2, 1);
	grid->addImage(prewitt_on_X_dir, w, h, 1, 1, "prewitt X dir");

	prewitt_on_Y_dir = applyFilterOnDirection(img, w, h, 2, 2);
	grid->addImage(prewitt_on_Y_dir, w, h, 1, 2, "prewitt Y dir");

	sobel_on_XY = combine_X_Y_dir(sobel_on_X_dir,sobel_on_Y_dir,w,h);
	grid->addImage(sobel_on_XY, w, h, 2, 1, "sobel combined");
	
	prewitt_on_XY = combine_X_Y_dir(prewitt_on_X_dir,prewitt_on_Y_dir,w,h);
	grid->addImage(prewitt_on_XY,w,h,2,2,"prewitt combined");

	grid->show();
	status = a.exec();


	delete []sobel_on_X_dir;
	delete []sobel_on_Y_dir;
	delete []prewitt_on_X_dir;
	delete []prewitt_on_Y_dir;

	return status;
}
