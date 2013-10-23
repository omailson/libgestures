#ifndef GESTUREMANAGER_P_H
#define GESTUREMANAGER_P_H

#include <list>
#include <map>

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

    // Known recognizers
    GestureRecognizer *m_panRecognizer;
    GestureRecognizer *m_tapRecognizer;
};

#endif // GESTUREMANAGER_P_H
