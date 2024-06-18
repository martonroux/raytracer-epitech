/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-raytracer-robin.glaude
** File description:
** IScene
*/


#ifndef ISCENE_HPP_
#define ISCENE_HPP_

#include <Maybe.hpp>
#include <math/MatrixN.hpp>
#include <math/Point3D.hpp>

#include "INode.hpp"
#include "../lights/ILight.hpp"
#include "../shapes/IShape.hpp"
#include "../cameras/ICamera.hpp"

using PosShapePair = std::pair<Math::Point3D, std::shared_ptr<ray::IShape>>;

namespace ray {

    class IScene : virtual public INode {
    public:
        // DESTRUCT
        virtual ~IScene() = default;

        // RUNTIME
        [[nodiscard]] virtual Maybe<PosShapePair> hit(const ray::Ray& ray) const = 0;

        // GETTERS
        [[nodiscard]] virtual std::vector<std::shared_ptr<ray::ILight>> getLights() const = 0;
        [[nodiscard]] virtual std::vector<std::shared_ptr<ray::IShape>> getShapes() const = 0;
        [[nodiscard]] virtual std::vector<std::shared_ptr<ray::ICamera>> getCameras() const = 0;
        [[nodiscard]] virtual RGB getBackgroundColor() const = 0;
    };

}

#endif /* !ISCENE_HPP_ */
