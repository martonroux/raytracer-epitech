/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-raytracer-robin.glaude
** File description:
** AShape
*/

#include "AShape.hpp"
#include "../scenes/base/Scene.hpp"

void ray::AShape::applyMatrix()
{
   _transformMatrix = Scene::getTransformationMatrix(*this);
}

void ray::AShape::initValues()
{
    _material = Scene::getMaterial(*this);
    applyMatrix();
}

Math::Vector2D ray::AShape::getUVMapping(__attribute__((unused))Math::Point3D coords) const
{
    return {-1, -1}; // Defaulted to -1. In this case, noise generators will use random positions
}

Math::Point3D ray::AShape::getPosition() const
{
    Math::Point3D position = { _transformMatrix(0, 3),
                 _transformMatrix(1, 3),
                 _transformMatrix(2, 3) };
    return position;
}

Math::Vector3D ray::AShape::getScale() const
{
    Math::Vector3D scale = { _transformMatrix(0, 0),
                 _transformMatrix(1, 1),
                 _transformMatrix(2, 2) };
    return scale;
}

Math::Matrix<3,3> ray::AShape::getRotation() const
{
    Math::Matrix<3,3> rotation;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++)
            rotation(i, j) = _transformMatrix(i, j);
    }
    return rotation;
}

std::shared_ptr<ray::IMaterial> ray::AShape::getMaterial() const
{
    return _material;
}
