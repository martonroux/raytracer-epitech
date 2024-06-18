/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** loadObjMaterials.h
*/

#ifndef LOADOBJMATERIALS_H
#define LOADOBJMATERIALS_H

#include "../../materials/IMaterial.hpp"

struct Material {
    std::string name;
    std::string map_Kd;
    std::string bump;
    Math::Matrix<1, 3> Ka; // Ambient color
    Math::Matrix<1, 3> Kd; // Diffuse color
    Math::Matrix<1, 3> Ks; // Specular color
    Math::Matrix<1, 3> Ke; // Emissive color
    float Ns = 0.0f; // Specular exponent
    float d = 1.0f;  // Dissolve or transparency
    int illum = 0;   // Illumination model
    float Ni = 1.0f; // Optical density
};

std::map<std::string, std::shared_ptr<ray::IMaterial>> loadObjMaterials(const std::string& filename);

#endif
