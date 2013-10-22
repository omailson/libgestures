
#ifndef AINPUTEVENTHELPER_H
#define AINPUTEVENTHELPER_H

#include "gesturetouchevent.h"

GestureTouchEvent convertToGestureTouchEvent(const AInputEvent *event);
GestureTouchEvent::EventType convertToTouchEventType(const AInputEvent *ev);
GestureTouchPoint::State convertToTouchPointState(const AInputEvent *ev, size_t pointerIndex);

#endif // AINPUTEVENTHELPER_H
