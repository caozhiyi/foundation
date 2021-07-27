cmake_minimum_required(VERSION 3.10)

project(foundation CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CXX_FLAGS
 -O2
 -std=c++11
 -rdynamic
 -lpthread
 -fPIC
 -lstdc++
 -pipe
 )

if (WIN32)
  add_definitions(-D__win__)
  add_definitions(-D__use_iocp__)
endif ()

if(CMAKE_BUILD_BITS EQUAL 32)
  list(APPEND CXX_FLAGS "-m32")
else()
  list(APPEND CXX_FLAGS "-m64")
endif()

include_directories(.)
aux_source_directory(${PROJECT_SOURCE_DIR}/foundation/alloter src_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/foundation/buffer src_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/foundation/log src_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/foundation/os src_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/foundation/network src_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/foundation/structure src_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/foundation/thread src_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/foundation/timer src_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/foundation/util src_files)

IF (WIN32)
	aux_source_directory(${PROJECT_SOURCE_DIR}/foundation/network/win src_files)
	aux_source_directory(${PROJECT_SOURCE_DIR}/foundation/os/win src_files)
ELSEIF (APPLE)
	aux_source_directory(${PROJECT_SOURCE_DIR}/foundation/network/posix src_files)
	aux_source_directory(${PROJECT_SOURCE_DIR}/foundation/os/posix src_files)
ELSEIF (UNIX)
	aux_source_directory(${PROJECT_SOURCE_DIR}/foundation/network/posix src_files)
	aux_source_directory(${PROJECT_SOURCE_DIR}/foundation/os/posix src_files)
ENDIF ()

# output
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_library(${PROJECT_NAME} STATIC ${src_files})

add_subdirectory(third/gtest)
add_subdirectory(utest)