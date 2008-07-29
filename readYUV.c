#include "datatype.h"
#include "jdatatype.h"
#include "config.h"
#include "prototype.h"

void read_400_format (JPEG_ENCODER_STRUCTURE *jpeg_encoder_structure, UINT8 *input_ptr)
{
	INT32 i, j;
	INT16 *Y1_Ptr = Y1;

	UINT16 rows = jpeg_encoder_structure->rows;
	UINT16 cols = jpeg_encoder_structure->cols;
	UINT16 incr = jpeg_encoder_structure->incr;

	for (i=rows; i>0; i--)
	{
		for (j=cols; j>0; j--)
			*Y1_Ptr++ = *input_ptr++;

		for (j=8-cols; j>0; j--)
			*Y1_Ptr++ = *(Y1_Ptr-1);

		input_ptr += incr;
	}

	for (i=8-rows; i>0; i--)
	{
		for (j=8; j>0; j--)
			*Y1_Ptr++ = *(Y1_Ptr - 8);
	}
}

void read_420_format (JPEG_ENCODER_STRUCTURE *jpeg_encoder_structure, UINT8 *input_ptr)
{
	INT32 i, j;
	UINT16 Y1_rows, Y3_rows, Y1_cols, Y2_cols;

	INT16 *Y1_Ptr = Y1;
	INT16 *Y2_Ptr = Y2;
	INT16 *Y3_Ptr = Y3;
	INT16 *Y4_Ptr = Y4;
	INT16 *CB_Ptr = CB;
	INT16 *CR_Ptr = CR;

	INT16 *Y1Ptr = Y1 + 8;
	INT16 *Y2Ptr = Y2 + 8;
	INT16 *Y3Ptr = Y3 + 8;
	INT16 *Y4Ptr = Y4 + 8;

	UINT16 rows = jpeg_encoder_structure->rows;
	UINT16 cols = jpeg_encoder_structure->cols;
	UINT16 incr = jpeg_encoder_structure->incr;

	if (rows <= 8)
	{
		Y1_rows = rows;
		Y3_rows = 0;
	}
	else
	{
		Y1_rows = 8;
		Y3_rows = (UINT16) (rows - 8);
	}

	if (cols <= 8)
	{
		Y1_cols = cols;
		Y2_cols = 0;
	}
	else
	{
		Y1_cols = 8;
		Y2_cols = (UINT16) (cols - 8);
	}

	for (i=Y1_rows>>1; i>0; i--)
	{
		for (j=Y1_cols>>1; j>0; j--)
		{
			*Y1_Ptr++ = *input_ptr++;
			*Y1_Ptr++ = *input_ptr++;
			*Y1Ptr++ = *input_ptr++;
			*Y1Ptr++ = *input_ptr++;
			*CB_Ptr++ = *input_ptr++;
			*CR_Ptr++ = *input_ptr++;
		}

		for (j=Y2_cols>>1; j>0; j--)
		{
			*Y2_Ptr++ = *input_ptr++;
			*Y2_Ptr++ = *input_ptr++;
			*Y2Ptr++ = *input_ptr++;
			*Y2Ptr++ = *input_ptr++;
			*CB_Ptr++ = *input_ptr++;
			*CR_Ptr++ = *input_ptr++;
		}

		if (cols <= 8)
		{
			for (j=8-Y1_cols; j>0; j--)
			{
				*Y1_Ptr++ = *(Y1_Ptr - 1);
				*Y1Ptr++ = *(Y1Ptr - 1);
			}

			for (j=8; j>0; j--)
			{
				*Y2_Ptr++ = *(Y1_Ptr - 1);
				*Y2Ptr++ = *(Y1Ptr - 1);
			}
		}
		else
		{
			for (j=8-Y2_cols; j>0; j--)
			{
				*Y2_Ptr++ = *(Y2_Ptr - 1);
				*Y2Ptr++ = *(Y2Ptr - 1);
			}
		}

		for (j=(16-cols)>>1; j>0; j--)
		{
			*CB_Ptr++ = *(CB_Ptr-1);
			*CR_Ptr++ = *(CR_Ptr-1);
		}

		Y1_Ptr += 8;
		Y2_Ptr += 8;
		Y1Ptr += 8;
		Y2Ptr += 8;

		input_ptr += incr;
	}

	for (i=Y3_rows>>1; i>0; i--)
	{
		for (j=Y1_cols>>1; j>0; j--)
		{
			*Y3_Ptr++ = *input_ptr++;
			*Y3_Ptr++ = *input_ptr++;
			*Y3Ptr++ = *input_ptr++;
			*Y3Ptr++ = *input_ptr++;
			*CB_Ptr++ = *input_ptr++;
			*CR_Ptr++ = *input_ptr++;
		}

		for (j=Y2_cols>>1; j>0; j--)
		{
			*Y4_Ptr++ = *input_ptr++;
			*Y4_Ptr++ = *input_ptr++;
			*Y4Ptr++ = *input_ptr++;
			*Y4Ptr++ = *input_ptr++;
			*CB_Ptr++ = *input_ptr++;
			*CR_Ptr++ = *input_ptr++;
		}

		if (cols <= 8)
		{
			for (j=8-Y1_cols; j>0; j--)
			{
				*Y3_Ptr++ = *(Y3_Ptr - 1);
				*Y3Ptr++ = *(Y3Ptr - 1);
			}

			for (j=8; j>0; j--)
			{
				*Y4_Ptr++ = *(Y3_Ptr - 1);
				*Y4Ptr++ = *(Y3Ptr - 1);
			}
		}
		else
		{
			for (j=8-Y2_cols; j>0; j--)
			{
				*Y4_Ptr++ = *(Y4_Ptr - 1);
				*Y4Ptr++ = *(Y4Ptr - 1);
			}
		}

		for (j=(16-cols)>>1; j>0; j--)
		{
			*CB_Ptr++ = *(CB_Ptr-1);
			*CR_Ptr++ = *(CR_Ptr-1);
		}

		Y3_Ptr += 8;
		Y4_Ptr += 8;
		Y3Ptr += 8;
		Y4Ptr += 8;

		input_ptr += incr;
	}

	if (rows <= 8)
	{
		for (i=8-rows; i>0; i--)
		{
			for (j=8; j>0; j--)
			{
				*Y1_Ptr++ = *(Y1_Ptr - 8);
				*Y2_Ptr++ = *(Y2_Ptr - 8);
			}
		}

		for (i=8; i>0; i--)
		{
			Y1_Ptr -= 8;
			Y2_Ptr -= 8;

			for (j=8; j>0; j--)
			{
				*Y3_Ptr++ = *Y1_Ptr++;
				*Y4_Ptr++ = *Y2_Ptr++;
			}
		}
	}
	else
	{
		for (i=(16-rows); i>0; i--)
		{
			for (j=8; j>0; j--)
			{
				*Y3_Ptr++ = *(Y3_Ptr - 8);
				*Y4_Ptr++ = *(Y4_Ptr - 8);
			}
		}
	}

	for (i=((16-rows)>>1); i>0; i--)
	{
		for (j=8; j>0; j--)
		{
			*CB_Ptr++ = *(CB_Ptr-8);
			*CR_Ptr++ = *(CR_Ptr-8);
		}
	}
}

