
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
    , m_distOrigin(0)
    , m_originA(0, 0)
    , m_originB(0, 0)
    , m_lastPosA(0, 0)
    , m_lastPosB(0, 0)
{
}

void PinchGesturePrivate::setOrigin(const Vector2D &p0, const Vector2D &p1)
{
    m_originA.setX(p0.x());
    m_originA.setY(p0.y());
    m_originB.setX(p1.x());
    m_originB.setY(p1.y());

    Vector2D diff = m_originA - m_originB;
    m_distOrigin = diff.length();
}

void PinchGesturePrivate::updateGesture(const Vector2D &p0, const Vector2D &p1)
{
    Vector2D diff = p0 - p1;
    double dist = diff.length();

    q->scale = dist / m_distOrigin;
    q->centerX = (p0.x() + p1.x()) / 2;
    q->centerY = (p0.y() + p1.y()) / 2;
}

bool PinchGesturePrivate::isPinch(const Vector2D &p0, const Vector2D &p1) const
{
    Vector2D movementA = p0 - m_originA;
    Vector2D movementB = p1 - m_originB;
    Vector2D axis = m_originA - m_originB;

    float d0 = Vector2D::cos(movementA, axis);
    float d1 = Vector2D::cos(movementB, axis);

    // d0 * d1 < 0 (oposite direction)
    return d0 * d1 < 0 && fabs(d0) > MIN_COS && fabs(d1) > MIN_COS;
}

bool PinchGesturePrivate::canRecognizePinch(const Vector2D &p0, const Vector2D &p1) const
{
    Vector2D diffA = (m_originA - p0);
    Vector2D diffB = (m_originB - p1);

    return diffA.lengthSquared() > MIN_MOVEMENT_LENGTH_SQUARED
           && diffB.lengthSquared() > MIN_MOVEMENT_LENGTH_SQUARED;
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
    Vector2D p0 (event.touchPoints[0].x, event.touchPoints[0].y);
    Vector2D p1 (event.touchPoints[1].x, event.touchPoints[1].y);

    PinchGesture *pinchGesture = static_cast<PinchGesture *>(gesture);
    switch(pinchGesture->d->state) {
        case PinchGesturePrivate::NoGesture:
        case PinchGesturePrivate::PinchOneFinger:
            if (event.type == GestureTouchEvent::TouchMove
                    && event.flags & GestureTouchEvent::GESTURE_EVENT_TINY_MOVE)
                return Action::Ignore;

            if (event.type != GestureTouchEvent::TouchStart)
                return Action::CancelGesture; // Ignore?

            if (event.numTouchPoints == 1)
                pinchGesture->d->state = PinchGesturePrivate::PinchOneFinger;
            else if (event.numTouchPoints == 2) {
                pinchGesture->d->state = PinchGesturePrivate::PinchTwoFingers;
                pinchGesture->d->setOrigin(p0, p1);
            } else if (event.numTouchPoints > 2)
                return Action::CancelGesture;

            return Action::MayBeGesture;

        case PinchGesturePrivate::PinchTwoFingers:
            if (event.type != GestureTouchEvent::TouchMove) {
                pinchGesture->d->state = PinchGesturePrivate::PinchFinished;
                pinchGesture->setGestureState(Gesture::GestureFinished);
                return Action::FinishGesture;
            }

            if (!pinchGesture->d->canRecognizePinch(p0, p1))
                return Action::MayBeGesture;

            if (!pinchGesture->d->isPinch(p0, p1))
                return Action::CancelGesture;

            pinchGesture->setGestureState(Gesture::GestureStarted);
            pinchGesture->d->state = PinchGesturePrivate::PinchMoving;

            return Action::TriggerGesture;

        case PinchGesturePrivate::PinchMoving:
            if (event.type != GestureTouchEvent::TouchMove) {
                pinchGesture->d->state = PinchGesturePrivate::PinchFinished;
                pinchGesture->setGestureState(Gesture::GestureFinished);
                return Action::FinishGesture;
            }

            pinchGesture->setGestureState(Gesture::GestureUpdated);
            pinchGesture->d->updateGesture(p0, p1);

            return Action::TriggerGesture;
                
        case PinchGesturePrivate::PinchFinished:
            return Action::FinishGesture;
    }

    return Ignore;
}
