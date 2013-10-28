#include <stdlib.h>
#include <gesture.h>
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

GestureRecognizer::Action PanRecognizer::recognize(Gesture *gesture, const GestureTouchEvent &ev)
{
    PanGesture *panGesture = static_cast<PanGesture*>(gesture);
    switch (panGesture->d->state) {
        case PanGesturePrivate::NoGesture:
            if (ev.type == GestureTouchEvent::TouchStart) {
                panGesture->d->state = PanGesturePrivate::WaitingMove;
                panGesture->x = ev.touchPoints[0].x;
                panGesture->y = ev.touchPoints[0].y;
                return MayBeGesture;
            }
            break;

        case PanGesturePrivate::WaitingMove:
            if (ev.type == GestureTouchEvent::TouchMove) {
                if (ev.flags & GestureTouchEvent::GESTURE_EVENT_TINY_MOVE)
                    return Ignore;

                panGesture->d->state = PanGesturePrivate::Moving;
                panGesture->setGestureState(Gesture::GestureStarted);

                panGesture->deltaX = ev.touchPoints[0].x - panGesture->x;
                panGesture->deltaY = ev.touchPoints[0].y - panGesture->y;
                panGesture->x = ev.touchPoints[0].x;
                panGesture->y = ev.touchPoints[0].y;

                return TriggerGesture;
            }
            break;

        case PanGesturePrivate::Moving:
            if (ev.type == GestureTouchEvent::TouchMove) {
                panGesture->setGestureState(Gesture::GestureUpdated);
                panGesture->deltaX = ev.touchPoints[0].x - panGesture->x;
                panGesture->deltaY = ev.touchPoints[0].y - panGesture->y;
                panGesture->x = ev.touchPoints[0].x;
                panGesture->y = ev.touchPoints[0].y;

                return TriggerGesture;
            } else if (ev.type == GestureTouchEvent::TouchEnd) {
                panGesture->setGestureState(Gesture::GestureFinished);
                panGesture->deltaX = ev.touchPoints[0].x - panGesture->x;
                panGesture->deltaY = ev.touchPoints[0].y - panGesture->y;
                panGesture->x = ev.touchPoints[0].x;
                panGesture->y = ev.touchPoints[0].y;

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
