#define WZZ 1.23f
#define g 9.81f
#define Rs 0.7f

uint getIndex4D(const unsigned int i,
                const unsigned int j,
                const unsigned int k,
                const unsigned int h, 
                const unsigned int Pk,
                const unsigned int Lmz,
                const unsigned int Mmz,
                const unsigned int Nmz) {
    return h + Pk * k + Pk * Lmz * j + Pk * Lmz * Mmz * i;
}

__kernel __attribute__ ((reqd_work_group_size(1, 1, 1)))
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
    for (int h = 0; h < Pk; h++) {
        for (int k = 0; k < Lmz; k++) {
            for (int j = 0; j < Mmz; j++) {
                    for (int i = 0; i < Nmz; i++) {
                        const unsigned int ind = getIndex4D(i, j, k, h, Pk, Lmz, Mmz, Nmz);
                        float a = (WZZ + US[ind] / 0.321f) * Rs * VS[ind];
                        float Tp = TS[ind] * pow( 1000.f / HS[ind], 2.f/7.f );
                        float Tv = Tp * (1.f + 0.6078f * QS[ind]);
                        Qc[ind] = a - (0.5f * Tv + (1.f - Zmz[k]) * g * F_x[j + i * Mmz] / 0.321f);
                    }
            }
        }
    }
}