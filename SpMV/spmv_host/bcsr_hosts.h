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
#include "./bcsr_host_init.h"
#include "./bcsr_host_1x1.h"
#include "./bcsr_host_1x2.h"
#include "./bcsr_host_1x3.h"
#include "./bcsr_host_1x4.h"
#include "./bcsr_host_1x5.h"
#include "./bcsr_host_1x6.h"
#include "./bcsr_host_1x7.h"
#include "./bcsr_host_1x8.h"
#include "./bcsr_host_2x1.h"
#include "./bcsr_host_2x2.h"
#include "./bcsr_host_2x3.h"
#include "./bcsr_host_2x4.h"
#include "./bcsr_host_2x5.h"
#include "./bcsr_host_2x6.h"
#include "./bcsr_host_2x7.h"
#include "./bcsr_host_2x8.h"
#include "./bcsr_host_3x1.h"
#include "./bcsr_host_3x2.h"
#include "./bcsr_host_3x3.h"
#include "./bcsr_host_3x4.h"
#include "./bcsr_host_3x5.h"
#include "./bcsr_host_3x6.h"
#include "./bcsr_host_3x7.h"
#include "./bcsr_host_3x8.h"
#include "./bcsr_host_4x1.h"
#include "./bcsr_host_4x2.h"
#include "./bcsr_host_4x3.h"
#include "./bcsr_host_4x4.h"
#include "./bcsr_host_4x5.h"
#include "./bcsr_host_4x6.h"
#include "./bcsr_host_4x7.h"
#include "./bcsr_host_4x8.h"
#include "./bcsr_host_5x1.h"
#include "./bcsr_host_5x2.h"
#include "./bcsr_host_5x3.h"
#include "./bcsr_host_5x4.h"
#include "./bcsr_host_5x5.h"
#include "./bcsr_host_5x6.h"
#include "./bcsr_host_5x7.h"
#include "./bcsr_host_5x8.h"
#include "./bcsr_host_6x1.h"
#include "./bcsr_host_6x2.h"
#include "./bcsr_host_6x3.h"
#include "./bcsr_host_6x4.h"
#include "./bcsr_host_6x5.h"
#include "./bcsr_host_6x6.h"
#include "./bcsr_host_6x7.h"
#include "./bcsr_host_6x8.h"
#include "./bcsr_host_7x1.h"
#include "./bcsr_host_7x2.h"
#include "./bcsr_host_7x3.h"
#include "./bcsr_host_7x4.h"
#include "./bcsr_host_7x5.h"
#include "./bcsr_host_7x6.h"
#include "./bcsr_host_7x7.h"
#include "./bcsr_host_7x8.h"
#include "./bcsr_host_8x1.h"
#include "./bcsr_host_8x2.h"
#include "./bcsr_host_8x3.h"
#include "./bcsr_host_8x4.h"
#include "./bcsr_host_8x5.h"
#include "./bcsr_host_8x6.h"
#include "./bcsr_host_8x7.h"
#include "./bcsr_host_8x8.h"

