#ifndef LONGPRESS_H
#define LONGPRESS_H

#include "gesture.h"
#include "gesturerecognizer.h"

class LongPressGesturePrivate;

class LongPressGesture : public Gesture
{
public:
    LongPressGesture();
    ~LongPressGesture();

    GestureType gestureType() const;
    int x;
    int y;

private:
    LongPressGesturePrivate *d;
    friend class LongPressRecognizer;
};

class LongPressRecognizer : public GestureRecognizer
{
public:
    LongPressRecognizer();
    ~LongPressRecognizer();

    bool useTimer() const { return true; }
    Action recognize(Gesture *gesture, long long int timestamp);
    Action recognize(Gesture *gesture, const GestureTouchEvent &event);
    Gesture *create();
};

#endif // LONGPRESS_H
