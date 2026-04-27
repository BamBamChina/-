#pragma once

namespace Constants {
    // Нормированные единицы (G = 1, расстояние между телами = 1, сумма масс = 1)
    inline constexpr double G = 1.0;
    
    // Реальные значения (для масштабирования визуализации, опционально)
    inline constexpr double AU = 1.496e11;      // астрономическая единица (м)
    inline constexpr double RJ = 7.0e7;         // радиус Юпитера (м)
    inline constexpr double RS = 6.96e8;        // радиус Солнца (м)
    inline constexpr double day = 86400.0;      // секунд в сутках
    inline constexpr double year = 365.25 * day;
    
    // Для системы Солнце-Юпитер (реальные значения, нормировка к расстоянию Юпитер-Солнце)
    inline constexpr double rJ_real = 7.78e11;   // расстояние Юпитер-Солнце (м)
    inline constexpr double omegaJ_real = 1.68e-8; // угловая скорость Юпитера (рад/с)
    inline constexpr double muJ_real = 0.000953875; // реальное μ для Юпитера
    
    inline constexpr double PI = 3.141592653589793;
}