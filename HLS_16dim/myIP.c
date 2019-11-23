#include "myLib.h"

void myFuncAccel (unsigned int size, unsigned int dim, dataType_t threshold, dataType_t * data0, dataType_t * data1, dataType_t * data2)
{
#pragma HLS INTERFACE axis port=data0 depth=256// 5. For simulation only
#pragma HLS INTERFACE axis port=data1 depth=16000
#pragma HLS INTERFACE axis port=data2 depth=16000

	unsigned int CUR_SIZE = 1000;
	unsigned int CUR_DIM = 16;

	unsigned int i, k, l;

	dataType_t data0_cache [256];
	dataType_t data2_tmp [16];

	for ( i = 0; i < CUR_DIM * CUR_DIM; i ++ )
	{
		data0_cache[i] = data0[i];
	}

	for ( i = 0 ; i < CUR_SIZE ; i ++ )
	{
#pragma HLS PIPELINE II=16
#pragma HLS loop_tripcount min=1000 max=1000 avg=1000
		dataType_t data1_cache [16];
		for (k = 0; k<CUR_DIM; k++)
		{
#pragma HLS LOOP_TRIPCOUNT min=16 max=16 avg=16
			data1_cache[k] = data1[i*CUR_DIM+k];
		}

		int r = 1;
		for ( k = 0 ; k < CUR_DIM ; k ++ )
		{
#pragma HLS LOOP_TRIPCOUNT min=16 max=16 avg=16
			dataType_t acc = 0.0;
			for ( l = 0 ; l < CUR_DIM ; l ++ )
			{
#pragma HLS LOOP_TRIPCOUNT min=16 max=16 avg=16
				acc += data0_cache [k * CUR_DIM + l] * data1_cache[l];
			}
			r = ( acc > threshold ) && r;
			data2_tmp [ k ] = acc;
		}

		if ( r )
		{
			for ( l = 0 ; l < CUR_DIM ; l ++ )
			{
	#pragma HLS LOOP_TRIPCOUNT min=16 max=16 avg=16
				data2 [ i*CUR_DIM + l ] = 0.0;
			}
		}
		else
		{
			for ( l = 0 ; l < CUR_DIM ; l ++ )
			{
#pragma HLS LOOP_TRIPCOUNT min=16 max=16 avg=16
				data2 [ i*CUR_DIM + l ] = data2_tmp[l];
			}
		}
	}
}
