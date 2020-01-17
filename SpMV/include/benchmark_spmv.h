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
#include <stdio.h>
#include "sparse_formats.h"
#include "../config.h"
#include "timer.h"

   
template <typename IndexType, typename ValueType>
size_t bytes_per_spmv(const csr_matrix<IndexType,ValueType>& mtx)
{
    size_t bytes = 0;
    bytes += 2*sizeof(IndexType) * mtx.num_rows;     // row pointer
    bytes += 1*sizeof(IndexType) * mtx.num_nonzeros; // column index
    bytes += 2*sizeof(ValueType) * mtx.num_nonzeros; // A[i,j] and x[j]
    bytes += 2*sizeof(ValueType) * mtx.num_rows;     // y[i] = y[i] + ...
    return bytes;
}

template <typename IndexType, typename ValueType>
size_t bytes_per_spmv(const coo_matrix<IndexType,ValueType>& mtx)
{
    size_t bytes = 0;
    bytes += 2*sizeof(IndexType) * mtx.num_nonzeros; // row and column indices
    bytes += 2*sizeof(ValueType) * mtx.num_nonzeros; // A[i,j] and x[j]

    std::vector<size_t> occupied_rows(mtx.num_rows, 0);
    for(size_t n = 0; n < mtx.num_nonzeros; n++)
        occupied_rows[mtx.I[n]] = 1;
    for(size_t n = 0; n < mtx.num_rows; n++)
        if(occupied_rows[n] == 1)
            bytes += 2*sizeof(ValueType);            // y[i] = y[i] + ...
    return bytes;
}

template <typename SparseMatrix, typename SpMV>
double benchmark_spmv(SparseMatrix & sp_host, SpMV spmv, const int min_iterations, const int max_iterations, const double seconds, const char * method_name)
{

    typedef typename SparseMatrix::value_type ValueType;
    typedef typename SparseMatrix::index_type IndexType;

    //initialize host arrays
    ValueType * x_host = new_array<ValueType>(sp_host.num_cols);
    ValueType * y_host = new_array<ValueType>(sp_host.num_rows);

    for(IndexType i = 0; i < sp_host.num_cols; i++)
        x_host[i] = rand() / (RAND_MAX + 1.0); 
    std::fill(y_host, y_host + sp_host.num_rows, 0);

    // warmup    
    timer time_one_iteration;
    spmv(sp_host, x_host, y_host);
    double estimated_time = time_one_iteration.seconds_elapsed();
//    printf("estimated time for once %f\n", (float) estimated_time);

    // determine # of seconds dynamically
    int num_iterations;
    num_iterations = max_iterations;

    if (estimated_time == 0)
        num_iterations = max_iterations;
    else
        num_iterations = std::min(max_iterations, std::max(min_iterations, (int) (seconds / estimated_time)) ); 
    printf("\tPerforming %d iterations\n", num_iterations);

    // time several SpMV iterations
    timer t;
    for(int i = 0; i < num_iterations; i++)
       spmv(sp_host, x_host, y_host);
    double msec_per_iteration = t.milliseconds_elapsed() / (double) num_iterations;
    double sec_per_iteration = msec_per_iteration / 1000.0;
    double GFLOPs = (sec_per_iteration == 0) ? 0 : (2.0 * (double) sp_host.num_nonzeros / sec_per_iteration) / 1e9;
    double GBYTEs = (sec_per_iteration == 0) ? 0 : ((double) bytes_per_spmv(sp_host) / sec_per_iteration) / 1e9;
    sp_host.gflops = GFLOPs;
    sp_host.time = msec_per_iteration;
    const char * location = "cpu" ;
    printf("\tbenchmarking %-20s [%s]: %8.4f ms ( %5.2f GFLOP/s %5.1f GB/s)\n", \
            method_name, location, msec_per_iteration, GFLOPs, GBYTEs); 

    //deallocate buffers
    delete_array(x_host);
    delete_array(y_host);

    return msec_per_iteration;
}

template <typename SparseMatrix, typename SpMV>
double benchmark_spmv_on_host(SparseMatrix & sp_host, SpMV spmv, char * method_name = NULL)
{
    return benchmark_spmv<SparseMatrix, SpMV>(sp_host, spmv, MIN_ITER, MAX_ITER, TIME_LIMIT, method_name);
}

