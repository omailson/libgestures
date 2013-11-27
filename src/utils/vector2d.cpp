
#include "scalar.h"
#include "vector2d.h"
#include <math.h>

Vector2D::Vector2D()
    : m_x(0)
    , m_y(0)
{
}

Vector2D::Vector2D(float x, float y)
{
    m_x = x;
    m_y = y;
}

Vector2D & Vector2D::operator+= (const Vector2D &other)
{
    setX(x() + other.x());
    setY(y() + other.y());
    return *this;
}

Vector2D & Vector2D::operator-= (const Vector2D &other)
{
    setX(x() - other.x());
    setY(y() - other.y());
    return *this;
}

Vector2D Vector2D::operator+ (const Vector2D &other)
{
    return Vector2D(x() + other.x(), y() + other.y());
}

Vector2D Vector2D::operator- (const Vector2D &other) const
{
    return Vector2D(x() - other.x(), y() - other.y());
}

inline Vector2D operator* (float scalar, const Vector2D &vector)
{
    return Vector2D(vector.x() * scalar, vector.y() * scalar);
}

inline Vector2D Vector2D::operator* (float scalar)
{
    return Vector2D(x() * scalar, y() * scalar);
}

inline Vector2D Vector2D::operator/ (float scalar)
{
    return Vector2D(x() / scalar, y() / scalar);
}

bool Vector2D::operator== (const Vector2D &other)
{
    return fuzzyCompare(m_x, other.m_x) && fuzzyCompare(m_y, other.m_y);
}

void Vector2D::rotate90()
{
    float x = m_x;
    m_x = -m_y;
    m_y = x;
}

void Vector2D::rotate90Clockwise()
{
    float x = m_x;
    m_x = m_y;
    m_y = -x;
}

void Vector2D::normalize()
{
    double length = double(m_x) * double(m_x) +
                    double(m_y) * double(m_y);

    if (fuzzyCompare(length, 1.0) || fuzzyCompare(length + 1.0, 1.0))
        return;

    length = sqrt(length);

    m_x /= length;
    m_y /= length;
}

bool Vector2D::isNull() const
{
    return fuzzyCompare(m_x + 1, 1) && fuzzyCompare(m_y + 1, 1);
}

float Vector2D::length() const
{
    return sqrt(lengthSquared());
}

float Vector2D::lengthSquared() const
{
    return x() * x() + y() * y();
}

float Vector2D::dotProduct(const Vector2D &vec1, const Vector2D &vec2)
{
    return vec1.x() * vec2.x() + vec1.y() * vec2.y();
}

Vector2D Vector2D::project(const Vector2D &a, const Vector2D &b)
{
    return Vector2D::dotProduct(a, b) * b / b.lengthSquared();
}

Vector2D Vector2D::reflect(const Vector2D &v, const Vector2D &axis)
{
    Vector2D p = Vector2D::project(v, axis);
    Vector2D n = v - p;
    return v - 2 * n;
}

float Vector2D::cos(const Vector2D &vec1, const Vector2D &vec2)
{
    return Vector2D::dotProduct(vec1, vec2) / (vec1.length() * vec2.length());
}
