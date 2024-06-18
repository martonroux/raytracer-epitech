/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** isBehind.h
*/

#ifndef ISBEHIND_HPP
#define ISBEHIND_HPP

#include "math/Point3D.hpp"
#include "math/Vector3D.hpp"

bool isBehind(Math::Point3D pos, Math::Point3D lightPos, Math::Vector3D lightDir);

#endif
