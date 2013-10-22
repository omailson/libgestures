#ifndef GESTURETOUCHEVENT_H
#define GESTURETOUCHEVENT_H

#define GESTURE_MAXIMUM_TOUCH_POINTS_PER_TOUCH_EVENT 10

struct GestureTouchPoint
{
    enum State {
        TouchReleased,
        TouchPressed,
        TouchMoved,
        TouchStationary,
        TouchCancelled
    };

    State state;
    int x;
    int y;
    int verticalRadius;
    int horizontalRadius;
    float rotationAngle;
    float pressure;
    unsigned int id;
};

struct GestureTouchEvent
{
    enum EventType {
        TouchStart,
        TouchMove,
        TouchEnd,
        TouchCancel
    };

    EventType type;
    double timestamp;
    unsigned int numTouchPoints;
    GestureTouchPoint touchPoints[GESTURE_MAXIMUM_TOUCH_POINTS_PER_TOUCH_EVENT];
};

#endif // GESTURETOUCHEVENT_H