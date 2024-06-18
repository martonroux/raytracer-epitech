//
// Created by Guillaume Tran on 08/05/2024.
//

#include "Object.hpp"
#include <utils/getClosest.h>
#include <regex>
#include <filesystem>

void ray::Object::initValues()
{
    AShape::initValues();
    applyMatrix();
    parse(_objPath);
}

Math::Point3D createPoint(std::istringstream &ss)
{
    Math::Point3D point;
    ss >> point.X >> point.Y >> point.Z;
    return point;
}

Math::Point3D PointMatrix(const Math::Point3D &point, const Math::Matrix<4,4> &matrix)
{
    Math::Matrix<4, 1> pointMatrix;
    pointMatrix(0, 0) = point.X;
    pointMatrix(1, 0) = point.Y;
    pointMatrix(2, 0) = point.Z;
    pointMatrix(3, 0) = 1;
    Math::Matrix<4, 1> result = matrix * pointMatrix;
    return Math::Point3D(result(0, 0), result(1, 0), result(2, 0));
}

void ray::Object::parse(std::string objPath)
{
    std::ifstream file(objPath);
    std::string line;
    std::shared_ptr<ray::IMaterial> currentMaterial = _material;
    // get max scale between x, y and z
    if (!file.is_open())
        throw std::runtime_error("Can't open file " + objPath + "\n");
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string type;
        ss >> type;
        if (type == "mtllib") {
            ss >> type;
            std::filesystem::path fullPath(objPath);
            _materials = loadObjMaterials(fullPath.parent_path().generic_string() + "/" + type);
        }
        if (type == "usemtl") {
            ss >> type;
            if (_materials.find(type) != _materials.end())
                currentMaterial = _materials[type];
        }
        if (type == "v") {
            _points.push_back(PointMatrix(createPoint(ss), _transformMatrix));
        }
        if (type == "vn") {
            Math::Vector3D normal;
            ss >> normal.X >> normal.Y >> normal.Z;
            _normals.push_back(normal);
        }
        if (type == "vt") {
            Math::Vector2D uvT = {0, 0};
            ss >> uvT.first >> uvT.second;
            _uvValues.push_back(uvT);
        }
        if (type == "f") {
            parseFace(ss, line, objPath, currentMaterial);
        }
    }
    std::cout << "Loaded file: " << objPath << ". Number of polygons: " << _triangles.size() << std::endl;
}

Maybe<PosShapePair> ray::Object::hit(const ray::Ray &ray) const
{
    bool found = false;
    int index = 0;
    int current = 0;
    Math::Point3D hit;

    for (auto &triangle : _triangles) {
        auto maybeHit = triangle->hit(ray);
        if (maybeHit.has_value()) {
            if (found == false) {
                hit = maybeHit.value().first;
                current = index;
                found = true;
            } else {
                Math::Point3D closest = getClosest({hit, maybeHit.value().first}, ray.origin);

                if (closest == maybeHit.value().first) {
                    hit = maybeHit.value().first;
                    current = index;
                }
            }
        }
        index++;
    }
    if (found)
        return Maybe<std::pair<Math::Point3D,std::shared_ptr<ray::IShape>>>{
            std::make_pair(hit, _triangles[current])};
    return Maybe<std::pair<Math::Point3D, std::shared_ptr<ray::IShape>>>{};
}

Math::Vector3D ray::Object::getNormale(const Math::Point3D& point, const ray::Ray& camRay) const
{
    return _triangles[0]->getNormale(point, camRay);
}

ray::Ray ray::Object::getRefraction(
    __attribute__((unused))const std::shared_ptr<ray::IScene>& scene,
    Math::Point3D pos, Math::Vector3D dir) const
{
    return {pos + dir * 0.0001, dir};
}

void ray::Object::parseFace(std::istringstream &ss, std::string line, std::string file, std::shared_ptr<ray::IMaterial> currentMaterial)
{
    std::vector<Math::Vector3D> indices;
    std::regex pattern(R"((\d+)([/](\d+)?)?([/](\d+))?)");
    std::string temp;

    while (ss >> temp) {
        std::smatch matches;

        if (std::regex_match(temp, matches, pattern)) {
            Math::Vector3D tempVec = {-1, -1, -1};
            if (matches[1].matched)
                tempVec.X = std::stoi(matches[1]) - 1;
            if (matches[3].matched)
                tempVec.Y = std::stoi(matches[3]) - 1;
            if (matches[5].matched)
                tempVec.Z = std::stoi(matches[5]) - 1;
            indices.push_back(tempVec);
        } else {
            throw NodeError("IMaterial: Parse error in OBJ file [" + file + "]. Line [" + line + "] does not match Regex.", "Object.cpp");
        }
    }

    for (int i = 0; i < static_cast<int>(indices.size()) - 2; i++) {
        std::shared_ptr<ObjTriangle> triangle = std::make_shared<ObjTriangle>();
        triangle->setPoint(_points[(int)indices[0].X], _points[(int)indices[i + 1].X], _points[(int)indices[i + 2].X]);

        if (indices[0].Y != -1 &&
            indices[i + 1].Y != -1 &&
            indices[i + 2].Y != -1 && static_cast<int>(_uvValues.size()) >= i + 2) // Texture mapping
            triangle->setUvCoords(_uvValues[(int)indices[0].Y], _uvValues[(int)indices[i + 1].Y], _uvValues[(int)indices[i + 2].Y]);

        if (indices[0].Z != -1 &&
            indices[i + 1].Z != -1 &&
            indices[i + 2].Z != -1 && static_cast<int>(_normals.size()) >= i + 2) // Normal mapping
            triangle->setNormalMapCoords(_normals[(int)indices[0].Z], _normals[(int)indices[i + 1].Z], _normals[(int)indices[i + 2].Z]);
        triangle->initNormale(); // Mandatory, even if we have a normal mapping
        triangle->setMaterial(currentMaterial);
        triangle->initValues();
        triangle->setTransformationMatrix(_transformMatrix);
        _triangles.push_back(triangle);
    }
}

extern "C" ray::INode *create(std::map<std::string, std::string> &attributes)
{
    if (attributes.find("path") == attributes.end())
        throw std::runtime_error("Object node must have a path attribute\n");
    auto *object = new ray::Object();
    object->setPath(attributes.at("path"));
    return object;
}
