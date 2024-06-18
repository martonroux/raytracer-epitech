/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** Shear.hpp
*/

#ifndef SHEAR_HPP
#define SHEAR_HPP

#include <map>

#include "../ATransform.hpp"
#include "math/Vector3D.hpp"

namespace ray {

    enum class ShearType {
        XAxis = 0,
        YAxis = 1,
        ZAxis = 2
    };

    class Shear : public ATransform {
    public:
        explicit Shear(ShearType type, Math::Vector3D shear);
        ~Shear() override = default;
    };

} // ray

extern "C" ray::INode *create(const std::map<std::string, std::string> &attributes);

#endif //SHEAR_HPP
