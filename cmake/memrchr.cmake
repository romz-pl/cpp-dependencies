#
# Whether your platform provides a fast memrchr function.
# If it does not, turn this off and a slower replacement will be used.
#

set(DEFAULT_MEMRCHR ON)

option(HAS_MEMRCHR "Platform has memrchr function" ${DEFAULT_MEMRCHR})

if(NOT HAS_MEMRCHR)
  list(APPEND COMPILE_FLAGS -DNO_MEMRCHR)
endif()

