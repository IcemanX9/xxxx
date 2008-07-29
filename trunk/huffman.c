#include "datatype.h"
#include "jdatatype.h"
#include "config.h"
#include "prototype.h"
#include "huffdata.h"

#define PUTBITS { bits_in_next_word = (INT16) (bitindex + numbits - 32);if (bits_in_next_word < 0) { lcode = (lcode << numbits) | data;	bitindex += numbits;} else{lcode = (lcode << (32 - bitindex)) | (data >> bits_in_next_word);if ((*output_ptr++ = (UINT8)(lcode >> 24)) == 0xff) *output_ptr++ = 0; if ((*output_ptr++ = (UINT8)(lcode >> 16)) == 0xff)*output_ptr++ = 0; if ((*output_ptr++ = (UINT8)(lcode >> 8)) == 0xff) *output_ptr++ = 0;if ((*output_ptr++ = (UINT8) lcode) == 0xff) *output_ptr++ = 0; lcode = data; bitindex = bits_in_next_word; }}

UINT8* huffman (JPEG_ENCODER_STRUCTURE *jpeg_encoder_structure, UINT16 component, UINT8 *output_ptr)
{
	//UINT16 i;
	UINT32 i;
	UINT16 *DcCodeTable, *DcSizeTable, *AcCodeTable, *AcSizeTable;

	//INT16 *Temp_Ptr, Coeff, LastDc;
	INT16 *Temp_Ptr;
	INT32 Coeff, LastDc;
	//UINT16 AbsCoeff, HuffCode, HuffSize, RunLength=0, DataSize=0, index;
	UINT32 AbsCoeff, HuffCode, HuffSize, RunLength=0, DataSize=0, index;

	//INT16 bits_in_next_word;
	INT32 bits_in_next_word;
	//UINT16 numbits;
	UINT32 numbits;
	UINT32 data;

	Temp_Ptr = Temp;
	Coeff = *Temp_Ptr++;

	if (component == 1)
	{
		DcCodeTable = luminance_dc_code_table;
		DcSizeTable = luminance_dc_size_table;
		AcCodeTable = luminance_ac_code_table;
		AcSizeTable = luminance_ac_size_table;

		LastDc = jpeg_encoder_structure->ldc1;
		jpeg_encoder_structure->ldc1 = Coeff;
	}
	else
	{
		DcCodeTable = chrominance_dc_code_table;
		DcSizeTable = chrominance_dc_size_table;
		AcCodeTable = chrominance_ac_code_table;
		AcSizeTable = chrominance_ac_size_table;

		if (component == 2)
		{
			LastDc = jpeg_encoder_structure->ldc2;
			jpeg_encoder_structure->ldc2 = Coeff;
		}
		else
		{
			LastDc = jpeg_encoder_structure->ldc3;
			jpeg_encoder_structure->ldc3 = Coeff;
		}
	}

	Coeff -= LastDc;

	AbsCoeff = (Coeff < 0) ? -Coeff-- : Coeff;

	while (AbsCoeff != 0)
	{
		AbsCoeff >>= 1;
		DataSize++;
	}

	HuffCode = DcCodeTable [DataSize];
	HuffSize = DcSizeTable [DataSize];

	Coeff &= (1 << DataSize) - 1;
	data = (HuffCode << DataSize) | Coeff;
	numbits = HuffSize + DataSize;

	PUTBITS

	for (i=63; i>0; i--)
	{
		if ((Coeff = *Temp_Ptr++) != 0)
		{
			while (RunLength > 15)
			{
				RunLength -= 16;
				data = AcCodeTable [161];
				numbits = AcSizeTable [161];
				PUTBITS
			}

			AbsCoeff = (Coeff < 0) ? -Coeff-- : Coeff;

			if (AbsCoeff >> 8 == 0)
				DataSize = bitsize [AbsCoeff];
			else
				DataSize = bitsize [AbsCoeff >> 8] + 8;

			index = RunLength * 10 + DataSize;
			HuffCode = AcCodeTable [index];
			HuffSize = AcSizeTable [index];

			Coeff &= (1 << DataSize) - 1;
			data = (HuffCode << DataSize) | Coeff;
			numbits = HuffSize + DataSize;

			PUTBITS
			RunLength = 0;
		}
		else
			RunLength++;
	}

	if (RunLength != 0)
	{
		data = AcCodeTable [0];
		numbits = AcSizeTable [0];
		PUTBITS
	}
	return output_ptr;
}

/* For bit Stuffing and EOI marker */
UINT8* close_bitstream (UINT8 *output_ptr)
{
	UINT16 i, count;
	UINT8 *ptr;

	if (bitindex > 0)
	{
		lcode <<= (32 - bitindex);
		count = (bitindex + 7) >> 3;

		ptr = (UINT8 *) &lcode + 3;

		for (i=count; i>0; i--)
		{
			if ((*output_ptr++ = *ptr--) == 0xff)
				*output_ptr++ = 0;
		}
	}

	// End of image marker
	*output_ptr++ = 0xFF;
	*output_ptr++ = 0xD9;
	return output_ptr;
}
