/*
** EPITECH PROJECT, 2024
** raytracer_bootstrap
** File description:
** Vector2D.hpp
*/

#ifndef VECTOR2D_HPP
#define VECTOR2D_HPP

#include <utility>

namespace Math {

    class Vector2D : public std::pair<double, double> {
    public:
        // CONSTR DESTRUCT
        Vector2D(double X, double Y) : std::pair<double, double>(X, Y) {}
        explicit Vector2D(std::pair<double, double> pair) : std::pair<double, double>(pair) {}

        ~Vector2D() = default;

        Vector2D operator+(const Vector2D& other) const;
        Vector2D& operator+=(const Vector2D& other);
        Vector2D operator-(const Vector2D& other) const;
        Vector2D& operator-=(const Vector2D& other);
        Vector2D operator*(const Vector2D& other) const;
        Vector2D operator*(double val) const;
        Vector2D& operator*=(const Vector2D& other);
        Vector2D& operator*=(double val);
        Vector2D operator/(const Vector2D& other) const;
        Vector2D operator/(double val) const;
        Vector2D& operator/=(const Vector2D& other);
        Vector2D& operator/=(double val);

        [[nodiscard]] double dot(const Vector2D& other) const;

        // GETTERS
        [[nodiscard]] double length() const;
    };

}

#endif //VECTOR2D_HPP
