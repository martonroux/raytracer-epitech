/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** Rectangle3D.cpp
*/

#include "math/Rectangle3D.hpp"

namespace Math {
    Math::Point3D Rectangle3D::pointAt(double u, double v) const
    {
        Math::Point3D ret;

        u -= 0.5;
        v -= 0.5;

        ret.X = origin.X + bottom_side.X * u;
        ret.Y = origin.Y + left_side.Y * v;
        ret.Z = origin.Z;
        return ret;
    }
} // Raytracer
