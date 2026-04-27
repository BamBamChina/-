#include "vector2.hpp"
#include <iostream>
#include <cmath>

Vector2::Vector2() : x(0), y(0) {}
Vector2::Vector2(double x, double y) : x(x), y(y) {}


double Vector2::getX() const {
    return x;
}

double Vector2::getY() const {
    return y;
}

double Vector2::norm() const {
    // return vector norm 
    return std::sqrt(x * x + y * y);
}

Vector2 Vector2::operator=(const Vector2& other) {
    x = other.x;
    y = other.y;
    return *this;
}


double Vector2::operator^(const Vector2& other) const {
    // scalar production
    return x * other.x + y * other.y;
}

Vector2 Vector2::operator+(const Vector2& other) const {
    return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-(const Vector2& other) const {
    return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator*(const double k) {
    // production vector and number
    Vector2 sum(x * k, y * k);
    return sum;
}


Vector2 Vector2::operator/(const double k) {
    // frac vector and number
    Vector2 sum(x / k, y / k);
    return sum;
}

double Vector2::scalar_prod(const Vector2 a, const Vector2 b) {
    // scalar production
    return a ^ b;
}

double Vector2::vector_prod(const Vector2 a, const Vector2 b) {
    // vector production (2d variant)
    return a.getX() * b.getY() - a.getY() * b.getX();
}



