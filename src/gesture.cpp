
#include "gesture.h"

Gesture::Gesture()
    : m_gestureState(GestureState::GestureStarted)
{
}

Gesture::GestureType Gesture::gestureType() const
{
    return GestureType::NoGesture;
}

Gesture::GestureState Gesture::gestureState() const
{
    return m_gestureState;
}
