/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** Ray.hpp
*/

#ifndef RAY_HPP
#define RAY_HPP

#include "math/Point3D.hpp"
#include "math/Vector3D.hpp"

namespace ray {

    class Ray {
    public:
        Math::Point3D origin;
        Math::Vector3D direction;

        Ray() = default;
        Ray(Math::Point3D origin, Math::Vector3D direction) : origin(origin), direction(direction) {}
        Ray(const Ray &other) = default;
        Ray(Ray&& other) = default;
        ~Ray() = default;

        Ray& operator=(const Ray& other) = default;
        Ray& operator=(Ray&& other) = default;
    };

} // Raytracer

#endif //RAY_HPP
