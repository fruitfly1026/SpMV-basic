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

// A simple timer class

#include "mem.h"
#include "time.h"
#include "rdtsc.h"
#include "config.h"

class timer
{
    long long int start;
    long long int end;

    public:
    timer()
    { 
        start = rdtsc();
    }

    float milliseconds_elapsed()
    { 
        float elapsed_time;
	 end = rdtsc();
        elapsed_time = 1000*(end - start)/FREQ_CPU;
        return elapsed_time;
    }
    float seconds_elapsed()
    { 
        return (end - start)/FREQ_CPU;
    }
};


