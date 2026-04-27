#pragma once

#include "vector2.hpp"
#include <cmath>

// Ускорения во вращающейся (синодической) системе координат
// mu = m2/(m1+m2) — отношение масс
// Тела: m1 (масса 1-mu) в точке (-mu, 0), m2 (масса mu) в точке (1-mu, 0)
std::pair<Vector2, Vector2> acceleration_rotating(std::pair<Vector2, Vector2> yv, double t, double mu) {
    Vector2 r = yv.first;
    Vector2 v = yv.second;
    
    double x = r.getX();
    double y = r.getY();
    
    double x1 = -mu;           // положение m1
    double x2 = 1.0 - mu;      // положение m2
    
    double r1 = std::hypot(x - x1, y);
    double r2 = std::hypot(x - x2, y);
    
    // Производные эффективного потенциала
    double dUdx = x - (1.0 - mu) * (x - x1) / (r1 * r1 * r1) - mu * (x - x2) / (r2 * r2 * r2);
    double dUdy = y - (1.0 - mu) * y / (r1 * r1 * r1) - mu * y / (r2 * r2 * r2);
    
    // Ускорения в синодической системе (Кориолис + центробежная + гравитация)
    double ax = 2.0 * v.getY() + dUdx;
    double ay = -2.0 * v.getX() + dUdy;
    
    return {v, Vector2(ax, ay)};
}
