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
	// int zoom;
	// std::cout<<"zoom = ";
	// std::cin>>zoom;
	
	unsigned char *zoomed_image;


	zoomed_image = doubleImageResolution(img, w, h);
	grid->addImage(zoomed_image, w*zoom, h*zoom, 0, 2, "zoomed image");

	

	grid->show();
	status = a.exec();


	delete []zoomed_image;

	return status;
}
