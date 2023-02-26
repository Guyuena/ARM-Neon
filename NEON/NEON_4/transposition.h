#include <stdio.h>
#include <stdlib.h>
#include <arm_neon.h>
#include <png.h>
#include <string.h>
#include "util.h"
#include "util.c"

void tran_32(	
				uint32_t *bildcol_, 	//Eingangsdaten
				uint32_t *bildcol_t_,	//Ausgangsdaten
				size_t gx, 				//Eingangsbildbereit
				size_t gy				//Ausgangsbildbereit
			);

void tran_16(	
				uint16_t *in_, 			//Eingangsdaten
				uint16_t *out_, 		//Ausgangsdaten
				size_t gx, 				//Eingangsbildbereit
				size_t gy				//Ausgangsbildbereit
			);

void tran_8(	
				png_bytep gbild_, 		//Eingangsdaten
				png_bytep gbild_t_, 	//Ausgangsdaten
				size_t gx, 				//Eingangsbildbereit
				size_t gy				//Ausgangsbildbereit
			);