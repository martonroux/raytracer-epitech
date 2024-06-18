/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** ICamera.hpp
*/

#ifndef ICAMERA_HPP
#define ICAMERA_HPP

#include "INode.hpp"
#include "Ray.hpp"

namespace ray {

    class ICamera : virtual public ray::INode {
    public:
        virtual ~ICamera() = default;

        [[nodiscard]] virtual ray::Ray ray(double u, double v) const = 0;
    };

}


#endif
