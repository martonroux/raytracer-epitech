/*
** EPITECH PROJECT, 2024
** bootstrap-raytracer
** File description:
** Vector3D.cpp
*/

#include "math/Vector3D.hpp"
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include "math/Point3D.hpp"

namespace Math {
    Vector3D::Vector3D(Point3D point)
    {
        X = point.X;
        Y = point.Y;
        Z = point.Z;
    }

    Vector3D & Vector3D::operator=(Math::Point3D point)
    {
        this->X = point.X;
        this->Y = point.Y;
        this->Z = point.Z;
        return *this;
    }

    bool Vector3D::operator==(const Vector3D &other) const
    {
        return X == other.X && Y == other.Y && Z == other.Z;
    }

    bool Vector3D::operator!=(const Vector3D &other) const
    {
        return X != other.X || Y != other.Y || Z != other.Z;
    }

    Vector3D Vector3D::operator+(const Vector3D &other) const
    {
        return Vector3D{X + other.X, Y + other.Y, Z + other.Z};
    }

    Vector3D & Vector3D::operator+=(const Vector3D &other)
    {
        X += other.X;
        Y += other.Y;
        Z += other.Z;
        return *this;
    }

    Vector3D Vector3D::operator-(const Vector3D &other) const
    {
        return Vector3D{X - other.X, Y - other.Y, Z - other.Z};
    }

    Vector3D & Vector3D::operator-=(const Vector3D &other)
    {
        X -= other.X;
        Y -= other.Y;
        Z -= other.Z;
        return *this;
    }

    Vector3D Vector3D::operator*(const Vector3D &other) const
    {
        return Vector3D{X * other.X, Y * other.Y, Z * other.Z};
    }

    Vector3D Vector3D::operator*(double val) const
    {
        return Vector3D{X * val, Y * val, Z * val};
    }

    Vector3D & Vector3D::operator*=(const Vector3D &other)
    {
        X *= other.X;
        Y *= other.Y;
        Z *= other.Z;
        return *this;
    }

    Vector3D & Vector3D::operator*=(double val)
    {
        X *= val;
        Y *= val;
        Z *= val;
        return *this;
    }

    Vector3D Vector3D::operator/(const Vector3D &other) const
    {
        return Vector3D{X / other.X, Y / other.Y, Z / other.Z};
    }

    Vector3D Vector3D::operator/(double val) const
    {
        return Vector3D{X / val, Y / val, Z / val};
    }

    Vector3D & Vector3D::operator/=(const Vector3D &other)
    {
        X /= other.X;
        Y /= other.Y;
        Z /= other.Z;
        return *this;
    }

    Vector3D & Vector3D::operator/=(double val)
    {
        X /= val;
        Y /= val;
        Z /= val;
        return *this;
    }

    double Vector3D::dot(const Vector3D &other) const
    {
        return X * other.X + Y * other.Y + Z * other.Z;
    }

    Vector3D Vector3D::product(const Vector3D &other) const
    {
        return Vector3D{Y * other.Z - Z * other.Y, Z * other.X - X * other.Z, X * other.Y - Y * other.X};
    }

    Vector3D Vector3D::refract(const Vector3D &normale, double n1,
        double n2) const
    {
        const double n = n1 / n2;
        const double cosI = -normale.dot(*this);
        const double sinT2 = n * n * (1.0 - cosI * cosI);
        if (sinT2 > 1.0) return {0, 0, 0};

        const double cosT = sqrt(1.0 - sinT2);
        return *this * n + normale * (n * cosI - cosT);
    }

    double Vector3D::length() const
    {
        return std::sqrt(X * X + Y * Y + Z * Z);
    }

    Maybe<double> parseSingleAttr(char attrName, std::string str)
    {
        double res;

        if (str[0] != attrName)
            return Maybe<double>{};
        if (str.find(':') == std::string::npos)
            return Maybe<double>{};
        str.erase(0, str.find(':') + 1);

        try {
            res = std::stod(str);
        } catch (std::invalid_argument&) {
            return Maybe<double>{};
        }
        return Maybe{res};
    }

    Maybe<Vector3D> Vector3D::fromStr(std::string str)
    {
        if (str[0] != '{' || str[str.size() - 1] != '}')
            return Maybe<Vector3D>{};
        str.erase(0, 1);
        str.erase(str.size() - 1, 1);
        str.erase(std::remove(str.begin(), str.end(), ' '), str.end());

        Maybe<double> maybeNum1 = parseSingleAttr('x', str);
        if (!maybeNum1.has_value())
            return Maybe<Vector3D>{};

        str.erase(0, str.find(';') + 1);
        Maybe<double> maybeNum2 = parseSingleAttr('y', str);
        if (!maybeNum2.has_value())
            return Maybe<Vector3D>{};

        str.erase(0, str.find(';') + 1);
        Maybe<double> maybeNum3 = parseSingleAttr('z', str);
        if (!maybeNum3.has_value())
            return Maybe<Vector3D>{};

        return Maybe{Vector3D{maybeNum1.value(), maybeNum2.value(), maybeNum3.value()}};
    }

} // Math
