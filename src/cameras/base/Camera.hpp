/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** Camera.hpp
*/

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <map>

#include "Ray.hpp"
#include "math/Rectangle3D.hpp"
#include "math/Point3D.hpp"
#include "../ACamera.hpp"

namespace ray {

    class Camera : public ACamera {
    private:
        Math::Point3D origin{0, 0, 0};
        std::pair<unsigned int, unsigned int> resolution;
        Math::Rectangle3D screen;
    public:

        Camera(double width, double height, double fieldOfView);

        [[nodiscard]] ray::Ray ray(double u, double v) const override;
    };

} // Raytracer

extern "C" ray::INode *create(const std::map<std::string, std::string> &attributes);

#endif //CAMERA_HPP
