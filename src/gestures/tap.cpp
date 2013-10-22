#include <stdlib.h>

#include <gesture.h>
#include "tap.h"

TapGesture::TapGesture()
    : Gesture()
    , state(NoGesture)
{
}

TapGesture::~TapGesture()
{
}

Gesture::GestureType TapGesture::gestureType() const
{
    return Gesture::Tap;
}

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

GestureRecognizer::Action TapRecognizer::recognize(Gesture *gesture, const NIXTouchEvent &ev)
{
    TapGesture *tapGesture = static_cast<TapGesture*>(gesture);
    switch(tapGesture->state) {
        case TapGesture::NoGesture:
            if (ev.type == kNIXInputEventTypeTouchStart) {
                tapGesture->state = TapGesture::TapStarted;
                tapGesture->x = ev.x;
                tapGesture->y = ev.y;
                tapGesture->verticalRadius = ev.touchPoints[0].verticalRadius;
                tapGesture->horizontalRadius = ev.touchPoints[0].horizontalRadius;
                return MayBeGesture;
            }
            return CancelGesture;
            break;

        case TapGesture::TapStarted:
            if (ev.type == kNIXInputEventTypeTouchEnd) {
                tapGesture->state = TapGesture::TapFinished;
                tapGesture->setGestureState(Gesture::GestureFinished);

                return FinishGesture;
            } else if (ev.type == kNIXInputEventTypeTouchMove) {
                int moved = abs(tapGesture->x - ev.x) + abs(tapGesture->y - ev.y);
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
