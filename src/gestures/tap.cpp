#include "tap.h"

#include <stdlib.h>

TapRecognizer::TapRecognizer()
    : GestureRecognizer()
{
}

bool TapRecognizer::useTimer() const
{
    return false;
}

Gesture *TapRecognizer::create()
{
    return new TapGesture();
}

GestureRecognizer::Action TapRecognizer::recognize(TapGesture *gesture, const NIXTouchEvent &ev)
{
    switch(gesture->state) {
        case TapGesture::NoGesture:
            if (ev.type == kNIXInputEventTypeTouchStart) {
                gesture->state = TapGesture::TapStarted;
                gesture->x = ev.x;
                gesture->y = ev.y;
                gesture->verticalRadius = ev.touchPoints[0].verticalRadius;
                gesture->horizontalRadius = ev.touchPoints[0].horizontalRadius;
                return MayBeGesture;
            }
            return CancelGesture;
            break;

        case TapGesture::TapStarted:
            if (ev.type == kNIXInputEventTypeTouchEnd) {
                gesture->state = TapGesture::TapFinished;
                gesture->setGestureState(Gesture::GestureFinished);

                return TriggerGesture;
            } else if (ev.type == kNIXInputEventTypeTouchMove) {
                int moved = abs(gesture->x - ev.x) + abs(gesture->y - ev.y);
                if (moved > 40)
                    return CancelGesture;
                else
                    return Ignore;
            }

            return CancelGesture;
            break;

        case TapGesture::TapFinished:
            // Just to avoid warnings
            break;
    }

    return CancelGesture;
}
