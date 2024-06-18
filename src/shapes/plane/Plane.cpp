//
// Created by Guillaume Tran on 26/04/2024.
//

#include "Plane.hpp"

Math::Vector3D rotateVectorAlong(Math::Vector3D toRotate, Math::Vector3D axis, double angle)
{
    return toRotate * std::cos(angle) +
        axis.product(toRotate) * std::sin(angle) +
            axis * axis.dot(toRotate) * (1 - std::cos(angle));
}

Math::Vector3D getPerpendicularVector(Math::Vector3D vec)
{
    Math::Vector3D res = vec.product({0, 0, 1});
    if (res != Math::Vector3D{0, 0, 0})
        return res;
    res = vec.product({0, 1, 0});
    if (res != Math::Vector3D{0, 0, 0})
        return res;
    return vec.product({1, 0, 0});
}

Maybe<PosShapePair> ray::Plane::hit(const ray::Ray& ray) const
{
    Math::Vector3D vectorNormale = getNormale(Math::Point3D{0,0,0}, ray);

    if (vectorNormale.dot(ray.direction) == 0) // perpendicular
        return {};
    Math::Point3D point = getPosition();
    double t = 0;
    double denom = vectorNormale.dot({point.X, point.Y, point.Z});
    t = (denom - (vectorNormale.X * ray.origin.X + vectorNormale.Y * ray.origin.Y + vectorNormale.Z * ray.origin.Z)) / vectorNormale.dot(ray.direction);
    if (t < 0) {
        return {};
    }
    return Maybe<PosShapePair>{std::make_pair(ray.origin + ray.direction * t, nullptr)};
}

void ray::Plane::setPosition()
{
    center = Scene::getPosition(*this);
}

void ray::Plane::initValues()
{
    Math::Matrix<4, 1> tempNormale;
    AShape::initValues();
    applyMatrix();
    setPosition();
    tempNormale(0,0) = 0;
    tempNormale(1,0) = 1;
    tempNormale(2,0) = 0;
    tempNormale(3,0) = 0;
    tempNormale = _transformMatrix * tempNormale;
    _normale = Math::Vector3D{ tempNormale(0,0), tempNormale(1,0), tempNormale(2,0) };
    _u = getPerpendicularVector(_normale);
    _v = rotateVectorAlong(_u, _normale, M_PI / 2);
}

Math::Vector3D ray::Plane::getNormale(__attribute__((unused))const Math::Point3D& point, const ray::Ray& camRay) const
{
    if (_material->hasNormalMap())
        return _material->normalFromMap(getUVMapping(point), getRotation());
    if (_normale.dot(camRay.direction) > 0)
        return _normale * -1;
    return _normale;
}

Math::Vector2D ray::Plane::getUVMapping(Math::Point3D coords) const
{
    Math::Vector2D res{0, 0};
    Math::Vector3D w = {
        coords.X - center.X,
        coords.Y - center.Y,
        coords.Z - center.Z
    };

    res.first = std::abs(w.dot(_u)) / 400;
    res.second = std::abs(w.dot(_v)) / 400;
    return res;
}

ray::Ray ray::Plane::getRefraction(
   __attribute__((unused))const std::shared_ptr<ray::IScene>& scene,
   Math::Point3D pos, Math::Vector3D dir) const
{
   return {pos + dir * 0.0001, dir};
}
extern "C" ray::INode *create(__attribute__((unused))std::map<std::string, std::string>& properties)
{
    return new ray::Plane();
}
