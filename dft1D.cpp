#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <math.h>
#include <fstream>


#define PI 3.1415927
#define MAG(i)		(sqrt(Fr[i]*Fr[i] + Fi[i]*Fi[i]))
#define PHASE(i)	(atan2(Fi[i],Fr[i]))
using namespace std;

void dft1D(FILE* inputFile, char* dir, FILE* outputFile);

int main(int argc, char* argv[]) {
	if (argc!= 4) {
		cout << "Invalid number of arguments. Enter in the form (dft1D in dir out)" << endl;
	}
	
	else {
		char* dir = argv[2];
		FILE *inputFile;
		FILE *outputFile;

		char* inputName = argv[1];
		char* outputName = argv[3];

		inputFile = fopen(inputName,"r");
		outputFile = fopen(outputName, "w+");
	
		dft1D(inputFile, dir, outputFile);
		//cout << "main runs" << endl;

		fclose(inputFile);
		fclose(outputFile);
	}

	std::cin.get();
	return 0;
}

void dft1D(FILE* inputFile, char* dir, FILE* outputFile) {
	//cout << dir << endl;
	//variables a and b takes on values from each line of textFile
	int a, b;
	int width;
	int height;

	fscanf_s(inputFile, "%d %d", &width, &height);

	int u, x, N;
	N = height;

	double c, s, real, imag;

	double* Fr = new double[N];
	double* Fi = new double[N];
	double* fr = new double[N];
	double* fi = new double[N];

	if (*dir == '0') {
		//cout << "fourier transform" << endl;
		for (u = 0; u < N; u++) {

			fscanf(inputFile, "%d %d", &a, &b);
			fr[u] = a;
			fi[u] = b;

		}

		for (u = 0; u < N; u++) {		/* compute spectrum over all freq u */
			real = imag = 0;			/* reset real, imag component of F(u) */
			for (x = 0; x < N; x++) {	/* visit each input pixel */
				c = cos(2.*PI*u*x / N);
				s = -sin(2.*PI*u*x / N);
				real += (fr[x] * c - fi[x] * s);
				imag += (fr[x] * s + fi[x] * c);
			}
			Fr[u] = real / N;
			Fi[u] = imag / N;
		}
		for (u = 0; u < N; u++) {
			printf("Fr[%d] = %f\t\tFi[%d] = %f\n", u, Fr[u], u, Fi[u]);
			printf("|F(%d)| = %f\t\tPhase = %f\n", u, MAG(u), PHASE(u));
			fprintf(outputFile, "%f %f %f %f\n", Fr[u], Fi[u], MAG(u), PHASE(u));
		}
		printf("\n");
	}
	else {
		//cout << "inverse fourier transform" << endl;
		for (int x = 0; x < N; x++) {
			float c, d;

			fscanf(inputFile, "%f %f", &c, &d);
			Fr[x] = c;
			Fi[x] = d;
			//cout << Fr[x] << " " << Fi[x] << endl;
		}
		for (u = 0; u < N; u++)
		{
			printf("|F(%d)| = %f\t\tPhase = %f\n", u, MAG(u), PHASE(u));
			cout << "|F("<<u << ")| = " << MAG(u) << " Phase = "<< PHASE(u) << endl;
		}
		printf("\n");

		for (x = 0; x < N; x++) {		/* compute each output pixel */
			real = imag = 0;
			for (u = 0; u < N; u++) {
				c = cos(2.*PI*u*x / N);
				s = sin(2.*PI*u*x / N);
				real += (Fr[u] * c - Fi[u] * s);
				imag += (Fr[u] * s + Fi[u] * c);
			}
			fr[x] = real;
			fi[x] = imag;
		}
		for (x = 0; x < N; x++)
		{
			printf("fr[%d] = %f\t\tfi[%d] = %f\n", x, fr[x], x, fi[x]);
			fprintf(outputFile, "%f %f %f %f\n", Fr[u], Fi[u], MAG(u), PHASE(u));
		}

	}
}
