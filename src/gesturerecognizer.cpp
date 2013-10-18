
#include "gesturerecognizer.h"

GestureRecognizer::GestureRecognizer()
{
}

GestureRecognizer::Action GestureRecognizer::recognize(Gesture *gesture, NIXTouchEvent *event)
{
    return Action::Ignore;
}

Gesture *GestureRecognizer::create()
{
    return 0;
}
