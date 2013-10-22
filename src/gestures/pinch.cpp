
#include <stdlib.h>
#include <math.h>
#include <gesture.h>
#include "pinch.h"
#include "pinch_p.h"
#include "events/gesturetouchevent.h"

/****************
 * PinchGesture *
 ****************/
PinchGesture::PinchGesture()
    : Gesture()
    , d(new PinchGesturePrivate(this))
{
}

PinchGesture::~PinchGesture()
{
    delete d;
}

Gesture::GestureType PinchGesture::gestureType() const
{
    return Gesture::Pinch;
}

/***********************
 * PinchGesturePrivate *
 ***********************/
PinchGesturePrivate::PinchGesturePrivate(PinchGesture *q_ptr)
    : state(NoGesture)
    , q(q_ptr)
{
}

void PinchGesturePrivate::setOrigin(int xa, int ya, int xb, int yb)
{
    int dx = xa - xb;
    int dy = ya - yb;    

    distOrigin = sqrt(dx * dx + dy * dy);
}

void PinchGesturePrivate::updateGesture(int xa, int ya, int xb, int yb)
{
    int dx = xa - xb;
    int dy = ya - yb;
    
    double dist = sqrt(dx * dx + dy * dy);

    q->scale = dist / distOrigin;
    q->centerX = (xa + xb) / 2;
    q->centerY = (ya + yb) / 2;
}

/*******************
 * PinchRecognizer *
 *******************/
PinchRecognizer::PinchRecognizer()
    : GestureRecognizer()
{
}

PinchRecognizer::~PinchRecognizer()
{
}

bool PinchRecognizer::useTimer() const
{
    return false;
}

Gesture *PinchRecognizer::create()
{
    return new PinchGesture();
}

GestureRecognizer::Action PinchRecognizer::recognize(Gesture *gesture, const GestureTouchEvent &event)
{
    PinchGesture *pinchGesture = static_cast<PinchGesture *>(gesture);
    switch(pinchGesture->d->state) {
        case PinchGesturePrivate::NoGesture:
        case PinchGesturePrivate::PinchOneFinger:
            if (event.type != GestureTouchEvent::TouchStart)
                return Action::CancelGesture; // Ignore?

            if (event.numTouchPoints == 1)
                pinchGesture->d->state = PinchGesturePrivate::PinchOneFinger;
            else if (event.numTouchPoints == 2)
                pinchGesture->d->state = PinchGesturePrivate::PinchTwoFingers;
            else if (event.numTouchPoints > 2)
                return Action::CancelGesture;

            return Action::MayBeGesture;

        case PinchGesturePrivate::PinchTwoFingers:
            pinchGesture->d->setOrigin(event.touchPoints[0].x, event.touchPoints[0].y,
                                       event.touchPoints[1].x, event.touchPoints[1].y);

        case PinchGesturePrivate::PinchMoving:
            if (event.type != GestureTouchEvent::TouchMove) {
                pinchGesture->d->state = PinchGesturePrivate::PinchFinished;
                pinchGesture->setGestureState(Gesture::GestureFinished);
                return Action::CancelGesture;
            }

            if (pinchGesture->d->state == PinchGesturePrivate::PinchTwoFingers)
                pinchGesture->setGestureState(Gesture::GestureStarted);
            else
                pinchGesture->setGestureState(Gesture::GestureUpdated);

            pinchGesture->d->state = PinchGesturePrivate::PinchMoving;
            pinchGesture->d->updateGesture(event.touchPoints[0].x, event.touchPoints[0].y,
                                           event.touchPoints[1].x, event.touchPoints[1].y);

            return Action::TriggerGesture;
                
        case PinchGesturePrivate::PinchFinished:
            return Action::FinishGesture;
    }

    return Ignore;
}
