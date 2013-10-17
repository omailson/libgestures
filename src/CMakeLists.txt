include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(gestures_SOURCES
  gesture.cpp
  gesturerecognizer.cpp
  gestures/pan.cpp
)

add_library(gestures SHARED ${gestures_SOURCES})
