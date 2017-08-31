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
//! Compute y += A*x for a Packet Matrix matrix A and column vectors x and y
//! @param pkt        pkt_matrix
//! @param x          column vector
//! @param y          column vector
////////////////////////////////////////////////////////////////////////////////
template <typename IndexType, typename ValueType>
void spmv_pkt_serial_host(const pkt_matrix<IndexType,ValueType>& pkt, 
                          const ValueType * x, 
                                ValueType * y)
{
    //initialize temp arrays for permuted x and y
    ValueType * x_perm = new_array<ValueType>(pkt.num_cols); 
    ValueType * y_perm = new_array<ValueType>(pkt.num_rows); 
    
    for(IndexType i = 0; i < pkt.num_rows; i++){
        //permute x and y    
        x_perm[i] = x[pkt.permute_new_to_old[i]];
        y_perm[i] = y[pkt.permute_new_to_old[i]];
    }

    for(IndexType N = 0; N < pkt.num_packets; N++){
        IndexType base_row = pkt.row_ptr[N];

        for(IndexType H = 0; H < pkt.threads_per_packet; H++){
            IndexType thread_id = H + N*pkt.threads_per_packet;

            IndexType pos = pkt.packets.pos_start[thread_id];
            IndexType end = pkt.packets.pos_end[thread_id];
            
            while(pos != end){
                const IndexType packed_index = pkt.packets.index_array[pos];

                const IndexType row = base_row + pkt_unpack_row_index(packed_index);
                const IndexType col = base_row + pkt_unpack_col_index(packed_index);
                const ValueType val = pkt.packets.data_array[pos];          

                y_perm[row] += val*x_perm[col];         

                pos    += pkt.threads_per_packet;
            }

        }
    }

    // remaining entries are stored in COO format
    spmv_coo_serial_host<IndexType,ValueType>(pkt.coo, x_perm, y_perm);
   
    for(IndexType i = 0; i < pkt.num_rows; i++){
        //unpermute y    
        y[i] = y_perm[pkt.permute_old_to_new[i]];
    }
    

    //delete temp arrays
    delete_array(x_perm);
    delete_array(y_perm);
}

