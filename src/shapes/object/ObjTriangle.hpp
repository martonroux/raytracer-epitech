/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** ObjTriangle.hpp
*/

#ifndef RAYTRACER_TRIANGLE_HPP
#define RAYTRACER_TRIANGLE_HPP

#include <map>
#include <cmath>
#include "Ray.hpp"
#include "Maybe.hpp"
#include "../../scenes/base/Scene.hpp"
#include "../../../src/shapes/AShape.hpp"
#include "math/Vector3D.hpp"
#include "math/Vector2D.hpp"
#include <SFML/Graphics.hpp>

namespace ray {
    enum class NormalType {
        BASIC,
        VERTICES,
        MAP
    };

    class ObjTriangle : public ray::AShape {
    public:
        ObjTriangle() = default;
        ~ObjTriangle() override = default;

        void initValues() override;

        Maybe<PosShapePair> hit(const ray::Ray &ray) const override;
        Math::Vector3D getNormale(const Math::Point3D& point, const ray::Ray& camRay) const override;
        ray::Ray getRefraction(
            const std::shared_ptr<ray::IScene>& scene,
            Math::Point3D pos, Math::Vector3D dir) const override;
        Math::Vector2D getUVMapping(Math::Point3D coords) const override;

        void initNormale();
        void setPoint(Math::Point3D p1, Math::Point3D p2, Math::Point3D p3);
        void setUvCoords(Math::Vector2D uv1, Math::Vector2D uv2, Math::Vector2D uv3);
        void setNormalMapCoords(Math::Vector3D n1, Math::Vector3D n2, Math::Vector3D n3);
        void setNormalMapImage(const std::string& imgPath);
        void setMaterial(std::shared_ptr<ray::IMaterial> material);
        void setTransformationMatrix(Math::Matrix<4, 4> matrix) { _transformMatrix = matrix; }

        Math::Point3D getp1() const { return _p1;}
        Math::Point3D getp2() const { return _p2;}
        Math::Point3D getp3() const { return _p3;}
    private:
        Math::Point3D _p1;
        Math::Point3D _p2;
        Math::Point3D _p3;
        Math::Vector2D _uv1 = {0, 0};
        Math::Vector2D _uv2 = {1, 0};
        Math::Vector2D _uv3 = {0, 1};
        Math::Vector3D _n1;
        Math::Vector3D _n2;
        Math::Vector3D _n3;
        sf::Image _normalMap;
        NormalType _normalType = NormalType::BASIC;

        Math::Vector3D _normal;
        Math::Vector3D _direction;

        Math::Vector3D calcNormale(Math::Point3D coords) const;
    };

}

Math::Point3D StringToPoint3D(std::string str);

#endif //RAYTRACER_TRIANGLE_HPP
