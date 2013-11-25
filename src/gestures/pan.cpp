#include "pan.h"
#include "pan_p.h"

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

PanRecognizerPrivate::PanRecognizerPrivate()
    : x(0)
    , y(0)
    , width(0)
    , height(0)
{
}

PanRecognizer::PanRecognizer()
    : GestureRecognizer()
    , d(new PanRecognizerPrivate)
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
                gesture->d->startX = ev.touchPoints[0].x;
                gesture->d->startY = ev.touchPoints[0].y;
                gesture->side = computeSwipeSide(gesture->d->startX, gesture->d->startY);
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

                if (gesture->side != PanGesture::NoSide) {
                    // Check whether the finger is moving to the appropriate direction
                    double distance = fastSqrt(gesture->deltaX*gesture->deltaX + gesture->deltaY*gesture->deltaY);
                    double deltaX = gesture->deltaX / distance;
                    double deltaY = gesture->deltaY / distance;
                    if (gesture->side & PanGesture::Left
                            && deltaX < cos(30 * PI / 180.0))
                        gesture->side &= ~PanGesture::Left;
                    if (gesture->side & PanGesture::Right
                            && deltaX > -cos(30 * PI / 180.0))
                        gesture->side &= ~PanGesture::Right;
                    if (gesture->side & PanGesture::Top
                            && deltaY < sin((90 - 30) * PI / 180.0))
                        gesture->side &= ~PanGesture::Top;
                    if (gesture->side & PanGesture::Bottom
                            && deltaY > -sin((90 - 30) * PI / 180.0))
                        gesture->side &= ~PanGesture::Bottom;
                }

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

PanGesture::SwipeSide PanRecognizer::computeSwipeSide(int x, int y) const
{
    if (d->width == 0 || d->height == 0)
        return PanGesture::NoSide;

    int side = PanGesture::NoSide;

    if (x < d->x)
        side |= PanGesture::Left;
    if (x > d->x + d->width)
        side |= PanGesture::Right;
    if (y < d->y)
        side |= PanGesture::Top;
    if (y > d->y + d->height)
        side |= PanGesture::Bottom;

    return (PanGesture::SwipeSide) side;
}

void PanRecognizer::setBounds(int x, int y, int width, int height)
{
    d->x = x;
    d->y = y;
    d->width = width;
    d->height = height;
}

Gesture *PanRecognizer::create()
{
    return new PanGesture();
}

PanRecognizer::~PanRecognizer()
{
    delete d;
}
