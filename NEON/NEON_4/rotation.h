#include <stdio.h>
#include <stdlib.h>
#include <arm_neon.h>
#include <png.h>
#include <string.h>
#include <math.h>
#include "util.h"
#include "util.c"

unsigned char *neon_rota(	int winkel, 				//winkle im Grad
							unsigned char *gbild_in_, 	//Eingangsbild
							size_t gx, 					//Eingangsbildbereit
							size_t gy, 					//Eingangsbildhöhe
							size_t *x_out, 				//Ausgangsbildbereit
							size_t *y_out				//Ausgangsbildhöhe
						);