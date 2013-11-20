
#ifndef PINCH_P_H
#define PINCH_P_H

#include "utils/vector2d.h"

#define MIN_COS 0.45f
#define MIN_MOVEMENT_LENGTH_SQUARED 70

class PinchGesture;
class PinchGesturePrivate {
public:
    enum PinchState {
        NoGesture,
        PinchOneFinger,
        PinchTwoFingers,
        PinchMoving,
        PinchFinished
    };

    PinchGesturePrivate(PinchGesture *q_ptr);

    void setOrigin(const Vector2D &p0, const Vector2D &p1);
    void updateGesture(const Vector2D &p0, const Vector2D &p1);
    bool isPinch(const Vector2D &p0, const Vector2D &p1) const;
    bool canRecognizePinch(const Vector2D &p0, const Vector2D &p1) const;

    PinchState state;
private:
    PinchGesture *q;

    double m_distOrigin;
    Vector2D m_originA;
    Vector2D m_originB;
    Vector2D m_lastPosA;
    Vector2D m_lastPosB;
};

#endif
