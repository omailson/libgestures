
#ifndef PINCH3F_H
#define PINCH3F_H

#include "gesture.h"
#include "gesturerecognizer.h"

class Pinch3fGesturePrivate;
class Pinch3fGesture : public Gesture {
public:
    Pinch3fGesture();
    ~Pinch3fGesture();

    GestureType gestureType() const;

    int centerX;
    int centerY;
    double scale;

private:
    Pinch3fGesturePrivate *d;
    friend class Pinch3fRecognizer;
};

class Pinch3fRecognizer : public GestureRecognizer {
public:
    Pinch3fRecognizer();

    bool useTimer() const;
    Action recognize(Gesture *gesture, const GestureTouchEvent &event);
    Gesture *create();
};

#endif
