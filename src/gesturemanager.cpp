
#include "gesturemanager.h"
#include "gestures/pan.h"
#include "gestures/tap.h"

GestureManager::GestureManager()
    : m_availableGestures(0)
{
    registerKnowRecognizers();
}

GestureManager::~GestureManager()
{
    delete m_panRecognizer;
    delete m_tapRecognizer;
}

Gesture* GestureManager::sendEvent(NIXTouchEvent *event, unsigned int timestamp)
{
    if (!event && m_availableGestures == 0)
        return NULL;

    if (m_availableGestures == 0)
        createGestures();

    GestureRecognizer *candidateRecognizer = 0;
    GestureRecognizer::Action candidateAction = GestureRecognizer::Ignore;

    std::list<GestureRecognizer*>::iterator it = m_recognizers.begin();
    for (; it != m_recognizers.end(); ++it) {
        GestureRecognizer *recognizer = *it;
        Gesture *gesture = m_gestures[recognizer];
        if (gesture == NULL)
            continue;

        GestureRecognizer::Action action;
        if (recognizer->useTimer()) {
            action = recognizer->recognize(gesture, timestamp);
            if (action != GestureRecognizer::CancelGesture)
                action = recognizer->recognize(gesture, *event);
        } else {
            if (!event)
                continue;

            action = recognizer->recognize(gesture, *event);
        }

        switch(action) {
            case GestureRecognizer::TriggerGesture:
            case GestureRecognizer::FinishGesture:
            // case GestureRecognizer::Ignore: // XXX
                candidateRecognizer = recognizer;
                candidateAction = action;
                break;

            case GestureRecognizer::CancelGesture:
                m_gestures[recognizer] = NULL;
                m_availableGestures--;
                delete gesture;
                break;
        }
    }

    if (candidateRecognizer && m_availableGestures == 1) {
        Gesture *gesture = NULL;
        if (candidateAction == GestureRecognizer::FinishGesture) {
            gesture = m_gestures[candidateRecognizer];
            m_gestures[candidateRecognizer] = 0;
            m_availableGestures--;
        }

        return gesture;
    }

    return NULL;
}

void GestureManager::createGestures()
{
    if (m_availableGestures != 0)
        return;

    std::list<GestureRecognizer*>::iterator it = m_recognizers.begin();
    for (; it != m_recognizers.end(); ++it) {
        GestureRecognizer *recognizer = *it;
        m_gestures[recognizer] = recognizer->create();
        m_availableGestures++;
    }
}

void GestureManager::registerRecognizer(GestureRecognizer *recognizer)
{
    m_recognizers.push_back(recognizer);
    m_gestures.insert(std::pair<GestureRecognizer *, Gesture *>(recognizer, NULL));
}

void GestureManager::registerKnowRecognizers()
{
    m_panRecognizer = new PanRecognizer;
    m_tapRecognizer = new TapRecognizer;
    registerRecognizer(m_panRecognizer);
    registerRecognizer(m_tapRecognizer);
}
