

//************************************************************************************
//
//
//			32-bit-tief Bild transposition
//
//
//
//************************************************************************************

void tran_32(uint32_t *bildcol_, uint32_t *bildcol_t_, size_t gx, size_t gy)
{
	//2D-var für originales und getransponiertes Bild
	uint32_t **bildcol = malloc(sizeof(char *) * gy);
	uint32_t **bildcol_t = malloc(sizeof(char *) * gx);
	for(int i = 0 ; i < gy ; i++)
	{
		bildcol[i] = bildcol_ + i * gx;
	}
	for(int i = 0 ; i < gx ; i++)
	{
		bildcol_t[i] = bildcol_t_ + i * gy;
	}

	//Neon-Register definieren
	uint32x2x2_t reg3222_0, reg3222_1, reg3222_2, reg3222_3, reg3222_4, reg3222_5, reg3222_6, reg3222_7;
	uint32x2x2_t reg3222_8, reg3222_9, reg3222_10, reg3222_11, reg3222_12, reg3222_13, reg3222_14, reg3222_15;

	int gx_r = gx % 16;
	int gy_r = gy % 2;
	int gx_l = gx - 15;
	int gy_l = gy - 1;
	int gx_k = gx - gx_r;
	int gy_k = gy - gy_r;
	int x, y; 

	for(y = 0 ; y < gy_l ; y += 2)
	{
		for(x = 0 ; x < gx_l ; x += 16)
		{
			//laden 2 Reihen Daten
			reg3222_0.val[0] = vld1_u32(&bildcol[y    ][x    ]);
			reg3222_0.val[1] = vld1_u32(&bildcol[y + 1][x    ]);
			reg3222_1.val[0] = vld1_u32(&bildcol[y    ][x + 2]);
			reg3222_1.val[1] = vld1_u32(&bildcol[y + 1][x + 2]);
			reg3222_2.val[0] = vld1_u32(&bildcol[y    ][x + 4]);
			reg3222_2.val[1] = vld1_u32(&bildcol[y + 1][x + 4]);
			reg3222_3.val[0] = vld1_u32(&bildcol[y    ][x + 6]);
			reg3222_3.val[1] = vld1_u32(&bildcol[y + 1][x + 6]);
			reg3222_4.val[0] = vld1_u32(&bildcol[y    ][x + 8]);
			reg3222_4.val[1] = vld1_u32(&bildcol[y + 1][x + 8]);
			reg3222_5.val[0] = vld1_u32(&bildcol[y    ][x + 10]);
			reg3222_5.val[1] = vld1_u32(&bildcol[y + 1][x + 10]);
			reg3222_6.val[0] = vld1_u32(&bildcol[y    ][x + 12]);
			reg3222_6.val[1] = vld1_u32(&bildcol[y + 1][x + 12]);
			reg3222_7.val[0] = vld1_u32(&bildcol[y    ][x + 14]);
			reg3222_7.val[1] = vld1_u32(&bildcol[y + 1][x + 14]);

			//transposition
			reg3222_8 = vtrn_u32(reg3222_0.val[0],reg3222_0.val[1]);
			reg3222_9 = vtrn_u32(reg3222_1.val[0],reg3222_1.val[1]);
			reg3222_10 = vtrn_u32(reg3222_2.val[0],reg3222_2.val[1]);
			reg3222_11 = vtrn_u32(reg3222_3.val[0],reg3222_3.val[1]);
			reg3222_12 = vtrn_u32(reg3222_4.val[0],reg3222_4.val[1]);
			reg3222_13 = vtrn_u32(reg3222_5.val[0],reg3222_5.val[1]);
			reg3222_14 = vtrn_u32(reg3222_6.val[0],reg3222_6.val[1]);
			reg3222_15 = vtrn_u32(reg3222_7.val[0],reg3222_7.val[1]);

			//store
			vst1_u32(&bildcol_t[x    ][y], reg3222_8.val[0]);
			vst1_u32(&bildcol_t[x + 1][y], reg3222_8.val[1]);
			vst1_u32(&bildcol_t[x + 2][y], reg3222_9.val[0]);
			vst1_u32(&bildcol_t[x + 3][y], reg3222_9.val[1]);
			vst1_u32(&bildcol_t[x + 4][y], reg3222_10.val[0]);
			vst1_u32(&bildcol_t[x + 5][y], reg3222_10.val[1]);
			vst1_u32(&bildcol_t[x + 6][y], reg3222_11.val[0]);
			vst1_u32(&bildcol_t[x + 7][y], reg3222_11.val[1]);
			vst1_u32(&bildcol_t[x + 8][y], reg3222_12.val[0]);
			vst1_u32(&bildcol_t[x + 9][y], reg3222_12.val[1]);
			vst1_u32(&bildcol_t[x + 10][y], reg3222_13.val[0]);
			vst1_u32(&bildcol_t[x + 11][y], reg3222_13.val[1]);
			vst1_u32(&bildcol_t[x + 12][y], reg3222_14.val[0]);
			vst1_u32(&bildcol_t[x + 13][y], reg3222_14.val[1]);
			vst1_u32(&bildcol_t[x + 14][y], reg3222_15.val[0]);
			vst1_u32(&bildcol_t[x + 15][y], reg3222_15.val[1]);
		}
	}

	//Rest transponieren
	for(y = gy_k ; y < gy ; y++)
	{
	  for(x = 0 ; x < gx ; x++)
	  {
		bildcol_t[x][y] = bildcol[y][x];
		}
	}
	for(x = gx_k ; x < gx ; x++)
	{    
	  for(y = 0 ; y < gy_k ; y++)
	  {
		bildcol_t[x][y] = bildcol[y][x];
	  }
	}
	free(bildcol);
	free(bildcol_t);
}

