#ifndef TAP_H
#define TAP_H

#include "gesture.h"
#include "gesturerecognizer.h"

class TapGesture : public Gesture
{
public:
    enum TapState {
        NoGesture,
        TapStarted,
        TapFinished
    };

    TapGesture();
    GestureType gestureType() const;

    TapState state;
    int x;
    int y;
    int verticalRadius;
    int horizontalRadius;
};

class TapRecognizer : public GestureRecognizer
{
public:
    TapRecognizer();

    bool useTimer() const;
    GestureRecognizer::Action recognize(TapGesture *gesture, const NIXTouchEvent &event);
    Gesture *create();
};

#endif // TAP_H
