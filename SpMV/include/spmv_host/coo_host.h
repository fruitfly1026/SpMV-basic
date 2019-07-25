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
void __spmv_coo_serial_host_sse_float(const int num_nonzeros,
                            const int * rows,
                            const int * cols, 
                            const float * data, 
                            const float * x,
                                  float * y);

void __spmv_coo_serial_host_sse_double(const int num_nonzeros,
                            const int * rows,
                            const int * cols, 
                            const double * data, 
                            const double * x,
                                  double * y);

}
#endif

////////////////////////////////////////////////////////////////////////////////
//! Compute y += A*x for a sparse COO matrix A and column vectors x and y
//! @param num_nonzeros   number of nonzeros in A
//! @param rows           COO row indices array
//! @param cols           COO column index array
//! @param data           COO data array
//! @param x              column vector
//! @param y              column vector
////////////////////////////////////////////////////////////////////////////////
template <typename IndexType, typename ValueType>
void __spmv_coo_serial_host_simple(const IndexType num_nonzeros,
                            const IndexType * rows, 
                            const IndexType * cols, 
                            const ValueType * data, 
                            const ValueType * x,  
                                  ValueType * y)
{
    for (IndexType i = 0; i < num_nonzeros; i++){   
        y[rows[i]] += data[i] * x[cols[i]];
    }
}

template <typename IndexType, typename ValueType>
void spmv_coo_serial_host_simple(const coo_matrix<IndexType, ValueType>& coo, 
                          const ValueType * x,  
                                ValueType * y)
{
    __spmv_coo_serial_host_simple(coo.num_nonzeros, coo.I, coo.J, coo.V, x, y);
}

#if 0
//SSE optimization kernels
void tmp_spmv_coo_serial_host_sse(const coo_matrix<int, float>& coo, 
                              const float * x,  
                                float * y)
{
    __spmv_coo_serial_host_sse_float(coo.num_nonzeros, coo.I, coo.J, coo.V, x, y);
}

void tmp_spmv_coo_serial_host_sse(const coo_matrix<int, double>& coo, 
                              const double * x,  
                                double * y)
{
    __spmv_coo_serial_host_sse_double(coo.num_nonzeros, coo.I, coo.J, coo.V, x, y);
}
template <typename IndexType, typename ValueType>
void spmv_coo_serial_host_sse(const coo_matrix<IndexType, ValueType>& coo, 
                          const ValueType * x,  
                                ValueType * y)
{
    tmp_spmv_coo_serial_host_sse(coo, x, y);
}
#endif

