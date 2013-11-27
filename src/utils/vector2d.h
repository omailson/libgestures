#ifndef VECTOR2D_H
#define VECTOR2D_H


class Vector2D
{
public:
    Vector2D();
    Vector2D(float x, float y);

    float x() const { return m_x; };
    void setX(float x) { m_x = x; };

    float y() const { return m_y; };
    void setY(float y) { m_y = y; };

    Vector2D & operator+= (const Vector2D &other);
    Vector2D operator+ (const Vector2D &other);

    Vector2D & operator-= (const Vector2D &other);
    Vector2D operator- (const Vector2D &other) const;

    Vector2D operator* (float scalar);
    Vector2D operator/ (float scalar);

    bool operator== (const Vector2D &other);

    void rotate90();
    void rotate90Clockwise();
    void normalize();
    bool isNull() const;

    float length() const;
    float lengthSquared() const;

    static float dotProduct(const Vector2D &vec1, const Vector2D &vec2);
    static Vector2D project(const Vector2D &a, const Vector2D &b);
    static Vector2D reflect(const Vector2D &a, const Vector2D &b);

    static float cos(const Vector2D &vec1, const Vector2D &vec2);

private:
    float m_x;
    float m_y;
};

#endif
