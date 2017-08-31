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

#include <algorithm>
#include "assert.h"
#include <math.h>
#include "sparse_operations.h"

////////////////////////////////////////////////////////////////////////////////
//! Convert CSR format to DIA format
// If the matrix has more than 'max_diags' occupied diagonals, then a dia_matrix
// with dimensions (0,0) and 0 nonzeros is returned.
////////////////////////////////////////////////////////////////////////////////
template <class IndexType, class ValueType>
dia_matrix<IndexType, ValueType>
 csr_to_dia(const csr_matrix<IndexType,ValueType>& csr, const IndexType max_diags, FILE *fp_feature, const IndexType alignment = 16)
{
    dia_matrix<IndexType, ValueType> dia;
    dia.num_rows     = csr.num_rows;
    dia.num_cols     = csr.num_cols;
    dia.num_nonzeros = csr.num_nonzeros;
    dia.diag_offsets = NULL;
    dia.diag_data    = NULL;
    dia.tag          = 0;

    // compute number of occupied diagonals and enumerate them
    IndexType complete_ndiags = 0;
    const IndexType unmarked = (IndexType) -1; // works for both signed and unsigned
    IndexType * diag_map = new_array<IndexType>(csr.num_rows + csr.num_cols);
    std::fill(diag_map, diag_map + csr.num_rows + csr.num_cols, unmarked);
    IndexType * diag_map_2 = new_array<IndexType>(csr.num_rows + csr.num_cols);
    std::fill(diag_map_2, diag_map_2 + csr.num_rows + csr.num_cols, 0);

    for(IndexType i = 0; i < csr.num_rows; i++)
    {
        for(IndexType jj = csr.Ap[i]; jj < csr.Ap[i+1]; jj++)
        {
            IndexType j = csr.Aj[jj];
            IndexType map_index = (csr.num_rows - i) + j; //offset shifted by + num_rows
            if(diag_map[map_index] == unmarked)
            {
                diag_map[map_index] = complete_ndiags;
                complete_ndiags ++;
            }
            diag_map_2[map_index] ++;
        }
    }

        IndexType j_ndiags = 0;
        double ratio;
        IndexType NTdiags = 0;
        double *array_ndiags = new_array<double>(10);
        std::fill(array_ndiags, array_ndiags + 10, 0.0);
        for (IndexType i=0; i<csr.num_cols+csr.num_rows; i++)
        {
            if (diag_map_2[i] != 0)
            {
                j_ndiags ++;
                ratio = (double)diag_map_2[i] /csr.num_rows;
                if (ratio < 0.1 )
                  array_ndiags[0] ++;
                else if (ratio < 0.2 )
                  array_ndiags[1] ++;
                else if (ratio < 0.3 )
                  array_ndiags[2] ++;
                else if (ratio < 0.4 )
                  array_ndiags[3] ++;
                else if (ratio < 0.5 )
                  array_ndiags[4] ++;
                else if (ratio < 0.6 )
                  array_ndiags[5] ++;
                else if (ratio < 0.7 )
                  array_ndiags[6] ++;
                else if (ratio < 0.8 )
                  array_ndiags[7] ++;
                else if (ratio < 0.9 )
                  array_ndiags[8] ++;
                else if (ratio <= 1.0 )
                  array_ndiags[9] ++;

                if (ratio >= 0.6 )
                    NTdiags ++;
            }
        }
        assert( j_ndiags == complete_ndiags);
        free (diag_map_2);
#ifdef COLLECT_FEATURES
        fprintf(fp_feature, "Ndiags : %d\n", complete_ndiags );
#endif
        for ( int i=0; i<10; i++)
        {
          array_ndiags[i] /= complete_ndiags;
#ifdef COLLECT_FEATURES
          if ( i == 0 )
            fprintf(fp_feature, "Num_diags ER in ( %d %%, %d %% ) : %lf \n", i*10, (i+1)*10, array_ndiags[i] );
          else if ( i == 9 )
            fprintf(fp_feature, "Num_diags ER in [ %d %%, %d %% ] : %lf \n", i*10, (i+1)*10, array_ndiags[i] );
          else
            fprintf(fp_feature, "Num_diags ER in [ %d %%, %d %% ) : %lf \n", i*10, (i+1)*10, array_ndiags[i] );
#endif
        }

        double NTdiags_ratio = (double)NTdiags/complete_ndiags;
        double ER_DIA = (double)csr.num_nonzeros / (complete_ndiags * csr.num_rows);
#ifdef COLLECT_FEATURES
        fprintf(fp_feature, "NTdiags_ratio : %lf  ( TH is 0.6 )\n", NTdiags_ratio );
        fprintf(fp_feature, "ER_DIA : %lf\n", ER_DIA );
#endif
        
        free(array_ndiags);

    dia.complete_ndiags = complete_ndiags;

    if(complete_ndiags > max_diags)
    {                                                                      
       printf("\tNumber of diagonals (%d) excedes limit (%d)\n", dia.complete_ndiags, max_diags);
       dia.num_rows     = 0;
       dia.num_cols     = 0;
       dia.num_nonzeros = 0;
       dia.stride       = 0; 
       dia.gflops	= 0;
       delete_array(diag_map);                                     
       return dia;
   }

    // length of each diagonal in memory
    dia.stride = alignment * ((dia.num_rows + alignment - 1)/ alignment);

    dia.diag_offsets = new_array<int>(dia.complete_ndiags);
    dia.diag_data    = new_array<ValueType>(dia.complete_ndiags * dia.stride);

    for(IndexType n = 0; n < csr.num_rows + csr.num_cols; n++)
        if(diag_map[n] != unmarked)
            dia.diag_offsets[diag_map[n]] = (int) n - (int) csr.num_rows;

    std::fill(dia.diag_data, dia.diag_data + dia.complete_ndiags * dia.stride, ValueType(0));

    for(IndexType i = 0; i < csr.num_rows; i++){
        for(IndexType jj = csr.Ap[i]; jj < csr.Ap[i+1]; jj++){
            IndexType j = csr.Aj[jj];
            IndexType map_index = (csr.num_rows - i) + j; //offset shifted by + num_rows
            IndexType diag = diag_map[map_index];
            dia.diag_data[diag * dia.stride + i] = csr.Ax[jj];
        }
    }

    delete_array(diag_map);

    return dia;
}



