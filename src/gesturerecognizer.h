
#ifndef GESTURERECOGNIZER_H
#define GESTURERECOGNIZER_H

#include "gesture.h"
#include "NIXEvents.h"

class GestureRecognizer {
public:
    enum Action {
        Ignore,
        MayBeGesture,
        TriggerGesture,
        FinishGesture,
        CancelGesture
        /* ConsumeEventHint*/
    };

    virtual Action recognize(Gesture *gesture, const NIXTouchEvent &event) = 0;

    virtual Gesture *create() = 0;

    GestureRecognizer();
};

#endif // GESTURERECOGNIZER_H
