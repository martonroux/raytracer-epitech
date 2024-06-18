/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** ATransform.cpp
*/

#include "ATransform.hpp"

Math::Matrix<4, 4> ray::ATransform::getIdentityMatrix()
{
    return Math::Matrix<4, 4>{{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
}

ray::ATransform::ATransform() : ANode(ray::type::TRANSFORM)
{
    _matrix = getIdentityMatrix();
}
