/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** GlassMaterial.cpp
*/

#include "GlassMaterial.hpp"

namespace ray {
    GlassMaterial::GlassMaterial(RGB color, double transparency) : AMaterial(1.5),
    _kd(Math::Matrix<1, 3>{{
        {
            static_cast<double>(color.R) / 255,
            static_cast<double>(color.G) / 255,
            static_cast<double>(color.B) / 255
        }}}),
    _phong({},
        0.05,
        50,
        1,
        0,
        transparency,
        Math::Matrix<1, 3>{{{1, 1, 1}}})
    {
    }

    RGB GlassMaterial::getColor(int recursion, Math::Point3D collisionPoint,
        Math::Vector3D normale, Math::Point3D camPos,
        const std::shared_ptr<ray::IShape> &shape,
        const std::shared_ptr<ray::IScene> &scene) const
    {
        Math::Vector3D view = {camPos.X - collisionPoint.X, camPos.Y - collisionPoint.Y, camPos.Z - collisionPoint.Z};
        view /= view.length();

        if (recursion > REFLECTION_RECURSION_LIMIT)
            return {
                static_cast<unsigned int>(_kd(0, 0) * 255),
                static_cast<unsigned int>(_kd(0, 1) * 255),
                static_cast<unsigned int>(_kd(0, 2) * 255)
            };
        return _phong.calculateColor(_kd, _ka, scene, shape, view, collisionPoint, normale, recursion);
    }

    void GlassMaterial::setSkybox()
    {
        _phong.setIa(1);
    }
} // ray

extern "C" ray::INode *create(const std::map<std::string, std::string> &attributes)
{
    if (attributes.find("color") == attributes.end())
        throw ray::NodeError("IMaterial: missing color attribute", "GlassMaterial.cpp");
    if (attributes.find("transparency") == attributes.end())
        throw ray::NodeError("IMaterial: missing transparency attribute (number between 0 and 1 for transparency)", "GlassMaterial.cpp");

    Maybe<RGB> color = RGB::fromStr(attributes.at("color"));
    double transparency = std::stod(attributes.at("transparency"));

    if (!color.has_value())
        throw ray::NodeError("IMaterial: invalid color attribute", "GlassMaterial.cpp");
    if (transparency < 0 || transparency > 1)
        throw ray::NodeError("IMaterial: transparency must be a number between 1 and 100", "GlassMaterial.cpp");

    ray::INode* node = new ray::GlassMaterial(color.value(), transparency);

    if (attributes.find("skybox") != attributes.end() && attributes.at("skybox") == "true")
        dynamic_cast<ray::GlassMaterial*>(node)->setSkybox();
    return node;
}
