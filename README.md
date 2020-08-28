# README #

This is a repo for SpMV implementations for CSR, COO, DIA, ELL, and BCSR, with the input matrix in Matrix Market (.mtx) format. 

For `simpler` branch, only simple SpMV implementations for CSR and COO are supported. 

### Install ###

* `cd SpMV`
* `make` (You may need to change some settings in Makefile).

### Run tests ###

* `./spmv [mtx file] [--precision=32/64]`

### Source code structure ###

* `include` contains include files, spmv implementations in `include/spmv_host`, and sparse format conversion in `include/sparse_conversions.h`.
* `src` contains I/O code for \*.mtx file
* `test_scripts` is for testing plenty of matrices together by giving the matrix list.
* `example_matrices` saves some small sparse matrices in mtx format.
* `config*.h` are for different SpMV kernel execution. (Ignore them for now.)

### Contributors and Contacts ###

* Jiajia Li (fruitfly1026@gmail.com)

### License ###

This repo is released under the MIT License. Please see the 'LICENSE' file for details. Source code is developed based on the code written by Nathan Bell and Michael Garland at NVIDIA.
