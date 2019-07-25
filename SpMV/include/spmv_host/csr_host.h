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
void __spmv_csr_serial_host_sse_float(const int num_rows,
                            const int * Ap,
                            const int * Aj, 
                            const float * Ax, 
                            const float * x,
                                  float * y);

void __spmv_csr_serial_host_sse_double(const int num_rows,
                            const int * Ap,
                            const int * Aj, 
                            const double * Ax, 
                            const double * x,
                                  double * y);

}
#endif

////////////////////////////////////////////////////////////////////////////////
//! Compute y += A*x for a sparse CSR matrix A and column vectors x and y
//! @param num_rows   number of rows in A
//! @param Ap         CSR pointer array
//! @param Aj         CSR index array
//! @param Ax         CSR data array
//! @param x          column vector
//! @param y          column vector
////////////////////////////////////////////////////////////////////////////////
template <typename IndexType, typename ValueType>
void __spmv_csr_serial_host_simple(const IndexType num_rows, 
                            const IndexType * Ap, 
                            const IndexType * Aj, 
                            const ValueType * Ax, 
                            const ValueType * x,    
                                  ValueType * y)    
{
    for (IndexType i = 0; i < num_rows; i++){
        const IndexType row_start = Ap[i];
        const IndexType row_end   = Ap[i+1];
        ValueType sum = y[i];
        for (IndexType jj = row_start; jj < row_end; jj++) {            
            const IndexType j = Aj[jj];  //column index
            sum += x[j] * Ax[jj];
        }
        y[i] = sum; 
    }
}

template <typename IndexType, typename ValueType>
void spmv_csr_serial_host_simple(const csr_matrix<IndexType, ValueType>& csr, 
                          const ValueType * x,  
                                ValueType * y)
{
    __spmv_csr_serial_host_simple(csr.num_rows, csr.Ap, csr.Aj, csr.Ax, x, y);
}

#if 0
//SSE optimization kernels
void tmp_spmv_csr_serial_host_sse(const csr_matrix<int, float>& csr, 
                              const float * x,  
                                float * y)
{
    __spmv_csr_serial_host_sse_float(csr.num_rows, csr.Ap, csr.Aj, csr.Ax, x, y);
}

void tmp_spmv_csr_serial_host_sse(const csr_matrix<int, double>& csr, 
                              const double * x,  
                                double * y)
{
    __spmv_csr_serial_host_sse_double(csr.num_rows, csr.Ap, csr.Aj, csr.Ax, x, y);
}
template <typename IndexType, typename ValueType>
void spmv_csr_serial_host_sse(const csr_matrix<IndexType, ValueType>& csr, 
                          const ValueType * x,  
                                ValueType * y)
{
    tmp_spmv_csr_serial_host_sse(csr, x, y);
}

#endif
