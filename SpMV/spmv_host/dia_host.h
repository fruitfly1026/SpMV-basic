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
void __spmv_dia_serial_host_sse_float(const int num_rows,
                            const int num_cols,
                            const int complete_ndiags,
                            const int stride,
                            const int       * offsets, 
                            const float * data, 
                            const float * x,
                                  float * y);

void __spmv_dia_serial_host_sse_double(const int num_rows,
                            const int num_cols,
                            const int complete_ndiags,
                            const int stride,
                            const int       * offsets, 
                            const double * data, 
                            const double * x,
                                  double * y);

}
#endif

////////////////////////////////////////////////////////////////////////////////
//! Compute y += A*x for a sparse DIA matrix A and dense vectors x and y
//! @param num_rows   number of rows in A
//! @param num_cols   number of columns in A
//! @param complete_ndiags  number of diagonals 
//! @param offsets    DIA offset array
//! @param data       DIA data array
//! @param x          column vector
//! @param y          column vector
////////////////////////////////////////////////////////////////////////////////
template <typename IndexType, typename ValueType>
void __spmv_dia_serial_host_simple(const IndexType num_rows,
                            const IndexType num_cols,
                            const IndexType complete_ndiags,
                            const IndexType stride,
                            const int       * offsets, 
                            const ValueType * data, 
                            const ValueType * x,
                                  ValueType * y)
{
    for(IndexType i = 0; i < complete_ndiags; i++){
        const int k = offsets[i];  //diagonal offset

        const IndexType i_start = std::max((int)0,-k);
        const IndexType j_start = std::max((int)0, k);

        //number of elements to process
        const IndexType N = std::min(num_rows - i_start, num_cols - j_start);

        const ValueType * d_ = data + i*stride + i_start;
        const ValueType * x_ = x + j_start;
              ValueType * y_ = y + i_start;

        for(IndexType n = 0; n < N; n++){
            y_[n] += d_[n] * x_[n]; 
        }
    }
}

#if 0
template <typename IndexType, typename ValueType>
void __spmv_dia_serial_host_cache(const IndexType num_rows,
                            const IndexType num_cols,
                            const IndexType complete_ndiags,
                            const IndexType stride,
                            const int       * offsets, 
                            const ValueType * data, 
                            const ValueType * x,
                                  ValueType * y)
{
}
#endif

template <typename IndexType, typename ValueType>
void spmv_dia_serial_host_simple(const dia_matrix<IndexType, ValueType>& dia, 
                          const ValueType * x,  
                                ValueType * y)
{
    __spmv_dia_serial_host_simple(dia.num_rows, dia.num_cols, dia.complete_ndiags, dia.stride,
                           dia.diag_offsets, dia.diag_data, 
                           x, y);
}

#if 0
//SSE optimization kernels
void tmp_spmv_dia_serial_host_sse(const dia_matrix<int, float>& dia, 
                              const float * x,  
                                float * y)
{
    __spmv_dia_serial_host_sse_float(dia.num_rows, dia.num_cols, dia.complete_ndiags,dia.stride, 
                             dia.diag_offsets, dia.diag_data, 
                             x, y);
}

void tmp_spmv_dia_serial_host_sse(const dia_matrix<int, double>& dia, 
                              const double * x,  
                                double * y)
{
    __spmv_dia_serial_host_sse_double(dia.num_rows, dia.num_cols, dia.complete_ndiags, dia.stride,
                           dia.diag_offsets, dia.diag_data, 
                           x, y);
}
template <typename IndexType, typename ValueType>
void spmv_dia_serial_host_sse(const dia_matrix<IndexType, ValueType>& dia, 
                          const ValueType * x,  
                                ValueType * y)
{
    tmp_spmv_dia_serial_host_sse(dia, x, y);
}
#endif