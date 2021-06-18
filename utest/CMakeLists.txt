cmake_minimum_required(VERSION 3.10)

project(foundation_utest CXX)

aux_source_directory(${PROJECT_SOURCE_DIR}/alloter utest_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/log utest_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/network utest_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/structure utest_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/timer utest_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/util utest_files)
aux_source_directory(${PROJECT_SOURCE_DIR} utest_files)

# include
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${gtest_include_dir})

message(STATUS ${gtest_include_dir})

add_executable(${PROJECT_NAME} ${utest_files})

target_link_libraries(${PROJECT_NAME} foundation)
target_link_libraries(${PROJECT_NAME} gtest_main)
enable_testing()