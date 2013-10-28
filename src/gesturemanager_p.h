#ifndef GESTUREMANAGER_P_H
#define GESTUREMANAGER_P_H

#include <list>
#include <map>

class GestureMoveEventFilter;

class GestureManagerPrivate {
public:
    GestureManagerPrivate(GestureManager* parent);
    ~GestureManagerPrivate();

    void registerKnowRecognizers();
    void createGestures();

    GestureManager *m_parent;

    std::list<GestureRecognizer *> m_recognizers;
    std::map<GestureRecognizer *, Gesture *> m_gestures;
    int m_availableGestures;
    GestureMoveEventFilter *m_moveEventFilter;

    // Known recognizers
    GestureRecognizer *m_panRecognizer;
    GestureRecognizer *m_tapRecognizer;
    GestureRecognizer *m_pinchRecognizer;
};

#endif // GESTUREMANAGER_P_H
