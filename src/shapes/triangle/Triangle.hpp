//
// Created by Guillaume Tran on 07/05/2024.
//

#ifndef RAYTRACER_TRIANGLE_HPP
#define RAYTRACER_TRIANGLE_HPP

#include <map>
#include <cmath>
#include "Ray.hpp"
#include "../../../include/Maybe.hpp"
#include "../../scenes/base/Scene.hpp"
#include "../../../src/shapes/AShape.hpp"
#include "../../../include/math/Vector3D.hpp"

namespace ray {
    class Triangle : public ray::AShape {

    public:
        Triangle() = default;
        ~Triangle() override = default;

        void initValues() override;

        Maybe<PosShapePair> hit(const ray::Ray &ray) const override;
        Math::Vector3D getNormale(const Math::Point3D& point, const ray::Ray& camRay) const override;
        ray::Ray getRefraction(
            const std::shared_ptr<ray::IScene>& scene,
            Math::Point3D pos, Math::Vector3D dir) const override;

        void setPoint(Math::Point3D p1, Math::Point3D p2, Math::Point3D p3);
        Math::Vector2D getUVMapping(Math::Point3D coords) const override;
        void initPoints();

    private:
        Math::Point3D _p1;
        Math::Point3D _p2;
        Math::Point3D _p3;

        Math::Vector3D _normal;
    };

}

Math::Point3D StringToPoint3D(std::string str);

#endif //RAYTRACER_TRIANGLE_HPP
