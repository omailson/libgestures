#ifndef SWIPE_H
#define SWIPE_H

#include "gesture.h"
#include "gesturerecognizer.h"

class SwipeGesturePrivate;

class SwipeGesture : public Gesture
{
public:
    enum SwipeSide {
        NoSide = 0,
        Left = 1,
        Top = 2,
        Right = 4,
        Bottom = 8
    };

    SwipeGesture();
    ~SwipeGesture();

    GestureType gestureType() const;

    int side;
    int x;
    int y;
    int deltaX;
    int deltaY;

private:
    SwipeGesturePrivate *d;
    friend class SwipeRecognizer;
};

class SwipeRecognizer : public GestureRecognizer
{
public:
    SwipeRecognizer();
    ~SwipeRecognizer();

    Gesture *create();
    Action recognize(Gesture *gesture, const GestureTouchEvent &event);

    void setBounds(int x, int y, int width, int height);

private:
    int m_x;
    int m_y;
    int m_width;
    int m_height;

    SwipeGesture::SwipeSide computeSwipeSide(int x, int y) const;
    void updateGesture(SwipeGesture *gesture, const GestureTouchEvent &event) const;
};

#endif // SWIPE_H
