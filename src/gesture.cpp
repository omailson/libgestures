
#include "gesture.h"

Gesture::Gesture()
    : m_gestureType(GestureType::NoGesture),
      m_gestureState(GestureState::GestureStarted)
{
}

Gesture::GestureType Gesture::gestureType() const
{
    return m_gestureType;
}

Gesture::GestureState Gesture::gestureState() const
{
    return m_gestureState;
}
