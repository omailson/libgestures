#include "swipe.h"
#include "swipe_p.h"
#include "events/gesturetouchevent.h"

#include <math.h>

#define PI 3.14159265

static inline float fastSqrt(float value)
{
    union {
        float f;
        int i;
    } u;

    float v = value;
    const float half = 0.5f * v;

    u.f = v;
    u.i = 0x5f375a86 - (u.i >> 1);

    v = u.f;
    v *= (1.5f - half * v * v);
    v *= (1.5f - half * v * v);
    v *= (1.5f - half * v * v);

    return value * v;
}

SwipeGesturePrivate::SwipeGesturePrivate()
    : state(NoGesture)
{
}

SwipeGesture::SwipeGesture()
    : Gesture()
    , d(new SwipeGesturePrivate)
{
}

SwipeGesture::~SwipeGesture()
{
    delete d;
}

Gesture::GestureType SwipeGesture::gestureType() const
{
    return Swipe;
}

SwipeRecognizer::SwipeRecognizer()
    : GestureRecognizer()
    , m_x(0)
    , m_y(0)
    , m_width(0)
    , m_height(0)
{
}

SwipeRecognizer::~SwipeRecognizer()
{
}

Gesture *SwipeRecognizer::create()
{
    return new SwipeGesture;
}

GestureRecognizer::Action SwipeRecognizer::recognize(Gesture *baseGesture, const GestureTouchEvent &ev)
{
    if (m_x + m_y + m_width + m_height == 0)
        return CancelGesture;

    SwipeGesture *gesture = static_cast<SwipeGesture*>(baseGesture);
    switch (gesture->d->state) {
        case SwipeGesturePrivate::NoGesture:
            if (ev.type == GestureTouchEvent::TouchStart) {
                // Save start point
                gesture->d->startX = ev.touchPoints[0].x;
                gesture->d->startY = ev.touchPoints[0].y;
                gesture->x = ev.touchPoints[0].x;
                gesture->y = ev.touchPoints[0].y;
                gesture->side = computeSwipeSide(gesture->d->startX, gesture->d->startY);
                gesture->d->state = SwipeGesturePrivate::WaitingMove;

                // Check if it's outside bounds
                if (gesture->side == SwipeGesture::NoSide)
                    return CancelGesture;

                return MayBeGesture;
            }
            break;

        case SwipeGesturePrivate::WaitingMove:
            if (ev.type == GestureTouchEvent::TouchMove) {
                if (ev.flags & GestureTouchEvent::GESTURE_EVENT_TINY_MOVE)
                    return Ignore;

                gesture->d->state = SwipeGesturePrivate::Moving;
                gesture->setGestureState(Gesture::GestureStarted);
                gesture->deltaX = ev.touchPoints[0].x - gesture->x;
                gesture->deltaY = ev.touchPoints[0].y - gesture->y;

                // Check whether the finger is moving to the appropriate direction
                double distance = fastSqrt(gesture->deltaX*gesture->deltaX + gesture->deltaY*gesture->deltaY);
                double deltaX = gesture->deltaX / distance;
                double deltaY = gesture->deltaY / distance;
                if (gesture->side & SwipeGesture::Left
                        && deltaX < cos(30 * PI / 180.0))
                    gesture->side &= ~SwipeGesture::Left;
                if (gesture->side & SwipeGesture::Right
                        && deltaX > -cos(30 * PI / 180.0))
                    gesture->side &= ~SwipeGesture::Right;
                if (gesture->side & SwipeGesture::Top
                        && deltaY < sin((90 - 30) * PI / 180.0))
                    gesture->side &= ~SwipeGesture::Top;
                if (gesture->side & SwipeGesture::Bottom
                        && deltaY > -sin((90 - 30) * PI / 180.0))
                    gesture->side &= ~SwipeGesture::Bottom;

                updateGesture(gesture, ev);

                if (gesture->side != SwipeGesture::NoSide)
                    return TriggerGesture;
            }
            break;

        case SwipeGesturePrivate::Moving:
            if (ev.type == GestureTouchEvent::TouchMove) {
                gesture->setGestureState(Gesture::GestureUpdated);
                updateGesture(gesture, ev);

                return TriggerGesture;
            } else if (ev.type == GestureTouchEvent::TouchEnd) {
                gesture->setGestureState(Gesture::GestureFinished);
                updateGesture(gesture, ev);

                return FinishGesture;
            }
            break;
    }

    return CancelGesture;
}

void SwipeRecognizer::updateGesture(SwipeGesture *gesture, const GestureTouchEvent &ev) const
{
    if (gesture->side & (SwipeGesture::Left | SwipeGesture::Right)) {
        gesture->deltaX = ev.touchPoints[0].x - gesture->x;
        gesture->x = ev.touchPoints[0].x;
    }

    if (gesture->side & (SwipeGesture::Top | SwipeGesture::Bottom)) {
        gesture->deltaY = ev.touchPoints[0].y - gesture->y;
        gesture->y = ev.touchPoints[0].y;
    }
}

void SwipeRecognizer::setBounds(int x, int y, int width, int height)
{
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
}

SwipeGesture::SwipeSide SwipeRecognizer::computeSwipeSide(int x, int y) const
{
    int side = SwipeGesture::NoSide;

    if (x < m_x)
        side |= SwipeGesture::Left;
    if (x > m_x + m_width)
        side |= SwipeGesture::Right;
    if (y < m_y)
        side |= SwipeGesture::Top;
    if (y > m_y + m_height)
        side |= SwipeGesture::Bottom;

    return (SwipeGesture::SwipeSide) side;
}
