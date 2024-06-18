/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** Translation.cpp
*/

#include "Translation.hpp"

namespace ray {

    Translation::Translation(Math::Vector3D translation)
    {
        Math::Matrix<4, 4> translationMatrix = getIdentityMatrix();

        translationMatrix(0, 3) = translation.X;
        translationMatrix(1, 3) = translation.Y;
        translationMatrix(2, 3) = translation.Z;

        _matrix = translationMatrix * _matrix;
    }

}

ray::INode *create(const std::map<std::string, std::string> &attributes)
{
    if (attributes.find("translation") == attributes.end())
        throw ray::NodeError("ITransform: missing translation attribute", "Translation.cpp");
    Maybe<Math::Vector3D> translation = Math::Vector3D::fromStr(attributes.at("translation"));
    if (!translation.has_value())
        throw ray::NodeError("ITransform: invalid translation attribute", "Translation.cpp");
    return new ray::Translation(translation.value());
}
