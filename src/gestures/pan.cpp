#include "pan.h"
#include "pan_p.h"

#include "events/gesturetouchevent.h"
#include "utils/vector2d.h"
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
    , side(PanGesture::NoSide)
    , numTouchPoints(0)
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

                gesture->numTouchPoints = ev.numTouchPoints;
                gesture->d->panTouchPoints[0].x = ev.touchPoints[0].x;
                gesture->d->panTouchPoints[0].y = ev.touchPoints[0].y;
                gesture->x = ev.touchPoints[0].x;
                gesture->y = ev.touchPoints[0].y;

                gesture->d->startX = ev.touchPoints[0].x;
                gesture->d->startY = ev.touchPoints[0].y;
                gesture->side = computeSwipeSide(gesture->d->startX, gesture->d->startY);
                return MayBeGesture;
            }
            break;

        case PanGesturePrivate::WaitingMove:
            if (ev.type == GestureTouchEvent::TouchStart) {
                gesture->numTouchPoints = ev.numTouchPoints;
                int xsum = 0;
                int ysum = 0;
                for (unsigned int i = 0; i < ev.numTouchPoints; ++i) {
                    gesture->d->panTouchPoints[i].x = ev.touchPoints[i].x;
                    gesture->d->panTouchPoints[i].y = ev.touchPoints[i].y;
                    xsum += ev.touchPoints[i].x;
                    ysum += ev.touchPoints[i].y;
                }
                gesture->x = xsum / ev.numTouchPoints;
                gesture->y = ysum / ev.numTouchPoints;

                return MayBeGesture;
            } else if (ev.type == GestureTouchEvent::TouchMove) {
                if (ev.flags & GestureTouchEvent::GESTURE_EVENT_TINY_MOVE)
                    return Ignore;

                // If there are 2 fingers, we need to make sure this is not a pinch
                if (ev.numTouchPoints == 2) {
                    if (!(ev.flags & GestureTouchEvent::GESTURE_EVENT_CAN_RECOGNIZE_DIRECTION))
                        return Ignore;

                    // Check if the 2 fingers are moving in the same direction
                    Vector2D diffA(ev.touchPoints[0].x - gesture->d->panTouchPoints[0].x,
                            ev.touchPoints[0].y - gesture->d->panTouchPoints[0].y);
                    Vector2D diffB(ev.touchPoints[1].x - gesture->d->panTouchPoints[1].x,
                            ev.touchPoints[1].y - gesture->d->panTouchPoints[1].y);

                    // The amount of degrees those two vectors can be apart of
                    // each other
                    const float cos30 = 0.8660254037844387;
                    float diffCos = Vector2D::cos(diffA, diffB);

                    if (diffCos < cos30)
                        return CancelGesture;
                }

                gesture->d->state = PanGesturePrivate::Moving;
                gesture->setGestureState(Gesture::GestureStarted);

                updateGesture(gesture, ev);

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

void PanRecognizer::updateGesture(PanGesture *gesture, const GestureTouchEvent &ev)
{
    int xsum = 0;
    int ysum = 0;
    for (int i = 0; i < ev.numTouchPoints; ++i) {
        xsum += ev.touchPoints[i].x;
        ysum += ev.touchPoints[i].y;
    }

    float x = xsum / ev.numTouchPoints;
    float y = ysum / ev.numTouchPoints;
    gesture->deltaX = x - gesture->x;
    gesture->deltaY = y - gesture->y;
    gesture->x = x;
    gesture->y = y;
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
