/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** RedBasicMaterial.cpp
*/

#include "BasicMaterial.hpp"

ray::BasicMaterial::BasicMaterial(RGB color,
    double refractionIndex,
    double shadowQuality,
    double ambiantOccQuality,
    double roughness) : AMaterial(refractionIndex),
        _kd(Math::Matrix<1, 3>{{{
            static_cast<double>(color.R) / 255,
            static_cast<double>(color.G) / 255,
            static_cast<double>(color.B) / 255
        }}}),
        _phong({},
            0.05,
            50,
            shadowQuality,
            ambiantOccQuality,
            0,
            Math::Matrix<1, 3>{{{roughness, roughness, roughness}}})
{
}

RGB ray::BasicMaterial::getColor(int recursive, Math::Point3D collisionPoint,
                                 Math::Vector3D normale, Math::Point3D camPos,
                                 const std::shared_ptr<ray::IShape>& shape,
                                 const std::shared_ptr<ray::IScene> &scene) const
{
    Math::Vector3D view = {camPos.X - collisionPoint.X, camPos.Y - collisionPoint.Y, camPos.Z - collisionPoint.Z};
    view /= view.length();

    if (recursive > REFLECTION_RECURSION_LIMIT)
        return {
            static_cast<unsigned int>(_kd(0, 0) * 255),
            static_cast<unsigned int>(_kd(0, 1) * 255),
            static_cast<unsigned int>(_kd(0, 2) * 255)
        };
    return _phong.calculateColor(_kd, _ka, scene, shape, view, collisionPoint, normale, recursive);
}

void ray::BasicMaterial::setSkybox()
{
    _phong.setIa(1);
}
