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

if(CMAKE_BUILD_BITS EQUAL 32)
  list(APPEND CXX_FLAGS "-m32")
else()
  list(APPEND CXX_FLAGS "-m64")
endif()

aux_source_directory(${PROJECT_SOURCE_DIR}/src/alloter src_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/buffer src_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/log src_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/os src_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/network src_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/structure src_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/thread src_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/timer src_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/util src_files)

IF (WIN32)
	aux_source_directory(${PROJECT_SOURCE_DIR}/src/network/win src_files)
	aux_source_directory(${PROJECT_SOURCE_DIR}/src/os/win src_files)
ELSEIF (APPLE)
	aux_source_directory(${PROJECT_SOURCE_DIR}/src/network/posix src_files)
	aux_source_directory(${PROJECT_SOURCE_DIR}/src/os/posix src_files)
ELSEIF (UNIX)
	aux_source_directory(${PROJECT_SOURCE_DIR}/src/network/posix src_files)
	aux_source_directory(${PROJECT_SOURCE_DIR}/src/os/posix src_files)
ENDIF ()

# output
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_library(${PROJECT_NAME} STATIC ${src_files})