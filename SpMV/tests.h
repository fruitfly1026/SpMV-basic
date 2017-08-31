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

#include "config.h"
#include "sparse_io.h"
#include "sparse_operations.h"
#include "test_spmv.h"
#include "benchmark_spmv.h"
#include "spmv_host/dia_host.h"
#include "spmv_host/ell_host.h"
#include "spmv_host/csr_host.h"
#include "spmv_host/coo_host.h"
#include "spmv_host/bcsr_hosts.h"


template <typename IndexType, typename ValueType>
int test_dia_matrix_kernels(const csr_matrix<IndexType,ValueType>& csr, int kernel_tag, double *gflops, FILE *fp_feature)  
{
    printf("\n####  Testing DIA Kernels  ####\n");
    double max_fill = 20;
    IndexType max_diags = static_cast<IndexType>( (max_fill * csr.num_nonzeros) / csr.num_rows + 1 );

    // CREATE DIA MATRIX
    printf("\tcreating dia_matrix:");
    dia_matrix<IndexType,ValueType> dia = csr_to_dia<IndexType,ValueType>(csr, max_diags, fp_feature);
    printf("\n");

    if (dia.num_nonzeros == 0 && csr.num_nonzeros != 0)
    {     
	    printf("\tNumber of diagonals (%d) excedes limit (%d)\n", dia.complete_ndiags, max_diags);
	    return 0;
    }

    printf("\tFound %d diagonals\n", dia.complete_ndiags);
    double occupy_ratio = (double)dia.num_nonzeros / (dia.complete_ndiags*dia.num_rows);
    printf("\tTotal DIA occupy ratio: %.1lf %% \n", occupy_ratio*100);

    // TEST FORMAT
    if ( kernel_tag == 1)
    {
      test_spmv_kernel(csr, spmv_csr_serial_host_simple<IndexType,ValueType>, 
                       dia, spmv_dia_serial_host_simple<IndexType,ValueType>, 
                       "dia_serial_simple");

      benchmark_spmv_on_host(dia, spmv_dia_serial_host_simple<IndexType, ValueType>,   "dia_serial_simple" );
    }
    #if 0
    else if ( kernel_tag == 2 )
    {
      test_spmv_kernel(csr, spmv_csr_serial_host_simple<IndexType,ValueType>, 
                       dia, spmv_dia_serial_host_sse<IndexType,ValueType>, 
                       "dia_serial_sse");

      benchmark_spmv_on_host(dia, spmv_dia_serial_host_sse<IndexType, ValueType>,   "dia_serial_sse" );
    }
    #endif

    *gflops = dia.gflops;
    delete_host_matrix(dia);
    return 0;
}

template <typename IndexType, typename ValueType>
int test_ell_matrix_kernels(const csr_matrix<IndexType,ValueType>& csr, int kernel_tag, double *gflops, FILE *fp_feature)  
{
    printf("\n####  Testing ELL Kernels  ####\n");
    double max_fill = 20;
    IndexType max_cols_per_row = static_cast<IndexType>( (max_fill * csr.num_nonzeros) / csr.num_rows + 1 );

    // CREATE ELL MATRIX
    printf("\tcreating ell_matrix:");
    ell_matrix<IndexType,ValueType> ell = csr_to_ell<IndexType,ValueType>(csr, max_cols_per_row, fp_feature);
    printf("\n");
    if (ell.num_nonzeros == 0 && csr.num_nonzeros != 0)
    {      
	    printf("\tmax_RD (%d) excedes limit (%d)\n", ell.max_RD, max_cols_per_row);
	    return 0;
    }

    printf("\tELL has %d columns per row\n", ell.max_RD);
    double nzs_ratio = (double)csr.num_nonzeros/(ell.max_RD *ell.num_rows);
    printf("\tNonzeros occupy ratio: %.1lf %%\n", nzs_ratio*100);
    printf("\tMin nzs per row: %d, Max nzs per row: %d\n", ell.min_RD, ell.max_RD); 

    // TEST FORMAT
    if ( kernel_tag == 1)
    {
      test_spmv_kernel(csr, spmv_csr_serial_host_simple<IndexType,ValueType>, 
                       ell, spmv_ell_serial_host_simple<IndexType,ValueType>, 
                       "ell_serial_simple");

      benchmark_spmv_on_host(ell, spmv_ell_serial_host_simple<IndexType, ValueType>,     "ell_serial_simple" );
    }
    #if 0
    else if ( kernel_tag == 2)
    {
      test_spmv_kernel(csr, spmv_csr_serial_host_simple<IndexType,ValueType>, 
                       ell, spmv_ell_serial_host_sse<IndexType,ValueType>, 
                       "ell_serial_sse");

      benchmark_spmv_on_host(ell, spmv_ell_serial_host_sse<IndexType, ValueType>,     "ell_serial_sse" );
    }
    #endif

    *gflops = ell.gflops;
    delete_host_matrix(ell);
    return 0;
}

