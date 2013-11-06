
#include "gesturerecognizer.h"

GestureRecognizer::GestureRecognizer()
{
}

GestureRecognizer::~GestureRecognizer()
{
}

bool GestureRecognizer::useTimer() const
{
    return false;
}

GestureRecognizer::Action GestureRecognizer::recognize(Gesture *gesture, long long int timestamp)
{
    return Ignore;
}
