
#include <android/input.h>
#include <unistd.h>

#include "ainputeventhelper.h"

GestureTouchEvent convertToGestureTouchEvent(const AInputEvent *ev)
{
    GestureTouchEvent touchEv;
    touchEv.timestamp = (double) AMotionEvent_getEventTime(ev);
    touchEv.type = convertToTouchEventType(ev);

    touchEv.numTouchPoints = AMotionEvent_getPointerCount(ev);
    for (unsigned int i = 0; i < touchEv.numTouchPoints; i++) {
        GestureTouchPoint touchPoint;
        touchPoint.state = convertToTouchPointState(ev, i);
        touchPoint.id = AMotionEvent_getPointerId(ev, i);
        touchPoint.x = AMotionEvent_getX(ev, i);
        touchPoint.y = AMotionEvent_getY(ev, i);
        touchPoint.verticalRadius = AMotionEvent_getTouchMajor(ev, i) / 2;
        touchPoint.horizontalRadius = AMotionEvent_getTouchMinor(ev, i) / 2;

        touchEv.touchPoints[i] = touchPoint;
    }

    return touchEv;
}

GestureTouchEvent::EventType convertToTouchEventType(const AInputEvent *ev)
{
    int32_t action = AMotionEvent_getAction(ev);
    action = action & AMOTION_EVENT_ACTION_MASK;

    if (action == AMOTION_EVENT_ACTION_DOWN
            || action == AMOTION_EVENT_ACTION_POINTER_DOWN)
        return GestureTouchEvent::TouchStart;
    else if (action == AMOTION_EVENT_ACTION_UP
            || action == AMOTION_EVENT_ACTION_POINTER_UP)
        return GestureTouchEvent::TouchEnd;
    else if (action == AMOTION_EVENT_ACTION_MOVE)
        return GestureTouchEvent::TouchMove;
    else
        return GestureTouchEvent::TouchCancel;
}

GestureTouchPoint::State convertToTouchPointState(const AInputEvent *ev, size_t pointerIndex)
{
    int32_t action = AMotionEvent_getAction(ev);

    // Simple actions performed by the primary pointer
    if (action == AMOTION_EVENT_ACTION_DOWN)
        return GestureTouchPoint::TouchPressed;
    else if (action == AMOTION_EVENT_ACTION_UP)
        return GestureTouchPoint::TouchReleased;

    // Actions that occur regardless of the pointer index
    if (action == AMOTION_EVENT_ACTION_MOVE)
        return GestureTouchPoint::TouchMoved;
    else if (action == AMOTION_EVENT_ACTION_CANCEL)
        return GestureTouchPoint::TouchCancelled;

    // Index where the action occurred
    size_t actionIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
        >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

    if (pointerIndex != actionIndex)
        return GestureTouchPoint::TouchStationary;

    int32_t actionMasked = action & AMOTION_EVENT_ACTION_MASK;
    if (actionMasked == AMOTION_EVENT_ACTION_POINTER_DOWN)
        return GestureTouchPoint::TouchPressed;
    else if (actionMasked == AMOTION_EVENT_ACTION_POINTER_UP)
        return GestureTouchPoint::TouchReleased;

    return GestureTouchPoint::TouchStationary;
}
