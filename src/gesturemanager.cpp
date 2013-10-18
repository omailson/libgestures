
#include "gesturemanager.h"
#include "gestures/pan.h"

GestureManager::GestureManager()
{
    registerKnowRecognizers();
}

GestureManager::~GestureManager()
{
}

void GestureManager::registerRecognizer(GestureRecognizer *recognizer)
{
}

void GestureManager::registerKnowRecognizers()
{
    PanRecognizer panRecognizer;
    registerRecognizer((GestureRecognizer *)&panRecognizer);
}
