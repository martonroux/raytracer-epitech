/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** Camera.cpp
*/

#include "Camera.hpp"

#include <RGB.hpp>

namespace ray {
    Camera::Camera(double width, double height, double fieldOfView) : resolution(width, height), screen(
        Math::Point3D{
            0, 0, 0
        }, Math::Vector3D{width, 0, 0},
        Math::Vector3D{0, height, 0}
        )
    {
        origin = {0, 0, -(fieldOfView * 10)};
    }

    Ray Camera::ray(double u, double v) const
    {
        Math::Point3D pointAt = screen.pointAt(u, v);

        return {origin, Math::Vector3D{pointAt.X - origin.X, pointAt.Y - origin.Y, pointAt.Z - origin.Z}};
    }
} // Raytracer

ray::INode *create(const std::map<std::string, std::string> &attributes)
{
    if (attributes.find("width") == attributes.end() ||
        attributes.find("height") == attributes.end() ||
        attributes.find("fieldOfView") == attributes.end())
        throw ray::NodeError("ICamera: missing width, height or fieldOfView attribute", "Camera.cpp");
    double width = std::stod(attributes.at("width"));
    double height = std::stod(attributes.at("height"));
    double fieldOfView = std::stod(attributes.at("fieldOfView"));

    return new ray::Camera(width, height, fieldOfView);
}
