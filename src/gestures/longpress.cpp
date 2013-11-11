#include "longpress.h"
#include "longpress_p.h"
#include "events/gesturetouchevent.h"

LongPressGesturePrivate::LongPressGesturePrivate()
    : state(NoGesture)
    , lastTimestamp(0)
{
}

LongPressGesture::LongPressGesture()
    : Gesture()
    , d(new LongPressGesturePrivate)
{
}

LongPressGesture::~LongPressGesture()
{
    delete d;
}

Gesture::GestureType LongPressGesture::gestureType() const
{
    return LongPress;
}

LongPressRecognizer::LongPressRecognizer()
    : GestureRecognizer()
{
}

LongPressRecognizer::~LongPressRecognizer()
{
}

Gesture *LongPressRecognizer::create()
{
    return new LongPressGesture();
}

GestureRecognizer::Action LongPressRecognizer::recognize(Gesture *baseGesture, long long int timestamp)
{
    LongPressGesture *gesture = static_cast<LongPressGesture*>(baseGesture);
    switch (gesture->d->state) {
        case LongPressGesturePrivate::NoGesture:
            gesture->d->lastTimestamp = timestamp;
            break;

        case LongPressGesturePrivate::TapStarted:
            if (gesture->d->lastTimestamp == 0)
                gesture->d->lastTimestamp = timestamp;

            if (gesture->d->lastTimestamp + 1000 < timestamp) {
                gesture->setGestureState(Gesture::GestureStarted);
                gesture->d->state = LongPressGesturePrivate::GestureTriggered;
                return TriggerGesture;
            }
            break;

        case LongPressGesturePrivate::GestureTriggered:
            break;
    }
    return Ignore;
}

GestureRecognizer::Action LongPressRecognizer::recognize(Gesture *baseGesture, const GestureTouchEvent &ev)
{
    LongPressGesture *gesture = static_cast<LongPressGesture*>(baseGesture);
    switch (gesture->d->state) {
        case LongPressGesturePrivate::NoGesture:
            if (ev.type == GestureTouchEvent::TouchStart) {
                gesture->d->state = LongPressGesturePrivate::TapStarted;
                gesture->x = ev.touchPoints[0].x;
                gesture->y = ev.touchPoints[0].y;
                return MayBeGesture;
            }
            break;

        case LongPressGesturePrivate::TapStarted:
            if (ev.type == GestureTouchEvent::TouchMove
                    && (ev.flags & GestureTouchEvent::GESTURE_EVENT_TINY_MOVE)) {
                return Ignore;
            }
            break;

        case LongPressGesturePrivate::GestureTriggered:
            if (ev.type == GestureTouchEvent::TouchMove) {
                gesture->x = ev.touchPoints[0].x;
                gesture->y = ev.touchPoints[0].y;
                gesture->setGestureState(Gesture::GestureUpdated);

                return TriggerGesture;
            } else if (ev.type == GestureTouchEvent::TouchEnd) {
                gesture->x = ev.touchPoints[0].x;
                gesture->y = ev.touchPoints[0].y;
                gesture->setGestureState(Gesture::GestureFinished);

                return FinishGesture;
            }
            break;
    }

    return CancelGesture;
}
