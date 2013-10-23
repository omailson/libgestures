
#ifndef GESTUREMANAGER_H
#define GESTUREMANAGER_H

struct NIXTouchEvent;
class Gesture;
class GestureRecognizer;
class GestureManagerPrivate;

class GestureManager {
public:
    GestureManager();
    ~GestureManager();

    Gesture* sendEvent(NIXTouchEvent *event, unsigned int timestamp);
    void registerRecognizer(GestureRecognizer *recognizer);

private:
    GestureManagerPrivate *d;
    friend class GestureManagerPrivate;
};

#endif // GESTUREMANAGER_H
