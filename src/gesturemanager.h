
#ifndef GESTUREMANAGER_H
#define GESTUREMANAGER_H

#include <list>
#include <map>
#include "gesturerecognizer.h"
#include "NIXEvents.h"

class GestureManager {
public:
    GestureManager();
    ~GestureManager();

    Gesture* sendEvent(NIXTouchEvent *event, unsigned int timestamp);
    void registerRecognizer(GestureRecognizer *recognizer);

private:
    std::list<GestureRecognizer *> m_recognizers;
    std::map<GestureRecognizer *, Gesture *> m_gestures;
    int m_availableGestures;

    // Known recognizers
    GestureRecognizer *m_panRecognizer;
    GestureRecognizer *m_tapRecognizer;

    void registerKnowRecognizers();
    void createGestures();
};

#endif // GESTUREMANAGER_H
