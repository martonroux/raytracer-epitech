/*
** EPITECH PROJECT, 2024
** raytracer_bootstrap
** File description:
** Vector2D.cpp
*/

#include "math/Vector2D.hpp"

Math::Vector2D Math::Vector2D::operator+(const Vector2D &other) const
{
    return {first + other.first, second + other.second};
}

Math::Vector2D & Math::Vector2D::operator+=(const Vector2D &other)
{
    first += other.first;
    second += other.second;
    return *this;
}

Math::Vector2D Math::Vector2D::operator-(const Vector2D &other) const
{
    return {first - other.first, second - other.second};
}

Math::Vector2D & Math::Vector2D::operator-=(const Vector2D &other)
{
    first -= other.first;
    second -= other.second;
    return *this;
}

Math::Vector2D Math::Vector2D::operator*(const Vector2D &other) const
{
    return {first * other.first, second * other.second};
}

Math::Vector2D Math::Vector2D::operator*(double val) const
{
    return {first * val, second * val};
}

Math::Vector2D & Math::Vector2D::operator*=(const Vector2D &other)
{
    first *= other.first;
    second *= other.second;
    return *this;
}

Math::Vector2D & Math::Vector2D::operator*=(double val)
{
    first *= val;
    second *= val;
    return *this;
}

Math::Vector2D Math::Vector2D::operator/(const Vector2D &other) const
{
    return {first / other.first, second / other.second};
}

Math::Vector2D Math::Vector2D::operator/(double val) const
{
    return {first / val, second / val};
}

Math::Vector2D & Math::Vector2D::operator/=(const Vector2D &other)
{
    first /= other.first;
    second /= other.second;
    return *this;
}

Math::Vector2D & Math::Vector2D::operator/=(double val)
{
    first /= val;
    second /= val;
    return *this;
}
