cmake_minimum_required(VERSION 3.5)
project (Test_Reduce)

set (CMAKE_CXX_COMPILER /usr/local/cuda/bin/nvcc)
set (CMAKE_CXX_STANDARD 14)

include_directories(/usr/local/cuda/include)
#include_directories(${MIGRAPHX_FOLDER}/src/include
#                    ${MIGRAPHX_FOLDER}/src/api/include
#                    ${MIGRAPHX_FOLDER}/src/targets/gpu/include
#                    ${MIGRAPHX_FOLDER}/src/targets/cpu/include
#                    ${MIGRAPHX_FOLDER}/test/include
#                    /opt/rocm/include
#                    /home/scxiao/Workplace/projects/MIGraph/${DEPS_FOLDER}/include)
#
#link_directories(${MIGRAPHX_BUILD}/lib
#                 ${MIGRAPHX_DEPS})

message("cmake_current_src_dir = " ${CMAKE_SOURCE_DIR})
message("cmake_current_bin_dir = " ${CMAKE_BINARY_DIR})

set(reduce_SRC reduce_thread.cpp reduce_cuda.cpp)
add_library(reduce SHARED ${reduce_SRC})

set(test_files test_reduce.cpp)

foreach(filepath_name ${test_files})
    get_filename_component(src_name ${filepath_name} NAME)
    get_filename_component(bin_name ${src_name} NAME_WE)
    message("source file: " ${src_name} " ---> bin: " ${bin_name})
    add_executable(${bin_name} ${src_name})
    target_link_libraries(${bin_name} reduce pthread)
endforeach(filepath_name)

