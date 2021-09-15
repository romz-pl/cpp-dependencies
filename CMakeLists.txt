cmake_minimum_required(VERSION 3.1)

project(cpp_dependencies LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(COMPILE_FLAGS "")

include(CTest)

include(${PROJECT_SOURCE_DIR}/cmake/warnings.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/coverage.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/mmap.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/memrchr.cmake)

add_subdirectory(src)

if(BUILD_TESTING)
  add_subdirectory(test)
endif()