//************************************************************************************
//
//
//			16-bit-tief Bild transposition
//
//
//
//************************************************************************************

void tran_16(uint16_t *in_, uint16_t *out_, size_t gx, size_t gy)
{
	uint16_t **in = malloc(sizeof(char *) * gy);
	uint16_t **out = malloc(sizeof(char *) * gy);
	for(int i = 0 ; i < gy ; i++)
	{
		in[i] = in_ + i * gx;
	}
	for(int i = 0 ; i < gx ; i++)
	{
		out[i] = out_ + i * gy;
	}


	uint16x4x2_t reg1642_0, reg1642_1, reg1642_2, reg1642_3;
	uint16x4x2_t reg1642_4, reg1642_5, reg1642_6, reg1642_7;
	uint16x4x2_t reg1642t_0, reg1642t_1, reg1642t_2, reg1642t_3;
	uint32x2x2_t reg3222_0, reg3222_1, reg3222_2, reg3222_3;
	uint32x2x2_t reg3222_4, reg3222_5, reg3222_6, reg3222_7;
	uint32x2x2_t reg3224t_0, reg3224t_1, reg3224t_2, reg3224t_3;

	int gx_r = gx % 8;
	int gy_r = gy % 4;
	int gx_l = gx - 7;
	int gy_l = gy - 3;
	int gx_k = gx - gx_r;
	int gy_k = gy - gy_r;
	int x, y; 

	for(y = 0 ; y < gy_l ; y +=4)
	{
		for(x = 0 ; x < gx_l ; x += 8)
		{
			reg1642_0.val[0] = vld1_u16(&in[y    ][x    ]);
			reg1642_0.val[1] = vld1_u16(&in[y + 1][x    ]);
			reg1642_1.val[0] = vld1_u16(&in[y + 2][x    ]);
			reg1642_1.val[1] = vld1_u16(&in[y + 3][x    ]);
			reg1642_2.val[0] = vld1_u16(&in[y    ][x + 4]);
			reg1642_2.val[1] = vld1_u16(&in[y + 1][x + 4]);
			reg1642_3.val[0] = vld1_u16(&in[y + 2][x + 4]);
			reg1642_3.val[1] = vld1_u16(&in[y + 3][x + 4]);

			reg1642t_0 = vtrn_u16(reg1642_0.val[0], reg1642_0.val[1]);
			reg1642t_1 = vtrn_u16(reg1642_1.val[0], reg1642_1.val[1]);
			reg1642t_2 = vtrn_u16(reg1642_2.val[0], reg1642_2.val[1]);
			reg1642t_3 = vtrn_u16(reg1642_3.val[0], reg1642_3.val[1]);

			reg3222_0.val[0] = vreinterpret_u32_u16(reg1642t_0.val[0]);
			reg3222_0.val[1] = vreinterpret_u32_u16(reg1642t_0.val[1]);
			reg3222_1.val[0] = vreinterpret_u32_u16(reg1642t_1.val[0]);
			reg3222_1.val[1] = vreinterpret_u32_u16(reg1642t_1.val[1]);
			reg3222_2.val[0] = vreinterpret_u32_u16(reg1642t_2.val[0]);
			reg3222_2.val[1] = vreinterpret_u32_u16(reg1642t_2.val[1]);
			reg3222_3.val[0] = vreinterpret_u32_u16(reg1642t_3.val[0]);
			reg3222_3.val[1] = vreinterpret_u32_u16(reg1642t_3.val[1]);

			reg3224t_0 = vtrn_u32(reg3222_0.val[0], reg3222_1.val[0]);
			reg3224t_1 = vtrn_u32(reg3222_0.val[1], reg3222_1.val[1]);
			reg3224t_2 = vtrn_u32(reg3222_2.val[0], reg3222_3.val[0]);
			reg3224t_3 = vtrn_u32(reg3222_2.val[1], reg3222_3.val[1]);

			reg1642_0.val[0] = vreinterpret_u16_u32(reg3224t_0.val[0]);
			reg1642_0.val[1] = vreinterpret_u16_u32(reg3224t_0.val[1]);
			reg1642_1.val[0] = vreinterpret_u16_u32(reg3224t_1.val[0]);
			reg1642_1.val[1] = vreinterpret_u16_u32(reg3224t_1.val[1]);
			reg1642_2.val[0] = vreinterpret_u16_u32(reg3224t_2.val[0]);
			reg1642_2.val[1] = vreinterpret_u16_u32(reg3224t_2.val[1]);
			reg1642_3.val[0] = vreinterpret_u16_u32(reg3224t_3.val[0]);
			reg1642_3.val[1] = vreinterpret_u16_u32(reg3224t_3.val[1]);

			vst1_u16(&out[x    ][y], reg1642_0.val[0]);
			vst1_u16(&out[x + 1][y], reg1642_1.val[0]);
			vst1_u16(&out[x + 2][y], reg1642_0.val[1]);
			vst1_u16(&out[x + 3][y], reg1642_1.val[1]);
			vst1_u16(&out[x + 4][y], reg1642_2.val[0]);
			vst1_u16(&out[x + 5][y], reg1642_3.val[0]);
			vst1_u16(&out[x + 6][y], reg1642_2.val[1]);
			vst1_u16(&out[x + 7][y], reg1642_3.val[1]);
		}
	}

	for(y = gy_k ; y < gy ; y++)
	{
	  for(x = 0 ; x < gx ; x++)
	  {
			out[x][y] = in[y][x];
		}
	}
	for(x = gx_k ; x < gx ; x++)
	{    
	  for(y = 0 ; y < gy_k ; y++)
	  {
			out[x][y] = in[y][x];
	  }
	}
	free(in);
	free(out);
}

