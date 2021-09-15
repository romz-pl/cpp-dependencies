#
# Switch between using the mmap logic for reading files (faster, because one copy less) or 
# a file read (slower, because a full copy, but portable).
#

set(DEFAULT_MMAP ON)

option(WITH_MMAP "Use mmapped files" ${DEFAULT_MMAP})

if(WITH_MMAP)
  list(APPEND COMPILE_FLAGS -DWITH_MMAP)
endif()

