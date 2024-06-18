//
// Created by Guillaume Tran on 25/04/2024.
//

#include "Sphere.hpp"

#include <utils/isBehind.h>

#include "../../scenes/base/Scene.hpp"

void ray::Sphere::setRadius()
{
    Math::Matrix<3, 1> pos = Math::Matrix<3, 1>{{{0}, {0}, {1}}};
    pos = getRotation() * pos;
    Math::Vector3D posVec = {pos(0, 0), pos(1, 0), pos(2, 0)};
    radius = posVec.length();
}

void ray::Sphere::setPosition()
{
    center = Scene::getPosition(*this);
}

double calcA(Math::Vector3D D)
{
    return pow(D.X, 2) + pow(D.Y, 2) + pow(D.Z, 2);
}

double calcB(Math::Point3D O, Math::Vector3D D, Math::Point3D origin)
{
    return 2 * (D.X * (-origin.X + O.X) + D.Y * (-origin.Y + O.Y) + D.Z * (-origin.Z + O.Z));
}

double calcC(Math::Point3D O, Math::Point3D origin, double R)
{
    return pow(-origin.X + O.X, 2) + pow(-origin.Y + O.Y, 2) + pow(-origin.Z + O.Z, 2) - pow(R, 2);
}

double calcDet(double a, double b, double c)
{
    return pow(b, 2) - 4 * a * c;
}

Math::Point3D getClosestRoot(double a, double b, double det, ray::Ray ray)
{
    double root1 = (-b + sqrt(det)) / (2 * a);
    double root2 = (-b - sqrt(det)) / (2 * a);

    Math::Point3D pos1 = {(ray.origin.X + ray.direction.X * root1), (ray.origin.Y + ray.direction.Y * root1), (ray.origin.Z + ray.direction.Z * root1)};
    Math::Point3D pos2 = {(ray.origin.X + ray.direction.X * root2), (ray.origin.Y + ray.direction.Y * root2), (ray.origin.Z + ray.direction.Z * root2)};

    if (isBehind(pos1, ray.origin, ray.direction))
        return pos2;
    if (isBehind(pos2, ray.origin, ray.direction))
        return pos1;
    Math::Vector3D vect1 = {pos1.X - ray.origin.X, pos1.Y - ray.origin.Y, pos1.Z - ray.origin.Z};
    Math::Vector3D vect2 = {pos2.X - ray.origin.X, pos2.Y - ray.origin.Y, pos2.Z - ray.origin.Z};

    if (vect1.length() < vect2.length())
        return pos1;
    return pos2;
}

void ray::Sphere::initValues()
{
    AShape::initValues();
    applyMatrix();
    setPosition();
    setRadius();
    rotationMatrix = getRotation();
}

Maybe<PosShapePair> ray::Sphere::hit(const ray::Ray &ray) const
{
    double a = calcA(ray.direction);
    double b = calcB(ray.origin, ray.direction, center);
    double c = calcC(ray.origin, center, radius);
    double det = calcDet(a, b, c);

    if (det < 0) {
        return {};
    }
    return Maybe<PosShapePair>{std::make_pair(getClosestRoot(a, b, det, ray), nullptr)};
}

Math::Vector3D ray::Sphere::getNormale(const Math::Point3D& point, __attribute__((unused))const ray::Ray& camRay) const
{
    if (_material->hasNormalMap())
        return _material->normalFromMap(getUVMapping(point), getRotation());
    Math::Vector3D normale = {point.X - center.X,
                              point.Y - center.Y,
                              point.Z - center.Z};
    return normale / normale.length();
}

Math::Vector2D ray::Sphere::getUVMapping(Math::Point3D coords) const
{
    Math::Matrix<3, 1> centered = Math::Matrix<3, 1>{{{coords.X - center.X}, {coords.Y - center.Y}, {coords.Z - center.Z}}};
    centered = rotationMatrix * centered;
    Math::Vector3D centeredCoords = {centered(0, 0), centered(1, 0), centered(2, 0)};

    centeredCoords = centeredCoords / centeredCoords.length();
    centeredCoords *= radius;
    double theta = std::atan2(centeredCoords.X, centeredCoords.Y);
    double phi = std::asin(centeredCoords.Z / radius);

    double u = 0.5 + theta / (2 * M_PI);
    double v = 0.5 + phi / M_PI;

    return {u, v};
}

ray::Ray ray::Sphere::getRefraction(
    __attribute__((unused))const std::shared_ptr<ray::IScene>& scene,
    Math::Point3D pos, Math::Vector3D dir) const
{
    Math::Vector3D refracted = dir.refract(getNormale(pos, {}), 1, _material->getRefractionIndex());
    Ray refractedRay = {pos + refracted * 0.0001, refracted};
    Maybe<PosShapePair> hitSelf = hit(refractedRay);

    if (hitSelf.has_value() == false) // What?
        return {pos + dir * 0.0001, dir};

    Math::Vector3D hitNormale = getNormale(hitSelf.value().first, refractedRay);
    Math::Vector3D refracted2 = refracted.refract(hitNormale * -1, _material->getRefractionIndex(), 1);
    return {hitSelf.value().first + hitNormale * 0.0001, refracted2};
}

extern "C" ray::INode *create(__attribute__((unused))const std::map<std::string, std::string> &attributes)
{
    return new ray::Sphere();
}
