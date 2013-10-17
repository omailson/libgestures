
#ifndef PAN_H
#define PAN_H

#include <gesture.h>
#include <gesturerecognizer.h>

class PanGesture : Gesture {
public:
    PanGesture();
    ~PanGesture();
};

class PanRecognizer : GestureRecognizer {
public:
    PanRecognizer();
    ~PanRecognizer();
};

#endif // PAN_H
