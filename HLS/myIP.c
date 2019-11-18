#include "myLib.h"

void myFuncAccel (unsigned int size, unsigned int dim, dataType_t threshold, dataType_t * data0, dataType_t * data1, dataType_t * data2)
{
#pragma HLS INTERFACE axis port=data0 depth=16// 5. For simulation only
#pragma HLS INTERFACE axis port=data1 depth=4000
#pragma HLS INTERFACE axis port=data2 depth=4000

	unsigned int CUR_SIZE = 1000;
	unsigned int CUR_DIM = 4;

	unsigned int i, k, l;

	dataType_t test [16];
	dataType_t data2_tmp [4];

	for ( i = 0; i < CUR_DIM * CUR_DIM; i ++ )
	{
		test[i] = data0[i];
	}

	for ( i = 0 ; i < CUR_SIZE ; i ++ )
	{
#pragma HLS PIPELINE
#pragma HLS loop_tripcount min=1000 max=1000 avg=1000
		int r = 1;
#pragma HLS dependence variable=r intra false
		for ( k = 0 ; k < CUR_DIM ; k ++ )
		{
//#pragma HLS PIPELINE rewind
#pragma HLS LOOP_TRIPCOUNT min=4 max=4 avg=4
			dataType_t acc = 0.0;
			for ( l = 0 ; l < CUR_DIM ; l ++ )
			{
#pragma HLS LOOP_TRIPCOUNT min=4 max=4 avg=4
				acc += test [ k * CUR_DIM + l ] * data1 [ i*CUR_DIM+ l ];
			}
			r = ( acc > threshold ) && r;
			data2_tmp [ k ] = acc;
		}

		if ( r )
		{
			for ( l = 0 ; l < CUR_DIM ; l ++ )
			{
	#pragma HLS LOOP_TRIPCOUNT min=4 max=4 avg=4
				data2 [ i*CUR_DIM + l ] = 0.0;
			}
		}
		else
		{
			for ( l = 0 ; l < CUR_DIM ; l ++ )
			{
#pragma HLS LOOP_TRIPCOUNT min=4 max=4 avg=4
				data2 [ i*CUR_DIM + l ] = data2_tmp[l];
			}
		}
	}
}
