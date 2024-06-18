/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** DirectionalLight.cpp
*/

#include "DirectionalLight.hpp"

#include "Ray.hpp"

namespace ray {
    DirectionalLight::DirectionalLight(RGB color, Math::Vector3D direction) : ALight(color),
        _direction(direction / direction.length())
    {
    }

    ray::Ray DirectionalLight::getIncidentVector(Math::Point3D pos) const
    {
        Math::Point3D pointAt = pos + _direction * -9999;
        return {pointAt, _direction * -1};
    }

    Math::Point3D DirectionalLight::getPos() const
    {
        return {0, 0, 0};
    }
} // ray

extern "C" ray::INode *create(const std::map<std::string, std::string> &attributes)
{
    if (attributes.find("color") == attributes.end())
        throw ray::NodeError("ILight: missing color attribute", "DirectionalLight.cpp");
    if (attributes.find("direction") == attributes.end())
        throw ray::NodeError("ILight: missing direction attribute", "DirectionalLight.cpp");

    Maybe<RGB> color = RGB::fromStr(attributes.at("color"));
    Maybe<Math::Vector3D> direction = Math::Vector3D::fromStr(attributes.at("direction"));

    if (!color.has_value())
        throw ray::NodeError("ILight: invalid color attribute", "DirectionalLight.cpp");
    if (!direction.has_value())
        throw ray::NodeError("ILight: invalid direction attribute", "DirectionalLight.cpp");
    return new ray::DirectionalLight(color.value(), direction.value());
}
