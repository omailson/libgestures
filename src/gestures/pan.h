#ifndef PAN_H
#define PAN_H

#include <gesture.h>
#include <gesturerecognizer.h>

class PanGesturePrivate;
class GestureTouchEvent;

class PanGesture : public Gesture {
public:
    PanGesture();
    ~PanGesture();

    GestureType gestureType() const;

    int x;
    int y;
    int deltaX;
    int deltaY;

private:
    PanGesturePrivate *d;
    friend class PanRecognizer;
};

class PanRecognizer : public GestureRecognizer {
public:
    PanRecognizer();
    ~PanRecognizer();

    Action recognize(Gesture *gesture, const GestureTouchEvent &event);
    Gesture *create();
};

#endif // PAN_H
