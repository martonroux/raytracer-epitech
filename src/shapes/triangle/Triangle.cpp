//
// Created by Guillaume Tran on 07/05/2024.
//

#include "Triangle.hpp"

void ray::Triangle::initValues()
{
    AShape::initValues();
    applyMatrix();
    initPoints();

    // Calculate Normale
    Math::Vector3D edge1 = {_p2.X - _p1.X, _p2.Y - _p1.Y, _p2.Z - _p1.Z};
    Math::Vector3D edge2 = {_p3.X - _p1.X, _p3.Y - _p1.Y, _p3.Z - _p1.Z};
    _normal = edge1.product(edge2);
    _normal = _normal / _normal.length();
}

Maybe<PosShapePair> ray::Triangle::hit(const ray::Ray &ray) const
{
    Math::Vector3D v0v1 = {_p2.X - _p1.X, _p2.Y - _p1.Y, _p2.Z - _p1.Z};

    double nDotRayDirection = _normal.dot(ray.direction);
    if (std::abs(nDotRayDirection) < 0.0000001)
        return Maybe<PosShapePair>{};
    double D = -_normal.dot({_p1.X, _p1.Y, _p1.Z});
    double t = -(_normal.dot({ray.origin.X, ray.origin.Y, ray.origin.Z}) + D) / _normal.dot(ray.direction);

    if (t < 0)
        return Maybe<PosShapePair>{};
    Math::Point3D P = ray.origin + ray.direction * t;
    Math::Vector3D C;
    Math::Vector3D vp0 = {P.X - _p1.X, P.Y - _p1.Y, P.Z - _p1.Z};
    C = v0v1.product(vp0);

    if (_normal.dot(C) < 0)
        return Maybe<PosShapePair>{};

    Math::Vector3D v1v2 = {_p3.X - _p2.X, _p3.Y - _p2.Y, _p3.Z - _p2.Z};
    Math::Vector3D vp1 = {P.X - _p2.X, P.Y - _p2.Y, P.Z - _p2.Z};
    C = v1v2.product(vp1);

    if (_normal.dot(C) < 0)
        return Maybe<PosShapePair>{};

    Math::Vector3D v2v0 = {_p1.X - _p3.X, _p1.Y - _p3.Y, _p1.Z - _p3.Z};
    Math::Vector3D vp2 = {P.X - _p3.X, P.Y - _p3.Y, P.Z - _p3.Z};
    C = v2v0.product(vp2);

    if (_normal.dot(C) < 0)
        return Maybe<PosShapePair>{};
    return Maybe<PosShapePair>{std::make_pair(P, std::make_shared<Triangle>(*this))};
}

Math::Vector3D ray::Triangle::getNormale(const Math::Point3D& point, const ray::Ray& camRay) const
{
    if (_material->hasNormalMap())
        return _material->normalFromMap(getUVMapping(point), getRotation());
    Math::Vector3D camDir = camRay.direction;
    if (_normal.dot(camDir) > 0)
        return _normal * -1;
    return _normal;
}

ray::Ray ray::Triangle::getRefraction(
    __attribute__((unused))const std::shared_ptr<ray::IScene>& scene,
    Math::Point3D pos, Math::Vector3D dir) const
{
    return {pos + dir * 0.0001, dir};
}

void ray::Triangle::setPoint(Math::Point3D p1, Math::Point3D p2, Math::Point3D p3)
{
    _p1 = p1;
    _p2 = p2;
    _p3 = p3;
}

Math::Vector2D ray::Triangle::getUVMapping(Math::Point3D coords) const
{
    // A -> p1
    // B -> p2
    // C -> p3
    Math::Vector3D AB = Math::Vector3D{_p2 - _p1};
    Math::Vector3D AC = Math::Vector3D{_p3 - _p1};

    Math::Vector3D PA = Math::Vector3D{_p1 - coords};
    Math::Vector3D PB = Math::Vector3D{_p2 - coords};
    Math::Vector3D PC = Math::Vector3D{_p3 - coords};

    double area = AB.product(AC).length() * (1.f / 2.f);
    double delta1 = PB.product(PC).length() / (2 * area);
    double delta2 = PC.product(PA).length() / (2 * area);
    double delta3 = PA.product(PB).length() / (2 * area);

    double sum = delta1 + delta2 + delta3;
    // double finalD1 = delta1 / sum; -> useless
    double finalD2 = delta2 / sum;
    double finalD3 = delta3 / sum;

    // We consider A -> (0,0), B -> (1,0), C -> (0,1)
    double u = finalD2;
    double v = finalD3;
    return {u, v};
}

void ray::Triangle::initPoints()
{
    Math::Matrix<4, 1> p1 = Math::Matrix<4, 1>{{{_p1.X}, {_p1.Y}, {_p1.Z}, {1}}};
    Math::Matrix<4, 1> p2 = Math::Matrix<4, 1>{{{_p2.X}, {_p2.Y}, {_p2.Z}, {1}}};
    Math::Matrix<4, 1> p3 = Math::Matrix<4, 1>{{{_p3.X}, {_p3.Y}, {_p3.Z}, {1}}};

    p1 = _transformMatrix * p1;
    p2 = _transformMatrix * p2;
    p3 = _transformMatrix * p3;

    _p1 = Math::Point3D{p1(0, 0), p1(1, 0), p1(2, 0)};
    _p2 = Math::Point3D{p2(0, 0), p2(1, 0), p2(2, 0)};
    _p3 = Math::Point3D{p3(0, 0), p3(1, 0), p3(2, 0)};
}

Math::Point3D StringToPoint3D(std::string str)
{
    Math::Point3D point;
    // get the 3 values from the string
    std::vector<std::string> values;
    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        values.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    values.push_back(str);
    point.X = std::stod(values[0]);
    point.Y = std::stod(values[1]);
    point.Z = std::stod(values[2]);
    return point;
}
