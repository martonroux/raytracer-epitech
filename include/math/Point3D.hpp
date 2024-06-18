/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** Point3D.hpp
*/

#ifndef POINT3D_HPP
#define POINT3D_HPP
#include "Vector3D.hpp"

namespace Math {

    class Point3D {
    public:
        double X, Y, Z;

        Point3D() : X(0), Y(0), Z(0) {}
        Point3D(double X, double Y, double Z) : X(X), Y(Y), Z(Z) {}

        Point3D(const Point3D &other) = default;
        Point3D(Point3D &&other) = default;

        ~Point3D() = default;

        Point3D& operator=(const Point3D &other) = default;
        Point3D& operator=(Point3D &&other) = default;

        Point3D operator+(const Vector3D& vect) const;
        Point3D operator-(const Vector3D& vect) const;
        Point3D operator-(const Point3D& point) const;

        bool operator==(const Point3D& other) const;
    };

} // Math

#endif //POINT3D_HPP
