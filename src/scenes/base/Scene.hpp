/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** Scene.hpp
*/

#ifndef SCENE_HPP
#define SCENE_HPP

#include <map>

#include "Maybe.hpp"
#include "../AScene.hpp"
#include "../../materials/IMaterial.hpp"
#include "../../transformations/ITransform.hpp"

namespace ray {

    class Scene : public AScene {
    private:
        template<typename T>
        [[nodiscard]] std::vector<std::shared_ptr<T>> getNodesType(ray::type type) const;
        std::vector<std::shared_ptr<ray::ILight>> _lights;
        std::vector<std::shared_ptr<ray::IShape>> _shapes;
        std::vector<std::shared_ptr<ray::ICamera>> _cameras;
        RGB _backgroundColor;

        void recursiveInitValues(std::shared_ptr<ray::INode> node);
    public:
        // CONSTR DESTRUCT
        explicit Scene(RGB backgroundColor) : _backgroundColor(backgroundColor) {}
        ~Scene() override = default;

        // RUNTIME
        Maybe<PosShapePair> hit(const ray::Ray &ray) const override;
        void initValues() override;

        // GETTERS
        [[nodiscard]] std::vector<std::shared_ptr<ray::ILight>> getLights() const override;
        [[nodiscard]] std::vector<std::shared_ptr<ray::IShape>> getShapes() const override;
        [[nodiscard]] std::vector<std::shared_ptr<ray::ICamera>> getCameras() const override;
        [[nodiscard]] RGB getBackgroundColor() const override;

        // STATIC
        [[nodiscard]] static std::shared_ptr<ray::IMaterial> getMaterial(const std::shared_ptr<ray::IShape>& shape);
        [[nodiscard]] static std::shared_ptr<ray::IMaterial> getMaterial(const ray::IShape& shape);
        [[nodiscard]] static Math::Matrix<4, 4> getTransformationMatrix(const std::shared_ptr<ray::INode>& node);
        [[nodiscard]] static Math::Matrix<4, 4> getTransformationMatrix(const ray::INode& node);
        [[nodiscard]] static Math::Point3D getPosition(const std::shared_ptr<ray::INode>& node);
        [[nodiscard]] static Math::Point3D getPosition(const ray::INode& node);
    };

} // ray


#endif //SCENE_HPP
