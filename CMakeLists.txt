cmake_minimum_required(VERSION 3.10)

project(OpenCLInterpolation)

set (CMAKE_CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(MPI REQUIRED)

# The following definition is required to build C++ with OpenMPI
add_definitions(-DOMPI_SKIP_MPICXX)

set (PROJECT_SRC "./main.cpp"
                 "./InterpolationUtils.cpp"
                 "./InterpolationProblem.cpp"
                 "./TimeUtils.cpp"
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

if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
  include_directories(SYSTEM ${MPI_INCLUDE_PATH})
endif()

if(MSVC)
  include_directories("$ENV{CUDA_PATH}/include")
  link_directories("$ENV{CUDA_PATH}/lib/x64/")
endif()

add_compile_options("$<$<CONFIG:DEBUG>:-DCL_DEBUG>")

add_executable(${PROJECT_NAME} ${PROJECT_SRC})

target_link_libraries(OpenCLInterpolation OpenCL)
target_link_libraries(OpenCLInterpolation Threads::Threads)
target_link_libraries(OpenCLInterpolation ${MPI_C_LIBRARIES})
