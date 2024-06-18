/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** Rectangle3D.hpp
*/

#ifndef RECTANGLE3D_HPP
#define RECTANGLE3D_HPP

#include "Point3D.hpp"
#include "Vector3D.hpp"

namespace Math {

    class Rectangle3D {
    public:
        Math::Point3D origin;
        Math::Vector3D bottom_side;
        Math::Vector3D left_side;

        Rectangle3D(Math::Point3D origin, Math::Vector3D bottom_size, Math::Vector3D left_side) : origin(origin), bottom_side(bottom_size), left_side(left_side) {}
        ~Rectangle3D() = default;

        [[nodiscard]] Math::Point3D pointAt(double u, double v) const;
    };

} // Raytracer

#endif //RECTANGLE3D_HPP
