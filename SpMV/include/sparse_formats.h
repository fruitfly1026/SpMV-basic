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

#include "mem.h"

////////////////////////////////////////////////////////////////////////////////
//! Defines the following sparse matrix formats
//
// DIA - Diagonal
// ELL - ELLPACK/ITPACK
// CSR - Compressed Sparse Row
// CSC - Compressed Sparse Column
// COO - Coordinate
// PKT - Packet
////////////////////////////////////////////////////////////////////////////////

template<typename IndexType>
struct matrix_shape
{
    typedef IndexType index_type;
    IndexType num_rows, num_cols, num_nonzeros;
    double time, gflops;
    int tag;
};

// DIAgonal matrix
template <typename IndexType, typename ValueType>
struct dia_matrix : public matrix_shape<IndexType> 
{
    typedef IndexType index_type;
    typedef ValueType value_type;

    IndexType stride;
    IndexType complete_ndiags;

    int       * diag_offsets;  //diagonal offsets (must be a signed type)
    ValueType * diag_data;     //nonzero values stored in a (num_diags x num_cols) matrix 
};

// ELLPACK/ITPACK matrix format
template <typename IndexType, typename ValueType>
struct ell_matrix : public matrix_shape<IndexType> 
{
    typedef IndexType index_type;
    typedef ValueType value_type;

    IndexType stride;
    IndexType max_RD;
    IndexType min_RD;

    IndexType * Aj;           //column indices stored in a (cols_per_row x stride) matrix
    ValueType * Ax;           //nonzero values stored in a (cols_per_row x stride) matrix
};

// COOrdinate matrix (aka IJV or Triplet format)
template <typename IndexType, typename ValueType>
struct coo_matrix : public matrix_shape<IndexType> 
{
    typedef IndexType index_type;
    typedef ValueType value_type;

    IndexType * I;  //row indices
    IndexType * J;  //column indices
    ValueType * V;  //nonzero values
};


/*
 *  Compressed Sparse Row matrix (aka CRS)
 */
template <typename IndexType, typename ValueType>
struct csr_matrix : public matrix_shape<IndexType>
{
    typedef IndexType index_type;
    typedef ValueType value_type;

    IndexType * Ap;  //row pointer
    IndexType * Aj;  //column indices
    ValueType * Ax;  //nonzeros
};

/*
 *  MIX matrix format
 */
template <typename IndexType, typename ValueType>
struct mix_matrix : public matrix_shape<IndexType>
{
    typedef IndexType index_type;
    typedef ValueType value_type;

    IndexType * Ap;  //row pointer
    IndexType * Aj;  //column indices
    ValueType * Ax;  //nonzeros
};

/*
 *  Block Compressed Sparse Row matrix (BCSR)
 */
template <typename IndexType, typename ValueType>
struct bcsr_matrix : public matrix_shape<IndexType>
{
    typedef IndexType index_type;
    typedef ValueType value_type;

    int row_block_size;
    int col_block_size;
    IndexType num_block_rows;
    IndexType num_block_cols;
    IndexType num_blocks;
    IndexType num_rows_left;
    double nzs_ratio;

    IndexType * bAp;  //row pointer
    IndexType * bAj;  //column indices
    ValueType * bAx;  //nonzeros
};

/*
 *  Hybrid ELL/COO format
 */
template <typename IndexType, typename ValueType>
struct hyb_matrix : public matrix_shape<IndexType>
{
    typedef IndexType index_type;
    typedef ValueType value_type;

    ell_matrix<IndexType,ValueType> ell; //ELL portion
    coo_matrix<IndexType,ValueType> coo; //COO portion
};



/*
 *  Packet matrix
 */
typedef unsigned int PackedIndexType;
template <typename IndexType, typename ValueType>
struct packet_array : public matrix_shape<IndexType>
{
    typedef IndexType index_type;
    typedef ValueType value_type;

    PackedIndexType * index_array;  // compressed row/col indices
    ValueType * data_array;         // nonzero values

    IndexType * pos_start;          // start ptr into index and data arrays for each thread
    IndexType * pos_end;      

    IndexType total_cycles;         // total amount of work in each thread lane
};

template <typename IndexType, typename ValueType>
struct pkt_matrix : public matrix_shape<IndexType>
{ 
    typedef IndexType index_type;
    typedef ValueType value_type;

