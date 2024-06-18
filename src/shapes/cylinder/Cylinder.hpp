/*
** EPITECH PROJECT, 2023
** B-OOP-400-MPL-4-1-raytracer-robin.glaude
** File description:
** Cylinder.hpp
*/

#ifndef B_OOP_400_MPL_4_1_RAYTRACER_ROBIN_GLAUDE_CYLINDer_HPP
#define B_OOP_400_MPL_4_1_RAYTRACER_ROBIN_GLAUDE_CYLINDer_HPP

#include <map>
#include <cmath>
#include "Ray.hpp"
#include "../../../include/Maybe.hpp"
#include "../../scenes/base/Scene.hpp"
#include "../../../src/shapes/AShape.hpp"
#include "../../../include/math/Vector3D.hpp"

namespace ray {
    class Cylinder : public ray::AShape {
    public:
        Cylinder() = default;
        ~Cylinder() override = default;

        void initValues() override;
        void setFinite(bool finite);

        bool cutCylinder(const Math::Point3D &hit) const;
        Math::Point3D getOrtho(const Math::Point3D& point) const;
        Maybe<PosShapePair> hit(const ray::Ray &ray) const override;
        Math::Vector3D getNormale(const Math::Point3D& point, const ray::Ray& camRay) const override;
        ray::Ray getRefraction(const std::shared_ptr<ray::IScene> &scene, Math::Point3D pos, Math::Vector3D dir) const override;

    private:
        bool _finite = true;
        double _radius = -1;
        double _height = -1;
        Math::Point3D _position;
        Math::Vector3D _direction;

        void setPosition();
        void setDirection();
        void setDimensions();
    };
}

extern "C" ray::INode *create(std::map<std::string, std::string> &attributes);

#endif