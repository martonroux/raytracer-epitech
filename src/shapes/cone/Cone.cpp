//
// Created by Guillaume Tran on 02/05/2024.
//

#include "Cone.hpp"

float toRadians(float angle)
{
    return angle;
}

void ray::Cone::transform()
{
    if (_transform == false) {
        applyMatrix();
        setPosition();
        _inverseRotationMatrix = getRotation().inverse();
        _transform = true;
    }
}

void ray::Cone::initValues()
{
    AShape::initValues();
    applyMatrix();
    setPosition();
    _inverseRotationMatrix = getRotation().inverse();

}

Math::Vector3D rotateVector(Math::Matrix<3,3> rotationMatrix, Math::Vector3D vector)
{
    Math::Vector3D res;
    res.X = rotationMatrix(0,0) * vector.X +
            rotationMatrix(0,1) * vector.Y +
            rotationMatrix(0,2) * vector.Z;
    res.Y = rotationMatrix(1,0) * vector.X +
            rotationMatrix(1,1) * vector.Y +
            rotationMatrix(1,2) * vector.Z;
    res.Z = rotationMatrix(2,0) * vector.X +
            rotationMatrix(2,1) * vector.Y +
            rotationMatrix(2,2) * vector.Z;
    return res;
}

Math::Point3D rotatePoint(Math::Matrix<3,3> rotationMatrix, Math::Point3D point)
{
   Math::Matrix<3,1> res;
   res(0,0) = point.X;
    res(1,0) = point.Y;
    res(2,0) = point.Z;
    res = rotationMatrix * res;
    return Math::Point3D{res(0,0), res(1,0), res(2,0)};
}
Maybe<PosShapePair> ray::Cone::hit(const ray::Ray& ray) const
{
    Math::Vector3D rayDir = rotateVector(_inverseRotationMatrix, ray.direction);
    Math::Point3D rayOrigin = rotatePoint(_inverseRotationMatrix, ray.origin - center);
    float A = rayOrigin.X - center.X;
    float B = rayOrigin.Z - center.Z;
    float D = _height - rayOrigin.Y + center.Y;
    float tanTheta = pow(tan(toRadians(_radius/_height)), 2);
    float a = pow(rayDir.X, 2) + pow(rayDir.Z, 2) - tanTheta * pow(rayDir.Y, 2);
    float b = 2 * (rayDir.X * A + rayDir.Z * B + tanTheta * rayDir.Y * D);
    float c = pow(A, 2) + pow(B, 2) - tanTheta * pow(D, 2);
    float delta = b * b - 4 * a * c;

    if (delta < 0) {
        return {};
    }
    float t1 = (-b - sqrt(delta)) / (2 * a);
    float t2 = (-b + sqrt(delta)) / (2 * a);
    float t = -1;
    if (t1 > t2) {
        t = t2;
    } else {
        t = t1;
    }

    float r = (rayOrigin.Y + t * rayDir.Y);
    if (_finite && (r < center.Y || r > center.Y + _height)) {
        return {};
    }
    return Maybe<PosShapePair>{std::make_pair(rayOrigin + rayDir * t, nullptr)};
}

Math::Vector3D ray::Cone::getNormale(const Math::Point3D& point, __attribute__((unused))const ray::Ray& camRay) const
{
    if (_material->hasNormalMap())
        return _material->normalFromMap(getUVMapping(point), getRotation());
    Math::Point3D p = point;
    float r = sqrt(pow(p.X - center.X, 2) + pow(p.Z - center.Z, 2));
    Math::Vector3D normal = {p.X - center.X, r * tan(toRadians(_radius/_height)), p.Z - center.Z};
    normal = rotateVector(getRotation(), normal);
    return normal / normal.length();
}

ray::Ray ray::Cone::getRefraction(
    __attribute__((unused))const std::shared_ptr<ray::IScene>& scene,
    Math::Point3D pos, Math::Vector3D dir) const
{
    return {pos + dir * 0.0001, dir};
}

void ray::Cone::setPosition()
{
    center = Scene::getPosition(*this);
}

void ray::Cone::setRadius(float radius)
{
    _radius = radius;
}

void ray::Cone::setHeight(float height)
{
    _height = height;
}

extern "C" ray::INode *create(std::map<std::string, std::string>& properties)
{
    ray::Cone* cone = new ray::Cone();
    if (properties.find("height") != properties.end()) {
        cone->setHeight(std::stof(properties["height"]));
    } else {
        throw std::invalid_argument("Cone must have a height");
    }
    if (properties.find("radius") != properties.end()) {
        cone->setRadius(std::stof(properties["radius"]));
    } else {
        throw std::invalid_argument("Cone must have a radius");
    }
    if (properties.find("finite") != properties.end()) {
        if (properties["finite"] == "true") {
            cone->setFinite(true);
        } else if (properties["finite"] == "false") {
            cone->setFinite(false);
        } else {
            throw std::invalid_argument("Cone finite must be true or false");
        }
    } else {
        cone->setFinite(false);
    }
    return cone;
}
