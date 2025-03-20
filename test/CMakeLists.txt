cmake_minimum_required(VERSION 3.0)

project(avprocess_unittest)
enable_testing()

if(WIN32)
    add_definitions(-DUNICODE -D_UNICODE)
endif()

set(SRC_FILE_TESTED 
    ClProcessorUnitTest.cc
    zutilscuUnitTest.cc)

include_directories(../inc)
include_directories(${PROJECT_SOURCE_DIR}/../../media_filter/inc)
include_directories(${PROJECT_SOURCE_DIR}/../zutils_cu/)

add_executable(${PROJECT_NAME} ${SRC_FILE_TESTED})
target_link_libraries(${PROJECT_NAME} gtest_main
    cuda zutils_cu
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
