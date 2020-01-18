#include <iostream>
#include <cmath>

#include <CL/cl2.hpp>

#include "InterpolationProblem.h"

int main(int argc, char ** argv) {
    unsigned Pk = 30, Lmz = 30, Mmz = 30, Nmz = 30;

    size_t i;
    cl_int err;
    std::vector<cl::Platform> platforms;
    OCL_CHECK(err, err = cl::Platform::get(&platforms));
    cl::Platform platform;
    for (i = 0; i < platforms.size(); i++) {
        platform = platforms[i];
        OCL_CHECK(err,
            std::string platformName =
            platform.getInfo<CL_PLATFORM_NAME>(&err));
        if (platformName == vendor_name) {
            std::cout << "Found Platform" << std::endl;
            std::cout << "Platform Name: " << platformName.c_str() << std::endl;
            break;
        }
    }
    if (i == platforms.size()) {
        std::cout << "Error: Failed to find Xilinx platform" << std::endl;
        exit(EXIT_FAILURE);
    }
    //Getting ACCELERATOR Devices and selecting 1st such device
    std::vector<cl::Device> devices;
    OCL_CHECK(err,
        err = platform.getDevices(CL_DEVICE_TYPE_ACCELERATOR, &devices));


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
