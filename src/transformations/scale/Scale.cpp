/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** Scale.cpp
*/

#include "Scale.hpp"
#include <cmath>


namespace ray {

    Scale::Scale(Math::Vector3D scale)
    {
        _matrix = Math::Matrix<4, 4>{{{scale.X, 0, 0, 0}, {0, scale.Y, 0, 0}, {0, 0, scale.Z, 0}, {0, 0, 0, 1}}};
    }

} // ray

ray::INode *create(const std::map<std::string, std::string> &attributes)
{
    if (attributes.find("scale") == attributes.end())
        throw ray::NodeError("ITransform: missing scale attribute", "Scale.cpp");
    Maybe<Math::Vector3D> scale = Math::Vector3D::fromStr(attributes.at("scale"));
    if (!scale.has_value())
        throw ray::NodeError("ITransform: invalid scale attribute", "Scale.cpp");
    return new ray::Scale(scale.value());
}
