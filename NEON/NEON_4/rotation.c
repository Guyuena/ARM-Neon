unsigned char *neon_rota(float winkel, unsigned char *gbild_in_, size_t gx, size_t gy, size_t *x_out, size_t *y_out)
{
	winkel = -1 * winkel;
	double pi = acos(-1);		//definieren pi = arccos(-1)
	float sin_c = (float)sin(winkel * pi / 180);		//berechnen sin(EingangsWinkel)
	float cos_c = (float)cos(winkel * pi / 180);		//berechnen cos(EingangsWinkel)

	*x_out = (int)(gx * fabs(cos_c) + gy * fabs(sin_c) + 0.5);		//berechnen die Größe des Ausgang-Bild 
	*y_out = (int)(gy * fabs(cos_c) + gx * fabs(sin_c) + 0.5);		//

	//Mittelwert berechnen																//W ,H : Breite und Höhe des Eingangsbild
																						//W',H': Breite und Höhe des Ausgangsbild
	float x_m = ((float)gx - 1) / 2 - (*x_out - 1) * cos_c / 2 - (*y_out - 1) * sin_c / 2;		//dx = 0,5W - 0,5W' * cos - 0,5H' * sin 
	float y_m = ((float)gy - 1) / 2 + (*x_out - 1) * sin_c / 2 - (*y_out - 1) * cos_c / 2;		//dy = 0,5H + 0,5W' * sin - 0,5H' * cos

	float koor_x[32];		//bauen Koordinatensystem für AusgangsBild
	float koor_y[32];
	int int_x;
	int int_y;
	int x, y;
	int m;
	if(x_out > y_out)		
		m = *x_out;
	else
		m = *y_out;


	unsigned char **gbild_in = malloc(sizeof(char *) * gy);
	unsigned char *gbild_out_ = malloc(sizeof(unsigned char) * (*x_out * *y_out + 16));
	unsigned char **gbild_out = malloc(sizeof(char *) * *y_out);
	float *koor = malloc(sizeof(float) * (m + 16));
	for(int i = 0 ; i < m + 16 ; i++)
	{
		koor[i] = (float)i;		//bauen Koordinatensystem für EingangsBild 
	}

	//2D-Var
	for(int i = 0 ; i < gy ; i++)
	{
		gbild_in[i] = gbild_in_ + i * gx;
	}
	for(int i = 0 ; i < *y_out ; i++)
	{
		gbild_out[i] = gbild_out_ + i * *x_out;
	}

	//Neon Reg
	float32x4_t f324_x0, f324_x1, f324_x2, f324_x3;
	float32x4_t f324_y, fsin, fcos, fx_m, fy_m;
	float32x4_t fx_out_00, fx_out_01, fx_out_02, fx_out_03;
	float32x4_t fx_out_10, fx_out_11, fx_out_12, fx_out_13;
	float32x4_t fy_out_00, fy_out_01, fy_out_02, fy_out_03;
	float32x4_t fy_out_10, fy_out_11, fy_out_12, fy_out_13;
	
	//laden sin, cos, und Mittelwert
	fsin = vdupq_n_f32(sin_c);
	fcos = vdupq_n_f32(cos_c);
	
	for(y = 0 ; y < *y_out ; y++)
	{

		//x_m = y * sin + dx
		//y_m = y * cos + dy
		float x_m_m = x_m + y * sin_c;
		float y_m_m = y_m + y * cos_c;
		fx_m = vdupq_n_f32(x_m_m);
		fy_m = vdupq_n_f32(y_m_m);


		for(x = 0 ; x < *x_out ; x += 16)
		{
			//laden X-Koordinaten
			f324_x0 = vld1q_f32(koor + x    );
			f324_x1 = vld1q_f32(koor + x + 4);
			f324_x2 = vld1q_f32(koor + x + 8);
			f324_x3 = vld1q_f32(koor + x + 12);

			//X: out1 = dx + X * cos 
			fx_out_10 = vmlaq_f32(fx_m, f324_x0, fcos);
			fx_out_11 = vmlaq_f32(fx_m, f324_x1, fcos);
			fx_out_12 = vmlaq_f32(fx_m, f324_x2, fcos);
			fx_out_13 = vmlaq_f32(fx_m, f324_x3, fcos);

			//Y: out1 = dy - X * sin 
			fy_out_10 = vmlsq_f32(fy_m, f324_x0, fsin);
			fy_out_11 = vmlsq_f32(fy_m, f324_x1, fsin);
			fy_out_12 = vmlsq_f32(fy_m, f324_x2, fsin);
			fy_out_13 = vmlsq_f32(fy_m, f324_x3, fsin);

			//zurückschreiben
			vst1q_f32(&koor_x[0], fx_out_10);
			vst1q_f32(&koor_x[4], fx_out_11);
			vst1q_f32(&koor_x[8], fx_out_12);
			vst1q_f32(&koor_x[12], fx_out_13);

			vst1q_f32(&koor_y[0], fy_out_10);
			vst1q_f32(&koor_y[4], fy_out_11);
			vst1q_f32(&koor_y[8], fy_out_12);
			vst1q_f32(&koor_y[12], fy_out_13);


			for(int i = 0 ; i < 16 ; i++)
			{
				
				int_x = (int)(round(koor_x[i]));		//nächsten Koordinate
				int_y = (int)(round(koor_y[i]));

				if(int_x >= 0 && int_y >= 0 && int_x < gx && int_y < gy)		//wenn die Koordinaten sind im originalen Bild, zurückschreiben
					gbild_out[y][x + i] = gbild_in[int_y][int_x];
				else
					gbild_out[y][x + i] = 0;									//sonst schreiben 255 (weiß)
				//if(x + i == 416)
					//printf("%d 	%d 	%d 	%d 	%f 	%f 	%d\n", x + i, y, int_x, int_y, koor_x[i], koor_y[i], gbild_out[y][x + i]);
				
			}
		}
	}
	/*
	for(int i = 0 ; i < *y_out ; i++)
	{
		for(int ii = 0 ; ii < *x_out ; ii++)
		{
			printf("%d 	",gbild_out[i][ii] );
		}
		printf("\n");
	}
	*/
	//printf("%p\n",gbild_out_ );
	free(gbild_in);
	free(gbild_out);
	free(koor);
	return(gbild_out_);		//Zeiger rückgeben
}