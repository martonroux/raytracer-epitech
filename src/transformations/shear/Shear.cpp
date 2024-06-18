/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** Shear.cpp
*/

#include "Shear.hpp"

namespace ray {

    Math::Matrix<4, 4> getXShearMatrix(Math::Vector3D shear)
    {
        Math::Matrix<4, 4> res = Math::Matrix<4, 4>::identity();

        res(0, 1) = shear.Y;
        res(0, 2) = shear.Z;
        return res;
    }

    Math::Matrix<4, 4> getYShearMatrix(Math::Vector3D shear)
    {
        Math::Matrix<4, 4> res = Math::Matrix<4, 4>::identity();

        res(1, 0) = shear.X;
        res(1, 2) = shear.Z;
        return res;
    }

    Math::Matrix<4, 4> getZShearMatrix(Math::Vector3D shear)
    {
        Math::Matrix<4, 4> res = Math::Matrix<4, 4>::identity();

        res(2, 0) = shear.X;
        res(2, 1) = shear.Y;
        return res;
    }

    Shear::Shear(ShearType type, Math::Vector3D shear)
    {
        switch (type) {
            case ShearType::XAxis: {
                if (shear.X != 0)
                    throw NodeError("IShear: Tried to do a shear on the X axis whilst X component of Vector3D isn't zero.", "Shear.cpp");
                _matrix = getXShearMatrix(shear);
                break;
            }
            case ShearType::YAxis: {
                if (shear.Y != 0)
                    throw NodeError("IShear: Tried to do a shear on the Y axis whilst Y component of Vector3D isn't zero.", "Shear.cpp");
                _matrix = getYShearMatrix(shear);
                break;
            }
            case ShearType::ZAxis: {
                if (shear.Z != 0)
                    throw NodeError("IShear: Tried to do a shear on the Z axis whilst Z component of Vector3D isn't zero.", "Shear.cpp");
                _matrix = getZShearMatrix(shear);
                break;
            }
        }
    }

} // ray

extern "C" const char* getAttribute(const std::map<std::string, std::string>& attributes)
{
    bool xShear = false;
    bool yShear = false;
    bool zShear = false;

    if (attributes.find("x_shear") != attributes.end())
        xShear = true;
    if (attributes.find("y_shear") != attributes.end())
        yShear = true;
    if (attributes.find("z_shear") != attributes.end())
        zShear = true;

    if ((xShear || yShear || zShear) == false)
        throw ray::NodeError("IShear: missing shear attribute [one of: x_shear (vector3D), y_shear (vector3D), z_shear (vector3D)]", "Shear.cpp");

    if ((xShear && yShear) || (xShear && zShear) || (yShear && zShear))
        throw ray::NodeError("IShear: can only apply shear to one axis at a time", "Shear.cpp");

    if (xShear)
        return "x_shear";
    if (yShear)
        return "y_shear";
    return "z_shear";
}

extern "C" ray::INode *create(const std::map<std::string, std::string> &attributes)
{
    const char * attr = getAttribute(attributes);

    Maybe<Math::Vector3D> shear = Math::Vector3D::fromStr(attributes.at(attr));

    if (!shear.has_value())
        throw ray::NodeError("IShear: invalid shear attribute. Should be Vector3D", "Shear.cpp");

    if (attr[0] == 'x')
        return new ray::Shear(ray::ShearType::XAxis, shear.value());
    if (attr[0] == 'y')
        return new ray::Shear(ray::ShearType::YAxis, shear.value());
    return new ray::Shear(ray::ShearType::ZAxis, shear.value());
}
