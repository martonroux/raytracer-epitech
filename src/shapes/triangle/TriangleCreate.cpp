//
// Created by Guillaume Tran on 08/05/2024.
//

#include "Triangle.hpp"


extern "C" ray::INode *create(std::map<std::string, std::string>& properties)
{
    ray::Triangle *triangle = new ray::Triangle();
    if (properties.find("p1") != properties.end() && properties.find("p2") != properties.end() && properties.find("p3") != properties.end()) {
        Maybe<Math::Vector3D> vec1 = Math::Vector3D::fromStr(properties["p1"]);
        Maybe<Math::Vector3D> vec2 = Math::Vector3D::fromStr(properties["p2"]);
        Maybe<Math::Vector3D> vec3 = Math::Vector3D::fromStr(properties["p3"]);

        if (vec1.has_value() == false)
            throw ray::NodeError("IShape: p1 attribute is not a valid vector", "Triangle.cpp");
        if (vec2.has_value() == false)
            throw ray::NodeError("IShape: p2 attribute is not a valid vector", "Triangle.cpp");
        if (vec3.has_value() == false)
            throw ray::NodeError("IShape: p3 attribute is not a valid vector", "Triangle.cpp");

        Math::Point3D p1 = Math::Point3D{vec1.value().X, vec1.value().Y, vec1.value().Z};
        Math::Point3D p2 = Math::Point3D{vec2.value().X, vec2.value().Y, vec2.value().Z};
        Math::Point3D p3 = Math::Point3D{vec3.value().X, vec3.value().Y, vec3.value().Z};
        triangle->setPoint(p1, p2, p3);
        return triangle;
    }
    throw ray::NodeError("IShape: missing p1, p2 or p3 attribute", "Triangle.cpp");
}
