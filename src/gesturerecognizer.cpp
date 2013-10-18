
#include "gesturerecognizer.h"

GestureRecognizer::GestureRecognizer()
{
}

bool GestureRecognizer::useTimer() const
{
    return false;
}

GestureRecognizer::Action GestureRecognizer::recognize(Gesture *gesture, unsigned int timestamp)
{
    return Ignore;
}
