cmake_minimum_required(VERSION 3.10)

# set the project name
project(Genetic_Algorithm)

enable_language(CXX)

# Find and enable OpenMP support
find_package(OpenMP REQUIRED)
if(OPENMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

# set the include path
include_directories(include)

# add the main code
add_subdirectory(src)

# add the tests
include(CTest)
enable_testing()
add_subdirectory(tests)

# add the utils code
add_subdirectory(utils)

