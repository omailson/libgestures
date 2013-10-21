
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

    GestureRecognizer();
    virtual ~GestureRecognizer();

    virtual bool useTimer() const;
    virtual Action recognize(Gesture *gesture, unsigned int timestamp);
    virtual Action recognize(Gesture *gesture, const NIXTouchEvent &event) = 0;
    virtual Gesture *create() = 0;
};

#endif // GESTURERECOGNIZER_H
