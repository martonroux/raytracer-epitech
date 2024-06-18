/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-raytracer-robin.glaude
** File description:
** IMaterial
*/


#ifndef IMATERIAL_HPP_
#define IMATERIAL_HPP_

#define REFLECTION_RECURSION_LIMIT 4

#include <math/MatrixN.hpp>

#include "math/Vector2D.hpp"
#include "INode.hpp"
#include "RGB.hpp"
#include "math/Point3D.hpp"

namespace ray {

    class IScene;
    class IShape;

    class IMaterial : virtual public INode {
    public:
        virtual ~IMaterial() = default;

        [[nodiscard]] virtual RGB getColor(int recursion, Math::Point3D collisionPoint,
            Math::Vector3D normale, Math::Point3D camPos,
            const std::shared_ptr<ray::IShape>& shape,
            const std::shared_ptr<ray::IScene>& scene) const = 0;

        [[nodiscard]] virtual double getRefractionIndex() const = 0;
        [[nodiscard]] virtual bool hasNormalMap() const = 0;
        [[nodiscard]] virtual Math::Vector3D normalFromMap(Math::Vector2D uv, Math::Matrix<3, 3> rotationMatrix) const = 0;

        virtual void setNormalMap(std::string normalMap) = 0;
    };

}

#endif /* !IMATERIAL_HPP_ */
