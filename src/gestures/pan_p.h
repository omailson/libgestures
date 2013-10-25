#ifndef PAN_GESTURE_P_H
#define PAN_GESTURE_P_H

class PanGesturePrivate {
public:
    enum PanState {
        NoGesture,
        WaitingMove,
        Moving
    };

    PanGesturePrivate();
    PanState state;
};

#endif // PAN_GESTURE_P_H
