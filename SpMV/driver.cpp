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


#include <iostream>
#include <stdio.h>
#include "cmdline.h"
#include "gallery.h"
#include "tests.h"
#include "config.h"
#include "timer.h"

void usage(int argc, char** argv)
{
    std::cout << "Usage:\n";
    std::cout << "\t" << argv[0] << " with following parameters:\n";
    std::cout << "\t" << " my_matrix.mtx\n";
    std::cout << "\t" << " --precision=32(or 64)\n";
    std::cout << "Note: my_matrix.mtx must be real-valued sparse matrix in the MatrixMarket file format.\n"; 
}


template <typename IndexType, typename ValueType>
void run_all_kernels(int argc, char **argv)
{
    char * mm_filename = NULL;
    for(int i = 1; i < argc; i++){
        if(argv[i][0] != '-'){
            mm_filename = argv[i];
            break;
        }
    }

    csr_matrix<IndexType,ValueType> csr;
    
    if(mm_filename == NULL)
    {
//        csr = laplacian_5pt<IndexType,ValueType>(MAT_GRID_SIZE);  //TODO automatic value
        printf("No input MM file!\n");
        return;
    }
    else
        csr = read_csr_matrix<IndexType,ValueType>(mm_filename);
    printf("Using %d-by-%d matrix with %d nonzero values\n", csr.num_rows, csr.num_cols, csr.num_nonzeros); 

    // fill matrix with random values: some matrices have extreme values, 
    // which makes correctness testing difficult, especially in single precision
    srand(13);
    for(IndexType i = 0; i < csr.num_nonzeros; i++){
        csr.Ax[i] = 1.0 - 2.0 * (rand() / (RAND_MAX + 1.0)); 
    }
    
    printf("\nfile=%s\n", mm_filename);
    printf("rows=%d\n", csr.num_rows);
    printf("cols=%d\n", csr.num_cols);
    printf("nonzeros=%d\n", csr.num_nonzeros);
 
    double csr_gflops, coo_gflops;
    double max_gflops = 0;
    char *best_format;
    double best_nonzeros_ratio = 0;

    // timer run_time_struct;
    
    coo_matrix<IndexType,ValueType> coo = csr_to_coo<IndexType,ValueType>(csr); 
    delete_host_matrix(coo);

    // test_csr_matrix_kernels(csr, &csr_gflops);
    // test_coo_matrix_kernels(csr, &coo_gflops);

    // double run_time = run_time_struct.milliseconds_elapsed();
    // printf ("Run time: %8.4lf ms\n", run_time);

    // if (max_gflops < csr_gflops)
    // {
	   // max_gflops = csr_gflops;
	   // best_format = "CSR";
    // }
    // if (max_gflops < coo_gflops)
    // {
	   // max_gflops = coo_gflops;
	   // best_format = "COO";
    // }
    // printf ("\n\nThe best format: %s\n", best_format);

    delete_host_matrix(csr);
}

int main(int argc, char** argv)
{
    if (get_arg(argc, argv, "help") != NULL){
        usage(argc, argv);
        return EXIT_SUCCESS;
    }

    int precision = 32;
    char * precision_str = get_argval(argc, argv, "precision");
    if(precision_str != NULL)
        precision = atoi(precision_str);
    printf("Using %d-bit floating point precision\n", precision);

    if(precision ==  32)
        run_all_kernels<int, float>(argc,argv);
    else if(precision == 64)
        run_all_kernels<int, double>(argc,argv);
    else{
        usage(argc, argv);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

