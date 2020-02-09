#define WZZ 1.23f
#define g 9.81f
#define Rs 0.7f

#define MAX_SIZE 100

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

    float localUS[MAX_SIZE];
    float localVS[MAX_SIZE];
    float localHS[MAX_SIZE];
    float localQS[MAX_SIZE];
    float localTS[MAX_SIZE];
    float localF_x[MAX_SIZE];
    float localQc[MAX_SIZE];
    
    float localZmz = Zmz[k];

#ifdef __xilinx__
    __attribute__((xcl_pipeline_loop))
#endif
    for (int i = 0; i < MAX_SIZE; i++) {
        if (i >= Nmz) {
            break;
        }
        const unsigned int ind = h + Pk * k + Pk * Lmz * j + Pk * Lmz * Mmz * i;
        localUS[i] = US[ind];
        localVS[i] = VS[ind];
        localTS[i] = TS[ind];
        localHS[i] = HS[ind];
        localQS[i] = QS[ind];
        localF_x[i] = F_x[j + i * Mmz];
    }

#ifdef __xilinx__
    __attribute__((xcl_pipeline_loop))
#endif
    for (int i = 0; i < MAX_SIZE; i++) {
        if (i >= Nmz) {
            break;
        }
        float a = (WZZ + localUS[i] / 0.321f) * Rs * localVS[i];
        float Tp = localTS[i] * pow( 1000.f / localHS[i], 0.28571428571f );
        float Tv = Tp * (1.f + 0.6078f * localQS[i]);
        localQc[i] = a - (0.5f * Tv + (1.f - localZmz) * g * localF_x[i] / 0.321f);
    }
    
#ifdef __xilinx__
    __attribute__((xcl_pipeline_loop))
#endif
    for (int i = 0; i < MAX_SIZE; i++) {
        if (i >= Nmz) {
            break;
        }
        const unsigned int ind = h + Pk * k + Pk * Lmz * j + Pk * Lmz * Mmz * i;
        Qc[ind] = localQc[i];
    }
}