    IndexType threads_per_packet;    // # of threads in a block, e.g. 256
    IndexType max_rows_per_packet;   // maximum over all packets

    IndexType num_packets;
    IndexType * row_ptr;             // packet i corresponds to rows row_ptr[i] through row_ptr[i+1] - 1
    IndexType * permute_old_to_new;  
    IndexType * permute_new_to_old;

    packet_array<IndexType,ValueType> packets;
    coo_matrix<IndexType,ValueType> coo;
};

// store row index in upper 16 bits and col index in lower 16 bits
#define pkt_pack_indices(row,col)          (  (row << 16) + col  )
#define pkt_unpack_row_index(packed_index) ( packed_index >> 16  )  
#define pkt_unpack_col_index(packed_index) (packed_index & 0xFFFF)


////////////////////////////////////////////////////////////////////////////////
//! sparse matrix memory management 
////////////////////////////////////////////////////////////////////////////////

template <typename IndexType, typename ValueType>
void delete_dia_matrix(dia_matrix<IndexType,ValueType>& dia){
    delete_array(dia.diag_offsets);  delete_array(dia.diag_data);
}

template <typename IndexType, typename ValueType>
void delete_ell_matrix(ell_matrix<IndexType,ValueType>& ell){
    delete_array(ell.Aj);  delete_array(ell.Ax);
}

template <typename IndexType, typename ValueType>
void delete_csr_matrix(csr_matrix<IndexType,ValueType>& csr){
    delete_array(csr.Ap);  delete_array(csr.Aj);   delete_array(csr.Ax);
}

template <typename IndexType, typename ValueType>
void delete_bcsr_matrix(bcsr_matrix<IndexType,ValueType>& bcsr){
    delete_array(bcsr.bAp);  delete_array(bcsr.bAj);  delete_array(bcsr.bAx);
}

template <typename IndexType, typename ValueType>
void delete_coo_matrix(coo_matrix<IndexType,ValueType>& coo){
    delete_array(coo.I);   delete_array(coo.J);   delete_array(coo.V);
}

template <typename IndexType, typename ValueType>
void delete_packet_array(packet_array<IndexType,ValueType>& pa){
    delete_array(pa.index_array); delete_array(pa.data_array);
    delete_array(pa.pos_start);   delete_array(pa.pos_end);
}

template <typename IndexType, typename ValueType>
void delete_hyb_matrix(hyb_matrix<IndexType,ValueType>& hyb){
    delete_ell_matrix(hyb.ell);
    delete_coo_matrix(hyb.coo);
}

template <typename IndexType, typename ValueType>
void delete_pkt_matrix(pkt_matrix<IndexType, ValueType>& pm)
{
    delete_array(pm.row_ptr);   
    delete_array(pm.permute_new_to_old);   
    delete_array(pm.permute_old_to_new);

    delete_packet_array(pm.packets);  
    delete_coo_matrix(pm.coo);
}


////////////////////////////////////////////////////////////////////////////////
//! host functions
////////////////////////////////////////////////////////////////////////////////

template <typename IndexType, typename ValueType>
void delete_host_matrix(dia_matrix<IndexType,ValueType>& dia){ delete_dia_matrix(dia); }

template <typename IndexType, typename ValueType>
void delete_host_matrix(ell_matrix<IndexType,ValueType>& ell){ delete_ell_matrix(ell); }

template <typename IndexType, typename ValueType>
void delete_host_matrix(coo_matrix<IndexType,ValueType>& coo){ delete_coo_matrix(coo); }

template <typename IndexType, typename ValueType>
void delete_host_matrix(csr_matrix<IndexType,ValueType>& csr){ delete_csr_matrix(csr); }

template <typename IndexType, typename ValueType>
void delete_host_matrix(bcsr_matrix<IndexType,ValueType>& bcsr){ delete_bcsr_matrix(bcsr); }

template <class IndexType, class ValueType>
void delete_host_matrix(hyb_matrix<IndexType,ValueType>& hyb){  delete_hyb_matrix(hyb); }

template <typename IndexType, typename ValueType>
void delete_host_matrix(pkt_matrix<IndexType, ValueType>& pm){ delete_pkt_matrix(pm); }

