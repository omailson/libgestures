
#ifndef PAN_H
#define PAN_H

#include <gesture.h>
#include <gesturerecognizer.h>

class PanGesture : public Gesture {
public:
    PanGesture();
    ~PanGesture();

    GestureType gestureType() const;
};

class PanRecognizer : public GestureRecognizer {
public:
    PanRecognizer();
    ~PanRecognizer();

    Action recognize(Gesture *gesture, const NIXTouchEvent &event);
    Gesture *create();
};

#endif // PAN_H
