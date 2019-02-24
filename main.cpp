#include <iostream>
#include <cmath>

#include "InterpolationProblem.h"

int main(int argc, char ** argv) {

    unsigned Pk = 40, Lmz = 100, Mmz = 102, Nmz = 103;
    if (argc != 1) {
        if (argc == 5) {
            Pk = std::stol(argv[1]);
            Lmz = std::stol(argv[2]);
            Mmz = std::stol(argv[3]);
            Nmz = std::stol(argv[4]);
        }
        else {
            std::cerr << "Wrong usage. Please provide 4 arguments, or run witout parameters\n";
            return -1;
        }
    }

    InterpolationProblem interpolationProblem(Pk, Lmz, Mmz, Nmz);
    interpolationProblem.solve();

    return 0;
}
