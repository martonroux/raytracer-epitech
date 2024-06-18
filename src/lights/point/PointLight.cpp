/*
** EPITECH PROJECT, 2024
** raytracer_bootstrap
** File description:
** PointLight.cpp
*/

#include "PointLight.hpp"
#include "../../scenes/base/Scene.hpp"

ray::PointLight::PointLight(RGB color) : ALight(color)
{
}

void ray::PointLight::initValues()
{
    _myPos = Scene::getPosition(*this);
}

ray::Ray ray::PointLight::getIncidentVector(Math::Point3D pos) const
{
    Math::Vector3D res = {_myPos.X - pos.X, _myPos.Y - pos.Y, _myPos.Z - pos.Z};

    res /= res.length();
    return {_myPos, res};
}

Math::Point3D ray::PointLight::getPos() const
{
    return _myPos;
}

extern "C" ray::INode *create(const std::map<std::string, std::string> &attributes)
{
    if (attributes.find("color") == attributes.end())
        throw ray::NodeError("ILight: missing color attribute", "PointLight.cpp");
    Maybe<RGB> color = RGB::fromStr(attributes.at("color"));
    if (!color.has_value())
        throw ray::NodeError("ILight: invalid color attribute", "PointLight.cpp");
    return new ray::PointLight(color.value());
}
