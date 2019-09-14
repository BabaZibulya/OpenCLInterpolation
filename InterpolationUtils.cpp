#include "InterpolationUtils.h"

#include <cmath>

/*
void FillInitialData() {

    for (int h = 0; h < Pk; h++)
        for (int k = 0; k < Lmz; k++)
            for (int j = 0; j < Mmz; j++)
                for (int i = 0; i < Nmz; i++) {
                    US[h][k][j][i] = 1+sin((float)i*j+i-k-h);
                    VS[h][k][j][i] = 2+cos((float)3*i*h-j+h*k);
                    HS[h][k][j][i] = 3+sin((float)2*i*k+j*h);
                    QS[h][k][j][i] = 4+cos((float)i+j+k+h+10);
                    TS[h][k][j][i] = 5+sin((float)(j+i)*k-h);
                }

    for (int j = 0; j < Mmz; j++)
        for (int i = 0; i < Nmz; i++)
            F_X[j][i] = sin((float)i*j-i+123);

    for (int i = 0; i < Nmz; i++)
        Zmz[i] = cos((float)2*i);

}
*/

FourDimContiniousArray<float> createUS(unsigned Pk, unsigned Lmz, unsigned Mmz, unsigned Nmz)
{
    FourDimContiniousArray<float> US(Pk, Lmz, Mmz, Nmz);
    US.fill([](unsigned h, unsigned k, unsigned j, unsigned i)
    {
        return 1+sin((float)i*j+i-k-h);;
    });
    return US;
}

FourDimContiniousArray<float> createVS(unsigned Pk, unsigned Lmz, unsigned Mmz, unsigned Nmz)
{
    FourDimContiniousArray<float> VS(Pk, Lmz, Mmz, Nmz);
    VS.fill([](unsigned h, unsigned k, unsigned j, unsigned i)
    {
        return 2+cos((float)3*i*h-j+h*k);
    });
    return VS;
}

FourDimContiniousArray<float> createHS(unsigned Pk, unsigned Lmz, unsigned Mmz, unsigned Nmz)
{
    FourDimContiniousArray<float> HS(Pk, Lmz, Mmz, Nmz);
    HS.fill([](unsigned h, unsigned k, unsigned j, unsigned i)
    {
        return 3+sin((float)2*i*k+j*h);
    });
    return HS;
}

FourDimContiniousArray<float> createQS(unsigned Pk, unsigned Lmz, unsigned Mmz, unsigned Nmz)
{
    FourDimContiniousArray<float> QS(Pk, Lmz, Mmz, Nmz);
    QS.fill([](unsigned h, unsigned k, unsigned j, unsigned i)
    {
        return 4+cos((float)i+j+k+h+10);
    });
    return QS;
}

FourDimContiniousArray<float> createTS(unsigned Pk, unsigned Lmz, unsigned Mmz, unsigned Nmz)
{
    FourDimContiniousArray<float> TS(Pk, Lmz, Mmz, Nmz);
    TS.fill([](unsigned h, unsigned k, unsigned j, unsigned i)
    {
        return 5+sin((float)(j+i)*k-h);
    });
    return TS;
}

FourDimContiniousArray<float> createQc(unsigned Pk, unsigned Lmz, unsigned Mmz, unsigned Nmz)
{
    FourDimContiniousArray<float> Qc(Pk, Lmz, Mmz, Nmz);
    Qc.fill([](unsigned h, unsigned k, unsigned j, unsigned i)
    {
        return 0.f;
    });
    return Qc;
}

FourDimContiniousArray<float> createUS(const Interval& pkInterval, unsigned Lmz, unsigned Mmz, unsigned Nmz)
{
    FourDimContiniousArray<float> US(pkInterval.right - pkInterval.left, Lmz, Mmz, Nmz);
    US.fill([pkLeft = pkInterval.left](unsigned h, unsigned k, unsigned j, unsigned i)
    {
        return 1+sin((float)i*j+i-k-(h + pkLeft));
    });
    return US;
}

FourDimContiniousArray<float> createVS(const Interval& pkInterval, unsigned Lmz, unsigned Mmz, unsigned Nmz)
{
    FourDimContiniousArray<float> VS(pkInterval.right - pkInterval.left, Lmz, Mmz, Nmz);
    VS.fill([pkLeft = pkInterval.left](unsigned h, unsigned k, unsigned j, unsigned i)
    {
        return 2+cos((float)3*i*(h + pkLeft)-j+(h + pkLeft)*k);
    });
    return VS;
}

FourDimContiniousArray<float> createHS(const Interval& pkInterval, unsigned Lmz, unsigned Mmz, unsigned Nmz)
{
    FourDimContiniousArray<float> HS(pkInterval.right - pkInterval.left, Lmz, Mmz, Nmz);
    HS.fill([pkLeft = pkInterval.left](unsigned h, unsigned k, unsigned j, unsigned i)
    {
        return 3+sin((float)2*i*k+j*(h + pkLeft));
    });
    return HS;
}

FourDimContiniousArray<float> createQS(const Interval& pkInterval, unsigned Lmz, unsigned Mmz, unsigned Nmz)
{
    FourDimContiniousArray<float> QS(pkInterval.right - pkInterval.left, Lmz, Mmz, Nmz);
    QS.fill([pkLeft = pkInterval.left](unsigned h, unsigned k, unsigned j, unsigned i)
    {
        return 4+cos((float)i+j+k+(pkLeft+h)+10);
    });
    return QS;
}

FourDimContiniousArray<float> createTS(const Interval& pkInterval, unsigned Lmz, unsigned Mmz, unsigned Nmz)
{
    FourDimContiniousArray<float> TS(pkInterval.right - pkInterval.left, Lmz, Mmz, Nmz);
    TS.fill([pkLeft = pkInterval.left](unsigned h, unsigned k, unsigned j, unsigned i)
    {
        return 5+sin((float)(j+i)*k-(pkLeft + h));
    });
    return TS;
}

FourDimContiniousArray<float> createQc(const Interval& pkInterval, unsigned Lmz, unsigned Mmz, unsigned Nmz)
{
    FourDimContiniousArray<float> Qc(pkInterval.right - pkInterval.left, Lmz, Mmz, Nmz);
    Qc.fill([](unsigned, unsigned, unsigned, unsigned)
    {
        return 0.f;
    });
    return Qc;
}

TwoDimContiniousArray<float> createF_X(unsigned Mmz, unsigned Nmz)
{
    TwoDimContiniousArray<float> F_X(Mmz, Nmz);
    F_X.fill([](unsigned j, unsigned i) { return sin((float)i*j-i+123); });
    return F_X;
}

OneDimContiniousArray<float> createZmz(unsigned Nmz)
{
    OneDimContiniousArray<float> Zmz(Nmz);
    Zmz.fill([](unsigned i) { return cos((float)2*i); });
    return Zmz;
}