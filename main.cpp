#include <iostream>
#include <cmath>

#include "InterpolationProblem.h"
#include "VectorAdd.h"
#include "Convolution.h"

#include <stdlib.h>

int main(int argc, char ** argv) {
    // 160 192 224 256 288 320 384 416 448 480
    //for (size_t n = 160; n <= 320; n += 16) {
    //    unsigned Pk = n, Lmz = 60, Mmz = 60, Nmz = 30;

    //    if (argc != 1) {
    //        if (argc == 5) {
    //            Pk = std::stol(argv[1]);
    //            Lmz = std::stol(argv[2]);
    //            Mmz = std::stol(argv[3]);
    //            Nmz = std::stol(argv[4]);
    //        }
    //        else {
    //            std::cerr << "Wrong usage. Please provide 4 arguments, or run witout parameters\n";
    //            return -1;
    //        }
    //    }

    //    InterpolationProblem interpolationProblem(Pk, Lmz, Mmz, Nmz);
    //    interpolationProblem.solve();
    //}

    //for (size_t n = 1; n < 10; n++) {
    //    VectorAdd problem(4096 * n);
    //    problem.solve();
    //}

    

    //Convolution conv("C:\\Users\\Bondarenko-PC\\Documents\\Projects\\OpenCLInterpolation\\img\\airplane.bmp");
    //Convolution conv("C:\\Users\\Bondarenko-PC\\Documents\\Projects\\OpenCLInterpolation\\img\\lenna.bmp");
    Convolution conv("img/lena_gray.bmp");
    conv.solve();

    getchar();

    return 0;
}
