cmake_minimum_required(VERSION 3.10)

project(install_gtest)

include(ExternalProject)

find_package(Git REQUIRED)

message(STATUS "install gtest.")

ExternalProject_Add(
    gtest
    PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    TIMEOUT 10
    CONFIGURE_COMMAND ""
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_SOURCE_DIR}
    BUILD_COMMAND cmake ../gtest -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_SOURCE_DIR}
    INSTALL_COMMAND make install #DESTDIR=${CMAKE_CURRENT_SOURCE_DIR}
    LOG_DOWNLOAD ON
)

ExternalProject_Get_Property(gtest source_dir)

set(gtest_include_dir ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
set(gtest_lib_dir ${CMAKE_CURRENT_SOURCE_DIR}/lib PARENT_SCOPE)

message(STATUS ${gtest_include_dir})
message(STATUS ${gtest_lib_dir})

message(STATUS "install gtest success.")