////////////////////////////////////////////////////////////////////////////////
//! Convert CSR format to HYB (hybrid ELL/COO) format
// If the ELL portion of the HYB matrix will have 'num_cols_per_row' columns.
// Nonzero values that do not fit within the ELL structure are placed in the 
// COO format portion of the HYB matrix.
////////////////////////////////////////////////////////////////////////////////
template <class IndexType, class ValueType>
hyb_matrix<IndexType, ValueType>
 csr_to_hyb(const csr_matrix<IndexType,ValueType>& csr, const IndexType max_RD, const IndexType min_RD, const IndexType alignment = 16)
{
    hyb_matrix<IndexType, ValueType> hyb;

    ell_matrix<IndexType, ValueType> & ell = hyb.ell;
    coo_matrix<IndexType, ValueType> & coo = hyb.coo;

    hyb.num_rows = csr.num_rows;
    hyb.num_cols = csr.num_cols;
    hyb.num_nonzeros = csr.num_nonzeros;
    hyb.tag = 0;

    //initialize shapes
    ell.num_rows = csr.num_rows;
    ell.num_cols = csr.num_cols;
    ell.tag = 0;
    coo.num_rows = csr.num_rows;
    coo.num_cols = csr.num_cols;
    coo.tag = 0;
   
    ell.stride = alignment * ((ell.num_rows + alignment - 1)/ alignment);
    ell.max_RD = max_RD;
    ell.min_RD = min_RD;

    // compute number of nonzeros in the ELL and COO portions
    ell.num_nonzeros = 0;
    for(IndexType i = 0; i < csr.num_rows; i++)
        ell.num_nonzeros += std::min(ell.max_RD, csr.Ap[i+1] - csr.Ap[i]); 

    coo.num_nonzeros = csr.num_nonzeros - ell.num_nonzeros;

    // allocate storage for ELL and COO matrices
    ell.Aj = new_array<IndexType>(ell.max_RD * ell.stride);
    ell.Ax = new_array<ValueType>(ell.max_RD * ell.stride);

    if(coo.num_nonzeros > 0){
        coo.I = new_array<IndexType>(coo.num_nonzeros);
        coo.J = new_array<IndexType>(coo.num_nonzeros);
        coo.V = new_array<ValueType>(coo.num_nonzeros);
    } else {
        coo.I = NULL;
        coo.J = NULL;
        coo.V = NULL;
    }

    // pad out ELL format with zeros
    std::fill(ell.Aj, ell.Aj + ell.max_RD * ell.stride, 0);
    std::fill(ell.Ax, ell.Ax + ell.max_RD * ell.stride, 0);

    for(IndexType i = 0, coo_nnz = 0; i < csr.num_rows; i++){
        IndexType n = 0;
        IndexType jj = csr.Ap[i];

        // copy up to num_cols_per_row values of row i into the ELL
        while(jj < csr.Ap[i+1] && n < ell.max_RD){
            ell.Aj[ell.stride * n + i] = csr.Aj[jj];
            ell.Ax[ell.stride * n + i] = csr.Ax[jj];
            jj++, n++;
        }

        // copy any remaining values in row i into the COO
        while(jj < csr.Ap[i+1]){
            coo.I[coo_nnz] = i;
            coo.J[coo_nnz] = csr.Aj[jj];
            coo.V[coo_nnz] = csr.Ax[jj];
            jj++; coo_nnz++;
        }
    }

    return hyb;
}


