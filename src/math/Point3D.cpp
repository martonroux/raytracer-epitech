/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** Point3D.cpp
*/

#include "math/Point3D.hpp"

namespace Math {
    Point3D Point3D::operator+(const Vector3D &vect) const
    {
        return Point3D{X + vect.X, Y + vect.Y, Z + vect.Z};
    }

    Point3D Point3D::operator-(const Point3D &point) const
    {
        return Point3D{X - point.X, Y - point.Y, Z - point.Z};
    }

    Point3D Point3D::operator-(const Vector3D &vect) const
    {
        return Point3D{X - vect.X, Y - vect.Y, Z - vect.Z};
    }

    bool Point3D::operator==(const Point3D &other) const
    {
        return X == other.X && Y == other.Y && Z == other.Z;
    }
} // Math
