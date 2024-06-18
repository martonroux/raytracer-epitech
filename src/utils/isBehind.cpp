/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** isBehind.cpp
*/

#include "utils/isBehind.h"

bool isBehind(Math::Point3D pos, Math::Point3D lightPos, Math::Vector3D lightDir)
{
    Math::Vector3D lightToPos = {pos.X - lightPos.X, pos.Y - lightPos.Y, pos.Z - lightPos.Z};
    double angle = lightToPos.dot(lightDir);

    return angle < 0;
}
