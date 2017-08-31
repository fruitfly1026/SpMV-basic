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
#if 0
extern "C"
{
void __spmv_ell_serial_host_sse_float(const int num_rows,
                            const int num_cols,
                            const int max_RD,
                            const int stride,
                            const int   * Aj, 
                            const float * Ax, 
                            const float * x,
                                  float * y);

void __spmv_ell_serial_host_sse_double(const int num_rows,
                            const int num_cols,
                            const int max_RD,
                            const int stride,
                            const int   * Aj, 
                            const double * Ax, 
                            const double * x,
                                  double * y);

}
#endif
////////////////////////////////////////////////////////////////////////////////
//! Compute y += A*x for a sparse ELL matrix A and column vectors x and y
//! @param num_rows          number of rows in A
//! @param num_cols          number of columns in A
//! @param max_RD  number columns in each row (smaller rows are zero padded)
//! @param stride            seperation between row entries (stride >= num_rows, for alignment)
//! @param Aj                ELL column indices
//! @param Ax                ELL nonzero values
//! @param x                 column vector
//! @param y                 column vector
////////////////////////////////////////////////////////////////////////////////
template <typename IndexType, typename ValueType>
void __spmv_ell_serial_host_simple(const IndexType num_rows,
                            const IndexType num_cols,
                            const IndexType max_RD,
                            const IndexType stride,
                            const IndexType * Aj, 
                            const ValueType * Ax, 
                            const ValueType * x,
                                  ValueType * y)
{
    for(IndexType n = 0; n < max_RD; n++){
        const IndexType * Aj_n = Aj + n * stride;
        const ValueType * Ax_n = Ax + n * stride;
        for(IndexType i = 0; i < num_rows; i++){
            y[i] += Ax_n[i] * x[Aj_n[i]];
        }
    }
}

template <typename IndexType, typename ValueType>
void spmv_ell_serial_host_simple(const ell_matrix<IndexType, ValueType>& ell, 
                          const ValueType * x,  
                                ValueType * y)
{
    __spmv_ell_serial_host_simple(ell.num_rows, ell.num_cols, ell.max_RD, ell.stride,
                           ell.Aj, ell.Ax,
                           x, y);
}

#if 0
//SSE optimization kernels
void tmp_spmv_ell_serial_host_sse(const ell_matrix<int, float>& ell, 
                              const float * x,  
                                float * y)
{
    __spmv_ell_serial_host_sse_float(ell.num_rows, ell.num_cols, ell.max_RD,ell.stride, 
                             ell.Aj, ell.Ax, 
                             x, y);
}

void tmp_spmv_ell_serial_host_sse(const ell_matrix<int, double>& ell, 
                              const double * x,  
                                double * y)
{
    __spmv_ell_serial_host_sse_double(ell.num_rows, ell.num_cols, ell.max_RD,ell.stride, 
                             ell.Aj, ell.Ax, 
                             x, y);
}
template <typename IndexType, typename ValueType>
void spmv_ell_serial_host_sse(const ell_matrix<IndexType, ValueType>& ell, 
                          const ValueType * x,  
                                ValueType * y)
{
    tmp_spmv_ell_serial_host_sse(ell, x, y);
}

#endif