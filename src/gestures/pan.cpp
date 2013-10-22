
#include <gesture.h>
#include "pan.h"

PanGesture::PanGesture()
    : Gesture()
{
}

PanGesture::~PanGesture()
{
}

Gesture::GestureType PanGesture::gestureType() const
{
    return Gesture::Pan;
}

PanRecognizer::PanRecognizer()
    : GestureRecognizer()
{
}

GestureRecognizer::Action PanRecognizer::recognize(Gesture *gesture, const NIXTouchEvent &event)
{
    return Ignore;
}

Gesture *PanRecognizer::create()
{
    return new PanGesture();
}

PanRecognizer::~PanRecognizer()
{
}
