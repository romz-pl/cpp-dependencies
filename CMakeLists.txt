cmake_minimum_required(VERSION 3.1)

project(cpp_dependencies LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include(CTest)

# Coverage build doesn't work with MSVC
option(BUILD_COVERAGE "Build cpp_dependencies for coverage" OFF)

if (WIN32)
  set(DEFAULT_MMAP OFF)
else()
  set(DEFAULT_MMAP ON)
endif()

# Switch between using the mmap logic for reading files (faster, because one copy less) or a file read (slower, because a full copy, but portable).
option(WITH_MMAP "Use mmapped files" ${DEFAULT_MMAP})

if (WIN32 OR APPLE)
  set(DEFAULT_MEMRCHR OFF)
else()
  set(DEFAULT_MEMRCHR ON)
endif()

# Whether your platform provides a fast memrchr function. If it does not, turn this off and a slower replacement will be used.
option(HAS_MEMRCHR "Platform has memrchr function" ${DEFAULT_MEMRCHR})


if("${CMAKE_CXX_COMPILER_ID}" MATCHES GNU)
  include(CheckCXXCompilerFlag)

  set(COMPILE_FLAGS -Wall -Wextra)

  check_cxx_compiler_flag("-Wpedantic" PEDANTIC_SUPPORTED)
  if(PEDANTIC_SUPPORTED)
    list(APPEND COMPILE_FLAGS -Wpedantic)
  endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(COMPILE_FLAGS /W4)
endif()

if(WITH_MMAP)
  list(APPEND COMPILE_FLAGS -DWITH_MMAP)
endif()

if(NOT HAS_MEMRCHR)
  list(APPEND COMPILE_FLAGS -DNO_MEMRCHR)
endif()

add_subdirectory(src)

if(BUILD_TESTING)
  add_subdirectory(test)
endif()

