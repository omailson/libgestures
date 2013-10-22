
#include "gesturemanager.h"
#include "gesturemanager_p.h"
#include "gestures/pan.h"
#include "gestures/tap.h"
#include "gestures/pinch.h"
#include "events/gesturetouchevent.h"

GestureManagerPrivate::GestureManagerPrivate(GestureManager *parent)
    : m_parent(parent)
    , m_availableGestures(0)
    , m_panRecognizer(0)
    , m_tapRecognizer(0)
    , m_pinchRecognizer(0)
{
}

GestureManagerPrivate::~GestureManagerPrivate()
{
    delete m_panRecognizer;
    delete m_tapRecognizer;
    delete m_pinchRecognizer;
}

void GestureManagerPrivate::createGestures()
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

void GestureManagerPrivate::registerKnowRecognizers()
{
    m_panRecognizer = new PanRecognizer;
    m_tapRecognizer = new TapRecognizer;
    m_pinchRecognizer = new PinchRecognizer;
    m_parent->registerRecognizer(m_panRecognizer);
    m_parent->registerRecognizer(m_tapRecognizer);
    m_parent->registerRecognizer(m_pinchRecognizer);
}

GestureManager::GestureManager()
    : d(new GestureManagerPrivate(this))
{
    d->registerKnowRecognizers();
}

GestureManager::~GestureManager()
{
    delete d;
}

Gesture* GestureManager::sendEvent(GestureTouchEvent *event, unsigned int timestamp)
{
    if (!event && d->m_availableGestures == 0)
        return NULL;

    if (d->m_availableGestures == 0)
        d->createGestures();

    GestureRecognizer *candidateRecognizer = 0;
    GestureRecognizer::Action candidateAction = GestureRecognizer::Ignore;

    std::list<GestureRecognizer*>::iterator it = d->m_recognizers.begin();
    for (; it != d->m_recognizers.end(); ++it) {
        GestureRecognizer *recognizer = *it;
        Gesture *gesture = d->m_gestures[recognizer];
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
                d->m_gestures[recognizer] = NULL;
                d->m_availableGestures--;
                delete gesture;
                break;
        }
    }

    if (candidateRecognizer && d->m_availableGestures == 1) {
        Gesture *gesture = d->m_gestures[candidateRecognizer];
        if (candidateAction == GestureRecognizer::FinishGesture) {
            d->m_gestures[candidateRecognizer] = 0;
            d->m_availableGestures--;
        }

        return gesture;
    }

    return NULL;
}

void GestureManager::registerRecognizer(GestureRecognizer *recognizer)
{
    d->m_recognizers.push_back(recognizer);
    d->m_gestures.insert(std::pair<GestureRecognizer *, Gesture *>(recognizer, NULL));
}
