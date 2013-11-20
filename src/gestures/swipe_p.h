#ifndef SWIPE_P_H
#define SWIPE_P_H

class SwipeGesturePrivate
{
public:
    enum SwipeState {
        NoGesture,
        WaitingMove,
        Moving
    };

    SwipeGesturePrivate();
    SwipeState state;
    int startX;
    int startY;
};

#endif // SWIPE_P_H
