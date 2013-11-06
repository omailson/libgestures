#include "doubletap.h"
#include "doubletap_p.h"
#include "events/gesturetouchevent.h"

DoubleTapGesturePrivate::DoubleTapGesturePrivate()
    : state(NoGesture)
    , lastTimestamp(0)
{
}

DoubleTapGesture::DoubleTapGesture()
    : Gesture()
    , d(new DoubleTapGesturePrivate)
{
}

DoubleTapGesture::~DoubleTapGesture()
{
    delete d;
};

Gesture::GestureType DoubleTapGesture::gestureType() const
{
    return Gesture::DoubleTap;
}

DoubleTapRecognizer::DoubleTapRecognizer()
    : GestureRecognizer()
{
}

DoubleTapRecognizer::~DoubleTapRecognizer()
{
}

Gesture *DoubleTapRecognizer::create()
{
    return new DoubleTapGesture();
}

GestureRecognizer::Action DoubleTapRecognizer::recognize(Gesture *baseGesture, long long int timestamp)
{
    DoubleTapGesture *gesture = static_cast<DoubleTapGesture*>(baseGesture);
    switch (gesture->d->state) {
        case DoubleTapGesturePrivate::NoGesture:
        case DoubleTapGesturePrivate::FirstTapStarted:
            gesture->d->lastTimestamp = timestamp;
            break;

        case DoubleTapGesturePrivate::StartTrackingTime:
            // Do not store timestamp
            // Check whether time has passed
            if (gesture->d->lastTimestamp + 1000 < timestamp)
                return CancelGesture;
            break;

        case DoubleTapGesturePrivate::SecondTapStarted:
            break;
    }

    return Ignore;
}

GestureRecognizer::Action DoubleTapRecognizer::recognize(Gesture *baseGesture, const GestureTouchEvent &ev)
{
    if (ev.type == GestureTouchEvent::TouchMove
            && (ev.flags & GestureTouchEvent::GESTURE_EVENT_TINY_MOVE))
        return Ignore;

    DoubleTapGesture *gesture = static_cast<DoubleTapGesture*>(baseGesture);

    switch (gesture->d->state) {
        case DoubleTapGesturePrivate::NoGesture:
            if (ev.type == GestureTouchEvent::TouchStart) {
                gesture->d->state = DoubleTapGesturePrivate::FirstTapStarted;
                gesture->x = ev.touchPoints[0].x;
                gesture->y = ev.touchPoints[0].y;
                return MayBeGesture;
            }
            break;

        case DoubleTapGesturePrivate::FirstTapStarted:
            if (ev.type == GestureTouchEvent::TouchEnd) {
                gesture->d->state = DoubleTapGesturePrivate::StartTrackingTime;
                return MayBeGesture;
            }
            break;

        case DoubleTapGesturePrivate::StartTrackingTime:
            if (ev.type == GestureTouchEvent::TouchStart) {
                // TODO check whether the position of this touch is close to the other touch
                gesture->d->state = DoubleTapGesturePrivate::SecondTapStarted;
                return MayBeGesture;
            }
            break;

        case DoubleTapGesturePrivate::SecondTapStarted:
            if (ev.type == GestureTouchEvent::TouchEnd) {
                return FinishGesture;
            }
    }

    return CancelGesture;
}
