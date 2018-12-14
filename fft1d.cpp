#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "math.h"
#define PI 3.141592653589793

using namespace std;

typedef struct {
	int len;	// length of complex number list
	float * real;	// pointer to real number list
	float * imag;	// pointer to imaginary number list
}
complexS, *complexP;


void fft1D(complexP q1, char* dir, complexP q2);
void initialfft1D(FILE* inputFile, char* dir, FILE* outputFile);


int main(int argc, char* argv[]) {
	if (argc != 4) {
		cout << "Invalid number of arguments. Enter in the form (fft1D in dir out)" << endl;
	}

	else {
		char* dir = argv[2];
		FILE *inputFile;
		FILE *outputFile;

		char* inputName = argv[1];
		char* outputName = argv[3];

		inputFile = fopen(inputName, "r");
		outputFile = fopen(outputName, "w+");

		initialfft1D(inputFile, dir, outputFile);
		//cout << "main runs" << endl;

		fclose(inputFile);
		fclose(outputFile);
	}

	std::cin.get();
	return 0;
}

void initialfft1D(FILE* inputFile, char* dir, FILE* outputFile) {

	int width;
	int height;

	fscanf_s(inputFile, "%d %d", &width, &height);

	complexP q1;
	complexP q2;

	q1 = (complexP)malloc(sizeof(complexS));
	q1->len = height;
	q1->real = (float *)malloc(sizeof(float) * q1->len);
	q1->imag = (float *)malloc(sizeof(float) * q1->len);

	q2 = (complexP)malloc(sizeof(complexS));
	q2->len = height;
	q2->real = (float *)malloc(sizeof(float) * q2->len);
	q2->imag = (float *)malloc(sizeof(float) * q2->len);

	float *r1a, *i1a, *r2a, *i2a;
	
	r1a = (float *)q1->real;
	i1a = (float *)q1->imag;
	
	r2a = (float *)q2->real;
	i2a = (float *)q2->imag;

	for (int i = 0; i < height; i++) {
		float a, b;
		fscanf_s(inputFile, "%f %f", &a, &b);
		r1a[i] = a;
		i1a[i] = b;
	}

	fft1D(q1, dir, q2);

	for (int j = 0; j < height; j++) {
		float a, b;
		fprintf(outputFile, "%f %f\n", r2a[j], i2a[j]);
	}

}


void fft1D(complexP q1, char* dir, complexP q2)
{
	int i, N, N2;
	float * r1, *i1, *r2, *i2, *ra, *ia, *rb, *ib;
	float FCTR, fctr, a, b, c, s, num[2];
	complexP qa, qb;

	N = q1->len;
	
	r1 = (float *)q1->real;
	i1 = (float *)q1->imag;
	r2 = (float *)q2->real;
	i2 = (float *)q2->imag;



	if (N == 2) {
		a = r1[0] + r1[1];	// F(0)=f(0)+f(1);F(1)=f(0)-f(1)
		b = i1[0] + i1[1];	// a,b needed when r1=r2
		r2[1] = r1[0] - r1[1];
		i2[1] = i1[0] - i1[1];
		r2[0] = a;
		i2[0] = b;

	}
	else {
		N2 = N / 2;
		qa = (complexP)malloc(sizeof(complexS));
		qa->len = N2;
		qa->real = (float *)malloc(sizeof(float) * qa->len);
		qa->imag = (float *)malloc(sizeof(float) * qa->len);

		qb = (complexP)malloc(sizeof(complexS));
		qb->len = N2;
		qb->real = (float *)malloc(sizeof(float) * qb->len);
		qb->imag = (float *)malloc(sizeof(float) * qb->len);

		ra = (float *)qa->real;
		ia = (float *)qa->imag;
		rb = (float *)qb->real;
		ib = (float *)qb->imag;

		// split list into 2 halves; even and odd
		for (i = 0; i < N2; i++) {
			ra[i] = *r1++;
			ia[i] = *i1++;
			rb[i] = *r1++;
			ib[i] = *i1++;
		}

		// compute fft on both lists
		fft1D(qa, dir, qa);
		fft1D(qb, dir, qb);

		// build up coefficients
		if (*dir != '0')	// forward
			FCTR = -2 * PI / N;
		else	FCTR = 2 * PI / N;

		for (fctr = i = 0; i < N2; i++, fctr += FCTR) {
			c = cos(fctr);
			s = sin(fctr);
			a = c * rb[i] - s * ib[i];
			r2[i] = ra[i] + a;
			r2[i + N2] = ra[i] - a;

			a = s * rb[i] + c * ib[i];
			i2[i] = ia[i] + a;
			i2[i + N2] = ia[i] - a;
		}

		free(qa);
		free(qb);
	}

	if (*dir != '0') {	// inverse : divide by logN
		//cout << "dir" << endl;
		for (i = 0; i < N; i++) {
			q2->real[i] = q2->real[i] / 2;
			q2->imag[i] = q2->imag[i] / 2;
		}
	}



}
