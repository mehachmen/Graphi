# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\graphi_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\graphi_autogen.dir\\ParseCache.txt"
  "graphi_autogen"
  )
endif()
