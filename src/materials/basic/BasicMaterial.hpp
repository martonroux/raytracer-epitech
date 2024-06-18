/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** RedBasicMaterial.hpp
*/

#ifndef REDBASICMATERIAL_HPP
#define REDBASICMATERIAL_HPP

#include "../IMaterial.hpp"
#include "INode.hpp"
#include "RGB.hpp"
#include "../AMaterial.hpp"
#include "../Phong.hpp"
#include "../../lights/ILight.hpp"
#include "../../scenes/base/Scene.hpp"

namespace ray {

    class BasicMaterial : public ray::AMaterial {
        Math::Matrix<1, 3> _kd;
        Math::Matrix<1, 3> _ka = Math::Matrix<1, 3>({{1, 1, 1}});
        Phong::Model _phong;
    public:
        BasicMaterial(RGB color,
            double refractionIndex, double shadowQuality,
            double ambiantOccQuality, double roughness);
        ~BasicMaterial() override = default;

        [[nodiscard]] RGB getColor(int recursive, Math::Point3D collisionPoint,
            Math::Vector3D normale, Math::Point3D camPos,
            const std::shared_ptr<ray::IShape>& shape,
            const std::shared_ptr<ray::IScene>& scene) const override;

        void setSkybox();
    };

}

#endif //REDBASICMATERIAL_HPP
