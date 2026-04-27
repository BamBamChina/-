cmake_minimum_required(VERSION 3.10)
project(LagrangePoints)

set(CMAKE_CXX_STANDARD 17)

# Исходные файлы
set(SOURCES
    main.cpp
    vector2.cpp
)

set(HEADERS
    vector2.hpp
    RK4.hpp
    difequations.hpp
    constants.hpp
)

add_executable(lagrange ${SOURCES} ${HEADERS})

target_include_directories(lagrange PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})