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

#include "../config.h"
#include "sparse_io.h"
#include "sparse_operations.h"
#include "test_spmv.h"
#include "benchmark_spmv.h"
#include "spmv_host/csr_host.h"
#include "spmv_host/coo_host.h"


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
