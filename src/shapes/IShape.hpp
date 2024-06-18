/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-raytracer-robin.glaude
** File description:
** IShape
*/


#ifndef ISHAPE_HPP_
#define ISHAPE_HPP_

#include <math/Vector2D.hpp>

#include "INode.hpp"
#include "Ray.hpp"
#include "Maybe.hpp"
#include "../materials/IMaterial.hpp"

using PosShapePair = std::pair<Math::Point3D, std::shared_ptr<ray::IShape>>;

namespace ray {

    class IShape : virtual public INode {
    public:
        virtual ~IShape() = default;

        [[nodiscard]] virtual Maybe<PosShapePair> hit(const ray::Ray& ray) const = 0;
        [[nodiscard]] virtual Math::Vector3D getNormale(const Math::Point3D& point, const ray::Ray& camRay) const = 0;
        [[nodiscard]] virtual std::shared_ptr<ray::IMaterial> getMaterial() const = 0;

        [[nodiscard]] virtual Math::Vector2D getUVMapping(Math::Point3D coords) const = 0;
        [[nodiscard]] virtual ray::Ray getRefraction(
                                const std::shared_ptr<ray::IScene>& scene,
                                Math::Point3D pos, Math::Vector3D dir) const = 0;
    };

}

#endif /* !ISHAPE_HPP_ */