void read_422_format (JPEG_ENCODER_STRUCTURE *jpeg_encoder_structure, UINT8 *input_ptr)
{
	INT32 i, j;
	UINT16 Y1_cols, Y2_cols;

	INT16 *Y1_Ptr = Y1;
	INT16 *Y2_Ptr = Y2;
	INT16 *CB_Ptr = CB;
	INT16 *CR_Ptr = CR;

	UINT16 rows = jpeg_encoder_structure->rows;
	UINT16 cols = jpeg_encoder_structure->cols;
	UINT16 incr = jpeg_encoder_structure->incr;

	if (cols <= 8)
	{
		Y1_cols = cols;
		Y2_cols = 0;
	}
	else
	{
		Y1_cols = 8;
		Y2_cols = (UINT16) (cols - 8);
	}

	for (i=rows; i>0; i--)
	{
		for (j=Y1_cols>>1; j>0; j--)
		{
			*Y1_Ptr++ = *input_ptr++;
			*CB_Ptr++ = *input_ptr++;
			*Y1_Ptr++ = *input_ptr++;
			*CR_Ptr++ = *input_ptr++;
		}

		for (j=Y2_cols>>1; j>0; j--)
		{
			*Y2_Ptr++ = *input_ptr++;
			*CB_Ptr++ = *input_ptr++;
			*Y2_Ptr++ = *input_ptr++;
			*CR_Ptr++ = *input_ptr++;
		}

		if (cols <= 8)
		{
			for (j=8-Y1_cols; j>0; j--)
				*Y1_Ptr++ = *(Y1_Ptr - 1);

			for (j=8-Y2_cols; j>0; j--)
				*Y2_Ptr++ = *(Y1_Ptr - 1);
		}
		else
		{
			for (j=8-Y2_cols; j>0; j--)
				*Y2_Ptr++ = *(Y2_Ptr - 1);
		}

		for (j=(16-cols)>>1; j>0; j--)
		{
			*CB_Ptr++ = *(CB_Ptr-1);
			*CR_Ptr++ = *(CR_Ptr-1);
		}

		input_ptr += incr;
	}

	for (i=8-rows; i>0; i--)
	{
		for (j=8; j>0; j--)
		{
			*Y1_Ptr++ = *(Y1_Ptr - 8);
			*Y2_Ptr++ = *(Y2_Ptr - 8);
			*CB_Ptr++ = *(CB_Ptr - 8);
			*CR_Ptr++ = *(CR_Ptr - 8);
		}
	}
}

