#ifndef GESTUREEVENTFILTER_P_H
#define GESTUREEVENTFILTER_P_H

#include "events/gesturetouchevent.h"
#include "utils/vector2d.h"

#define MIN_MOVEMENT_LENGTH_SQUARED 70

class GestureEventFilter
{
public:
    GestureEventFilter();
    virtual ~GestureEventFilter();

    virtual void filter(GestureTouchEvent *ev) = 0;
};

class GestureMoveEventFilter : public GestureEventFilter
{
public:
    GestureMoveEventFilter();

    void setMoveThreshold(int moveThreshold) { m_moveThreshold = moveThreshold; }
    void filter(GestureTouchEvent *ev);

private:
    int m_moveThreshold;
    bool m_trackMovement;
    unsigned int m_numTouchPoints;
    GestureTouchPoint m_touchPoints[GESTURE_MAXIMUM_TOUCH_POINTS_PER_TOUCH_EVENT];

    void updateHistory(GestureTouchEvent *ev);
};

class PinchEventFilter : public GestureEventFilter
{
public:
    PinchEventFilter();

    void filter(GestureTouchEvent *ev);

private:
    void reset();

    Vector2D m_p0;
    Vector2D m_p1;
};

#endif // GESTUREEVENTFILTER_P_H
