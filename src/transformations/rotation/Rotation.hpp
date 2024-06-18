/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** Rotation.hpp
*/

#ifndef ROTATION_HPP
#define ROTATION_HPP

#include "../ATransform.hpp"
#include "math/Vector3D.hpp"
//include cos cols etc etc from math
#include <cmath>
#include <map>

namespace ray {

    class Rotation : public ATransform {
    public:
        explicit Rotation(Math::Vector3D rotation);
        ~Rotation() override = default;
    };

} // ray

extern "C" ray::INode *create(const std::map<std::string, std::string> &attributes);

#endif //ROTATION_HPP
