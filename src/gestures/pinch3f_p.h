
#ifndef PINCH3F_P_H
#define PINCH3F_P_H

#include "utils/vector2d.h"

class Pinch3fGesture;
class Pinch3fGesturePrivate {
public:
    enum Pinch3fState {
        NoGesture,
        Pinch3fOneFinger,
        Pinch3fTwoFingers,
        Pinch3fThreeFingers,
        Pinch3fMoving,
        Pinch3fFinished
    };

    Pinch3fGesturePrivate(Pinch3fGesture *q_ptr);

    bool isPinch(const Vector2D &p0, const Vector2D &p1, const Vector2D &p2) const;
    void begin(const Vector2D &p0, const Vector2D &p1, const Vector2D &p2);
    void update(const Vector2D &p0, const Vector2D &p1, const Vector2D &p2);

    Pinch3fState state;

private:
    Pinch3fGesture *q;

    double m_initialArea;
};

#endif
