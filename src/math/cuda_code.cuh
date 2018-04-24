#pragma once

#include <cstdio>

typedef unsigned short ushort;
typedef unsigned char ubyte;
typedef ubyte* ubyte_ptr;


/*
 * __global__ generates error over qtcreator
__global__ void 
gol_step
(
    ubyte_ptr m_data,
    ubyte_ptr m_data_out,
    const unsigned matrix_height,
    const unsigned matrix_width
);
*/

bool gol_iterate
(
    ubyte_ptr& m_data,
    ubyte_ptr& m_data_out,
    size_t matrix_height,
    size_t matrix_width,
    size_t iterations,
    ushort threads
); 
