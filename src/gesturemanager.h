
#ifndef GESTUREMANAGER_H
#define GESTUREMANAGER_H

struct GestureTouchEvent;
class Gesture;
class GestureRecognizer;
class GestureManagerPrivate;

class GestureManager {
public:
    GestureManager();
    ~GestureManager();

    Gesture* sendEvent(GestureTouchEvent *event, unsigned int timestamp);
    void registerRecognizer(GestureRecognizer *recognizer);
    void setMoveThreshold(int moveThreshold);

private:
    GestureManagerPrivate *d;
    friend class GestureManagerPrivate;
};

#endif // GESTUREMANAGER_H
