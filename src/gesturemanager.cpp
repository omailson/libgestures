
#include "gesturemanager.h"
#include "gesturemanager_p.h"
#include "events/gestureeventfilter_p.h"
#include "gestures/pan.h"
#include "gestures/tap.h"
#include "gestures/pinch.h"
#include "gestures/doubletap.h"
#include "gestures/longpress.h"
#include "events/gesturetouchevent.h"

GestureManagerPrivate::GestureManagerPrivate(GestureManager *parent)
    : m_parent(parent)
    , m_availableGestures(0)
    , m_moveEventFilter(new GestureMoveEventFilter)
    , m_triggeredGestureRecognizer(0)
{
}

GestureManagerPrivate::~GestureManagerPrivate()
{
    std::list<GestureRecognizer*>::iterator it = m_recognizers.begin();
    for (; it != m_recognizers.end(); ++it) {
        GestureRecognizer *recognizer = *it;
        delete m_gestures[recognizer];
        delete recognizer;
    }

    removeUnusedGestures();
    delete m_moveEventFilter;
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

void GestureManagerPrivate::removeUnusedGestures()
{
    std::list<Gesture*>::iterator it = m_gesturesToRemove.begin();
    for (; it != m_gesturesToRemove.end(); ++it)
        delete *it;

    m_gesturesToRemove.clear();
}

Gesture* GestureManagerPrivate::handleTriggeredGesture(GestureTouchEvent *event, long long int timestamp)
{
    m_moveEventFilter->filter(event);

    GestureRecognizer *recognizer = m_triggeredGestureRecognizer;
    GestureRecognizer::Action action;
    Gesture *gesture = m_gestures[recognizer];
    if (recognizer->useTimer() && timestamp > 0) {
        action = recognizer->recognize(gesture, timestamp);
        if (event && action != GestureRecognizer::CancelGesture)
            action = recognizer->recognize(gesture, *event);
    } else {
        if (!event)
            return NULL;

        action = recognizer->recognize(gesture, *event);
    }

    if (action == GestureRecognizer::FinishGesture
            || action == GestureRecognizer::CancelGesture) {
        m_gestures[recognizer] = 0;
        m_triggeredGestureRecognizer = 0;
        m_gesturesToRemove.push_back(gesture);
    }

    return gesture;
}

GestureManager::GestureManager()
    : d(new GestureManagerPrivate(this))
{
}

GestureManager::~GestureManager()
{
    delete d;
}

Gesture* GestureManager::sendEvent(GestureTouchEvent *event, long long int timestamp)
{
    d->removeUnusedGestures();

    if (d->m_triggeredGestureRecognizer)
        return d->handleTriggeredGesture(event, timestamp);

    if (!event && d->m_availableGestures == 0)
        return NULL;

    if (!event && timestamp <= 0)
        return NULL;

    if (d->m_availableGestures == 0)
        d->createGestures();

    d->m_moveEventFilter->filter(event);

    std::list<GestureRecognizer*>::iterator it = d->m_recognizers.begin();
    for (; it != d->m_recognizers.end(); ++it) {
        GestureRecognizer *recognizer = *it;
        Gesture *gesture = d->m_gestures[recognizer];
        if (gesture == NULL)
            continue;

        GestureRecognizer::Action action;
        if (recognizer->useTimer() && timestamp > 0) {
            action = recognizer->recognize(gesture, timestamp);
            if (event && action != GestureRecognizer::CancelGesture)
                action = recognizer->recognize(gesture, *event);
        } else {
            if (!event)
                continue;

            action = recognizer->recognize(gesture, *event);
        }

        switch(action) {
            case GestureRecognizer::Ignore:
            case GestureRecognizer::MayBeGesture:
                break;

            case GestureRecognizer::TriggerGesture:
            {
                std::map<GestureRecognizer*, Gesture*>::iterator it = d->m_gestures.begin();
                for (; it != d->m_gestures.end(); ++it) {
                    if (it->second != gesture) {
                        delete it->second;
                        d->m_gestures[it->first] = 0;
                    }
                }
                d->m_acceptedGestures.clear();
                d->m_availableGestures = 0;
                d->m_triggeredGestureRecognizer = 0;

                d->m_triggeredGestureRecognizer = recognizer;
                return gesture;
                break;
            }

            case GestureRecognizer::FinishGesture:
                d->m_acceptedGestures[recognizer] = action;
                break;

            case GestureRecognizer::CancelGesture:
                std::map<GestureRecognizer*, GestureRecognizer::Action>::iterator acceptedGesture;
                acceptedGesture = d->m_acceptedGestures.find(recognizer);
                if (acceptedGesture != d->m_acceptedGestures.end())
                    d->m_acceptedGestures.erase(acceptedGesture);

                d->m_gestures[recognizer] = NULL;
                d->m_availableGestures--;
                delete gesture;
                break;
        }
    }

    if (d->m_availableGestures == 1) {
        std::map<GestureRecognizer*, GestureRecognizer::Action>::iterator acceptedGesture;
        acceptedGesture = d->m_acceptedGestures.begin();
        if (acceptedGesture == d->m_acceptedGestures.end())
            return NULL;

        GestureRecognizer *recognizer = acceptedGesture->first;
        GestureRecognizer::Action action = acceptedGesture->second;
        Gesture *gesture = d->m_gestures[recognizer];
        if (action == GestureRecognizer::FinishGesture) {
            d->m_gestures[recognizer] = 0;
            d->m_availableGestures--;
            d->m_acceptedGestures.clear();
            d->m_gesturesToRemove.push_back(gesture);
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

void GestureManager::setMoveThreshold(int moveThreshold)
{
    d->m_moveEventFilter->setMoveThreshold(moveThreshold);
}
