/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** createSuperMaterial.cpp
*/

#include "SuperMaterial.hpp"

extern "C" ray::INode *create(const std::map<std::string, std::string> &attributes)
{
    if (attributes.find("specular_exponent") == attributes.end())
        throw ray::NodeError("IMaterial: missing specular_exponent attribute", "SuperMaterial.cpp");
    if (attributes.find("ka") == attributes.end())
        throw ray::NodeError("IMaterial: missing ka attribute", "SuperMaterial.cpp");
    if (attributes.find("ks") == attributes.end())
        throw ray::NodeError("IMaterial: missing ks attribute", "SuperMaterial.cpp");
    if (attributes.find("refraction_index") == attributes.end())
        throw ray::NodeError("IMaterial: missing refraction_index attribute", "SuperMaterial.cpp");
    if (attributes.find("transparency") == attributes.end())
        throw ray::NodeError("IMaterial: missing transparency attribute", "SuperMaterial.cpp");
    if (attributes.find("shadow_quality") == attributes.end())
        throw ray::NodeError("IMaterial: missing shadow_quality attribute (between 1 and 100)", "SuperMaterial.cpp");
    if (attributes.find("ambiant_occlusion") == attributes.end())
        throw ray::NodeError("IMaterial: missing ambiant_occlusion attribute (between 0 and 100)", "SuperMaterial.cpp");

    double specularExponent = std::stod(attributes.at("specular_exponent"));
    Maybe<Math::Vector3D> ka = Math::Vector3D::fromStr(attributes.at("ka"));
    Maybe<Math::Vector3D> ks = Math::Vector3D::fromStr(attributes.at("ks"));
    double refractionIndex = std::stod(attributes.at("refraction_index"));
    double transparency = std::stod(attributes.at("transparency"));
    double shadowQuality = std::stod(attributes.at("shadow_quality"));
    double ambiantOcclusion = std::stod(attributes.at("ambiant_occlusion"));

    if (ka.has_value() == false)
        throw ray::NodeError("IMaterial: ka attribute is incorrect", "SuperMaterial.cpp");
    if (ks.has_value() == false)
        throw ray::NodeError("IMaterial: ka attribute is incorrect", "SuperMaterial.cpp");
    if (refractionIndex < 1)
        throw ray::NodeError("IMaterial: refraction_index attribute should be >= 1", "SuperMaterial.cpp");
    if (transparency < 0 || transparency > 1)
        throw ray::NodeError("IMaterial: transparency attribute should be 0 <= transparency <= 1", "SuperMaterial.cpp");
    if (shadowQuality < 1 || shadowQuality > 100)
        throw ray::NodeError("IMaterial: transparency attribute should be 0 <= transparency <= 1", "SuperMaterial.cpp");

    auto* node = new ray::SuperMaterial(specularExponent,
        Math::Matrix<1, 3>{{{ka.value().X, ka.value().Y, ka.value().Z}}},
        Math::Matrix<1, 3>{{{ks.value().X, ks.value().Y, ks.value().Z}}},
        Math::Matrix<1, 3>{{{0, 0, 0}}},
        refractionIndex, transparency, shadowQuality, ambiantOcclusion);

    if (attributes.find("skybox") != attributes.end() && attributes.at("skybox") == "true")
        node->setSkybox();
    if (attributes.find("kd") == attributes.end()) {
        if (attributes.find("map_kd") == attributes.end())
            throw ray::NodeError(R"(IMaterial: Missing Kd attribute. Should be either: "kd" (vector with values) or "map_kd" (string with path))", "SuperMaterial.cpp");
        std::string mapKd = attributes.at("map_kd");
        node->setKd(mapKd);
    } else {
        Maybe<Math::Vector3D> kd = Math::Vector3D::fromStr(attributes.at("kd"));

        if (kd.has_value() == false)
            throw ray::NodeError("IMaterial: kd attribute is incorrect, should be Vector", "SuperMaterial.cpp");
        node->setKd(
            Math::Matrix<1, 3>{{{kd.value().X, kd.value().Y, kd.value().Z}}}
        );
    }
    if (attributes.find("bump") != attributes.end()) {
        std::string bump = attributes.at("bump");
        node->setNormalMap(bump);
    }

    return node;
}
