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
//! Compute y += A*x for a hybrid ELL/COO matrix A and column vectors x and y
//! @param hyb        hyb_matrix
//! @param x          column vector
//! @param y          column vector
////////////////////////////////////////////////////////////////////////////////
template <typename IndexType, typename ValueType>
void spmv_hyb_serial_host(const hyb_matrix<IndexType,ValueType>& hyb, 
                          const ValueType * x, 
                                ValueType * y)
{
    spmv_ell_serial_host(hyb.ell, x, y);
    spmv_coo_serial_host(hyb.coo, x, y);
}

