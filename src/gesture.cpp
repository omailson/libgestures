
#include "gesture.h"

Gesture::Gesture()
    : m_gestureState(GestureState::GestureStarted)
{
}

Gesture::~Gesture()
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

void Gesture::setGestureState(GestureState state)
{
    m_gestureState = state;
}
