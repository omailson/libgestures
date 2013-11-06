
#ifndef GESTURERECOGNIZER_H
#define GESTURERECOGNIZER_H

#include "gesture.h"

struct GestureTouchEvent;

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
    virtual Action recognize(Gesture *gesture, long long int timestamp);
    virtual Action recognize(Gesture *gesture, const GestureTouchEvent &event) = 0;
    virtual Gesture *create() = 0;
};

#endif // GESTURERECOGNIZER_H
