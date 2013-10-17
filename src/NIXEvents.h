/*
 * Copyright (C) 2012-2013 Nokia Corporation and/or its subsidiary(-ies).
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef NIXEvents_h
#define NIXEvents_h

#ifdef __cplusplus
extern "C" {
#endif

enum NIXInputEventType {
    kNIXInputEventTypeMouseDown,
    kNIXInputEventTypeMouseUp,
    kNIXInputEventTypeMouseMove,
    kNIXInputEventTypeWheel,
    kNIXInputEventTypeKeyDown,
    kNIXInputEventTypeKeyUp,
    kNIXInputEventTypeTouchStart,
    kNIXInputEventTypeTouchMove,
    kNIXInputEventTypeTouchEnd,
    kNIXInputEventTypeTouchCancel,
    kNIXInputEventTypeGestureSingleTap
};
typedef enum NIXInputEventType NIXInputEventType;

enum {
    kNIXInputEventModifiersShiftKey    = 1 << 0,
    kNIXInputEventModifiersControlKey  = 1 << 1,
    kNIXInputEventModifiersAltKey      = 1 << 2,
    kNIXInputEventModifiersMetaKey     = 1 << 3,
    kNIXInputEventModifiersCapsLockKey = 1 << 4,
    kNIXInputEventModifiersNumLockKey  = 1 << 5
};
typedef unsigned int NIXInputEventModifiers;

#define NIX_INPUT_EVENT_BASE \
    NIXInputEventType type; \
    double timestamp; \
    int x; \
    int y; \
    int globalX; \
    int globalY;

struct NIXInputEvent {
    NIX_INPUT_EVENT_BASE
};
typedef struct NIXInputEvent NIXInputEvent;

enum NIXTouchPointState {
    kNIXTouchPointStateTouchReleased,
    kNIXTouchPointStateTouchPressed,
    kNIXTouchPointStateTouchMoved,
    kNIXTouchPointStateTouchStationary,
    kNIXTouchPointStateTouchCancelled
};
typedef enum NIXTouchPointState NIXTouchPointState;

struct NIXTouchPoint {
    NIXTouchPointState state;
    int x;
    int y;
    int globalX;
    int globalY;
    int verticalRadius;
    int horizontalRadius;
    float rotationAngle;
    float pressure;
    unsigned id;
};
typedef struct NIXTouchPoint NIXTouchPoint;

enum { kNIXMaximumTouchPointsPerTouchEvent = 10 };

struct NIXTouchEvent {
    NIX_INPUT_EVENT_BASE
    unsigned numTouchPoints;
    NIXTouchPoint touchPoints[kNIXMaximumTouchPointsPerTouchEvent];
};
typedef struct NIXTouchEvent NIXTouchEvent;

#ifdef __cplusplus
}
#endif

#endif // NIXEvents_h
