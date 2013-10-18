
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

    virtual Action recognize(Gesture *gesture, NIXTouchEvent *event);

    virtual Gesture *create();

    GestureRecognizer();
};

#endif // GESTURERECOGNIZER_H
