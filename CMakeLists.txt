cmake_minimum_required(VERSION 3.12.3)

project(OpenCLSandbox)

find_package( OpenCL REQUIRED )

set (CMAKE_CXX_STANDARD 17)

set (PROJECT_SRC "./main.cpp"
                 "./src/CL.cpp"
                 "./src/CLPlatform.cpp"
                 "./src/CLDevice.cpp")

include_directories("./src"
                    $ENV{INTELOPENCLSDK}/include)

add_compile_options("$<$<CONFIG:DEBUG>:-DCL_DEBUG>")

add_executable(${PROJECT_NAME} ${PROJECT_SRC})

target_link_libraries(OpenCLSandbox -L$ENV{INTELOPENCLSDK}/lib64)
target_link_libraries(OpenCLSandbox OpenCL)
