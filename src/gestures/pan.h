#ifndef PAN_H
#define PAN_H

#include <gesture.h>
#include <gesturerecognizer.h>

class PanGesturePrivate;
class PanRecognizerPrivate;
class GestureTouchEvent;

class PanGesture : public Gesture {
public:
    enum SwipeSide {
        NoSide = 0,
        Left = 1,
        Top = 2,
        Right = 4,
        Bottom = 8
    };

    PanGesture();
    ~PanGesture();

    GestureType gestureType() const;

    int side;
    int x;
    int y;
    int deltaX;
    int deltaY;
    int numTouchPoints;

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

    void setBounds(int x, int y, int width, int height);

private:
    PanRecognizerPrivate *d;
    PanGesture::SwipeSide computeSwipeSide(int x, int y) const;
    void updateGesture(PanGesture *gesture, const GestureTouchEvent &event);
};

#endif // PAN_H