template <typename IndexType, typename ValueType>
int test_csr_matrix_kernels(csr_matrix<IndexType,ValueType>& csr, int kernel_tag, double *gflops, FILE *fp_feature)
{
    printf("\n####  Testing CSR Kernels  ####\n");

    // TEST KERNELS
    if ( kernel_tag == 1)
    {
      test_spmv_kernel(csr, spmv_csr_serial_host_simple<IndexType,ValueType>,   
                       csr, spmv_csr_serial_host_simple<IndexType,ValueType>, 
                       "csr_serial_simple");

      benchmark_spmv_on_host(csr,   spmv_csr_serial_host_simple<IndexType, ValueType>,       "csr_serial_simple" );
    }
    #if 0
    else if ( kernel_tag == 2)
    {
      test_spmv_kernel(csr, spmv_csr_serial_host_simple<IndexType,ValueType>,   
                       csr, spmv_csr_serial_host_sse<IndexType,ValueType>, 
                       "csr_serial_sse");

      benchmark_spmv_on_host(csr,   spmv_csr_serial_host_sse<IndexType, ValueType>,       "csr_serial_sse" );
    }
    #endif

    *gflops = csr.gflops;
    return 0;
}


template <typename IndexType, typename ValueType>
int test_coo_matrix_kernels(const csr_matrix<IndexType,ValueType>& csr, int kernel_tag, double *gflops, FILE *fp_feature)
{
    printf("\n####  Testing COO Kernels  ####\n");

    // CREATE COO MATRIX
    printf("\tcreating coo_matrix:");
    coo_matrix<IndexType,ValueType> coo = csr_to_coo<IndexType,ValueType>(csr, fp_feature);  
		//TODO change CSR to COO again, but COO is the input format from MM matrices.
    printf("\n");

    // TEST FORMAT
    if ( kernel_tag == 1 )
    {
      test_spmv_kernel(csr, spmv_csr_serial_host_simple<IndexType,ValueType>, 
                       coo, spmv_coo_serial_host_simple<IndexType,ValueType>, 
                       "coo_serial_simple");

      benchmark_spmv_on_host(coo, spmv_coo_serial_host_simple<IndexType, ValueType>,     "coo_serial_simple");
    }
    #if 0
    else if ( kernel_tag == 2 )
    {
      test_spmv_kernel(csr, spmv_csr_serial_host_simple<IndexType,ValueType>, 
                       coo, spmv_coo_serial_host_sse<IndexType,ValueType>, 
                       "coo_serial_sse");

      benchmark_spmv_on_host(coo, spmv_coo_serial_host_sse<IndexType, ValueType>,     "coo_serial_sse");
    }
    #endif
 
    *gflops = coo.gflops;
    delete_host_matrix(coo);
    return 0;
}


