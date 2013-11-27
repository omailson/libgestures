#ifndef GESTUREMANAGER_P_H
#define GESTUREMANAGER_P_H

#include <list>
#include <map>

#include "gesturerecognizer.h"

class GestureMoveEventFilter;
class PinchEventFilter;

class GestureManagerPrivate {
public:
    GestureManagerPrivate(GestureManager* parent);
    ~GestureManagerPrivate();

    void createGestures();
    void removeUnusedGestures();
    Gesture *handleTriggeredGesture(GestureTouchEvent *event, long long int timestamp);
    void filterEvent(GestureTouchEvent *event);

    GestureManager *m_parent;

    std::list<GestureRecognizer *> m_recognizers;
    std::list<Gesture *> m_gesturesToRemove;
    std::map<GestureRecognizer *, Gesture *> m_gestures;
    std::map<GestureRecognizer *, GestureRecognizer::Action> m_acceptedGestures;
    int m_availableGestures;
    GestureMoveEventFilter *m_moveEventFilter;
    PinchEventFilter *m_pinchEventFilter;
    GestureRecognizer *m_triggeredGestureRecognizer;
};

#endif // GESTUREMANAGER_P_H