////////////////////////////////////////////////////////////////////////////////
//! Convert CSR format to ELL format
// If the matrix has more than 'max_cols_per_row' columns in any row, then 
// an ell_matrix with dimensions (0,0) and 0 nonzeros is returned. Rows with 
// fewer than 'num_cols_per_row' columns are padded with zeros.
////////////////////////////////////////////////////////////////////////////////
template <class IndexType, class ValueType>
ell_matrix<IndexType, ValueType>
 csr_to_ell(const csr_matrix<IndexType,ValueType>& csr, const IndexType max_cols_per_row, FILE *fp_feature, const IndexType alignment = 16)
{
    IndexType num_rows = csr.num_rows;
    IndexType num_cols = csr.num_cols;
    IndexType num_nonzeros = csr.num_nonzeros;

    // Features in row
    IndexType max_RD = csr.Ap[1] - csr.Ap[0];
    IndexType min_RD = csr.Ap[1] - csr.Ap[0];
    double aver_RD = (double)num_nonzeros / num_rows;
    double dev_RD = (csr.Ap[1]-csr.Ap[0]-aver_RD) * (csr.Ap[1]-csr.Ap[0]-aver_RD);
    IndexType last_RD = csr.Ap[1] - csr.Ap[0];
    double row_bounce = 0;

    for(IndexType i = 1; i < num_rows; i++)
    {
        IndexType num_cols_per_row = csr.Ap[i+1] - csr.Ap[i];
        max_RD = std::max(max_RD, num_cols_per_row ); 
        min_RD = std::min(min_RD, num_cols_per_row); 
        dev_RD += (num_cols_per_row - aver_RD) * (num_cols_per_row - aver_RD);
        row_bounce += fabs (num_cols_per_row - last_RD);
        last_RD = num_cols_per_row;
    }
    dev_RD /= num_rows;
    row_bounce /= num_rows;
    double ER_RD = (double)num_nonzeros / (num_rows * max_RD);

#ifdef COLLECT_FEATURES
    fprintf(fp_feature, "aver_RD : %lf\n", aver_RD );
    fprintf(fp_feature, "max_RD : %d\n", max_RD );
    fprintf(fp_feature, "min_RD : %d\n", min_RD );
    fprintf(fp_feature, "dev_RD : %lf\n", dev_RD );
    fprintf(fp_feature, "ER_RD : %lf\n", ER_RD );
    fprintf(fp_feature, "row_bounce : %lf\n", row_bounce );
#endif

    //Features in column
    IndexType *array_CD = (IndexType*)malloc(num_cols * sizeof(IndexType));
    memset (array_CD, 0, sizeof(IndexType)*num_cols);

    for(IndexType i = 0; i < num_rows; i++)
    {
        for (IndexType jj = csr.Ap[i]; jj < csr.Ap[i+1]; jj ++ )
        {
            IndexType j = csr.Aj[jj];
            array_CD[j] ++;
        }
    }

    IndexType max_CD = array_CD[0];
    IndexType min_CD = array_CD[0];
    double aver_CD = (double)num_nonzeros / num_cols;
    double dev_CD = (array_CD[0]-aver_CD) * (array_CD[0]-aver_CD);
    IndexType last_CD = array_CD[0];
    double col_bounce = 0;

    for(IndexType j = 1; j < csr.num_cols; j++)
    {
        IndexType num_rows_per_col = array_CD[j];
        max_CD = std::max(max_CD, num_rows_per_col ); 
        min_CD = std::min(min_RD, num_rows_per_col); 
        dev_CD += (num_rows_per_col - aver_CD) * (num_rows_per_col - aver_CD);
        col_bounce += fabs (num_rows_per_col - last_CD);
        last_CD = num_rows_per_col;
    }
    dev_CD /= csr.num_cols;
    col_bounce /= num_cols;
    double ER_CD = (double)csr.num_nonzeros / (csr.num_cols * max_CD);
    free (array_CD);

#ifdef COLLECT_FEATURES
    fprintf(fp_feature, "aver_CD : %lf\n", aver_CD );
    fprintf(fp_feature, "max_CD : %d\n", max_CD );
    fprintf(fp_feature, "min_CD : %d\n", min_CD );
    fprintf(fp_feature, "dev_CD : %lf\n", dev_CD );
    fprintf(fp_feature, "ER_CD : %lf\n", ER_CD );
    fprintf(fp_feature, "col_bounce : %lf\n", col_bounce );
#endif

    if(max_RD >= max_cols_per_row)
    {             
	printf("\tnum_cols_per_row (%d) excedes limit (%d)\n", max_RD, max_cols_per_row);
       //too many columns
        ell_matrix<IndexType, ValueType> ell;
        ell.Aj = NULL;
        ell.Ax = NULL;
        ell.num_rows = 0;
        ell.num_cols = 0;
        ell.num_nonzeros = 0;
        ell.stride = 0;
        ell.max_RD = max_RD;
        ell.min_RD = min_RD;
        ell.gflops = 0;
        return ell;
    }

        // use CSR->HYB and grab the ELL portion
        return csr_to_hyb(csr, max_RD, min_RD, alignment).ell;
}


////////////////////////////////////////////////////////////////////////////////
//! Convert CSR format to COO format
// Storage for output is assumed to have been allocated
//! @param Ap             CSR pointer array
//! @param Aj             CSR index array
//! @param Ax             CSR data array
//! @param num_rows       number of rows
//! @param num_cols       number of columns
//! @param num_nonzeros   number of nonzeros
//! @param rows           COO row array
//! @param cols           COO column array
//! @param data           COO data array
////////////////////////////////////////////////////////////////////////////////
template <class IndexType, class ValueType>
void csr_to_coo(const IndexType * Ap,
                const IndexType * Aj,
                const ValueType  * Ax,
                const IndexType num_rows, 
                const IndexType num_cols, 
                const IndexType num_nonzeros,
                      IndexType * rows,
                      IndexType * cols,
                      ValueType * data)
{
    for(IndexType i = 0; i < num_rows; i++){        
        IndexType row_start = Ap[i];
        IndexType row_end   = Ap[i+1];
        for(IndexType jj = row_start; jj < row_end; jj++){
            rows[jj] = i;
        }
    }

    for(IndexType i = 0; i < num_nonzeros; i++){
        cols[i] = Aj[i];
        data[i] = Ax[i];
    }
}