//************************************************************************************
//
//
//			8-bit-tief Bild transposition
//
//
//
//************************************************************************************

void tran_8(png_bytep gbild_, png_bytep gbild_t_, size_t gx, size_t gy)
{
	//2D-var für originales und getransponiertes Bild
	png_bytep *gbild = malloc(sizeof(char *) * gy);
	png_bytep *gbild_t = malloc(sizeof(char *) * gx);

	for(int i = 0 ; i < gy ; i++)
	{
		gbild[i] = gbild_ + i * gx;
	}
	for(int i = 0 ; i < gx ; i++)
	{
		gbild_t[i] = gbild_t_ + i * gy;
	}


	//Neon-Register definieren
	uint8x8x2_t reg882_0, reg882_1, reg882_2, reg882_3;
	uint16x4x2_t reg1642_0, reg1642_1, reg1642_2, reg1642_3;
	uint32x2x2_t reg3222_0, reg3222_1, reg3222_2, reg3222_3;
	int gx_r = gx % 8;
	int gy_r = gy % 8;
	int gx_l = gx - 7;
	int gy_l = gy - 7;
	int gx_k = gx - gx_r;
	int gy_k = gy - gy_r;
	int x, y;


	for(y = 0 ; y < gy_l ; y+=8)
	{
		for(x = 0 ; x < gx_l ; x+=8)
		{
			//laden 8 Reihen Daten
		  reg882_0.val[0] = vld1_u8(&gbild[y][x]);
		  reg882_0.val[1] = vld1_u8(&gbild[y + 1][x]);
		  reg882_1.val[0] = vld1_u8(&gbild[y + 2][x]);
		  reg882_1.val[1] = vld1_u8(&gbild[y + 3][x]);
		  reg882_2.val[0] = vld1_u8(&gbild[y + 4][x]);
		  reg882_2.val[1] = vld1_u8(&gbild[y + 5][x]);
		  reg882_3.val[0] = vld1_u8(&gbild[y + 6][x]);
		  reg882_3.val[1] = vld1_u8(&gbild[y + 7][x]);

		  //je 2 Reihen transponieren
		  reg882_0 = vtrn_u8(reg882_0.val[0], reg882_0.val[1]);
		  reg882_1 = vtrn_u8(reg882_1.val[0], reg882_1.val[1]);
		  reg882_2 = vtrn_u8(reg882_2.val[0], reg882_2.val[1]);
		  reg882_3 = vtrn_u8(reg882_3.val[0], reg882_3.val[1]);

		  //8-bit-tief -> 16-bit-tief, dann 1 und 3 Reihe, 2 und 4 Reihen, 5 und 7 Reihen, 6 und 8 Reihen transponieren
		  reg1642_0 = vtrn_u16(vreinterpret_u16_u8(reg882_0.val[0]), vreinterpret_u16_u8(reg882_1.val[0]));
		  reg1642_1 = vtrn_u16(vreinterpret_u16_u8(reg882_0.val[1]), vreinterpret_u16_u8(reg882_1.val[1]));
		  reg1642_2 = vtrn_u16(vreinterpret_u16_u8(reg882_2.val[0]), vreinterpret_u16_u8(reg882_3.val[0]));
		  reg1642_3 = vtrn_u16(vreinterpret_u16_u8(reg882_2.val[1]), vreinterpret_u16_u8(reg882_3.val[1]));

		  //16-bit-tief -> 32-bit-tief, dann 1 und 5 Reihen, 2 und 6 Reihen, 3 und 7 Reihen, 4 und 8 Reihen transpinieren
		  reg3222_0 = vtrn_u32(vreinterpret_u32_u16(reg1642_0.val[0]), vreinterpret_u32_u16(reg1642_2.val[0]));
		  reg3222_1 = vtrn_u32(vreinterpret_u32_u16(reg1642_0.val[1]), vreinterpret_u32_u16(reg1642_2.val[1]));
		  reg3222_2 = vtrn_u32(vreinterpret_u32_u16(reg1642_1.val[0]), vreinterpret_u32_u16(reg1642_3.val[0]));
		  reg3222_3 = vtrn_u32(vreinterpret_u32_u16(reg1642_1.val[1]), vreinterpret_u32_u16(reg1642_3.val[1]));

		  //32-bit-tief -> 8-bit-tief
		  reg882_0.val[0] = vreinterpret_u8_u32(reg3222_0.val[0]);
		  reg882_0.val[1] = vreinterpret_u8_u32(reg3222_0.val[1]);
		  reg882_1.val[0] = vreinterpret_u8_u32(reg3222_1.val[0]);
		  reg882_1.val[1] = vreinterpret_u8_u32(reg3222_1.val[1]);
		  reg882_2.val[0] = vreinterpret_u8_u32(reg3222_2.val[0]);
		  reg882_2.val[1] = vreinterpret_u8_u32(reg3222_2.val[1]);
		  reg882_3.val[0] = vreinterpret_u8_u32(reg3222_3.val[0]);
		  reg882_3.val[1] = vreinterpret_u8_u32(reg3222_3.val[1]);

		  //store
		  vst1_u8(&gbild_t[x    ][y], reg882_0.val[0]);
		  vst1_u8(&gbild_t[x + 1][y], reg882_2.val[0]);
		  vst1_u8(&gbild_t[x + 2][y], reg882_1.val[0]);
		  vst1_u8(&gbild_t[x + 3][y], reg882_3.val[0]);
		  vst1_u8(&gbild_t[x + 4][y], reg882_0.val[1]);
		  vst1_u8(&gbild_t[x + 5][y], reg882_2.val[1]);
		  vst1_u8(&gbild_t[x + 6][y], reg882_1.val[1]);
		  vst1_u8(&gbild_t[x + 7][y], reg882_3.val[1]);
	  }
	}

	//Rest transponieren
	for(y = gy_k ; y < gy ; y++)
	{
	  for(x = 0 ; x < gx ; x++)
	  {
		gbild_t[x][y] = gbild[y][x];
		}
	}
	for(x = gx_k ; x < gx ; x++)
	{    
	  for(y = 0 ; y < gy_k ; y++)
	  {
		gbild_t[x][y] = gbild[y][x];
	  }
	}
	free(gbild);
	free(gbild_t);
}