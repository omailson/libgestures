
#include <android/input.h>
#include <unistd.h>

#include <NIXEvents.h>
#include <WebKit2/WKBaseNix.h>
#include <NIXView.h>

#include "ainputeventhelper.h"

NIXTouchEvent convertToNIXTouchEvent(const AInputEvent *ev)
{
    NIXTouchEvent nixEv;
    memset(&nixEv, 0, sizeof(NIXTouchEvent));
    nixEv.timestamp = (double) AMotionEvent_getEventTime(ev);
    nixEv.type = convertToTouchEventType(ev);

    nixEv.numTouchPoints = AMotionEvent_getPointerCount(ev);
    for (unsigned int i = 0; i < nixEv.numTouchPoints; i++) {
        NIXTouchPoint touchPoint;
        touchPoint.state = convertToTouchPointState(ev, i);
        touchPoint.id = AMotionEvent_getPointerId(ev, i);
        touchPoint.x = AMotionEvent_getX(ev, i);
        touchPoint.y = AMotionEvent_getY(ev, i);

        nixEv.touchPoints[i] = touchPoint;
    }

    return nixEv;
}

NIXInputEventType convertToTouchEventType(const AInputEvent *ev)
{
    int32_t action = AMotionEvent_getAction(ev);
    action = action & AMOTION_EVENT_ACTION_MASK;

    if (action == AMOTION_EVENT_ACTION_DOWN
            || action == AMOTION_EVENT_ACTION_POINTER_DOWN)
        return kNIXInputEventTypeTouchStart;
    else if (action == AMOTION_EVENT_ACTION_UP
            || action == AMOTION_EVENT_ACTION_POINTER_UP)
        return kNIXInputEventTypeTouchEnd;
    else if (action == AMOTION_EVENT_ACTION_MOVE)
        return kNIXInputEventTypeTouchMove;
    else
        return kNIXInputEventTypeTouchCancel;
}

NIXTouchPointState convertToTouchPointState(const AInputEvent *ev, size_t pointerIndex)
{
    int32_t action = AMotionEvent_getAction(ev);

    // Simple actions performed by the primary pointer
    if (action == AMOTION_EVENT_ACTION_DOWN)
        return kNIXTouchPointStateTouchPressed;
    else if (action == AMOTION_EVENT_ACTION_UP)
        return kNIXTouchPointStateTouchReleased;

    // Index where the action occurred
    size_t actionIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
        >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

    if (pointerIndex != actionIndex)
        return kNIXTouchPointStateTouchStationary;

    int32_t actionMasked = action & AMOTION_EVENT_ACTION_MASK;
    if (actionMasked == AMOTION_EVENT_ACTION_POINTER_DOWN)
        return kNIXTouchPointStateTouchPressed;
    else if (actionMasked == AMOTION_EVENT_ACTION_POINTER_UP)
        return kNIXTouchPointStateTouchReleased;
    else if (actionMasked == AMOTION_EVENT_ACTION_MOVE)
        return kNIXTouchPointStateTouchMoved;
    else if (actionMasked == AMOTION_EVENT_ACTION_CANCEL)
        return kNIXTouchPointStateTouchCancelled;

    return kNIXTouchPointStateTouchStationary;
}

NIXInputEventType convertToMouseEventType(const AInputEvent *ev)
{
    int32_t action = AMotionEvent_getAction(ev);

    if (action == AMOTION_EVENT_ACTION_DOWN)
        return kNIXInputEventTypeMouseDown;
    else if (action == AMOTION_EVENT_ACTION_UP)
        return kNIXInputEventTypeMouseUp;

    return kNIXInputEventTypeMouseUp;
}
