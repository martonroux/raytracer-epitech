/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** Skybox.hpp
*/

#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include "../sphere/Sphere.hpp"

namespace ray {

    class Skybox : public Sphere {
    public:
        Skybox() = default;
        ~Skybox() override = default;

        Math::Vector3D getNormale(const Math::Point3D &point, const ray::Ray &camRay) const override;
    };

} // ray

#endif //SKYBOX_HPP
