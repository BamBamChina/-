#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include "vector2.hpp"
#include "RK4.hpp"
#include "difequations.hpp"

// Простой чекер для сбора данных траектории
class TrajectorySaver {
private:
    std::vector<std::pair<double, Vector2>> data;
    double L4_x, L4_y;
    double mu;
    
public:
    TrajectorySaver(double mu) : mu(mu) {
        L4_x = 0.5 - mu;
        L4_y = std::sqrt(3.0) / 2.0;
    }
    
    bool checker(double t, std::pair<Vector2, Vector2> yv) {
        data.push_back({t, yv.first});
        
        double dx = yv.first.getX() - L4_x;
        double dy = yv.first.getY() - L4_y;
        double dist = std::hypot(dx, dy);
        
        // Выводим информацию каждые 5000 шагов
        if (data.size() % 5000 == 0) {
            std::cout << " t=" << t << " dist=" << dist << std::endl;
        }
        
        // Останавливаемся, если улетели далеко (для экономии времени)
        if (std::abs(yv.first.getX()) > 3.0 || std::abs(yv.first.getY()) > 3.0) {
            std::cout << " !!! Trajectory escaped! t=" << t << " pos=(" 
                      << yv.first.getX() << "," << yv.first.getY() << ")" << std::endl;
            return true;
        }
        
        return false;
    }
    
    void saveToCSV(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Cannot open file: " << filename << std::endl;
            return;
        }
        
        file << "t,x,y\n";
        for (const auto& point : data) {
            file << point.first << "," << point.second.getX() << "," << point.second.getY() << "\n";
        }
        file.close();
        std::cout << "Saved to " << filename << " (" << data.size() << " points)" << std::endl;
    }
    
    void clear() { data.clear(); }
    
    double getL4_x() const { return L4_x; }
    double getL4_y() const { return L4_y; }
};

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  ТОЧКИ ЛИБРАЦИИ: ИССЛЕДОВАНИЕ УСТОЙЧИВОСТИ" << std::endl;
    std::cout << "========================================" << std::endl;
    
    // ВЫБЕРИТЕ РЕЖИМ:
    // double mu = 0.01;              // УСТОЙЧИВЫЙ случай
    // double mu = (45 - std::sqrt(1833))/90 ;            // НЕУСТОЙЧИВЫЙ (ω₁=2ω₂)
    double mu = (15 - std::sqrt(213))/30 ;         // НЕУСТОЙЧИВЫЙ (ω₁=3ω₂)  
    // double mu = 0.000953875;       // Реальная система Солнце-Юпитер (устойчива)
    
    // Параметры интегрирования
    double step = 0.001;              // шаг (в нормированных единицах)
    double time_limit = 800.0;        // время симуляции (в периодах вращения)
    
    // Координаты L4
    double L4_x = 0.5 - mu;
    double L4_y = std::sqrt(3.0) / 2.0;
    
    std::cout << "\nПараметры задачи:" << std::endl;
    std::cout << "  μ = " << mu << std::endl;
    std::cout << "  L₄ = (" << L4_x << ", " << L4_y << ")" << std::endl;
    std::cout << "  Шаг = " << step << " (нормированных ед.)" << std::endl;
    std::cout << "  Время симуляции = " << time_limit << " (периодов)" << std::endl;
    
    // Определяем тип устойчивости
    bool is_unstable = (std::abs(mu - 0.0242938) < 1e-6 || std::abs(mu - 0.0135160) < 1e-6);
    std::cout << "  Тип: " << (is_unstable ? "НЕУСТОЙЧИВЫЙ (резонанс)" : "УСТОЙЧИВЫЙ") << std::endl;
    
    // Начальные условия: малое возмущение от L4
    double eps = 1e-2;
    Vector2 position0(L4_x + eps, L4_y);
    Vector2 velocity0(0.0, 0.0);
    
    std::pair<Vector2, Vector2> initial_state(position0, velocity0);
    
    // Интегрирование
    RK4<std::pair<Vector2, Vector2>(*)(std::pair<Vector2, Vector2>, double, double)> 
        integrator(acceleration_rotating, step, mu);
    
    TrajectorySaver saver(mu);
    
    std::cout << "\nНачинаем интегрирование..." << std::endl;
    integrator.integrate(initial_state, saver, time_limit);
    std::cout << "Интегрирование завершено." << std::endl;
    
    // Сохраняем траекторию
    // std::string filename = "trajectory_mu_" + std::to_string(mu) + ".csv";
    std::string filename = "trajectory_mu_0.013516.csv";

    saver.saveToCSV(filename);
    
    // Дополнительно: информация для анализа
    const auto& traj = integrator.get_trajectory();
    if (traj.size() > 1) {
        Vector2 last_pos = traj.back();
        double dx = last_pos.getX() - L4_x;
        double dy = last_pos.getY() - L4_y;
        double final_dist = std::hypot(dx, dy);
        
        std::cout << "\nРезультат:" << std::endl;
        std::cout << "  Количество шагов: " << traj.size() << std::endl;
        std::cout << "  Конечное отклонение от L₄: " << final_dist << std::endl;
        
        if (is_unstable && final_dist > 0.5) {
            std::cout << "  ВЫВОД: Точка либрации НЕУСТОЙЧИВА!" << std::endl;
        } else if (!is_unstable && final_dist < 0.1) {
            std::cout << "  ВЫВОД: Точка либрации УСТОЙЧИВА!" << std::endl;
        }
    }
    
    std::cout << "\nГотово! Запустите Python скрипт для создания GIF." << std::endl;
    
    return 0;
}