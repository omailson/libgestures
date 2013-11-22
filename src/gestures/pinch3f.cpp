
#include <math.h>
#include "gesture.h"
#include "pinch3f.h"
#include "pinch3f_p.h"
#include "events/gesturetouchevent.h"

inline static double triangleArea(const Vector2D &p0, const Vector2D &p1,
                                  const Vector2D &p2)
{
    double sum = p0.x() * p1.y() + p1.x() * p2.y() + p2.x() * p0.y();
    double sub = p0.y() * p1.x() + p1.y() * p2.x() + p2.y() * p0.x();
    
    return fabs((sum - sub) / 2);
}

/******************
 * Pinch3fGesture *
 ******************/
Pinch3fGesture::Pinch3fGesture()
    : Gesture()
    , scale(1)
    , d(new Pinch3fGesturePrivate(this))
{
}

Pinch3fGesture::~Pinch3fGesture()
{
    delete d;
}

Gesture::GestureType Pinch3fGesture::gestureType() const
{
    return Gesture::Pinch3f;
}

/*************************
 * Pinch3fGesturePrivate *
 *************************/
Pinch3fGesturePrivate::Pinch3fGesturePrivate(Pinch3fGesture *q_ptr)
    : state(NoGesture)
    , q(q_ptr)
    , m_initialArea(0)
{
}

bool Pinch3fGesturePrivate::isPinch(const Vector2D &p0, const Vector2D &p1,
                                    const Vector2D &p2) const
{
    Vector2D v0 = p0 - p1;
    Vector2D v1 = p1 - p2;
    double cos = Vector2D::cos(v0, v1);
    // three points are not collinear
    return fabs(cos - 1) > 0.2;
}

void Pinch3fGesturePrivate::begin(const Vector2D &p0, const Vector2D &p1,
                                  const Vector2D &p2)
{
    m_initialArea = triangleArea(p0, p1, p2);;
    q->centerX = (p0.x() + p1.x() + p2.x()) / 3;
    q->centerY = (p0.y() + p1.y() + p2.y()) / 3;
}

void Pinch3fGesturePrivate::update(const Vector2D &p0, const Vector2D &p1,
                                   const Vector2D &p2)
{
    double area = triangleArea(p0, p1, p2);;
    q->scale = area / m_initialArea;
    q->centerX = (p0.x() + p1.x() + p2.x()) / 3;
    q->centerY = (p0.y() + p1.y() + p2.y()) / 3;
}

/*********************
 * Pinch3fRecognizer *
 *********************/
Pinch3fRecognizer::Pinch3fRecognizer()
    : GestureRecognizer()
{
}

bool Pinch3fRecognizer::useTimer() const
{
    return false;
}

GestureRecognizer::Action Pinch3fRecognizer::recognize(Gesture *gesture, const GestureTouchEvent &event)
{
    Vector2D p0 (event.touchPoints[0].x, event.touchPoints[0].y);
    Vector2D p1 (event.touchPoints[1].x, event.touchPoints[1].y);
    Vector2D p2 (event.touchPoints[2].x, event.touchPoints[2].y);

    Pinch3fGesture *pinch3fGesture = static_cast<Pinch3fGesture *>(gesture);
    switch(pinch3fGesture->d->state) {
        case Pinch3fGesturePrivate::NoGesture:
        case Pinch3fGesturePrivate::Pinch3fOneFinger:
        case Pinch3fGesturePrivate::Pinch3fTwoFingers:
            if (event.type == GestureTouchEvent::TouchMove
                    && event.flags & GestureTouchEvent::GESTURE_EVENT_TINY_MOVE)
                return Action::Ignore;

            if (event.type != GestureTouchEvent::TouchStart)
                return Action::CancelGesture;

            if (event.numTouchPoints == 1)
                pinch3fGesture->d->state = Pinch3fGesturePrivate::Pinch3fOneFinger;
            else if (event.numTouchPoints == 2)
                pinch3fGesture->d->state = Pinch3fGesturePrivate::Pinch3fTwoFingers;
            else if (event.numTouchPoints == 3) {
                pinch3fGesture->d->state = Pinch3fGesturePrivate::Pinch3fThreeFingers;
                pinch3fGesture->d->begin(p0, p1, p2);
            } else
                return Action::CancelGesture;

            return Action::MayBeGesture;

        case Pinch3fGesturePrivate::Pinch3fThreeFingers:
            if (event.type != GestureTouchEvent::TouchMove) {
                pinch3fGesture->d->state = Pinch3fGesturePrivate::Pinch3fFinished;
                pinch3fGesture->setGestureState(Gesture::GestureFinished);
                return Action::CancelGesture;
            }

            if (!pinch3fGesture->d->isPinch(p0, p1, p2))
                return Action::CancelGesture;

            pinch3fGesture->setGestureState(Gesture::GestureStarted);
            pinch3fGesture->d->state = Pinch3fGesturePrivate::Pinch3fMoving;

            return Action::TriggerGesture;

        case Pinch3fGesturePrivate::Pinch3fMoving:
            if (event.type != GestureTouchEvent::TouchMove) {
                pinch3fGesture->d->state = Pinch3fGesturePrivate::Pinch3fFinished;
                pinch3fGesture->setGestureState(Gesture::GestureFinished);
                return Action::FinishGesture;
            }

            pinch3fGesture->setGestureState(Gesture::GestureUpdated);
            pinch3fGesture->d->update(p0, p1, p2);

            return Action::TriggerGesture;

        case Pinch3fGesturePrivate::Pinch3fFinished:
            return Action::FinishGesture;
    }

    return Action::FinishGesture;
}

Gesture *Pinch3fRecognizer::create()
{
    return new Pinch3fGesture();
}