template <class IndexType, class ValueType>
coo_matrix<IndexType, ValueType>
 csr_to_coo(const csr_matrix<IndexType,ValueType>& csr, FILE *fp_feature)
{  
    IndexType num_rows = csr.num_rows;
    IndexType num_cols = csr.num_cols;
    IndexType num_nonzeros = csr.num_nonzeros;

    IndexType *nzs_per_row = new_array<IndexType>(num_rows);
    for(IndexType i = 0; i < num_rows; i++)
        nzs_per_row[i] = csr.Ap[i+1] - csr.Ap[i];

    //Calculate the pow-law parameters
    double *nzs_distribution = new_array<double>(num_cols+1);
    std::fill(nzs_distribution, nzs_distribution + num_cols+1, 0.0);
    IndexType number;
    for(IndexType i = 0; i < num_rows; i++)
    {
        number = nzs_per_row[i];
        nzs_distribution[number] += 1;
    }
    free(nzs_per_row);
    
    IndexType total_count = 0;
    IndexType peak_pos = 0, peak_Rdegree = 0;
    double total_sum = 0, peak_ratio = 0;
//    fp = fopen(NZS_DISTRIBUTION, "w");
//    fprintf(fp, "nzs_distribution:\n");
    for(IndexType i = 1; i <= num_cols; i++)
    {
        if ( nzs_distribution[i] != 0.0)
        {
//            nzs_distribution[i] = nzs_distribution[i]/num_rows;  
            nzs_distribution[i] = nzs_distribution[i] / (num_rows-nzs_distribution[0]);  
//           fprintf(fp, "nzs %d, the ratio: %lf %%\n", i, nzs_distribution[i]*100);
            total_count ++;
            total_sum += nzs_distribution[i];
            if ( peak_ratio < nzs_distribution[i] )
            {
              peak_ratio = nzs_distribution[i];
              peak_pos = total_count;
              peak_Rdegree = i;
            }
        }
    }
//    fprintf(fp, "Different nzs_per_row count: %d\n", total_count);
//    fprintf(fp, "Total sum (should be 1): %f\n", total_sum);
//    fclose(fp);

    printf("\tDifferent nzs_per_row count: %d\n", total_count);
    printf("\tTotal sum (should be 1): %f\n", total_sum);
    printf("\tPeak ratio: %f,  corresponded row degree: %d\n", peak_ratio, peak_Rdegree);

    IndexType peak_pos_2 = 0;
    double *final_Pks = new_array<double>(total_count+1);
    int *final_ks = new_array<int>(total_count+1);
    for (IndexType i = 1, j = 1; i <= num_cols; i++)
    {
        if ( nzs_distribution[i] != 0.0)
        {
            final_Pks[j] = nzs_distribution[i];
            final_ks[j] = i;
            j ++;
        }
    }

    free(nzs_distribution);

    IndexType count = 0;
    double aver_x = 0, aver_y = 0;
    for(IndexType i = peak_pos; i <= total_count; i++)
    {
        count ++;
        aver_x += log10(final_ks[i]);
        aver_y += log10(final_Pks[i]);
    }
    assert ( count == (total_count - peak_pos + 1) );
    printf("\tUsed count: %d\n", count);
    aver_x /= count;
    aver_y /= count;
    printf("aver_x: %.2lf, aver_y: %.2lf\n", aver_x, aver_y);

    double a_up = 0, a_down = 0;
    double a = 0, b = 0, r, c;
    for(IndexType i = peak_pos; i <= total_count; i++)
    {
        a_up += (log10(final_ks[i]) - aver_x) * (log10(final_Pks[i]) - aver_y);
        a_down += (log10(final_ks[i]) - aver_x) * (log10(final_ks[i]) - aver_x);
    }
    a = a_up / a_down;
    b = aver_y - a*aver_x;
//    printf("a_up: %.2lf, a_down: %.2lf\n", a_up, a_down);
//    printf("a: %.2lf, b: %.2lf\n", a, b);
    r = 0 - a;
    c = pow(10, b);

    double test_sum = 0;
    for(IndexType i = 1; i < peak_pos; i++)
        test_sum += final_Pks[i] ;
    printf("\tpart test_sum: %.2lf\n", test_sum);
    for(IndexType i = peak_pos; i <= total_count; i++)
        test_sum += ( c* pow(final_ks[i], 0-r) );
    
    printf("\ttest_sum: %.2lf (should similar to 1)\n", test_sum);
    printf("\tr: %.2lf, c: %lf\n", r, c);
    printf("\tr should be in (0,4)\n");

#ifdef COLLECT_FEATURES
    fprintf(fp_feature, "R : %lf\n", r);
#endif

    free(final_Pks);
    free(final_ks);

    coo_matrix<IndexType, ValueType> coo;

    coo.num_rows     = csr.num_rows;
    coo.num_cols     = csr.num_cols;
    coo.num_nonzeros = csr.num_nonzeros;
    coo.tag = 0;

    coo.I = new_array<IndexType>(csr.num_nonzeros);
    coo.J = new_array<IndexType>(csr.num_nonzeros);
    coo.V = new_array<ValueType>(csr.num_nonzeros);

    csr_to_coo(csr.Ap,csr.Aj,csr.Ax,
               coo.num_rows,coo.num_cols,coo.num_nonzeros,
               coo.I,coo.J,coo.V);


    return coo;
}

