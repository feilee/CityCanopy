#include "cuda_code.cuh"
#include "../../third_party/others/OpenGlCudaHelper.h"
#include <algorithm>


__global__ void 
gol_step
(
    const ubyte_ptr m_data,
    ubyte_ptr       m_data_out,
    const unsigned  matrix_height,
    const unsigned  matrix_width
)
{
    unsigned n_size = matrix_height * matrix_width;

    for (unsigned cell_id = blockIdx.x * blockDim.x + threadIdx.x;
        cell_id < n_size;
        cell_id += blockDim.x * gridDim.x) 
    {

        uint x1 = cell_id % matrix_width;
        uint y1 = cell_id - x1;     // position 0 to matrix y1 as array

        uint x0 = (x1 + matrix_width - 1) % matrix_width;   // left
        uint x2 = (x1 + 1) % matrix_width;                  // right

        uint y0 = (y1 + n_size - matrix_width) % n_size;    // up
        uint y2 = (y1 + matrix_width) % n_size;             // down

        // Count alive cells.
        uint alive_cells = 
                  m_data[x0 + y0] + m_data[x1 + y0] + m_data[x2 + y0]
                + m_data[x0 + y1] +        0        + m_data[x2 + y1]
                + m_data[x0 + y2] + m_data[x1 + y2] + m_data[x2 + y2];

        m_data_out[x1 + y1] =
            (
                alive_cells == 3 || 
                alive_cells == 6 || 
                (alive_cells == 2 && m_data[x1 + y1])
            ) ? 1 : 0;
    }
}

bool gol_iterate
(
    ubyte_ptr&  m_data,
    ubyte_ptr&  m_data_out,
    const size_t matrix_height,
    const size_t matrix_width,
    const size_t iterations,
    const ushort threads
) 
{
    //if ((matrix_height * matrix_width) % threads != 0)
    //    return false;

    size_t required_blocks = (matrix_height * matrix_width) / threads;
    ushort blocks = (ushort) std::min((size_t)32768, required_blocks);

    for (size_t i = 0; i < iterations; ++i) 
    {
        gol_step<<<blocks, threads>>>
            (m_data, m_data_out, uint(matrix_height), uint(matrix_width));
        std::swap(m_data, m_data_out);
    }
    mf::checkCudaErrors(cudaDeviceSynchronize());

    return true;
}
