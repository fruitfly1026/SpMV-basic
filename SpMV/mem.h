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

#include <stdlib.h>

/////////////////////////////////////////////////////////////////////
// allocate memory on host and device
/////////////////////////////////////////////////////////////////////


template <typename T>
T * new_array(const size_t N) 
{ 
    //dispatch on location
        return (T*) malloc(N * sizeof(T));
}

template <typename T>
void delete_array(T * p) 
{ 
    //dispatch on location
        free(p);
}


/////////////////////////////////////////////////////////////////////
// transfer data between host and device
/////////////////////////////////////////////////////////////////////

template<typename T>
void memcpy_array(T * dst, const T * src, const size_t N)
{
        memcpy (dst, src, sizeof(T)*N);
}


template <typename T>
T * copy_array(const T * src, const size_t N)
{
    T * dst = new_array<T>(N);
    memcpy_array(dst, src, N);
    return dst;
}

