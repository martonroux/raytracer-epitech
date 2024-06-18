/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** createBasicMaterial.cpp
*/

#include "BasicMaterial.hpp"

extern "C" ray::INode *create(const std::map<std::string, std::string> &attributes)
{
    if (attributes.find("color") == attributes.end())
        throw ray::NodeError("IMaterial: missing color attribute", "BasicMaterial.cpp");
    if (attributes.find("shadow_quality") == attributes.end())
        throw ray::NodeError("IMaterial: missing shadow_quality attribute (number between 0 and 100 for render time)", "BasicMaterial.cpp");
    if (attributes.find("ambiant_occlusion") == attributes.end())
        throw ray::NodeError("IMaterial: missing ambiant_occlusion attribute (number between 0 and 100 for render time)", "BasicMaterial.cpp");
    if (attributes.find("roughness") == attributes.end())
        throw ray::NodeError("IMaterial: missing roughness attribute (number between 0 and 1 for material roughness)", "BasicMaterial.cpp");

    Maybe<RGB> color = RGB::fromStr(attributes.at("color"));
    double shadowQuality = std::stod(attributes.at("shadow_quality"));
    double ambiantOcclusion = std::stod(attributes.at("ambiant_occlusion"));
    double roughness = std::stod(attributes.at("roughness"));

    if (!color.has_value())
        throw ray::NodeError("IMaterial: invalid color attribute", "BasicMaterial.cpp");
    if (shadowQuality < 1 || shadowQuality > 100)
        throw ray::NodeError("IMaterial: shadow_quality must be a number between 1 and 100", "BasicMaterial.cpp");
    if (ambiantOcclusion < 0 || ambiantOcclusion > 100)
        throw ray::NodeError("IMaterial: ambiant_occlusion must be a number between 0 and 100", "BasicMaterial.cpp");
    if (roughness < 0 || roughness > 1)
        throw ray::NodeError("IMaterial: roughness must be a number between 0 and 1", "BasicMaterial.cpp");

    ray::INode* node = new ray::BasicMaterial(color.value(), 1, shadowQuality, ambiantOcclusion, roughness);

    if (attributes.find("skybox") != attributes.end() && attributes.at("skybox") == "true")
        dynamic_cast<ray::BasicMaterial*>(node)->setSkybox();
    return node;
}
