
#ifndef GESTURE_H
#define GESTURE_H

class Gesture {
public:
    enum GestureType {
        NoGesture
    };

    enum GestureState {
        GestureStarted,
        GestureUpdated,
        GestureFinished,
        GestureCanceled
    };

    virtual GestureType gestureType() const;
    GestureState gestureState() const;

    Gesture();

private:
    GestureState m_gestureState;
};

#endif // GESTURE_H
