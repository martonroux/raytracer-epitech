/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** SuperMaterial.cpp
*/

#include "SuperMaterial.hpp"

namespace ray {

    sf::Image openImage(const std::string& path)
    {
        sf::Image image;
        if (!image.loadFromFile(path)) {
            throw ray::NodeError("IMaterial: Error happened while opening image at path [" + path + "].", "TextureMaterial.cpp");
        }
        return image;
    }

    SuperMaterial::SuperMaterial(
        double specularExponent,
        Math::Matrix<1, 3> ka,
        Math::Matrix<1, 3> ks,
        __attribute__((unused))Math::Matrix<1, 3> ke,
        double refractionIndex,
        double transparency,
        double shadowQuality,
        double ambiantOccQuality) : AMaterial(refractionIndex),
            _mode(SuperMaterialMode::NORMAL_KD),
            _ka(ka),
            _phong(
                {},
                0.05,
                specularExponent,
                shadowQuality,
                ambiantOccQuality,
                transparency,
                ks
            )
    {
    }

    RGB SuperMaterial::getColor(int recursive, Math::Point3D collisionPoint,
                                Math::Vector3D normale, Math::Point3D camPos,
                                const std::shared_ptr<ray::IShape> &shape,
                                const std::shared_ptr<ray::IScene> &scene) const
    {
        Math::Vector3D view = {camPos.X - collisionPoint.X, camPos.Y - collisionPoint.Y, camPos.Z - collisionPoint.Z};
        Math::Matrix<1, 3> thisKd;
        Math::Matrix<1, 3> thisKa;
        view /= view.length();

        switch (_mode) {
            case SuperMaterialMode::NORMAL_KD:
                thisKd = _kd;
                thisKa = _ka;
                break;
            case SuperMaterialMode::TEXTURE_KD: {
                Math::Vector2D uv = shape->getUVMapping(collisionPoint);
                sf::Color color = _img.getPixel(
                        static_cast<unsigned int>(uv.first * _img.getSize().x) % _img.getSize().x,
                        _img.getSize().y - (static_cast<unsigned int>(uv.second * _img.getSize().y) % _img.getSize().y)
                    );

                thisKd = Math::Matrix<1, 3>{{
                    {
                        static_cast<double>(color.r) / 255,
                        static_cast<double>(color.g) / 255,
                        static_cast<double>(color.b) / 255
                    }}};
                thisKa = thisKd;
                break;
            }
        }
        if (recursive > REFLECTION_RECURSION_LIMIT)
            return RGB{
                static_cast<unsigned int>(thisKd(0, 0) * 255),
                static_cast<unsigned int>(thisKd(0, 1) * 255),
                static_cast<unsigned int>(thisKd(0, 2) * 255)
            };
        return _phong.calculateColor(thisKd, thisKa, scene, shape, view, collisionPoint, normale, recursive);
    }

    void SuperMaterial::setSkybox()
    {
        _phong.setIa(1);
    }

    void SuperMaterial::setKd(const std::string &texture)
    {
        _img = openImage(texture);
        _mode = SuperMaterialMode::TEXTURE_KD;
    }

    void SuperMaterial::setKd(Math::Matrix<1, 3> kd)
    {
        _kd = kd;
        _mode = SuperMaterialMode::NORMAL_KD;
    }

}
