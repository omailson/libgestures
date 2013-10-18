
#ifndef AINPUTEVENTHELPER_H
#define AINPUTEVENTHELPER_H

#include <NIXEvents.h>

NIXTouchEvent convertToNIXTouchEvent(const AInputEvent *event, WKViewRef webView);
NIXInputEventType convertToTouchEventType(const AInputEvent *ev);
NIXTouchPointState convertToTouchPointState(const AInputEvent *ev, size_t pointerIndex);
NIXInputEventType convertToMouseEventType(const AInputEvent *ev);

#endif // AINPUTEVENTHELPER_H
