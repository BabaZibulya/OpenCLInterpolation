#define WZZ 1.23
#define g 9.81
#define Rs 0.7

__kernel void interpol(
    __global double *US,
    __global double *VS,
    __global double *HS,
    __global double *QS,
    __global double *TS,
    __global double *F_x,
    __global double *Zmz,
    __global double *Qc,
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

    for (int i = 0; i < Nmz; i++) {
        const unsigned int ind = h + Pk * k + Pk * Lmz * j + Pk * Lmz * Mmz * i;
        double a = (WZZ + US[ind] / 0.321) * Rs * VS[ind];
        double Tp = TS[ind] * pow( 1000. / HS[ind], 2./7. );
        double Tv = Tp * (1. + 0.6078 * QS[ind]);
        Qc[ind] = a - (0.5 * Tv + (1. - Zmz[k]) * g * F_x[j + i * Mmz] / 0.321);
    }
}