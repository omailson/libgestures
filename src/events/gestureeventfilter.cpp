#include "events/gestureeventfilter_p.h"

#include <algorithm>

GestureEventFilter::GestureEventFilter()
{
}

GestureEventFilter::~GestureEventFilter()
{
}

GestureMoveEventFilter::GestureMoveEventFilter()
    : GestureEventFilter()
    , m_moveThreshold(0)
    , m_trackMovement(true)
{
}

void GestureMoveEventFilter::filter(GestureTouchEvent *ev)
{
    if (!ev)
        return;

    if (m_moveThreshold <= 0)
        return;

    if (ev->type == GestureTouchEvent::TouchStart) {
        if (ev->numTouchPoints == 1)
            m_trackMovement = true;
        updateHistory(ev);
    } else if (ev->type == GestureTouchEvent::TouchMove && m_trackMovement) {
        bool aboveThreshold = false;
        unsigned int a = 0, b = 0;
        while (!aboveThreshold && a < m_numTouchPoints
                && b < ev->numTouchPoints) {

            // Compare the correct finger ids
            while (m_touchPoints[a].id < ev->touchPoints[b].id)
                a++;
            while (m_touchPoints[a].id > ev->touchPoints[b].id)
                b++;

            int diff = abs(m_touchPoints[a].x - ev->touchPoints[b].x)
                + abs(m_touchPoints[a].y - ev->touchPoints[b].y);

            if (diff > m_moveThreshold)
                aboveThreshold = true;

            a++;
            b++;
        }

        if (aboveThreshold) {
            ev->flags &= ~GestureTouchEvent::GESTURE_EVENT_TINY_MOVE;
            m_trackMovement = false;
        } else {
            ev->flags |= GestureTouchEvent::GESTURE_EVENT_TINY_MOVE;
        }
    }
}

void GestureMoveEventFilter::updateHistory(GestureTouchEvent *ev)
{
    m_numTouchPoints = ev->numTouchPoints;
    for (unsigned int i = 0; i < ev->numTouchPoints; ++i) {
        if (ev->touchPoints[i].state == GestureTouchPoint::TouchPressed)
            m_touchPoints[i] = ev->touchPoints[i];
    }
}
