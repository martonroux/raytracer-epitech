/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** TextureMaterial.cpp
*/

#include "TextureMaterial.hpp"

namespace ray {

    sf::Image openImage(const std::string& path)
    {
        sf::Image image;
        if (!image.loadFromFile(path)) {
            throw ray::NodeError("IMaterial: Error happened while opening image at path [" + path + "].", "TextureMaterial.cpp");
        }
        return image;
    }

    TextureMaterial::TextureMaterial(std::string texturePath) : AMaterial(1),
        _img(openImage(texturePath)),
        _phong({},
            0.05,
            50,
            1,
            0,
            0,
            Math::Matrix<1, 3>{{{0, 0, 0}}})
    {
    }

    RGB TextureMaterial::getColor(int recursion, Math::Point3D collisionPoint,
        Math::Vector3D normale, Math::Point3D camPos,
        const std::shared_ptr<ray::IShape> &shape,
        const std::shared_ptr<ray::IScene> &scene) const
    {
        Math::Vector2D uv = shape->getUVMapping(collisionPoint);
        Math::Vector3D view = {camPos.X - collisionPoint.X, camPos.Y - collisionPoint.Y, camPos.Z - collisionPoint.Z};
        sf::Color color;
        Math::Matrix<1, 3> kd;

        view /= view.length();

        if (uv == Math::Vector2D{-1, -1})
            uv = Math::Vector2D{0, 0};
        color = _img.getPixel(
            static_cast<unsigned int>(uv.first * _img.getSize().x) % _img.getSize().x,
            _img.getSize().y - static_cast<unsigned int>(uv.second * _img.getSize().y) % _img.getSize().y - 1);
        if (recursion > REFLECTION_RECURSION_LIMIT)
            return {color.r, color.g, color.b};
        kd(0, 0) = static_cast<double>(color.r) / 255;
        kd(0, 1) = static_cast<double>(color.g) / 255;
        kd(0, 2) = static_cast<double>(color.b) / 255;
        return _phong.calculateColor(kd, kd, scene, shape, view, collisionPoint, normale, recursion);
    }

    void TextureMaterial::setSkybox()
    {
        _phong.setIa(1);
    }
} // ray

extern "C" ray::INode *create(const std::map<std::string, std::string> &attributes)
{
    if (attributes.find("texture") == attributes.end())
        throw ray::NodeError("IMaterial: missing texture attribute (path to image to use as texture, from root of repository)", "TextureMaterial.cpp");

    std::string texture = attributes.at("texture");

    ray::INode* node = new ray::TextureMaterial(texture);

    if (attributes.find("skybox") != attributes.end() && attributes.at("skybox") == "true")
        dynamic_cast<ray::TextureMaterial*>(node)->setSkybox();
    return node;
}
