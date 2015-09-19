cmake_minimum_required(VERSION 2.8)

project(lazy_thrust_examples)

find_package(CUDA)
find_path(THRUST_INCLUDE_DIR thrust/version.h PATHS $ENV{CUDA_PATH}/include)

include_directories(../include)
include_directories(${THRUST_INCLUDE_DIR})

if(CUDA_FOUND)
	option(USE_CUDA_DEVICE "Use the CUDA backend as the 'device'" OFF)
else()
	set(USE_CUDA_DEVICE OFF)
endif()

if(NOT USE_CUDA_DEVICE)
	add_definitions(-DTHRUST_DEVICE_SYSTEM=THRUST_DEVICE_SYSTEM_CPP)
endif()

add_subdirectory(example0)