template <typename IndexType, typename ValueType>
void spmv_bcsr_serial_host(const bcsr_matrix<IndexType, ValueType>& bcsr, 
                          const ValueType * x,  
                          ValueType * y)
{
  int row_block_size = bcsr.row_block_size;
  int col_block_size = bcsr.col_block_size;

  if ( row_block_size == 0 && col_block_size == 0 )
    spmv_bcsr_serial_host_init ( bcsr, x, y);
  if ( row_block_size == 1 && col_block_size == 1 )
    spmv_bcsr_serial_host_1x1 ( bcsr, x, y);
  if ( row_block_size == 1 && col_block_size == 2 )
    spmv_bcsr_serial_host_1x2 ( bcsr, x, y);
  if ( row_block_size == 1 && col_block_size == 3 )
    spmv_bcsr_serial_host_1x3 ( bcsr, x, y);
  if ( row_block_size == 1 && col_block_size == 4 )
    spmv_bcsr_serial_host_1x4 ( bcsr, x, y);
  if ( row_block_size == 1 && col_block_size == 5 )
    spmv_bcsr_serial_host_1x5 ( bcsr, x, y);
  if ( row_block_size == 1 && col_block_size == 6 )
    spmv_bcsr_serial_host_1x6 ( bcsr, x, y);
  if ( row_block_size == 1 && col_block_size == 7 )
    spmv_bcsr_serial_host_1x7 ( bcsr, x, y);
  if ( row_block_size == 1 && col_block_size == 8 )
    spmv_bcsr_serial_host_1x8 ( bcsr, x, y);
  if ( row_block_size == 2 && col_block_size == 1 )
    spmv_bcsr_serial_host_2x1 ( bcsr, x, y);
  if ( row_block_size == 2 && col_block_size == 2 )
    spmv_bcsr_serial_host_2x2 ( bcsr, x, y);
  if ( row_block_size == 2 && col_block_size == 3 )
    spmv_bcsr_serial_host_2x3 ( bcsr, x, y);
  if ( row_block_size == 2 && col_block_size == 4 )
    spmv_bcsr_serial_host_2x4 ( bcsr, x, y);
  if ( row_block_size == 2 && col_block_size == 5 )
    spmv_bcsr_serial_host_2x5 ( bcsr, x, y);
  if ( row_block_size == 2 && col_block_size == 6 )
    spmv_bcsr_serial_host_2x6 ( bcsr, x, y);
  if ( row_block_size == 2 && col_block_size == 7 )
    spmv_bcsr_serial_host_2x7 ( bcsr, x, y);
  if ( row_block_size == 2 && col_block_size == 8 )
    spmv_bcsr_serial_host_2x8 ( bcsr, x, y);
  if ( row_block_size == 3 && col_block_size == 1 )
    spmv_bcsr_serial_host_3x1 ( bcsr, x, y);
  if ( row_block_size == 3 && col_block_size == 2 )
    spmv_bcsr_serial_host_3x2 ( bcsr, x, y);
  if ( row_block_size == 3 && col_block_size == 3 )
    spmv_bcsr_serial_host_3x3 ( bcsr, x, y);
  if ( row_block_size == 3 && col_block_size == 4 )
    spmv_bcsr_serial_host_3x4 ( bcsr, x, y);
  if ( row_block_size == 3 && col_block_size == 5 )
    spmv_bcsr_serial_host_3x5 ( bcsr, x, y);
  if ( row_block_size == 3 && col_block_size == 6 )
    spmv_bcsr_serial_host_3x6 ( bcsr, x, y);
  if ( row_block_size == 3 && col_block_size == 7 )
    spmv_bcsr_serial_host_3x7 ( bcsr, x, y);
  if ( row_block_size == 3 && col_block_size == 8 )
    spmv_bcsr_serial_host_3x8 ( bcsr, x, y);
  if ( row_block_size == 4 && col_block_size == 1 )
    spmv_bcsr_serial_host_4x1 ( bcsr, x, y);
  if ( row_block_size == 4 && col_block_size == 2 )
    spmv_bcsr_serial_host_4x2 ( bcsr, x, y);
  if ( row_block_size == 4 && col_block_size == 3 )
    spmv_bcsr_serial_host_4x3 ( bcsr, x, y);
  if ( row_block_size == 4 && col_block_size == 4 )
    spmv_bcsr_serial_host_4x4 ( bcsr, x, y);
  if ( row_block_size == 4 && col_block_size == 5 )
    spmv_bcsr_serial_host_4x5 ( bcsr, x, y);
  if ( row_block_size == 4 && col_block_size == 6 )
    spmv_bcsr_serial_host_4x6 ( bcsr, x, y);
  if ( row_block_size == 4 && col_block_size == 7 )
    spmv_bcsr_serial_host_4x7 ( bcsr, x, y);
  if ( row_block_size == 4 && col_block_size == 8 )
    spmv_bcsr_serial_host_4x8 ( bcsr, x, y);
  if ( row_block_size == 5 && col_block_size == 1 )
    spmv_bcsr_serial_host_5x1 ( bcsr, x, y);
  if ( row_block_size == 5 && col_block_size == 2 )
    spmv_bcsr_serial_host_5x2 ( bcsr, x, y);
  if ( row_block_size == 5 && col_block_size == 3 )
    spmv_bcsr_serial_host_5x3 ( bcsr, x, y);
  if ( row_block_size == 5 && col_block_size == 4 )
    spmv_bcsr_serial_host_5x4 ( bcsr, x, y);
  if ( row_block_size == 5 && col_block_size == 5 )
    spmv_bcsr_serial_host_5x5 ( bcsr, x, y);
  if ( row_block_size == 5 && col_block_size == 6 )
    spmv_bcsr_serial_host_5x6 ( bcsr, x, y);
  if ( row_block_size == 5 && col_block_size == 7 )
    spmv_bcsr_serial_host_5x7 ( bcsr, x, y);
  if ( row_block_size == 5 && col_block_size == 8 )
    spmv_bcsr_serial_host_5x8 ( bcsr, x, y);
  if ( row_block_size == 6 && col_block_size == 1 )
    spmv_bcsr_serial_host_6x1 ( bcsr, x, y);
  if ( row_block_size == 6 && col_block_size == 2 )
    spmv_bcsr_serial_host_6x2 ( bcsr, x, y);
  if ( row_block_size == 6 && col_block_size == 3 )
    spmv_bcsr_serial_host_6x3 ( bcsr, x, y);
  if ( row_block_size == 6 && col_block_size == 4 )
    spmv_bcsr_serial_host_6x4 ( bcsr, x, y);
  if ( row_block_size == 6 && col_block_size == 5 )
    spmv_bcsr_serial_host_6x5 ( bcsr, x, y);
  if ( row_block_size == 6 && col_block_size == 6 )
    spmv_bcsr_serial_host_6x6 ( bcsr, x, y);
  if ( row_block_size == 6 && col_block_size == 7 )
    spmv_bcsr_serial_host_6x7 ( bcsr, x, y);
  if ( row_block_size == 6 && col_block_size == 8 )
    spmv_bcsr_serial_host_6x8 ( bcsr, x, y);
  if ( row_block_size == 7 && col_block_size == 1 )
    spmv_bcsr_serial_host_7x1 ( bcsr, x, y);
  if ( row_block_size == 7 && col_block_size == 2 )
    spmv_bcsr_serial_host_7x2 ( bcsr, x, y);
  if ( row_block_size == 7 && col_block_size == 3 )
    spmv_bcsr_serial_host_7x3 ( bcsr, x, y);
  if ( row_block_size == 7 && col_block_size == 4 )
    spmv_bcsr_serial_host_7x4 ( bcsr, x, y);
  if ( row_block_size == 7 && col_block_size == 5 )
    spmv_bcsr_serial_host_7x5 ( bcsr, x, y);
  if ( row_block_size == 7 && col_block_size == 6 )
    spmv_bcsr_serial_host_7x6 ( bcsr, x, y);
  if ( row_block_size == 7 && col_block_size == 7 )
    spmv_bcsr_serial_host_7x7 ( bcsr, x, y);
  if ( row_block_size == 7 && col_block_size == 8 )
    spmv_bcsr_serial_host_7x8 ( bcsr, x, y);
  if ( row_block_size == 8 && col_block_size == 1 )
    spmv_bcsr_serial_host_8x1 ( bcsr, x, y);
  if ( row_block_size == 8 && col_block_size == 2 )
    spmv_bcsr_serial_host_8x2 ( bcsr, x, y);
  if ( row_block_size == 8 && col_block_size == 3 )
    spmv_bcsr_serial_host_8x3 ( bcsr, x, y);
  if ( row_block_size == 8 && col_block_size == 4 )
    spmv_bcsr_serial_host_8x4 ( bcsr, x, y);
  if ( row_block_size == 8 && col_block_size == 5 )
    spmv_bcsr_serial_host_8x5 ( bcsr, x, y);
  if ( row_block_size == 8 && col_block_size == 6 )
    spmv_bcsr_serial_host_8x6 ( bcsr, x, y);
  if ( row_block_size == 8 && col_block_size == 7 )
    spmv_bcsr_serial_host_8x7 ( bcsr, x, y);
  if ( row_block_size == 8 && col_block_size == 8 )
    spmv_bcsr_serial_host_8x8 ( bcsr, x, y);

}

