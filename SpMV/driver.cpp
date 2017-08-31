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
    std::cout << "\t" << " --max_diags_limit=20\n";
    std::cout << "\t" << " --dia_fill_ratio=0.95\n";
    std::cout << "\t" << " --min_diags_ratio=0.1\n";
    std::cout << "\t" << " --ell_max_cols=100\n";
    std::cout << "\t" << " --ell_min_deviation=1\n";
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

#ifdef COLLECT_FEATURES
    FILE *fp_feature = fopen( MAT_FEATURES, "w");
#endif

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

#ifdef COLLECT_FEATURES
    fprintf(fp_feature, "M : %d\n", csr.num_rows);
    fprintf(fp_feature, "N : %d\n", csr.num_cols);
    fprintf(fp_feature, "NNZ : %d\n", csr.num_nonzeros);
#endif

#ifdef TESTING
//print in CSR format
    printf("Writing matrix in CSR format to test_CSR ...\n");
    FILE *fp = fopen("test_CSR", "w");
    fprintf(fp, "%d\t%d\t%d\n", csr.num_rows, csr.num_cols, csr.num_nonzeros);
    fprintf(fp, "csr.Ap:\n");
    for (IndexType i=0; i<csr.num_rows+1; i++)
    {
      fprintf(fp, "%d  ", csr.Ap[i]);
    }
    fprintf(fp, "\n\n");
    fprintf(fp, "csr.Aj:\n");
    for (IndexType i=0; i<csr.num_nonzeros; i++)
    {
      fprintf(fp, "%d  ", csr.Aj[i]);
    }
    fprintf(fp, "\n\n");
    fprintf(fp, "csr.Ax:\n");
    for (IndexType i=0; i<csr.num_nonzeros; i++)
    {
      fprintf(fp, "%f  ", csr.Ax[i]);
    }
    fprintf(fp, "\n");
    fclose(fp);
#endif 

    // fill matrix with random values: some matrices have extreme values, 
    // which makes correctness testing difficult, especially in single precision
    srand(13);
    for(IndexType i = 0; i < csr.num_nonzeros; i++){
        csr.Ax[i] = 1.0 - 2.0 * (rand() / (RAND_MAX + 1.0)); 
    }
    
    printf("\nfile=%s rows=%d cols=%d nonzeros=%d\n", mm_filename, csr.num_rows, csr.num_cols, csr.num_nonzeros);

    //Input the best kernel for each format
    int dia_kernel_tag = 0, ell_kernel_tag = 0, csr_kernel_tag = 0, coo_kernel_tag = 0, bcsr_kernel_tag = 0;
    FILE *fp_tag;
    fp_tag = fopen(KERNEL_TAGS, "r");
    fscanf(fp_tag, "%d", &dia_kernel_tag);
    fscanf(fp_tag, "%d", &ell_kernel_tag);
    fscanf(fp_tag, "%d", &csr_kernel_tag);
    fscanf(fp_tag, "%d", &coo_kernel_tag);
    fscanf(fp_tag, "%d", &bcsr_kernel_tag);
    fclose(fp_tag);

    printf("dia_kernel_tag : %d\n", dia_kernel_tag );
    printf("ell_kernel_tag : %d\n", ell_kernel_tag );
    printf("csr_kernel_tag : %d\n", csr_kernel_tag );
    printf("coo_kernel_tag : %d\n", coo_kernel_tag );
    printf("bcsr_kernel_tag : %d\n", bcsr_kernel_tag );
    
    double dia_gflops, ell_gflops, csr_gflops, coo_gflops, bcsr_gflops;
    double dia_diff, ell_diff, csr_diff, coo_diff, bcsr_diff;
    double max_gflops = 0;
    char *best_format;
    int best_row_block_size = 0, best_col_block_size = 0;
    double best_nonzeros_ratio = 0;

    timer run_time_struct;
    
    test_dia_matrix_kernels(csr, dia_kernel_tag, &dia_gflops, fp_feature);
    fflush(stdout);
    test_ell_matrix_kernels(csr, ell_kernel_tag, &ell_gflops, fp_feature);
    fflush(stdout);
    test_csr_matrix_kernels(csr, csr_kernel_tag, &csr_gflops, fp_feature);
    fflush(stdout);
    test_coo_matrix_kernels(csr, coo_kernel_tag, &coo_gflops, fp_feature);
    fflush(stdout);
    test_bcsr_matrix_kernels(csr, bcsr_kernel_tag, &bcsr_gflops, &best_row_block_size, &best_col_block_size, &best_nonzeros_ratio, fp_feature);
    fflush(stdout);

    double run_time = run_time_struct.milliseconds_elapsed();
    printf ("Run time: %8.4lf ms\n", run_time);

    if (max_gflops < dia_gflops)
    {
	   max_gflops = dia_gflops;
	   best_format = "DIA";
    }
    if (max_gflops < ell_gflops)
    {
	   max_gflops = ell_gflops;
	   best_format = "ELL";
    }
    if (max_gflops < csr_gflops)
    {
	   max_gflops = csr_gflops;
	   best_format = "CSR";
    }
    if (max_gflops < coo_gflops)
    {
	   max_gflops = coo_gflops;
	   best_format = "COO";
    }
    if (max_gflops < bcsr_gflops)
    {
     max_gflops = bcsr_gflops;
     best_format = "BCSR";
    }

   dia_diff = dia_gflops / max_gflops;
   ell_diff = ell_gflops / max_gflops;
   csr_diff = csr_gflops / max_gflops;
   coo_diff = coo_gflops / max_gflops;
   bcsr_diff = bcsr_gflops / max_gflops;

#ifdef COLLECT_FEATURES
    fprintf(fp_feature, "DIA perf : %lf\n", dia_gflops);
    fprintf(fp_feature, "ELL perf : %lf\n", ell_gflops);
    fprintf(fp_feature, "CSR perf : %lf\n", csr_gflops);
    fprintf(fp_feature, "COO perf : %lf\n", coo_gflops);
    fprintf(fp_feature, "BCSR perf : %lf\n", bcsr_gflops);
    fprintf (fp_feature, "Best_format : %s\n", best_format);
#endif
    fclose (fp_feature);

    printf ("\n\nThe best format: %s\n", best_format);
    printf ("DIA diff: %.2lf\n", dia_diff);
    printf ("ELL diff: %.2lf\n", ell_diff);
    printf ("CSR diff: %.2lf\n", csr_diff);
    printf ("COO diff: %.2lf\n", coo_diff);
    printf ("BCSR diff: %.2lf; best_row: %d; best_col: %d\n", bcsr_diff, best_row_block_size, best_col_block_size );

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

