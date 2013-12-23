#ifndef DOUBLETAP_H
#define DOUBLETAP_H

#include "gesture.h"
#include "gesturerecognizer.h"

class DoubleTapGesturePrivate;
struct GestureTouchEvent;

class DoubleTapGesture : public Gesture
{
public:
    DoubleTapGesture();
    ~DoubleTapGesture();

    GestureType gestureType() const;

    int x;
    int y;
    int verticalRadius;
    int horizontalRadius;

private:
    DoubleTapGesturePrivate *d;
    friend class DoubleTapRecognizer;
};

class DoubleTapRecognizer : public GestureRecognizer
{
public:
    DoubleTapRecognizer();
    ~DoubleTapRecognizer();

    bool useTimer() const { return true; }
    Action recognize(Gesture *gesture, long long int timestamp);
    Action recognize(Gesture *gesture, const GestureTouchEvent &event);
    Gesture *create();
};

#endif // DOUBLETAP_H