void read_444_format (JPEG_ENCODER_STRUCTURE *jpeg_encoder_structure, UINT8 *input_ptr)
{
	INT32 i, j;
	INT16 *Y1_Ptr = Y1;
	INT16 *CB_Ptr = CB;
	INT16 *CR_Ptr = CR;

	UINT16 rows = jpeg_encoder_structure->rows;
	UINT16 cols = jpeg_encoder_structure->cols;
	UINT16 incr = jpeg_encoder_structure->incr;

	for (i=rows; i>0; i--)
	{
		for (j=cols; j>0; j--)
		{
			*Y1_Ptr++ = *input_ptr++;
			*CB_Ptr++ = *input_ptr++;
			*CR_Ptr++ = *input_ptr++;
		}

		for (j=8-cols; j>0; j--)
		{
			*Y1_Ptr++ = *(Y1_Ptr-1);
			*CB_Ptr++ = *(CB_Ptr-1);
			*CR_Ptr++ = *(CR_Ptr-1);
		}

		input_ptr += incr;
	}

	for (i=8-rows; i>0; i--)
	{
		for (j=8; j>0; j--)
		{
			*Y1_Ptr++ = *(Y1_Ptr - 8);
			*CB_Ptr++ = *(CB_Ptr - 8);
			*CR_Ptr++ = *(CR_Ptr - 8);
		}
	}
}

void RGB_2_444 (UINT8 *input_ptr, UINT8 *output_ptr, UINT32 image_width, UINT32 image_height)
{
	UINT32 i, size;
	UINT8 R, G, B;
	INT32 Y, Cb, Cr;

	size = image_width * image_height;

	for (i=size; i>0; i--)
	{
		B = *input_ptr++;
		G = *input_ptr++;
		R = *input_ptr++;

		Y = ((77 * R + 150 * G + 29 * B) >> 8);
		Cb = ((-43 * R - 85 * G + 128 * B) >> 8) + 128;
		Cr = ((128 * R - 107 * G - 21 * B) >> 8) + 128;

		if (Y < 0)
			Y = 0;
		else if (Y > 255)
			Y = 255;

		if (Cb < 0)
			Cb = 0;
		else if (Cb > 255)
			Cb = 255;

		if (Cr < 0)
			Cr = 0;
		else if (Cr > 255)
			Cr = 255;

		*output_ptr++ = (UINT8) Y;
		*output_ptr++ = (UINT8) Cb;
		*output_ptr++ = (UINT8) Cr;
	}
}

#if 1
#define Y_SZ (8*8)
INT16 Y1_tmp[Y_SZ];
INT16 Y2_tmp[Y_SZ];
INT16 Y3_tmp[Y_SZ];
INT16 Y4_tmp[Y_SZ];
INT16 CB_tmp[Y_SZ];
INT16 CR_tmp[Y_SZ];

void read_ok(UINT8 *input_ptr, UINT8 *in_cb, UINT8 *in_cr)
{
	INT32 i, j;
#if 0
	INT16 *Y1_Ptr = Y1;
	INT16 *Y2_Ptr = Y2;
	INT16 *Y3_Ptr = Y3;
	INT16 *Y4_Ptr = Y4;
	INT16 *CB_Ptr = CB;
	INT16 *CR_Ptr = CR;
#else
	INT16 *Y1_Ptr = Y1_tmp;
	INT16 *Y2_Ptr = Y2_tmp;
	INT16 *Y3_Ptr = Y3_tmp;
	INT16 *Y4_Ptr = Y4_tmp;
	INT16 *CB_Ptr = CB_tmp;
	INT16 *CR_Ptr = CR_tmp;
#endif
	for (i=0; i<8; i++) {
		for (j=0; j<8; j++) {
			*Y1_Ptr++ = *input_ptr++;
		}
		for (j=0; j<8; j++) {
			*Y2_Ptr++ = *input_ptr++;
		}
	}

	for (i=0; i<8; i++) {
		for (j=0; j<8; j++) {
			*Y3_Ptr++ = *input_ptr++;
		}
		for (j=0; j<8; j++) {
			*Y4_Ptr++ = *input_ptr++;
		}
	}

	for (i=0; i<8; i++) {
		for (j=0; j<8; j++) {
			*CB_Ptr++ = *in_cb++;
			*CR_Ptr++ = *in_cr++;
		}
	}
}


