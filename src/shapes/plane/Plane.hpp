//
// Created by Guillaume Tran on 26/04/2024.
//

#ifndef B_OOP_400_MPL_4_1_RAYTRACER_ROBIN_GLAUDE_PLANE_HPP
#define B_OOP_400_MPL_4_1_RAYTRACER_ROBIN_GLAUDE_PLANE_HPP

#include "../AShape.hpp"
#include "Ray.hpp"
#include "../../../include/Maybe.hpp"
#include <cmath>
#include <memory>
#include <map>
#include "../../scenes/base/Scene.hpp"

namespace ray {
    class Plane : public ray::AShape {
        Math::Point3D center;
        Math::Vector3D _normale;
        Math::Vector3D _u;
        Math::Vector3D _v;

    public:
        Plane() = default;
        ~Plane() override = default;

        void setPosition();
        void initValues() override;

        Maybe<PosShapePair> hit(const ray::Ray &ray) const override;
        Math::Vector3D getNormale(const Math::Point3D &point, const ray::Ray& camRay) const override;
        Math::Vector2D getUVMapping(Math::Point3D coords) const override;
        ray::Ray getRefraction(const std::shared_ptr<ray::IScene> &scene, Math::Point3D pos, Math::Vector3D dir) const override;
    };
}

extern "C" ray::INode *create(std::map<std::string, std::string>& properties);

#endif //B_OOP_400_MPL_4_1_RAYTRACER_ROBIN_GLAUDE_PLANE_HPP
