cmake_minimum_required(VERSION 3.10)

project(OpenCLSandbox)

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
                 "./src/CLBuffer.cpp")

include_directories("./src")

add_definitions( "-Wall -lstdc++fs" )

add_compile_options("$<$<CONFIG:DEBUG>:-DCL_DEBUG>")

add_executable(${PROJECT_NAME} ${PROJECT_SRC})

target_link_libraries(OpenCLSandbox OpenCL)