template <typename IndexType, typename ValueType>
int test_bcsr_matrix_kernels(csr_matrix<IndexType,ValueType>& csr, int kernel_tag, double *gflops, int *best_row_block_size, int *best_col_block_size, double *best_nonzeros_ratio, FILE *fp_feature)
{
    printf("\n####  Testing BCSR Kernels  ####\n");

    double min_fill_ratio = 0.2;
    int best_r = 0, best_c = 0;
    double best_nzs_ratio = 0;
    double max_bcsr_gflops = 0;

  //Find the neighbors
    int *states;
    states = (int *)malloc(sizeof(int)*csr.num_nonzeros);
    memset(states, 0, sizeof(int)*csr.num_nonzeros);
    for (IndexType i=0; i<csr.num_rows; i++)
    {   
      for (IndexType jj=csr.Ap[i]; jj<csr.Ap[i+1]; jj++)
      {   
        IndexType j = csr.Aj[jj];
        if ( (jj-1 >= csr.Ap[i]) && (csr.Aj[jj-1] == j-1) )
          if ( (jj+1 < csr.Ap[i+1]) && (csr.Aj[jj+1] == j+1) )
            states[jj] = 31;
          else
            states[jj] = 21;
        else
          if ( (jj+1<csr.Ap[i+1]) && (csr.Aj[jj+1] == j+1) )
            states[jj] = 21;
          else
          states[jj] = 11;

      int i_former = i-1;  //TODO: limiter
      if (i_former >= 0 )
      {
        for (IndexType jj_former = csr.Ap[i_former]; jj_former<csr.Ap[i_former+1]; jj_former++)
        {
          IndexType j_former = csr.Aj[jj_former];
          if ( j_former == j)
          {
            switch(states[jj])
            {
              case 11: states[jj] = 12;
                break;
              case 21: states[jj] = 22;
                 break;
              case 31: states[jj] = 32;
                 break;
            }
          }
        }
      }

      IndexType i_next = i+1;
      if (i_next < csr.num_rows)
      {
        for (IndexType jj_next = csr.Ap[i_next]; jj_next<csr.Ap[i_next+1]; jj_next++)
        {
          IndexType j_next = csr.Aj[jj_next];
          if ( j_next == j)
          {
            switch(states[jj])
            {
              case 11: states[jj] = 12;
                break;
              case 21: states[jj] = 22;
                 break;
              case 31: states[jj] = 32;
                 break;
              case 12: states[jj] = 13;
                  break;
              case 22: states[jj] = 23;
                 break;
              case 32: states[jj] = 33;
                  break;
            }
          }
        }
      }

      }  //end jj-loop
    }  //end i-loop

    int num_sb = 9;
    int *scoreboard;
    scoreboard = (int*)malloc(sizeof(int)*num_sb);
    memset(scoreboard, 0, sizeof(int)*num_sb);
    for (IndexType i=0; i<csr.num_nonzeros; i++)
    {
      switch(states[i])
      {
        case 11: scoreboard[0] ++;
           break;
        case 21: scoreboard[1] ++;
           break;
        case 12: scoreboard[2] ++;
           break;
        case 22: scoreboard[3] ++;
           break;
        case 31: scoreboard[4] ++;
           break;
        case 32: scoreboard[5] ++;
           break;
        case 13: scoreboard[6] ++;
           break;
        case 23: scoreboard[7] ++;
           break;
        case 33: scoreboard[8] ++;
           break;
      }
    }

#if 0
    FILE *fp_scoreboard = fopen("block_scoreboard", "w");
    fprintf(fp_scoreboard, "Scoreboard of the neighbors shape: (To inform blocking size from these)\n\n");
    fprintf(fp_scoreboard, "Scoreboard: (Horizontal and Vertical)\n");
    fprintf(fp_scoreboard, "Note: The sum of the ratios equals to 100%%\n");
    fprintf(fp_scoreboard, "State Values:   0\t\t 1\t\t 2\t\t 3\t\t 4\t\t 5\t\t 6\t\t 7\t\t 8\t\t\n");
    fprintf(fp_scoreboard, "Meanings:      11\t\t21\t\t12\t\t22\t\t31\t\t32\t\t13\t\t23\t\t33\t\t\n");
    fprintf(fp_scoreboard, "Numbers:      ");
    for (int i=0; i<num_sb; i++)
      fprintf(fp_scoreboard, "%3d    ", scoreboard[i]);
    fprintf(fp_scoreboard, "\nRatios:      ");
    for (int i=0; i<num_sb; i++)
      fprintf(fp_scoreboard, "%3.1f%%     ", 100*(double)scoreboard[i]/csr.num_nonzeros);
    fprintf(fp_scoreboard, "\n\n");
    fclose(fp_scoreboard);
#endif

    double blocks_ratio = 0, spots_ratio = 0;
    for (int i=1; i<num_sb; i++)
      blocks_ratio += (double)scoreboard[i]/csr.num_nonzeros;
    spots_ratio = 1 - blocks_ratio;

#ifdef COLLECT_FEATURES
    assert( num_sb == 9 );
    fprintf(fp_feature, "1x1_ratio : %f\n", (double)scoreboard[0]/csr.num_nonzeros );
    fprintf(fp_feature, "2x1_ratio : %f\n", (double)scoreboard[1]/csr.num_nonzeros );
    fprintf(fp_feature, "1x2_ratio : %f\n", (double)scoreboard[2]/csr.num_nonzeros );
    fprintf(fp_feature, "2x2_ratio : %f\n", (double)scoreboard[3]/csr.num_nonzeros );
    fprintf(fp_feature, "3x1_ratio : %f\n", (double)scoreboard[4]/csr.num_nonzeros );
    fprintf(fp_feature, "3x2_ratio : %f\n", (double)scoreboard[5]/csr.num_nonzeros );
    fprintf(fp_feature, "1x3_ratio : %f\n", (double)scoreboard[6]/csr.num_nonzeros );
    fprintf(fp_feature, "2x3_ratio : %f\n", (double)scoreboard[7]/csr.num_nonzeros );
    fprintf(fp_feature, "3x3_ratio : %f\n", (double)scoreboard[8]/csr.num_nonzeros );
    fprintf(fp_feature, "blocks_ratio : %f\n", blocks_ratio );
    fprintf(fp_feature, "spots_ratio : %f\n", spots_ratio );
#endif

    free(scoreboard);
    free(states);

    double *dense_gflops;
    dense_gflops = (double*)malloc(sizeof(double)*MAX_R*MAX_C);
    FILE *fp_dense_gflops = fopen(DENSE_GFLOPS, "r");
    for ( IndexType i=0; i<MAX_R; i++ )
    {
      for ( IndexType j=0; j<MAX_C; j++ )
      {
        fscanf(fp_dense_gflops, "%lf", &(dense_gflops[i*MAX_R+j]));
      }
    }
   fclose(fp_dense_gflops);

    double max_ER_block = 0;
    double estimate_gflops = 0, max_estimate_gflops = 0;
    int max_R_size = 0, max_C_size = 0;

    // CREATE BCSR MATRIX
      for ( int i=1; i<=MAX_R; i++ )
        for ( int j=1; j<=MAX_C; j++ )
      {
        if ( i!= 1 || j != 1)
        {
          printf("\n\tBlock size : (%d, %d)\n", i, j);
          printf("\tcreating bcsr_matrix:");
          timer t;
          bcsr_matrix<IndexType,ValueType> bcsr = csr_to_bcsr<IndexType,ValueType>(csr, i, j, min_fill_ratio, dense_gflops, &estimate_gflops, fp_feature);
          printf("\t[time]: %8.4f ms\n", t.milliseconds_elapsed());

          if (bcsr.num_nonzeros == 0 && csr.num_nonzeros != 0)
          {
            printf("\tNonzeros ration (%f) <= limit (%f)\n", bcsr.nzs_ratio, min_fill_ratio);
            continue;
          }

          if ( max_estimate_gflops < estimate_gflops )
          {
            max_estimate_gflops = estimate_gflops;
            max_R_size = i;
            max_C_size = j;
          }

#if 0
//print in BCSR format
    printf("Writing matrix in BCSR format to test_BCSR ...\n");
    FILE *fp = fopen("test_BCSR", "w");
    fprintf(fp, "%d\t%d\t%d\n", bcsr.num_rows, bcsr.num_cols, bcsr.num_nonzeros);
    fprintf(fp, "bcsr.row_block_size: %d\n", bcsr.row_block_size);
    fprintf(fp, "bcsr.col_block_size: %d\n", bcsr.col_block_size);
    fprintf(fp, "bcsr.num_block_rows: %d\n", bcsr.num_block_rows);
    fprintf(fp, "bcsr.num_block_cols: %d\n", bcsr.num_block_cols);
    fprintf(fp, "bcsr.num_blocks: %d\n", bcsr.num_blocks);
    fprintf(fp, "bcsr.bAp:\n");
    for (IndexType i=0; i<bcsr.num_block_rows+1; i++)
    {
      fprintf(fp, "%d  ", bcsr.bAp[i]);
    }
    fprintf(fp, "\n\n");
    fprintf(fp, "bcsr.bAj:\n");
    for (IndexType i=0; i<bcsr.num_blocks; i++)
    {
      fprintf(fp, "%d  ", bcsr.bAj[i]);
    }
    fprintf(fp, "\n\n");
    fprintf(fp, "bcsr.bAx:\n");
    for (IndexType i=0; i<bcsr.num_blocks*bcsr.row_block_size*bcsr.col_block_size; i++)
    {
      fprintf(fp, "%f  ", bcsr.bAx[i]);
    }
    fprintf(fp, "\n");
    fclose(fp);
#endif

        // TEST KERNELS
        if ( kernel_tag == 1)
        {
            test_spmv_kernel(csr, spmv_csr_serial_host_simple<IndexType,ValueType>,
                             bcsr, spmv_bcsr_serial_host<IndexType,ValueType>,
                             "bcsr_serial_host");

            benchmark_spmv_on_host(bcsr,   spmv_bcsr_serial_host<IndexType, ValueType>,       "bcsr_serial_host" );

            if ( max_bcsr_gflops < bcsr.gflops )
            {
              max_bcsr_gflops = bcsr.gflops;
              best_r = bcsr.row_block_size;
              best_c = bcsr.col_block_size;
              best_nzs_ratio = bcsr.nzs_ratio;
            }
            if ( max_ER_block < bcsr.nzs_ratio )
            {
              if ( bcsr.nzs_ratio != 1 )
                max_ER_block = bcsr.nzs_ratio;
            }
#ifdef COLLECT_FEATURES
    fprintf(fp_feature, "R_size : %d , C_size : %d , ER_block : %f, Estimate_gflops : %f, Actual_perf : %f\n", i, j, bcsr.nzs_ratio, estimate_gflops, bcsr.gflops );
#endif
        }

        delete_host_matrix(bcsr);
        }
      }

#ifdef COLLECT_FEATURES
    fprintf(fp_feature, "max_ER_block : %f\n", max_ER_block );
    fprintf(fp_feature, "max_estimate_gflops : %f\n", max_estimate_gflops );
    fprintf(fp_feature, "max_R_size : %d\n", max_R_size );
    fprintf(fp_feature, "max_C_size : %d\n", max_C_size );
    fprintf(fp_feature, "best_ER_block : %f\n", best_nzs_ratio );
    fprintf(fp_feature, "best_R_size : %d\n", best_r );
    fprintf(fp_feature, "best_C_size : %d\n", best_c );
#endif

    *gflops = max_bcsr_gflops;
    *best_row_block_size = best_r;
    *best_col_block_size = best_c;
    *best_nonzeros_ratio = best_nzs_ratio;

    return 0;
}
