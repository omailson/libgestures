#include <stdlib.h>

#include <gesture.h>
#include "tap.h"
#include "events/gesturetouchevent.h"

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

GestureRecognizer::Action TapRecognizer::recognize(Gesture *gesture, const GestureTouchEvent &ev)
{
    TapGesture *tapGesture = static_cast<TapGesture*>(gesture);
    switch(tapGesture->state) {
        case TapGesture::NoGesture:
            if (ev.type == GestureTouchEvent::TouchStart) {
                tapGesture->state = TapGesture::TapStarted;
                tapGesture->x = ev.touchPoints[0].x;
                tapGesture->y = ev.touchPoints[0].y;
                tapGesture->verticalRadius = ev.touchPoints[0].verticalRadius;
                tapGesture->horizontalRadius = ev.touchPoints[0].horizontalRadius;
                return MayBeGesture;
            }
            return CancelGesture;
            break;

        case TapGesture::TapStarted:
            if (ev.type == GestureTouchEvent::TouchEnd) {
                tapGesture->state = TapGesture::TapFinished;
                tapGesture->setGestureState(Gesture::GestureFinished);

                return FinishGesture;
            } else if (ev.type == GestureTouchEvent::TouchMove) {
                if (ev.flags & GestureTouchEvent::GESTURE_EVENT_TINY_MOVE)
                    return Ignore;
                else
                    return CancelGesture;
            }

            return CancelGesture;
            break;

        case TapGesture::TapFinished:
            // Just to avoid warnings
            break;
    }

    return CancelGesture;
}
