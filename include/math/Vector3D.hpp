/*
** EPITECH PROJECT, 2024
** bootstrap-raytracer
** File description:
** Vector3D.hpp
*/

#ifndef VECTOR3D_HPP
#define VECTOR3D_HPP
#include <string>

#include "Maybe.hpp"

namespace Math {

    class Point3D;

    class Vector3D {
    public:
        double X;
        double Y;
        double Z;

        // CONSTR DESTRUCT
        Vector3D() : X(0), Y(0), Z(0) {}
        Vector3D(double X, double Y, double Z) : X(X), Y(Y), Z(Z) {}

        Vector3D(const Vector3D& other) = default;
        Vector3D(Vector3D&& other) = default;
        Vector3D(Point3D point);

        ~Vector3D() = default;

        // OPERATORS
        Vector3D& operator=(const Vector3D& other) = default;
        Vector3D& operator=(Vector3D&& other) = default;
        Vector3D& operator=(Point3D point);

        bool operator==(const Vector3D& other) const;
        bool operator!=(const Vector3D& other) const;

        Vector3D operator+(const Vector3D& other) const;
        Vector3D& operator+=(const Vector3D& other);
        Vector3D operator-(const Vector3D& other) const;
        Vector3D& operator-=(const Vector3D& other);
        Vector3D operator*(const Vector3D& other) const;
        Vector3D operator*(double val) const;
        Vector3D& operator*=(const Vector3D& other);
        Vector3D& operator*=(double val);
        Vector3D operator/(const Vector3D& other) const;
        Vector3D operator/(double val) const;
        Vector3D& operator/=(const Vector3D& other);
        Vector3D& operator/=(double val);

        [[nodiscard]] double dot(const Vector3D& other) const;
        [[nodiscard]] Vector3D product(const Vector3D& other) const;
        [[nodiscard]] Vector3D refract(const Vector3D& normale, double n1, double n2) const;

        // GETTERS
        [[nodiscard]] double length() const;
        [[nodiscard]] static Maybe<Vector3D> fromStr(std::string str);
    };

} // Math

#endif //VECTOR3D_HPP
