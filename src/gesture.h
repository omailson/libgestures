
#ifndef GESTURE_H
#define GESTURE_H

class Gesture {
public:
    enum GestureType {
        NoGesture,
        Tap,
        Pan,
        Pinch,
        DoubleTap,
        LongPress,
        CustomGesture = 1000
    };

    enum GestureState {
        GestureStarted,
        GestureUpdated,
        GestureFinished,
        GestureCancelled
    };

    Gesture();
    virtual ~Gesture();

    virtual GestureType gestureType() const;
    GestureState gestureState() const;
    void setGestureState(GestureState state);

private:
    GestureState m_gestureState;
};

#endif // GESTURE_H
