include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(gestures_SOURCES
  gesture.cpp
)

add_library(gestures ${gestures_SOURCES})
