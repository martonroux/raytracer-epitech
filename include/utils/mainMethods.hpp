/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-raytracer-robin.glaude
** File description:
** mainMethods
*/

#ifndef MAINMETHODS_HPP_
#define MAINMETHODS_HPP_

#include "RGB.hpp"
#include "Ray.hpp"
#include "../../src/scenes/IScene.hpp"
#include "../../src/cameras/ICamera.hpp"
#include "INode.hpp"
#include "../../src/materials/IMaterial.hpp"
#include "../../src/cameras/base/Camera.hpp"
#include "../../src/scenes/base/Scene.hpp"

namespace ray {
    class RayTracerUtils {
        public:
            static std::shared_ptr<ray::INode> getScene(const std::vector<std::shared_ptr<ray::INode>>& nodes)
            {
                for (auto node : nodes) {
                    if (node->getType() == ray::type::SCENE) {
                        return node;
                    }
                }
                return nullptr;
            }

            static std::shared_ptr<ray::ICamera> getCamera(const std::shared_ptr<ray::IScene>& scene)
            {
                auto cameras = scene->getCameras();
                return cameras[0];
            }

            static RGB getHitColor(const PosShapePair& hit, ray::Ray ray, const std::shared_ptr<ray::IScene>& scene)
            {
                std::shared_ptr<ray::IMaterial> material = hit.second->getMaterial();

                return material->getColor(0, hit.first, hit.second->getNormale(hit.first, ray), ray.origin, hit.second, scene);
            }

            static RGB renderPixel(const std::shared_ptr<ray::IScene>& scene, const std::shared_ptr<ray::ICamera>& cam, double u, double v, RGB backgroundColor)
            {
                ray::Ray r = cam->ray(u, v);
                Maybe<PosShapePair> hit = scene->hit(r);

                if (hit.has_value() == false) {
                    return backgroundColor;
                } else {
                    return getHitColor(hit.value(), r, scene);
                }
            }

            static std::vector<std::string> renderTokenSpliter(const std::string &s, char type)
            {
                std::vector<std::string> tokens;
                std::string token;
                std::istringstream tokenStream(s);

                while (std::getline(tokenStream, token, type))
                    tokens.push_back(token);
                return tokens;
            }
    };
}

#endif /* !MAINMETHODS_HPP_ */
