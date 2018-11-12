cmake_minimum_required(VERSION 3.10)

project(OpenCLInterpolation)

find_package( OpenCL REQUIRED )

set (CMAKE_CXX_STANDARD 17)

set (PROJECT_SRC "./main.cpp"
                 "./InterpolationUtils.cpp"
                 "./src/CL.cpp"
                 "./src/CLPlatform.cpp"
                 "./src/CLDevice.cpp"
                 "./src/CLContext.cpp"
                 "./src/CLCommandQueue.cpp"
                 "./src/CLProgram.cpp"
                 "./src/CLUtils.cpp"
                 "./src/CLBuffer.cpp"
                 "./src/CLLog.cpp"
                 )

include_directories("./src")

add_definitions( "-Wall -Werror -lstdc++fs -O3" )

add_compile_options("$<$<CONFIG:DEBUG>:-DCL_DEBUG>")

add_executable(${PROJECT_NAME} ${PROJECT_SRC})

target_link_libraries(OpenCLInterpolation OpenCL)
