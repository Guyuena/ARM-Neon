#include <stdio.h>
#include <stdlib.h>
#include <arm_neon.h>
#include <png.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "util.h"
#include "util.c"		
#include "transposition.c"
#include "rotation.c"


int main(int argc, char const *argv[])
{
	png_bytep input, output;
	size_t x, y, x_out, y_out;		//Var für Bild und Muster
	readImageData("gray.png",&input,&x,&y);	//Bild lesen
	float winkel = atof(argv[1]);

	output = neon_rota(winkel, input, x, y, &x_out, &y_out);

	writeImageData("out2.png", output, x_out, y_out, 8);	


	free(input);
	free(output);
}