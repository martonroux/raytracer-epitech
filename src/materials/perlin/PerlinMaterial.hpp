/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** PerlinMaterial.hpp
*/

#ifndef PERLINMATERIAL_HPP
#define PERLINMATERIAL_HPP

#include <map>

#include "../AMaterial.hpp"
#include "../Phong.hpp"

namespace ray {

    class PerlinMaterial : public AMaterial {
        Math::Matrix<1, 3> _kd;
        Math::Matrix<1, 3> _ka = Math::Matrix<1, 3>({{1, 1, 1}});
        int _octaves;
        double _persistence;
        double _minPerlin;
        double _maxPerlin;
        Phong::Model _phong;
    public:
        // CONSTR DESTRUCT
        PerlinMaterial(RGB color,
            double refractionIndex,
            double shadowQuality,
            double ambiantOccQuality,
            double roughness,
            int octaves,
            double persistence,
            double minPerlin,
            double maxPerlin);
        ~PerlinMaterial() override = default;

        // RUNTIME
        [[nodiscard]] RGB getColor(
            int recursion,
            Math::Point3D collisionPoint,
            Math::Vector3D normale,
            Math::Point3D camPos,
            const std::shared_ptr<ray::IShape>& shape,
            const std::shared_ptr<ray::IScene> &scene) const override;

        void setSkybox();
    };

} // ray

extern "C" ray::INode *create(const std::map<std::string, std::string> &attributes);

#endif //PERLINMATERIAL_HPP
