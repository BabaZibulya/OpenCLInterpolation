
int coord(unsigned int i, unsigned int j, unsigned int width, int channel) {
    return 3 * (i + j * width) + channel;
}

__kernel void convolution(
    __global unsigned char *src,
    __global unsigned char *dst,
    const unsigned int width,
    const unsigned int height,
    __global float *kern,
    const unsigned int kernel_size)
{
    int i = get_global_id(0);
    int j = get_global_id(1);

    char resultR = 0;
    char resultG = 0;
    char resultB = 0;

    unsigned char radius = kernel_size / 2;

    for (int kj = 0; kj < kernel_size; ++kj) {
        for (int ki = 0; ki < kernel_size; ++ki) {
            int kernel_el = ki + kj * kernel_size;

            int coordI = clamp(i + ki - radius, 0, (int)width);
            int coordJ = clamp(j + kj - radius, 0, (int)height);

            resultR += src[coord(coordI, coordJ, width, 0)] * kern[kernel_el];
            resultG += src[coord(coordI, coordJ, width, 1)] * kern[kernel_el];
            resultB += src[coord(coordI, coordJ, width, 2)] * kern[kernel_el];
        }
    }

    dst[coord(i, j, width, 0)] = resultR;
    dst[coord(i, j, width, 1)] = resultG;
    dst[coord(i, j, width, 2)] = resultB;
}