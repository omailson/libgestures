
#ifndef SCALAR_H
#define SCALAR_H

#include <cmath>

static inline bool fuzzyCompare(float a, float b)
{
    return (abs(a - b) <= 0.00001f * fmin(abs(a), abs(b)));
}

static inline bool fuzzyCompare(double a, double b)
{
    return (abs(a - b) <= 0.000000000001 * fmin(abs(a), abs(b)));
}

#endif