template <class IndexType, class ValueType>
mix_matrix<IndexType, ValueType>
 csr_to_mix(const csr_matrix<IndexType,ValueType>& csr)
{
    mix_matrix<IndexType, ValueType> mix;
    mix.num_rows = csr.num_rows;
    mix.num_cols = csr.num_cols;
    mix.num_nonzeros = csr.num_nonzeros;
    mix.tag = 0;
    mix.Ap = csr.Ap;
    mix.Aj = csr.Aj;
    mix.Ax = csr.Ax;
    return mix;
}


template <class IndexType, class ValueType>
bcsr_matrix<IndexType, ValueType>
 csr_to_bcsr(const csr_matrix<IndexType,ValueType>& csr, int row_block_size, int col_block_size, double min_fill_ratio, double *dense_gflops, double *estimate_gflops, FILE *fp_feature)
{
    //BCSR format conversion
    bcsr_matrix<IndexType, ValueType> bcsr;

    IndexType num_block_rows = (csr.num_rows+row_block_size-1) / row_block_size;
    IndexType num_block_cols = (csr.num_cols+col_block_size-1) / col_block_size;
    IndexType num_rows_left = csr.num_rows % row_block_size;

    bcsr.num_rows     = csr.num_rows;
    bcsr.num_cols     = csr.num_cols;
    bcsr.num_nonzeros = csr.num_nonzeros;
    bcsr.tag = 0;
    bcsr.row_block_size = row_block_size; //should be divisble by num_rows
    bcsr.col_block_size = col_block_size;
    bcsr.num_block_rows = num_block_rows;
    bcsr.num_block_cols = num_block_cols;
    bcsr.num_rows_left = num_rows_left;

    IndexType *bAp;
    IndexType *bAj;
    ValueType *bAx;


    //tmp variables
        IndexType num_blocks = 0;
        IndexType *block_count;
        ValueType *blocks;
//        ValueType *blocks_left;
        IndexType *bAp_next;
        IndexType I, J;
        IndexType i, j, k, j0, di;
        ValueType a_ij;

        bAp = new_array<IndexType>(num_block_rows+1);
        bAp_next = new_array<IndexType>(num_block_rows+1);
        block_count = new_array<IndexType>(num_block_cols);
        memset (block_count, 0, sizeof(IndexType)*num_block_cols);

    //Phase I: Count the exact number of new blocks to create.
        bAp[0] = 0; 
        if ( num_rows_left == 0)
        {    
        for (I=0; I<num_block_rows; I++) 
        {    
          for (i=I*row_block_size, di=0; di<row_block_size; di++, i++) 
          {    
             for (k=csr.Ap[i]; k<csr.Ap[i+1]; k++) 
             {    
               j = csr.Aj[k];
               J = j/col_block_size;
               if (block_count[J] == 0)
               {    
                num_blocks ++;
                block_count[J] ++;
               }
             }
          }
          bAp[I+1] = num_blocks;
          for (i=I*row_block_size, di=0; di<row_block_size; di++, i++)
          {
            for (k=csr.Ap[i]; k<csr.Ap[i+1]; k++)
            {
              j = csr.Aj[k];
              J = j/col_block_size;
              block_count[J] = 0;
            }
          }
        }
        }
        else
        {
        for (I=0; I<num_block_rows-1; I++)
        {
          for (i=I*row_block_size, di=0; di<row_block_size; di++, i++)
          {
             for (k=csr.Ap[i]; k<csr.Ap[i+1]; k++)
             {
               j = csr.Aj[k];
               J = j/col_block_size;
               if (block_count[J] == 0)
               {
                num_blocks ++;
                block_count[J] ++;
               }
             }
          }
          bAp[I+1] = num_blocks;
          for (i=I*row_block_size, di=0; di<row_block_size; di++, i++)
          {
            for (k=csr.Ap[i]; k<csr.Ap[i+1]; k++)
            {
              j = csr.Aj[k];
              J = j/col_block_size;
              block_count[J] = 0;
            }
          }
        }
          for (i=(num_block_rows-1)*row_block_size, di=0; di<num_rows_left; di++, i++)
          {
             for (k=csr.Ap[i]; k<csr.Ap[i+1]; k++)
             {
               j = csr.Aj[k];
               J = j/col_block_size;
               if (block_count[J] == 0)
               {
                num_blocks ++;
                block_count[J] ++;
               }
             }
          }
          bAp[num_block_rows] = num_blocks;
          for (i=(num_block_rows-1)*row_block_size, di=0; di<num_rows_left; di++, i++)
          {
            for (k=csr.Ap[i]; k<csr.Ap[i+1]; k++)
            {
              j = csr.Aj[k];
              J = j/col_block_size;
              block_count[J] = 0;
            }
          }
        }

        bcsr.num_blocks = num_blocks;
        bcsr.nzs_ratio = (double)(bcsr.num_nonzeros)/(row_block_size*col_block_size*num_blocks);
        double estimate_gflops_tmp = 0;
        if (bcsr.nzs_ratio != 1 )
          estimate_gflops_tmp = dense_gflops[(row_block_size-1)*MAX_R+(col_block_size-1)] / (1-bcsr.nzs_ratio);
        *estimate_gflops = estimate_gflops_tmp;

        if(bcsr.nzs_ratio < min_fill_ratio)
        {
            printf("\tThe ratio of padding 0s (%lf %%) less than limit (%lf %%)\n", 100*(bcsr.nzs_ratio), 100*min_fill_ratio);
            bcsr.bAp = NULL;
            bcsr.bAj = NULL;
            bcsr.bAx = NULL;
            bcsr.num_rows = 0;
            bcsr.num_cols = 0;
            bcsr.num_nonzeros = 0;
            bcsr.num_block_rows = 0;
            bcsr.num_block_cols = 0;
            free(bAp_next);
            free(block_count);
            return bcsr;
        }
        bAj = new_array<IndexType>(num_blocks);
        bAx = new_array<ValueType>(num_blocks*row_block_size*col_block_size);
        blocks = new_array<ValueType>(row_block_size*col_block_size*num_block_cols);
        memset (blocks, 0, sizeof(ValueType)*row_block_size*col_block_size*num_block_cols);
//        blocks_left = new_array<ValueType>(num_rows_left*(csr.num_cols+col_block_size));
//        memset (blocks_left, 0, sizeof(ValueType)*num_rows_left*(csr.num_cols+col_block_size));

    //Phase II: Copy all blocks.
        memcpy_array( bAp_next, bAp, num_block_rows+1);
    #ifdef TESTING
          FILE *fp;
          if (num_rows_left == 0)
            fp = fopen("blocks", "w");
          else
            fp = fopen("blocks_left", "w");
    #endif
        if ( num_rows_left == 0)
        {
        for (I=0; I<num_block_rows; I++)
        {
          for (i=I*row_block_size, di=0; di<row_block_size; di++, i++)
          {
            for (k=csr.Ap[i]; k<csr.Ap[i+1]; k++)
            {
              j = csr.Aj[k];
              J = j/col_block_size;
#if 0
              if ((J+1)*col_block_size > csr.num_cols)
                j0 = csr.num_cols - col_block_size;
              else
                j0 = J * col_block_size;
#endif
              j0 = J * col_block_size;

              a_ij = csr.Ax[k];
              blocks[J*row_block_size*col_block_size + di*col_block_size + j - j0] = a_ij;
              block_count[J] ++;
            }
          }
    #ifdef TESTING
          fprintf(fp, "blocks: \n");
          for (IndexType tmp_ljj=0; tmp_ljj<row_block_size*(csr.num_cols+col_block_size); tmp_ljj++)
          {
        fprintf(fp, "%.2lf  ", blocks[tmp_ljj]);
          }
          fprintf(fp, "\n");
          fprintf(fp, "2nd: block_count: \n");
          for (IndexType tmp_ljj=0; tmp_ljj<num_block_cols; tmp_ljj++)
          {
        fprintf(fp, "%d  ", block_count[tmp_ljj]);
          }
          fprintf(fp, "\n");
    #endif
#if 1
          for (i=I*row_block_size, di=0; di<row_block_size; di++, i++)
          {
            for (k=csr.Ap[i]; k<csr.Ap[i+1]; k++)
            {
              j = csr.Aj[k];
              J = j/col_block_size;
//              j0 = ( (J*col_block_size + col_block_size)>csr.num_cols ) ? csr.num_cols-col_block_size:J*col_block_size;
              j0 = J * col_block_size;

              if (block_count[J]>0)
              {
                IndexType k_next = bAp_next[I];
                bAj[k_next] = j0;
                memcpy_array (bAx+k_next*row_block_size*col_block_size, blocks+J*row_block_size*col_block_size, row_block_size*col_block_size);
                bAp_next[I] ++;
                assert (bAp_next[I] <= bAp[I+1]);
                block_count[J] = 0;
                memset (blocks+J*col_block_size*row_block_size, 0, sizeof(ValueType)*row_block_size*col_block_size);
              }
            }
          }
#endif
#if 0
          for ( i = 0; i<row_block_size*col_block_size*num_block_cols; i += row_block_size*col_block_size )
          {
            int block_tag = 0;
            for ( k=0; k<row_block_size*col_block_size; k++ )
            {
              if ( blocks[i+k] != 0 )
              {
                block_tag == 1;
                continue;
              }
            }
            if ( block_tag == 1 )
            {
              IndexType k_next = bAp_next[I];
              bAj[k_next] = j0;
              memcpy_array (bAx+k_next*row_block_size*col_block_size, blocks+i, row_block_size*col_block_size);
              bAp_next[I] ++;
              assert (bAp_next[I] <= bAp[I+1]);
              block_count[i/(row_block_size*col_block_size)] = 0;
            }
          }
          memset (blocks, 0, sizeof(ValueType)*row_block_size*col_block_size*num_block_cols);
#endif
        }
        }
        else
        {
        for (I=0; I<num_block_rows-1; I++)
        {
          for (i=I*row_block_size, di=0; di<row_block_size; di++, i++)
          {
            for (k=csr.Ap[i]; k<csr.Ap[i+1]; k++)
            {
              j = csr.Aj[k];
              J = j/col_block_size;
#if 0
              if ((J+1)*col_block_size > csr.num_cols)
                j0 = csr.num_cols - col_block_size;
              else
                j0 = J * col_block_size;
#endif
              j0 = J * col_block_size;

              a_ij = csr.Ax[k];
              blocks[J*row_block_size*col_block_size + di*col_block_size + j - j0] = a_ij;
              block_count[J] ++;
            }
          }
    #ifdef TESTING
          fprintf(fp, "blocks_no_left: \n");
          for (IndexType tmp_ljj=0; tmp_ljj<row_block_size*(csr.num_cols+col_block_size); tmp_ljj++)
          {
        fprintf(fp, "%.2lf  ", blocks[tmp_ljj]);
          }
          fprintf(fp, "\n");
          fprintf(fp, "2nd: block_count: \n");
          for (IndexType tmp_ljj=0; tmp_ljj<num_block_cols; tmp_ljj++)
          {
        fprintf(fp, "%d  ", block_count[tmp_ljj]);
          }
          fprintf(fp, "\n");
    #endif
#if 1
          for (i=I*row_block_size, di=0; di<row_block_size; di++, i++)
          {
            for (k=csr.Ap[i]; k<csr.Ap[i+1]; k++)
            {
              j = csr.Aj[k];
              J = j/col_block_size;
//              j0 = ( (J*col_block_size + col_block_size)>csr.num_cols ) ? csr.num_cols-col_block_size:J*col_block_size;
              j0 = J * col_block_size;

              if (block_count[J]>0)
              {
                IndexType k_next = bAp_next[I];
                bAj[k_next] = j0;
                memcpy_array (bAx+k_next*row_block_size*col_block_size, blocks+J*row_block_size*col_block_size, row_block_size*col_block_size);
        //    printf("I: %d, i: %d, di: %d, k: %d, j:%d, J: %d, j0: %d\n", I, i, di, k, j, J, j0);
        //    printf("k_next: %d, bAx+ %d , blocks+ %d \n", k_next, k_next*row_block_size*col_block_size, J*row_block_size*col_block_size);
                bAp_next[I] ++;
                assert (bAp_next[I] <= bAp[I+1]);
                block_count[J] = 0;
                memset (blocks+J*col_block_size*row_block_size, 0, sizeof(ValueType)*row_block_size*col_block_size);
              }
            }
          }
#endif
#if 0
          for ( i = 0; i<row_block_size*col_block_size*num_block_cols; i += row_block_size*col_block_size )
          {
            int block_tag = 0;
            for ( k=0; k<row_block_size*col_block_size; k++ )
            {
              if ( blocks[i+k] != 0 )
              {
                block_tag == 1;
                break;
              }
            }
            if ( block_tag == 1 )
            {
              IndexType k_next = bAp_next[I];
              bAj[k_next] = i / (row_block_size*col_block_size);
              memcpy_array (bAx+k_next*row_block_size*col_block_size, blocks+i, row_block_size*col_block_size);
              bAp_next[I] ++;
              assert (bAp_next[I] <= bAp[I+1]);
              block_count[i/(row_block_size*col_block_size)] = 0;
            }
          }
          memset (blocks, 0, sizeof(ValueType)*row_block_size*col_block_size*num_block_cols);
#endif
        }

          for (i=(num_block_rows-1)*row_block_size, di=0; di<num_rows_left; di++, i++)
          {
            for (k=csr.Ap[i]; k<csr.Ap[i+1]; k++)
            {
              j = csr.Aj[k];
              J = j/col_block_size;
#if 0
              if ((J+1)*col_block_size > csr.num_cols)
                j0 = csr.num_cols - col_block_size;
              else
                j0 = J * col_block_size;
#endif
              j0 = J * col_block_size;

              a_ij = csr.Ax[k];
              blocks[J*row_block_size*col_block_size + di*col_block_size + j - j0] = a_ij;
              block_count[J] ++;
            }
          }
    #ifdef TESTING
          fprintf(fp, "blocks_left: \n");
          for (IndexType tmp_ljj=0; tmp_ljj<row_block_size*(csr.num_cols+col_block_size); tmp_ljj++)
          {
        fprintf(fp, "%.2lf  ", blocks[tmp_ljj]);
          }
          fprintf(fp, "\n");
          fprintf(fp, "2nd: block_count: \n");
          for (IndexType tmp_ljj=0; tmp_ljj<num_block_cols; tmp_ljj++)
          {
        fprintf(fp, "%d  ", block_count[tmp_ljj]);
          }
          fprintf(fp, "\n");
    #endif
#if 1
          for (i=(num_block_rows-1)*row_block_size, di=0; di<num_rows_left; di++, i++)
          {
            for (k=csr.Ap[i]; k<csr.Ap[i+1]; k++)
            {
              j = csr.Aj[k];
              J = j/col_block_size;
//              j0 = ( (J*col_block_size + col_block_size)>csr.num_cols ) ? csr.num_cols-col_block_size:J*col_block_size;
              j0 = J * col_block_size;

              if (block_count[J]>0)
              {
                IndexType k_next = bAp_next[num_block_rows-1];
                bAj[k_next] = j0;
                memcpy_array (bAx+k_next*row_block_size*col_block_size, blocks+J*row_block_size*col_block_size, row_block_size*col_block_size);
        //    printf("i: %d, di: %d, k: %d, j:%d, J: %d, j0: %d\n", i, di, k, j, J, j0);
        //    printf("k_next: %d, bAx+ %d , blocks+ %d \n", k_next, bAp[num_block_rows-1]*row_block_size*col_block_size+(k_next-bAp[num_block_rows-1])*num_rows_left*col_block_size, J*num_rows_left*col_block_size);
                bAp_next[num_block_rows-1] ++;
                assert (bAp_next[num_block_rows-1] <= bAp[num_block_rows-1+1]);
                block_count[J] = 0;
                memset (blocks+J*col_block_size*row_block_size, 0, sizeof(ValueType)*row_block_size*col_block_size);
              }
            }
          }
#endif

#if 0
          for ( i = 0; i<row_block_size*col_block_size*num_block_cols; i += row_block_size*col_block_size )
          {
            int block_tag = 0;
            for ( k=0; k<row_block_size*col_block_size; k++ )
            {
              if ( blocks[i+k] != 0 )
              {
                block_tag == 1;
                continue;
              }
            }
            if ( block_tag == 1 )
            {
              IndexType k_next = bAp[num_block_rows-1];
              bAj[k_next] = j0;
              memcpy_array (bAx+k_next*row_block_size*col_block_size, blocks+i, row_block_size*col_block_size);
              bAp_next[num_block_rows-1] ++;
              assert (bAp_next[num_block_rows-1] <= bAp[num_block_rows-1+1]);
              block_count[i/(row_block_size*col_block_size)] = 0;
            }
          }
          memset (blocks, 0, sizeof(ValueType)*row_block_size*col_block_size*num_block_cols);
#endif
        }
    #ifdef TESTING
          fclose(fp);
    #endif

        free(block_count);
        free(blocks);
//        free(blocks_left);
        free(bAp_next);

        bcsr.bAp = bAp;
        bcsr.bAj = bAj;
        bcsr.bAx = bAx;

    return bcsr;
}



