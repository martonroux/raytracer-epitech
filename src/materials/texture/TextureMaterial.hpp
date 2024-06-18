/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** TextureMaterial.hpp
*/

#ifndef TEXTUREMATERIAL_HPP
#define TEXTUREMATERIAL_HPP

#include <SFML/Graphics.hpp>

#include "../AMaterial.hpp"
#include "../Phong.hpp"

namespace ray {

    class TextureMaterial : public AMaterial {
        sf::Image _img;
        Phong::Model _phong;

    public:
        explicit TextureMaterial(std::string texturePath);
        ~TextureMaterial() override = default;

        RGB getColor(
            int recursion,
            Math::Point3D collisionPoint,
            Math::Vector3D normale,
            Math::Point3D camPos,
            const std::shared_ptr<ray::IShape> &shape,
            const std::shared_ptr<ray::IScene> &scene) const override;

        void setSkybox();
    };

} // ray

extern "C" ray::INode *create(const std::map<std::string, std::string> &attributes);

#endif //TEXTUREMATERIAL_HPP
