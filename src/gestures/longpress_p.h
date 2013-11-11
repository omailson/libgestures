#ifndef LONGPRESS_P_H
#define LONGPRESS_P_H

class LongPressGesturePrivate
{
public:
    enum LongPressState {
        NoGesture,
        TapStarted,
        GestureTriggered
    };

    LongPressGesturePrivate();
    LongPressState state;
    long long int lastTimestamp;
};

#endif // LONGPRESS_P_H