////////////////////////////////////////////////////////////////////////////////
//! Convert COO format to CSR format
// Storage for output is assumed to have been allocated
//! @param rows           COO row array
//! @param cols           COO column array
//! @param data           COO data array
//! @param num_rows       number of rows
//! @param num_cols       number of columns
//! @param num_nonzeros   number of nonzeros
//! @param Ap             CSR pointer array
//! @param Ai             CSR index array
//! @param Ax             CSR data array
////////////////////////////////////////////////////////////////////////////////
template <class IndexType, class ValueType>
void coo_to_csr(const IndexType * rows,
                const IndexType * cols,
                const ValueType * data,
                const IndexType num_rows, 
                const IndexType num_cols, 
                const IndexType num_nonzeros,
                      IndexType * Ap,
                      IndexType * Aj,
                      ValueType * Ax)
{
    for (IndexType i = 0; i < num_rows; i++)
        Ap[i] = 0;

    for (IndexType i = 0; i < num_nonzeros; i++)
        Ap[rows[i]]++;


    //cumsum the nnz per row to get Bp[]
    for(IndexType i = 0, cumsum = 0; i < num_rows; i++){     
        IndexType temp = Ap[i];
        Ap[i] = cumsum;
        cumsum += temp;
    }
    Ap[num_rows] = num_nonzeros;

    //write Aj,Ax into Bj,Bx
    for(IndexType i = 0; i < num_nonzeros; i++){
        IndexType row  = rows[i];
        IndexType dest = Ap[row];

        Aj[dest] = cols[i];
        Ax[dest] = data[i];

        Ap[row]++;
    }

    for(IndexType i = 0, last = 0; i <= num_rows; i++){
        IndexType temp = Ap[i];
        Ap[i]  = last;
        last   = temp;
    }
    
}


