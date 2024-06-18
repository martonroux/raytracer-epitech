//
// Created by Guillaume Tran on 02/05/2024.
//

#ifndef RAYTRACER_CONE_HPP
#define RAYTRACER_CONE_HPP

#include "../AShape.hpp"
#include "Ray.hpp"
#include "../../../include/Maybe.hpp"
#include <cmath>
#include <memory>
#include <map>
#include "../../scenes/base/Scene.hpp"

namespace ray {
    class Cone : public ray::AShape {
    public:
        Cone() = default;
        ~Cone() override = default;

        void initValues() override;

        Maybe<PosShapePair> hit(const ray::Ray &ray) const override;
        Math::Vector3D getNormale(const Math::Point3D& point, const ray::Ray& camRay) const override;
        ray::Ray getRefraction(const std::shared_ptr<ray::IScene> &scene, Math::Point3D pos, Math::Vector3D dir) const override;

        void setPosition();
        void setRadius(float radius);
        void setHeight(float height);
        void transform();
        void setAxis(Math::Vector3D axis);
        Math::Vector3D getAxis();
        void setFinite(bool finite) { _finite = finite; }

    private:
        bool _transform;
        Math::Point3D center;
        float _height;
        float _radius;
        Math::Vector3D _axis;
        bool _finite;
        Math::Matrix<3, 3> _inverseRotationMatrix;
    };
}

extern "C" ray::INode *create(std::map<std::string, std::string>& properties);

#endif //RAYTRACER_CONE_HPP
