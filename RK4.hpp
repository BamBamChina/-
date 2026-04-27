#pragma once

#include "vector2.hpp"
#include <vector>
#include <utility>
#include <functional>
#include <iostream>

template<typename Func>
class RK4 {
private:
    double step;
    Func function;
    double param;  // параметр (например, mu)
    std::vector<Vector2> trajectory;
    std::vector<Vector2> velocities;

public:
    RK4(Func func, double step, double param) 
        : function(func), step(step), param(param) {}

    std::pair<Vector2, Vector2> do_step(std::pair<Vector2, Vector2> yv, double t) {
        Vector2 y = yv.first;
        Vector2 v = yv.second;

        auto k1 = function(yv, t, param);

        auto yv2 = std::make_pair(y + k1.first * (step/2), v + k1.second * (step/2));
        auto k2 = function(yv2, t + step/2, param);

        auto yv3 = std::make_pair(y + k2.first * (step/2), v + k2.second * (step/2));
        auto k3 = function(yv3, t + step/2, param);

        auto yv4 = std::make_pair(y + k3.first * step, v + k3.second * step);
        auto k4 = function(yv4, t + step, param);

        Vector2 y_next = y + (k1.first + k2.first*2 + k3.first*2 + k4.first) * (step/6);
        Vector2 v_next = v + (k1.second + k2.second*2 + k3.second*2 + k4.second) * (step/6);

        return {y_next, v_next};
    }

    template<typename DataSaver>
    void integrate(std::pair<Vector2, Vector2> yv, DataSaver& saver, double time_limit) {
        double t = 0;
        auto cur = yv;
        
        trajectory.clear();
        velocities.clear();
        
        // Сохраняем начальную точку
        trajectory.push_back(cur.first);
        velocities.push_back(cur.second);
        saver.checker(t, cur);
        
        int step_count = 0;
        int max_steps = static_cast<int>(time_limit / step) + 1000;
        
        while (t < time_limit && step_count < max_steps) {
            cur = do_step(cur, t);
            t += step;
            
            trajectory.push_back(cur.first);
            velocities.push_back(cur.second);
            
            bool should_stop = saver.checker(t, cur);
            step_count++;
            
            if (should_stop) {
                std::cout << "Integration stopped at t=" << t << std::endl;
                break;
            }
        }
    }
    
    void clear() {
        trajectory.clear();
        velocities.clear();
    }
    
    const std::vector<Vector2>& get_trajectory() const { return trajectory; }
    const std::vector<Vector2>& get_velocities() const { return velocities; }
};