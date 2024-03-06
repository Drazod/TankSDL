#pragma once
#include <cmath>

class Vector2D
{
public:
    Vector2D(float setX, float setY) : x(setX), y(setY) {}
    Vector2D(const Vector2D& other) : x(other.x), y(other.y) {}
    Vector2D(float angleRad) : x(cos(angleRad)), y(sin(angleRad)) {}
    Vector2D() : x(0.0f), y(0.0f) {}

    float angle() const { return atan2(y, x); }

    float magnitude() const { return sqrt(x * x + y * y); }
    Vector2D normalize();
    Vector2D getNegativeReciprocal() const { return Vector2D(-y, x); }

    float dot(const Vector2D& other) const { return x * other.x + y * other.y; }
    float cross(const Vector2D& other) const { return x * other.y - y * other.x; }
    float angleBetween(const Vector2D& other) const { return atan2(cross(other), dot(other)); }

    // Addition operator overloads
    Vector2D operator+(const Vector2D& other) const { return Vector2D(x + other.x, y + other.y); }
    Vector2D& operator+=(const Vector2D& other) { x += other.x; y += other.y; return *this; }

    // Subtraction operator overloads
    Vector2D operator-(const Vector2D& other) const { return Vector2D(x - other.x, y - other.y); }
    Vector2D& operator-=(const Vector2D& other) { x -= other.x; y -= other.y; return *this; }

    // Multiplication by scalar operator overloads
    Vector2D operator*(float scalar) const { return Vector2D(x * scalar, y * scalar); }
    Vector2D& operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }

    // Division by scalar operator overloads
    Vector2D operator/(float scalar) const { return Vector2D(x / scalar, y / scalar); }
    Vector2D& operator/=(float scalar) { x /= scalar; y /= scalar; return *this; }

    float x, y;
};
