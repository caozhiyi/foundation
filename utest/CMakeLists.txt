cmake_minimum_required(VERSION 3.10)

project(foundation_utest CXX)

aux_source_directory(${PROJECT_SOURCE_DIR}/alloter src_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/log src_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/network src_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/structure src_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/timer src_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/util src_files)
aux_source_directory(${PROJECT_SOURCE_DIR} src_files)

message(STATUS ${PROJECT_SOURCE_DIR})

# output
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(gtest)

# include
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${gtest_include_dir})

add_executable(${PROJECT_NAME} ${src_files})

target_link_libraries(${PROJECT_NAME} foundation)
target_link_libraries(${PROJECT_NAME} install_gtest)