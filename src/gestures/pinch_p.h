
#ifndef PINCH_P_H
#define PINCH_P_H

class PinchGesture;
class PinchGesturePrivate {
public:
    enum PinchState {
        NoGesture,
        PinchOneFinger,
        PinchTwoFingers,
        PinchMoving,
        PinchFinished
    };

    PinchGesturePrivate(PinchGesture *q_ptr);

    void setOrigin(int xa, int ya, int xb, int yb);
    void updateGesture(int xa, int ya, int xb, int yb);

    PinchState state;

private:
    PinchGesture *q;

    double distOrigin;
};

#endif
