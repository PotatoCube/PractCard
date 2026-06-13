# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\FlashcardsApp_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\FlashcardsApp_autogen.dir\\ParseCache.txt"
  "FlashcardsApp_autogen"
  )
endif()
