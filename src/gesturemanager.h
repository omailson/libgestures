
#ifndef GESTUREMANAGER_H
#define GESTUREMANAGER_H

#include <list>
#include "gesturerecognizer.h"
#include "NIXEvents.h"

class GestureManager {
public:
    GestureManager();
    ~GestureManager();

    void sendEvent(NIXTouchEvent *event);
    void registerRecognizer(GestureRecognizer *recognizer);

private:
    std::list<GestureRecognizer *> m_recognizers;

    void registerKnowRecognizers();
};

#endif // GESTUREMANAGER_H
