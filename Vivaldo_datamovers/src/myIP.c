#include "myLib.h"

#pragma SDS data copy(data0[0:dim*dim], data1[0:dim*size], data2[0:dim*size])
#pragma SDS data access_pattern(data0:SEQUENTIAL, data1:SEQUENTIAL, data2:SEQUENTIAL)
#pragma SDS data mem_attribute(data0:PHYSICAL_CONTIGUOUS, data1:PHYSICAL_CONTIGUOUS, data2:PHYSICAL_CONTIGUOUS)
#pragma SDS data data_mover(data0:AXIDMA_SIMPLE, data1:AXIDMA_SG, data2:AXIDMA_SG)
#pragma SDS data sys_port(data0:ps7_S_AXI_HP0, data1:ps7_S_AXI_HP0, data2:ps7_S_AXI_HP0)
void myFuncAccel (unsigned int size, unsigned int dim, dataType_t threshold, dataType_t * data0, dataType_t * data1, dataType_t * data2)
{
	unsigned int CUR_DIM = 4;

	unsigned int i, k, l;

	dataType_t data0_cache [4*4];
	dataType_t data2_tmp [4];

	for ( i = 0; i < CUR_DIM * CUR_DIM; i ++ )
	{
		data0_cache[i] = data0[i];
	}

	for ( i = 0 ; i < size ; i ++ )
	{
#pragma HLS PIPELINE II=4
#pragma HLS loop_tripcount min=1000 max=1000 avg=1000
		dataType_t data1_cache [4];
		for (k = 0; k<CUR_DIM; k++)
		{
			data1_cache[k] = data1[i*CUR_DIM+k];
		}

		int r = 1;
		for ( k = 0 ; k < CUR_DIM ; k ++ )
		{
			dataType_t acc = 0.0;
			for ( l = 0 ; l < CUR_DIM ; l ++ )
			{
				acc += data0_cache [k * CUR_DIM + l] * data1_cache[l];
			}
			r = ( acc > threshold ) && r;
			data2_tmp [ k ] = acc;
		}


		for ( l = 0 ; l < CUR_DIM ; l ++ )
		{
			if ( r )
			{
				data2 [ i*CUR_DIM + l ] = 0.0;
			}
			else
			{
				data2 [ i*CUR_DIM + l ] = data2_tmp[l];
			}
		}
	}
}
