/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** sceneCreate.cpp
*/

#include "Scene.hpp"

extern "C" ray::INode *create(__attribute__((unused))const std::map<std::string, std::string> &attributes)
{
    if (attributes.find("background_color") == attributes.end())
        throw ray::NodeError("IScene: missing background_color attribute", "createScene.cpp");

    Maybe<RGB> color = RGB::fromStr(attributes.at("background_color"));

    if (!color.has_value())
        throw ray::NodeError("IScene: invalid background_color attribute", "createScene.cpp");

    return new ray::Scene(color.value());
}