////////////////////////////////////////////////////////////////////////////////
//! Convert COOrdinate format (triplet) to CSR format
//! @param coo        coo_matrix
////////////////////////////////////////////////////////////////////////////////
template <class IndexType, class ValueType>
csr_matrix<IndexType, ValueType>
 coo_to_csr(const coo_matrix<IndexType,ValueType>& coo, bool compact = false){  

    csr_matrix<IndexType, ValueType> csr;

    csr.num_rows     = coo.num_rows;
    csr.num_cols     = coo.num_cols;
    csr.num_nonzeros = coo.num_nonzeros;
    csr.tag = 0;

    csr.Ap = new_array<IndexType>(csr.num_rows + 1);
    csr.Aj = new_array<IndexType>(csr.num_nonzeros);
    csr.Ax = new_array<ValueType>(csr.num_nonzeros);

    coo_to_csr(coo.I, coo.J, coo.V,
               coo.num_rows, coo.num_cols, coo.num_nonzeros,
               csr.Ap, csr.Aj, csr.Ax);
    
    if (compact) {
        //sum duplicates together
        sum_csr_duplicates(csr.num_rows, csr.num_cols, csr.Ap, csr.Aj, csr.Ax);
        csr.num_nonzeros = csr.Ap[csr.num_rows];
    }

    return csr;
}

////////////////////////////////////////////////////////////////////////////////
//! Convert a csr_matrix to pkt_matrix format
////////////////////////////////////////////////////////////////////////////////

#include "csr_to_pkt.h"

