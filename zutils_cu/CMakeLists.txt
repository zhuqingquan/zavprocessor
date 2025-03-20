cmake_minimum_required(VERSION 3.8)
project(zutils_cu)

find_package(CUDA REQUIRED)

message(STATUS "cuda version: " ${CUDA_VERSION_STRING})
message(STATUS "cuda info include_dir: " ${CUDA_INCLUDE_DIRS})
message(STATUS "cuda info libraries: " ${CUDA_LIBRARIES})

set(CMAKE_CUDA_STANDARD 11)

include_directories(${CUDA_INCLUDE_DIRS})

#cuda_add_executable(vectorAdd vectorAdd.cu)
#target_link_libraries(vectorAdd ${CUDA_LIBRARIES})

cuda_add_library(${PROJECT_NAME} SHARED 
    cudahelper.cc
    data_normalize.cu
)
set_target_properties(${PROJECT_NAME} PROPERTIES 
    CUDA_SEPARABLE_COMPILATION ON
    POSITION_INDEPENDENT_CODE ON
)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:-G>)
endif()
target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:-allow-unsupported-compiler>)

target_link_directories(${PROJECT_NAME} PUBLIC ${CUDA_TOOLKIT_ROOT_DIR}/lib64)
target_link_directories(${PROJECT_NAME} PUBLIC ${CUDA_TOOLKIT_ROOT_DIR}/targets/x86_64-linux/lib)

target_link_libraries(${PROJECT_NAME} cuda cudart_static)