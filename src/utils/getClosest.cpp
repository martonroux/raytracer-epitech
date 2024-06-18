/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** getClosest.cpp
*/

#include "utils/getClosest.h"

Math::Point3D getClosest(std::vector<Math::Point3D> points, Math::Point3D origin)
{
    Math::Point3D closest = points[0];

    for (Math::Point3D point : points) {
        if (Math::Vector3D{point.X - origin.X, point.Y - origin.Y, point.Z - origin.Z}.length() < Math::Vector3D{closest.X - origin.X, closest.Y - origin.Y, closest.Z - origin.Z}.length())
            closest = point;
    }
    return closest;
}
