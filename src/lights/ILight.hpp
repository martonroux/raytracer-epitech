/*
** EPITECH PROJECT, 2024
** raytracer_bootstrap
** File description:
** ILight.hpp
*/

#ifndef ILIGHT_HPP
#define ILIGHT_HPP

#include "INode.hpp"
#include "Ray.hpp"
#include "math/Vector3D.hpp"
#include "RGB.hpp"
#include "math/Point3D.hpp"

namespace ray {

    class ILight : virtual public INode {
    public:
        virtual ~ILight() = default;

        [[nodiscard]] virtual RGB getColor() const = 0;
        [[nodiscard]] virtual ray::Ray getIncidentVector(Math::Point3D pos) const = 0;
        [[nodiscard]] virtual Math::Point3D getPos() const = 0;
    };

}

#endif
