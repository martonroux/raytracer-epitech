/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** Skybox.cpp
*/

#include "Skybox.hpp"

namespace ray {
    Math::Vector3D Skybox::getNormale(const Math::Point3D &point,
        const ray::Ray &camRay) const
    {
        return Sphere::getNormale(point, camRay) * -1;
    }

} // ray
