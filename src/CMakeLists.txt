include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(gestures_SOURCES
  gesture.cpp
  gesturerecognizer.cpp
  gesturemanager.cpp
  gestures/pan.cpp
  gestures/tap.cpp
  gestures/pinch.cpp
  gestures/pinch3f.cpp
  events/ainputeventhelper.cpp
  utils/vector2d.cpp
)

link_directories(/opt/android-ndk/platforms/android-18/arch-arm/usr/lib/)

add_library(gestures ${gestures_SOURCES})
target_link_libraries(gestures android)
