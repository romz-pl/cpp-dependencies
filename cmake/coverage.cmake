option(BUILD_COVERAGE "Build cpp_dependencies for coverage" OFF)

if(BUILD_COVERAGE)
  list(APPEND COMPILE_FLAGS --coverage)
else()
  list(APPEND COMPILE_FLAGS -O3)
endif()

