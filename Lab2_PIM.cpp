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

	/* generam o fereastra care va contine una sau mai multe imagini
	   plasate sub forma unei matrici 2D */
	ImageGrid *grid = new ImageGrid("Prelucrarea imaginilor");

	QString imageDir = "Images/";
	QString imageFile(imageDir + "ct.jpg");

	/*adaugam prima imagine, cea initiala, citita din fisier,
	in pozitia stanga-sus (linia 0, coloana 0)*/
	grid->addImage(imageFile, 0, 0);

	/* extragem imformatiile necesare din imagine:
	dimensiunile ei si un sir de octeti care contine valorile
	intensitatilor pentru fiecare pixel */
	int w, h;
	unsigned char *img = Tools::readImageGray8(imageFile, w, h);

	// exemplu de operatie: negativarea unei imagini

	/*parcurgem imaginea pixel cu pixel si determinam valoarea complementara
	pentru fiecare intensitate

	se recomanda ca acest gen de operatie sa se implementeze intr-o functie
	separata sau intr-o alta clasa
	*/

	double gm = 0.6f;

	unsigned char *negated = negateImage(img, w, h);
	unsigned char *binary = binaryImage(img, w, h, 30);
	unsigned char *brightness = brightnessImage(img, w, h, 150);
	unsigned char *contrast = contrastImage(img, w, h, 10, 50, 40);
	unsigned char *gamma = gammaImage(img, w, h, gm);
	unsigned char *compressed = compressedImage(img, w, h, 45);

	unsigned char *bitPlane = bitPlaneSliceImage(img, w, h, 0);
	unsigned char *bitPlane2 = bitPlaneSliceImage(img, w, h, 1);
	unsigned char *bitPlane3 = bitPlaneSliceImage(img, w, h, 2);
	unsigned char *bitPlane4 = bitPlaneSliceImage(img, w, h, 3);
	unsigned char *bitPlane5 = bitPlaneSliceImage(img, w, h, 4);
	unsigned char *bitPlane6 = bitPlaneSliceImage(img, w, h, 5);
	unsigned char *bitPlane7 = bitPlaneSliceImage(img, w, h, 6);
	unsigned char *bitPlane8 = bitPlaneSliceImage(img, w, h, 7);
	/* afisam imaginea astfel obtinuta la dreapta celei initiale;
	parametrii cu valorile 0, 1 semnifica prima linie, respectiv
	a doua coloana a imageGrid-ului
	*/

	int option;
	std::cout << "1-Prima parte 2-Extragerea celor 8 imagini(bit-slice)\n";
	std::cin >> option;
	if (option == 1)
	{
		grid->addImage(negated, w, h, 0, 1, "negativ");
		grid->addImage(binary, w, h, 0, 2, "binary");
		grid->addImage(brightness, w, h, 1, 0, "brightness");
		grid->addImage(contrast, w, h, 1, 1, "contrast");
		grid->addImage(gamma, w, h, 1, 2, "gamma");
		grid->addImage(compressed, w, h, 2, 1, "compressed");
	}
	else
	{

		grid->addImage(bitPlane8, w, h, 0, 1, "bit0");
		grid->addImage(bitPlane7, w, h, 0, 2, "bit1");
		grid->addImage(bitPlane6, w, h, 0, 3, "bit2");
		grid->addImage(bitPlane5, w, h, 1, 0, "bit3");
		grid->addImage(bitPlane4, w, h, 1, 1, "bit4");
		grid->addImage(bitPlane3, w, h, 1, 2, "bit5");
		grid->addImage(bitPlane2, w, h, 1, 3, "bit6");
		grid->addImage(bitPlane, w, h, 2, 0, "bit7");
	}
	grid->show();

	// delete[] negated;
	// delete[] binary;
	// delete[] brightness;
	// delete[] contrast;
	// delete[] gamma;
	// delete[] compressed;

	// delete[] bitPlane;
	// delete[] bitPlane2;
	// delete[] bitPlane3;
	// delete[] bitPlane4;
	// delete[] bitPlane5;
	// delete[] bitPlane6;
	// delete[] bitPlane7;
	// delete[] bitPlane8;

	return a.exec();
}
