#ifndef GESTUREEVENTFILTER_P_H
#define GESTUREEVENTFILTER_P_H

#include "events/gesturetouchevent.h"

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

#endif // GESTUREEVENTFILTER_P_H
