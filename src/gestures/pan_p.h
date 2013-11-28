#ifndef PAN_GESTURE_P_H
#define PAN_GESTURE_P_H

struct PanTouchPoint {
    int x;
    int y;
};

class PanGesturePrivate {
public:
    enum PanState {
        NoGesture,
        WaitingMove,
        Moving
    };

    PanGesturePrivate();
    PanState state;
    int startX;
    int startY;
    PanTouchPoint panTouchPoints[10];
};

class PanRecognizerPrivate {
public:
    PanRecognizerPrivate();

    int x;
    int y;
    int width;
    int height;
};

#endif // PAN_GESTURE_P_H
