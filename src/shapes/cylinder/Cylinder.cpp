/*
** EPITECH PROJECT, 2023
** B-OOP-400-MPL-4-1-raytracer-robin.glaude
** File description:
** Cylinder.cpp
*/

#include "Cylinder.hpp"

#include <utils/isBehind.h>

void ray::Cylinder::initValues()
{
    AShape::initValues();
    setDirection();
    setPosition();
    setDimensions();
}

void ray::Cylinder::setDimensions()
{
    Math::Vector3D scale = getScale();

    _radius = scale.X;
    if (_finite == true)
        _height = scale.Y;
}

void ray::Cylinder::setPosition()
{
    _position = Scene::getPosition(*this);
    _position.Y = -_position.Y;
}

void ray::Cylinder::setDirection()
{
    _direction = Math::Vector3D{0, 1, 0};
    Math::Matrix rotation = getRotation();
    std::vector<std::vector<double>> values = {{0}, {1}, {0}};
    Math::Matrix<3, 1> iniDirection(values);
    Math::Matrix result = rotation * iniDirection;
    _direction = Math::Vector3D(result(0, 0), result(1, 0), result(2, 0));
    _direction = _direction / _direction.length();
}

void ray::Cylinder::setFinite(bool finite)
{
    _finite = finite;
}

Math::Vector3D pointTimesVector(Math::Point3D P, Math::Vector3D V)
{
    Math::Vector3D result;
    result.X = P.X * V.X;
    result.Y = P.Y * V.Y;
    result.Z = P.Z * V.Z;
    return result;
}

Math::Vector3D pointSubVector(Math::Point3D P, Math::Vector3D V)
{
    Math::Vector3D result;
    result.X = P.X - V.X;
    result.Y = P.Y - V.Y;
    result.Z = P.Z - V.Z;
    return result;
}

double calcA(Math::Vector3D V, Math::Vector3D Va)
{
    Math::Vector3D Vb = V - ((V * Va) * Va);
    Vb = Vb * Vb;
    return Vb.X + Vb.Y + Vb.Z;
}

double calcB(Math::Vector3D V, Math::Vector3D Va, Math::Point3D P, Math::Point3D Pa)
{
    Math::Vector3D Vb = V - ((V * Va) * Va);
    Math::Point3D detP = P - Pa;
    Math::Vector3D Vc = pointTimesVector(detP, Va);
    Vc = Vc * Va;
    Vc = pointSubVector(detP, Vc);
    Math::Vector3D Vd = Vb * Vc;
    Vd.X = 2 * Vd.X;
    Vd.Y = 2 * Vd.Y;
    Vd.Z = 2 * Vd.Z;
    return Vd.X + Vd.Y + Vd.Z;
}

double calcC(Math::Point3D P, Math::Point3D Pa, Math::Vector3D Va, double R)
{
    Math::Point3D detP = P - Pa;
    Math::Vector3D Vb = pointTimesVector(detP, Va);
    Vb = Vb * Va;
    Vb = pointSubVector(detP, Vb);
    Vb = Vb * Vb;
    return Vb.X + Vb.Y + Vb.Z - (R * R);
}

