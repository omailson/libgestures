
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

    GestureType gestureType() const;
    GestureState gestureState() const;

    Gesture();

private:
    GestureType m_gestureType;
    GestureState m_gestureState;
};

#endif // GESTURE_H
