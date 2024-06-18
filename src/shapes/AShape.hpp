/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-raytracer-robin.glaude
** File description:
** AShape
*/

#ifndef ASHAPE_HPP_
#define ASHAPE_HPP_

#include "IShape.hpp"
#include "../../include/Ray.hpp"
#include "../../include/Maybe.hpp"
#include "../utils/ANode.hpp"
#include "../../include/math/MatrixN.hpp"

namespace ray {

    class AShape : public ray::ANode, public ray::IShape {
    protected:
        Math::Matrix<4, 4> _transformMatrix;
        std::shared_ptr<ray::IMaterial> _material;
        AShape() : ANode(ray::type::SHAPE) {}

        // INTERNAL FUNCTIONS
        Math::Point3D getPosition() const;
        Math::Vector3D getScale() const;
        Math::Matrix<3,3> getRotation() const;
        std::shared_ptr<ray::IMaterial> getMaterial() const override;
        void applyMatrix();

    public:
        virtual ~AShape() = default;

        void initValues() override;

        [[nodiscard]] Math::Vector2D getUVMapping(Math::Point3D coords) const override;
    };
}

#endif /* !ASHAPE_HPP_ */
