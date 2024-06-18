/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** Scale.hpp
*/

#ifndef SCALE_HPP
#define SCALE_HPP

#include <map>

#include "../ATransform.hpp"
#include "math/Vector3D.hpp"

namespace ray {

    class Scale : public ATransform {
    public:
        explicit Scale(Math::Vector3D scale);
        ~Scale() override = default;
    };

} // ray

extern "C" ray::INode *create(const std::map<std::string, std::string> &attributes);

#endif //SCALE_HPP
