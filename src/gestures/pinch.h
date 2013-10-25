
#ifndef PINCH_H
#define PINCH_H

#include <gesture.h>
#include <gesturerecognizer.h>

struct GestureTouchEvent;
class PinchGesturePrivate;

class PinchGesture : public Gesture {
public:
    PinchGesture();
    ~PinchGesture();

    GestureType gestureType() const;

    int centerX;
    int centerY;
    double scale;

private:
    PinchGesturePrivate *d;
    friend class PinchRecognizer;
};

class PinchRecognizer : public GestureRecognizer {
public:
    PinchRecognizer();
    ~PinchRecognizer();

    bool useTimer() const;
    Action recognize(Gesture *gesture, const GestureTouchEvent &event);
    Gesture *create();
};

#endif // PINCH_H