Math::Point3D getClosestRoot(double a, double b, double det, ray::Ray ray)
{
    double root1 = (-b + sqrt(det)) / (2 * a);
    double root2 = (-b - sqrt(det)) / (2 * a);

    Math::Point3D pos1 = {
        (ray.origin.X + ray.direction.X * root1),
        (ray.origin.Y + ray.direction.Y * root1),
        (ray.origin.Z + ray.direction.Z * root1)
        };
    Math::Point3D pos2 = {
        (ray.origin.X + ray.direction.X * root2),
        (ray.origin.Y + ray.direction.Y * root2),
        (ray.origin.Z + ray.direction.Z * root2)
        };

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

bool ray::Cylinder::cutCylinder(const Math::Point3D &hit) const
{
    Math::Point3D ortho = getOrtho(hit);
    Math::Vector3D dirOrtho = {ortho.X - _position.X, ortho.Y - _position.Y, ortho.Z - _position.Z};
    dirOrtho = dirOrtho / dirOrtho.length();
    if (dirOrtho != _direction)
        return false;
    Math::Point3D end = {
        _position.X + _direction.X * _height,
        _position.Y + _direction.Y * _height,
        _position.Z + _direction.Z * _height
        };
    if (ortho.X < end.X || ortho.Y > end.Y || ortho.Z < end.Z)
        return false;
    return true;
}

Maybe<PosShapePair> ray::Cylinder::hit(const ray::Ray &ray) const
{
    double a = calcA(ray.direction, _direction);
    double b = calcB(ray.direction, _direction, ray.origin, _position);
    double c = calcC(ray.origin, _position, _direction, _radius);
    double det = b * b - 4 * (a * c);
    if (det < 0 || std::fabs(det) < 0.001)
        return {};
    Math::Point3D hit = getClosestRoot(a, b, det, ray);
    if (_finite == true) {
        if (!cutCylinder(hit))
            return {};
    }
    return Maybe<PosShapePair>{std::make_pair(hit, nullptr)};
}

Math::Point3D ray::Cylinder::getOrtho(const Math::Point3D& point) const
{
    Math::Vector3D dirNormalised = _direction / _direction.length();
    double t = (dirNormalised.X * (point.X - _position.X) + dirNormalised.Y * (point.Y - _position.Y) + dirNormalised.Z * (point.Z - _position.Z))
    / (dirNormalised.X * dirNormalised.X + dirNormalised.Y * dirNormalised.Y + dirNormalised.Z * dirNormalised.Z);
    Math::Point3D ortho = {_position.X - t * dirNormalised.X, _position.Y - t * dirNormalised.Y, _position.Z - t * dirNormalised.Z};
    return ortho;
}

Math::Vector3D ray::Cylinder::getNormale(const Math::Point3D& point, __attribute__((unused))const ray::Ray& camRay) const
{
    if (_material->hasNormalMap())
        return _material->normalFromMap(getUVMapping(point), getRotation());
    Math::Vector3D normal {0, 0, 0};
    if (!_finite) {
        if (_direction.X == 0)
            normal.X = point.X - _position.X;
        if (_direction.Y == 0)
            normal.Y = point.Y - _position.Y;
        if (_direction.Z == 0)
            normal.Z = point.Z - _position.Z;
    } else {
        Math::Vector3D dirNormalised = _direction / _direction.length();
        double t = (dirNormalised.X * (point.X - _position.X) + dirNormalised.Y * (point.Y - _position.Y) + dirNormalised.Z * (point.Z - _position.Z))
        / (dirNormalised.X * dirNormalised.X + dirNormalised.Y * dirNormalised.Y + dirNormalised.Z * dirNormalised.Z);
        Math::Point3D ortho = {_position.X - t * dirNormalised.X, _position.Y - t * dirNormalised.Y, _position.Z - t * dirNormalised.Z};
        normal.X = point.X - ortho.X;
        normal.Y = point.Y - ortho.Y;
        normal.Z = point.Z - ortho.Z;
    }
    return normal / normal.length();
}

ray::Ray ray::Cylinder::getRefraction(
    __attribute__((unused))const std::shared_ptr<ray::IScene>& scene,
    Math::Point3D pos, Math::Vector3D dir) const
{
    return {pos + dir * 0.0001, dir};
}

extern "C" ray::INode *create(std::map<std::string, std::string> &attributes)
{
    ray::Cylinder* cylinder = new ray::Cylinder();
    if (attributes.find("finite") != attributes.end()) {
        if (attributes["finite"] == "true") {
            cylinder->setFinite(true);
        } else if (attributes["finite"] == "false") {
            cylinder->setFinite(false);
        } else {
            throw std::invalid_argument("Cylinder finite must be true or false");
        }
    } else {
        cylinder->setFinite(false);
    }
    return cylinder;
}
