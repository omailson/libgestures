#include "pan.h"
#include "pan_p.h"

#include "events/gesturetouchevent.h"

PanGesturePrivate::PanGesturePrivate()
    : state(NoGesture)
{
}

PanGesture::PanGesture()
    : Gesture()
    , d(new PanGesturePrivate())
{
}

PanGesture::~PanGesture()
{
    delete d;
}

Gesture::GestureType PanGesture::gestureType() const
{
    return Gesture::Pan;
}

PanRecognizer::PanRecognizer()
    : GestureRecognizer()
{
}

GestureRecognizer::Action PanRecognizer::recognize(Gesture *baseGesture, const GestureTouchEvent &ev)
{
    PanGesture *gesture = static_cast<PanGesture*>(baseGesture);
    switch (gesture->d->state) {
        case PanGesturePrivate::NoGesture:
            if (ev.type == GestureTouchEvent::TouchStart) {
                gesture->d->state = PanGesturePrivate::WaitingMove;
                gesture->x = ev.touchPoints[0].x;
                gesture->y = ev.touchPoints[0].y;
                return MayBeGesture;
            }
            break;

        case PanGesturePrivate::WaitingMove:
            if (ev.type == GestureTouchEvent::TouchMove) {
                if (ev.flags & GestureTouchEvent::GESTURE_EVENT_TINY_MOVE)
                    return Ignore;

                gesture->d->state = PanGesturePrivate::Moving;
                gesture->setGestureState(Gesture::GestureStarted);

                gesture->deltaX = ev.touchPoints[0].x - gesture->x;
                gesture->deltaY = ev.touchPoints[0].y - gesture->y;
                gesture->x = ev.touchPoints[0].x;
                gesture->y = ev.touchPoints[0].y;

                return TriggerGesture;
            }
            break;

        case PanGesturePrivate::Moving:
            if (ev.type == GestureTouchEvent::TouchMove) {
                gesture->setGestureState(Gesture::GestureUpdated);
                gesture->deltaX = ev.touchPoints[0].x - gesture->x;
                gesture->deltaY = ev.touchPoints[0].y - gesture->y;
                gesture->x = ev.touchPoints[0].x;
                gesture->y = ev.touchPoints[0].y;

                return TriggerGesture;
            } else if (ev.type == GestureTouchEvent::TouchEnd) {
                gesture->setGestureState(Gesture::GestureFinished);
                gesture->deltaX = ev.touchPoints[0].x - gesture->x;
                gesture->deltaY = ev.touchPoints[0].y - gesture->y;
                gesture->x = ev.touchPoints[0].x;
                gesture->y = ev.touchPoints[0].y;

                return FinishGesture;
            }
            break;
    }

    return CancelGesture;
}

Gesture *PanRecognizer::create()
{
    return new PanGesture();
}

PanRecognizer::~PanRecognizer()
{
}
