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

////////////////////////////////////////////////////////////////////////////////
//! Compute y += A*x for a sparse MIX matrix A and column vectors x and y
//! @param num_rows   number of rows in A
//! @param Ap         MIX pointer array
//! @param Aj         MIX index array
//! @param Ax         MIX data array
//! @param x          column vector
//! @param y          column vector
////////////////////////////////////////////////////////////////////////////////

template <typename IndexType, typename ValueType>
void spmv_mix_serial_host(const mix_matrix<IndexType, ValueType>& mix, 
                          const ValueType * x,  
                                ValueType * y)
{
    for (IndexType i = 0; i < mix.num_rows; i++)
    {
        const IndexType row_start = mix.Ap[i];
        const IndexType row_end   = mix.Ap[i+1];
        ValueType sum = y[i];
        for (IndexType jj = row_start; jj < row_end; jj++) {            
            sum += x[jj] * mix.Ax[jj];
        }
        y[i] = sum; 
    }
}


