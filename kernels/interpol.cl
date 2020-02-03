#define WZZ 1.23f
#define g 9.81f
#define Rs 0.7f

__kernel void interpol(
    __global float *US,
    __global float *VS,
    __global float *HS,
    __global float *QS,
    __global float *TS,
    __global float *F_x,
    __global float *Zmz,
    __global float *Qc,
    const unsigned int Pk,
    const unsigned int Lmz,
    const unsigned int Mmz,
    const unsigned int Nmz
)
{
    int h = get_global_id(0);
    int k = get_global_id(1);
    int j = get_global_id(2);

    if (h >= Pk || k >= Lmz || j >= Mmz) {
        return;
    }

#ifdef __xilinx__
    __attribute__((xcl_pipeline_loop))
#endif
    for (int i = 0; i < Nmz; i++) {
        const unsigned int ind = h + Pk * k + Pk * Lmz * j + Pk * Lmz * Mmz * i;
        float a = (WZZ + US[ind] / 0.321f) * Rs * VS[ind];
        float Tp = TS[ind] * pow( 1000.f / HS[ind], 2.f/7.f );
        float Tv = Tp * (1.f + 0.6078f * QS[ind]);
        Qc[ind] = a - (0.5f * Tv + (1.f - Zmz[k]) * g * F_x[j + i * Mmz] / 0.321f);
    }
}
