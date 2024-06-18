/*
** EPITECH PROJECT, 2024
** raytracer_bootstrap
** File description:
** PointLight.hpp
*/

#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include <map>
#include <Maybe.hpp>

#include "../ALight.hpp"

namespace ray {

    class PointLight : public ray::ALight {
        Math::Point3D _myPos;
    public:
        explicit PointLight(RGB color);
        ~PointLight() override = default;

        void initValues() override;

        [[nodiscard]] ray::Ray getIncidentVector(Math::Point3D pos) const override;
        [[nodiscard]] Math::Point3D getPos() const override;
    };

}

extern "C" ray::INode *create(const std::map<std::string, std::string> &attributes);

#endif //POINTLIGHT_HPP
