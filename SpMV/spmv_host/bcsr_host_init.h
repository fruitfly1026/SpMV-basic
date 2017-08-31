/*
 *  Copyright 2008-2009 NVIDIA Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */



#pragma once

////////////////////////////////////////////////////////////////////////////////
//! CPU SpMV kernels
////////////////////////////////////////////////////////////////////////////////

#include "../sparse_formats.h"

template <typename IndexType, typename ValueType>
void spmv_bcsr_serial_host_init(const bcsr_matrix<IndexType, ValueType>& bcsr, 
                          const ValueType * x,  
                          ValueType * y)
{
  IndexType num_rows = bcsr.num_rows;
  IndexType num_cols = bcsr.num_cols;
  IndexType num_nonzeros = bcsr.num_nonzeros;
  IndexType row_block_size = bcsr.row_block_size;
  IndexType col_block_size = bcsr.col_block_size;
  IndexType num_block_rows = bcsr.num_block_rows;
  IndexType num_rows_left = bcsr.num_rows_left;
  ValueType *bAx_tmp = bcsr.bAx;

  IndexType I, K, i, j, j0;
#ifdef TESTING
  printf("Printing x .... \n");
  FILE *fp = fopen("x_value","w");
  fprintf(fp, "Vector x:\n");
  for (IndexType tmp=0; tmp<bcsr.num_rows; tmp++)
  {
    fprintf(fp, "%.2lf  ", x[tmp]);
  }
  fprintf(fp, "\n");
  fclose(fp);
#endif
  
  if (bcsr.num_rows_left == 0)
  {
  for (I=0; I<num_block_rows; I++)
  {
    for (K=bcsr.bAp[I]; K<bcsr.bAp[I+1]; K++)
    {
      j0 = bcsr.bAj[K];
      //block calculation
      for (i=0; i<row_block_size; i++)
      {
        for (j=0; j<col_block_size; j++)
        {
          y[I*row_block_size+i] += bAx_tmp[K*row_block_size*col_block_size+i*col_block_size+j] * x[j0+j];
        }
      }
    }
  }
  }
  else
  {
  for (I=0; I<num_block_rows-1; I++)
  {
    for (K=bcsr.bAp[I]; K<bcsr.bAp[I+1]; K++)
    {
      j0 = bcsr.bAj[K];
      //block calculation
      for (i=0; i<row_block_size; i++)
      {
        for (j=0; j<col_block_size; j++)
        {
          y[I*row_block_size+i] += bAx_tmp[K*row_block_size*col_block_size+i*col_block_size+j] * x[j0+j];
        }
      }
    }
  }
    for (K=bcsr.bAp[num_block_rows-1]; K<bcsr.bAp[num_block_rows]; K++)
    {
      j0 = bcsr.bAj[K];
      for (i=0; i<num_rows_left; i++)
      {
        for (j=0; j<col_block_size; j++)
        {
          y[(num_block_rows-1)*row_block_size+i] += bAx_tmp[bcsr.bAp[num_block_rows-1]*row_block_size*col_block_size + (K-bcsr.bAp[num_block_rows-1])*num_rows_left*col_block_size+i*col_block_size+j] * x[j0+j];
        }
      }
    }
  }
#ifdef TESTING
  printf("Printing y .... \n");
  fp = fopen("y_value","w");
  fprintf(fp, "Vector y:\n");
  for (IndexType tmp=0; tmp<bcsr.num_cols; tmp++)
  {
    fprintf(fp, "%.2lf  ", y[tmp]);
  }
  fprintf(fp, "\n");
  fclose(fp);
#endif
  
}

