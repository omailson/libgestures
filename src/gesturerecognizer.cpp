
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

GestureRecognizer::Action GestureRecognizer::recognize(Gesture *, long long int)
{
    return Ignore;
}