inline void read_420_format_rogers (JPEG_ENCODER_STRUCTURE *jpeg_encoder_structure, UINT8 *input_ptr, UINT8 *in_cb, UINT8 *in_cr, UINT16 width, UINT16 height)
{
	//printf("out Y1 0x%x Y2 0x%x Y3 0x%x Y4 0x%x CB 0x%x CR 0x%x\n",Y1,Y2,Y3,Y4,CB,CR);
	//printf("outmp Y1 0x%x Y2 0x%x Y3 0x%x Y4 0x%x CB 0x%x CR 0x%x\n",Y1_tmp,Y2_tmp,Y3_tmp,Y4_tmp,CB_tmp,CR_tmp);
	//printf("input ptr 0x%x inCB 0x%x inCR 0x%x\n",input_ptr,in_cb,in_cr);
	translate_yuv((unsigned int)input_ptr,(unsigned int)in_cb,(unsigned int)in_cr);
}

#else
void read_420_format_rogers (JPEG_ENCODER_STRUCTURE *jpeg_encoder_structure, UINT8 *input_ptr, UINT8 *in_cb, UINT8 *in_cr, UINT16 width, UINT16 height)
{
	INT32 i, j;

	INT16 *Y1_Ptr = Y1;
	INT16 *Y2_Ptr = Y2;
	INT16 *Y3_Ptr = Y3;
	INT16 *Y4_Ptr = Y4;
	INT16 *CB_Ptr = CB;
	INT16 *CR_Ptr = CR;

//printf("out Y1 0x%x Y2 0x%x Y3 0x%x Y4 0x%x CB 0x%x CR 0x%x\n",Y1,Y2,Y3,Y4,CB,CR);
//printf("input ptr 0x%x inCB 0x%x inCR 0x%x\n",input_ptr,in_cb,in_cr);
	
#if 0
	{
		unsigned char *inputY1,*inputY2,*inputY3,*inputY4;
		unsigned short *Y1_16=Y1;
		unsigned short *Y2_16=Y2;
			unsigned short *Y3_16=Y3;
		unsigned short *Y4_16=Y4;
		unsigned short *CB_16=CB;
		unsigned short *CR_16=CR;
		unsigned char *inputCB=in_cb;
		unsigned char *inputCR=in_cr;
		
		for (i=0; i<8; i++) 
		{
			inputY1=input_ptr+(i*16);
			inputY2=inputY1+8;
			inputY3=inputY1+(8*16);
			inputY4=inputY3+8;

			for (j=0; j<8; j++) 
			{
				*Y1_16++ = *inputY1++;
				*Y2_16++ = *inputY2++;
				*Y3_16++ = *inputY3++;
				*Y4_16++ = *inputY4++;
				
				*CB_16++ = *inputCB++;
				*CR_16++ = *inputCR++;
			}
		}
	//	printf("Last y1 0x%x y2 0x%x y3 0x%x y4 0x%x cb 0x%x cr 0x%x\n",Y1_16,Y2_16,Y3_16,Y4_16,CB_16,CR_16);
		//printf("  in Y1 0x%x Y2 0x%x Y3 0x%x Y4 0x%x CB 0x%x CR 0x%x\n",inputY1,inputY2,inputY3,inputY4,inputCB,inputCR);
	}
#else
	for (i=0; i<8; i++) {
		for (j=0; j<8; j++) {
			*Y1_Ptr++ = *input_ptr++;
		}
		for (j=0; j<8; j++) {
			*Y2_Ptr++ = *input_ptr++;
		}
	}

//printf("y1 0x%x y2 0x%x input_ptr 0x%x\n",Y1_Ptr,Y2_Ptr,input_ptr);	
	for (i=0; i<8; i++) {
		for (j=0; j<8; j++) {
			*Y3_Ptr++ = *input_ptr++;
		}
		for (j=0; j<8; j++) {
			*Y4_Ptr++ = *input_ptr++;
		}
	}

//printf("y3 0x%x y4 0x%x input_ptr 0x%x\n",Y3_Ptr,Y4_Ptr,input_ptr);
	for (i=0; i<8; i++) {
		for (j=0; j<8; j++) {
			*CB_Ptr++ = *in_cb++;
			*CR_Ptr++ = *in_cr++;
		}
	}
	
//printf("CB 0x%x CR 0x%x input_cb 0x%x input_cr 0x%x\n",CB_Ptr,CR_Ptr,in_cb,in_cr);
#endif
}
#endif