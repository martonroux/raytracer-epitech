//
// Created by Guillaume Tran on 08/05/2024.
//

#ifndef RAYTRACER_OBJECT_HPP
#define RAYTRACER_OBJECT_HPP

#include <cmath>
#include "Ray.hpp"
#include "../../../include/Maybe.hpp"
#include "../../scenes/base/Scene.hpp"
#include "../../../src/shapes/AShape.hpp"
#include "../../../include/math/Vector3D.hpp"
#include "ObjTriangle.hpp"
#include "loadObjMaterials.h"
#include <vector>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace ray {

    class Object : public ray::AShape {
    public:
        void initValues() override;
        void parse(std::string objPath);

        Maybe<PosShapePair> hit(const ray::Ray &ray) const override;
        Math::Vector3D getNormale(const Math::Point3D& point, const ray::Ray& camRay) const override;
        ray::Ray getRefraction(
                    const std::shared_ptr<ray::IScene>& scene,
                    Math::Point3D pos,
                    Math::Vector3D dir) const override;

        void setPath(std::string path) { _objPath = path; }

    private:
        std::vector<Math::Point3D> _points;
        std::vector<std::shared_ptr<ObjTriangle>> _triangles;
        std::vector<Math::Vector3D> _normals;
        std::vector<Math::Vector2D> _uvValues;
        std::map<std::string, std::shared_ptr<ray::IMaterial>> _materials;
        std::string _objPath;

        void parseFace(std::istringstream &ss, std::string line, std::string file, std::shared_ptr<ray::IMaterial> currentMaterial);
    };

}

extern "C" ray::INode *create(std::map<std::string, std::string>& properties);

#endif //RAYTRACER_OBJECT_HPP
