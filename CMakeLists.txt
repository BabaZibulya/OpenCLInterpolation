cmake_minimum_required(VERSION 3.12.3)

project(TestQtCreator)

find_package( OpenCL REQUIRED )

set (CMAKE_CXX_STANDARD 14)

set (PROJECT_SRC "./main.cpp"
                 "./src/CLPlatform.cpp")

include_directories("./src")

add_compile_options("$<$<CONFIG:DEBUG>:-DCL_DEBUG>")

add_executable(${PROJECT_NAME} ${PROJECT_SRC})
target_link_libraries(${PROJECT_NAME} OpenCL::OpenCL)
