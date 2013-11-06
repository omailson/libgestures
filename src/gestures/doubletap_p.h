#ifndef DOUBLETAP_P_H
#define DOUBLETAP_P_H

class DoubleTapGesturePrivate
{
public:
    enum DoubleTapState {
        NoGesture,
        FirstTapStarted,
        StartTrackingTime,
        SecondTapStarted
    };

    DoubleTapGesturePrivate();
    DoubleTapState state;
    long long int lastTimestamp;
};

#endif // DOUBLETAP_P_H
