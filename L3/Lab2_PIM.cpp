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

	int option;
	std::cout << "\t1-Prelucrari 2-Extragerea celor 8 imagini(bit-slice)\n\t 3-egalizare[1] 4-egalizare[2]\n";
	std::cin >> option;

	char str[100] = "\0";

	if (option == 1)
	{
		strcpy(str, "lena512.bmp");
		// chess_board-svg.png
	}
	else if (option == 2)
	{
		strcpy(str, "lena512.bmp");
	}
	else if (option == 3)
	{
		strcpy(str, "photo.jpg");
	}
	else if (option == 4)
	{
		strcpy(str, "lena512.bmp");
	}

	QString imageDir = "Images/";
	QString imageFile(imageDir + str);
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
	int L = 256;

	if (option == 1)
	{
		grid->addImage(negated, w, h, 0, 1, "negativ");
		grid->addImage(binary, w, h, 0, 2, "binary");
		grid->addImage(brightness, w, h, 0, 3, "brightness");
		grid->addImage(contrast, w, h, 1, 0, "contrast");
		grid->addImage(gamma, w, h, 1, 1, "gamma");
		grid->addImage(compressed, w, h, 1, 2, "compressed");

		float *histo1 = new float[L];
		int *histo_int1 = new int[L];
		float *histo2 = new float[L];
		int *histo_int2 = new int[L];
		float *histo3 = new float[L];
		int *histo_int3 = new int[L];
		float *histo4 = new float[L];
		int *histo_int4 = new int[L];
		float *histo5 = new float[L];
		int *histo_int5 = new int[L];
		float *histo6 = new float[L];
		int *histo_int6 = new int[L];
		float *histo7 = new float[L];
		int *histo_int7 = new int[L];
		hist(img, h, w, L, histo1);
		for (int i = 0; i < L; i++)
		{
			histo_int1[i] = (int)(histo1[i] * L);
		}
		grid->addHistogram(histo_int1, L, 2, 0, "Histo 1");

		hist(negated, h, w, L, histo2);
		for (int i = 0; i < L; i++)
		{
			histo_int2[i] = (int)(histo2[i] * L);
		}
		grid->addHistogram(histo_int2, L, 2, 1, "Histo 2");

		hist(binary, h, w, L, histo3);
		for (int i = 0; i < L; i++)
		{
			histo_int3[i] = (int)(histo3[i] * L);
		}
		grid->addHistogram(histo_int3, L, 2, 2, "Histo 3");

		hist(brightness, h, w, L, histo4);
		for (int i = 0; i < L; i++)
		{
			histo_int4[i] = (int)(histo4[i] * L);
		}
		grid->addHistogram(histo_int4, L, 2, 3, "Histo 4");

		hist(contrast, h, w, L, histo5);
		for (int i = 0; i < L; i++)
		{
			histo_int5[i] = (int)(histo5[i] * L);
		}
		grid->addHistogram(histo_int5, L, 3, 0, "Histo 5");

		hist(gamma, h, w, L, histo6);
		for (int i = 0; i < L; i++)
		{
			histo_int6[i] = (int)(histo6[i] * L);
		}
		grid->addHistogram(histo_int6, L, 3, 1, "Histo 6");

		hist(compressed, h, w, L, histo7);
		for (int i = 0; i < L; i++)
		{
			histo_int7[i] = (int)(histo7[i] * L);
		}
		grid->addHistogram(histo_int7, L, 3, 2, "Histo 7");
		// delete[] histo;

		// 	 delete[] histo1;
		// delete[] histo_int1;
		// delete[] histo2;
		// delete[] histo_int2;
		// delete[] histo3;
		// delete[] histo_int3;
		// delete[] histo4;
		// delete[] histo_int4;
		// delete[] histo5;
		// delete[] histo_int5;
		// delete[] histo6;
		// delete[] histo_int6;
		// delete[] histo7;
		// delete[] histo_int7;
	}
	else
	{
		if (option == 2)
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
		else if (option == 3 || option == 4)
		{
			unsigned char *final = new unsigned char[w * h];
			egalizare_histo(img, final, h, w, L);
			grid->addImage(final, w, h, 1, 0, "Egalizare");

			float *histo = new float[L];
			int *histo_int = new int[L];
			float *eq_histo = new float[L];
			int *histo_int_final = new int[L];
			hist(img, h, w, L, histo);

			for (int i = 0; i < L; i++)
			{
				histo_int[i] = (int)(histo[i] * L);
			}
			grid->addHistogram(histo_int, L, 0, 1, "Histo initial");

			hist(final, h, w, L, eq_histo);

			for (int i = 0; i < L; i++)
			{
				histo_int_final[i] = (int)(eq_histo[i] * L);
			}
			grid->addHistogram(histo_int_final, L, 1, 1, "Histo egalizata");

			delete[] histo;
			delete[] eq_histo;
			// delete[] final;
			// delete [] histo_int;
			// delete [] histo_int_final;
		}
	}

	grid->show();

	int status = a.exec();

	delete[] negated;
	delete[] binary;
	delete[] brightness;
	delete[] contrast;
	delete[] gamma;
	delete[] compressed;

	delete[] bitPlane;
	delete[] bitPlane2;
	delete[] bitPlane3;
	delete[] bitPlane4;
	delete[] bitPlane5;
	delete[] bitPlane6;
	delete[] bitPlane7;
	delete[] bitPlane8;

	return status;
}
