#pragma once

class Vector2 {
private:
    double x;
    double y;

public:
    Vector2();
    Vector2(double x, double y);
    double getX() const;
    double getY() const;
    double norm() const;
    static double scalar_prod(const Vector2 a, const Vector2 b);
    static double vector_prod(const Vector2 a, const Vector2 b);
    Vector2 operator=(const Vector2& other);
    Vector2 operator+(const Vector2& other) const;
    Vector2 operator-(const Vector2& other) const;
    double operator^(const Vector2& other) const;
    Vector2 operator*(const double k);
    Vector2 operator/(const double k);


};

