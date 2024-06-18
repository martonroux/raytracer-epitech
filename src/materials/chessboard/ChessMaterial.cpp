/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** ChessBoard.cpp
*/

#include "ChessMaterial.hpp"

#include <random>

namespace ray {

    int checkerboard(int x, int y, int squareSize)
    {
        int xIndex = x / squareSize;
        int yIndex = y / squareSize;

        if ((xIndex + yIndex) % 2 == 0)
            return 1;
        return 0;
    }

    Math::Vector2D getRandomCoordinates()
    {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_real_distribution<double> dist(0, 1);
        return {dist(rng), dist(rng)};
    }

    ChessMaterial::ChessMaterial(RGB color,
        double refractionIndex, double shadowQuality,
        double ambiantOccQuality, double roughness, double chessSize,
        double minChess, double maxChess) : AMaterial(refractionIndex),
            _kd(Math::Matrix<1, 3>{{
                {
                    static_cast<double>(color.R) / 255,
                    static_cast<double>(color.G) / 255,
                    static_cast<double>(color.B) / 255
                }}}),
            _chessSize(chessSize),
            _minChess(minChess),
            _maxChess(maxChess),
            _phong({},
                0.05,
                50,
                shadowQuality,
                ambiantOccQuality,
                0,
                Math::Matrix<1, 3>{{{roughness, roughness, roughness}}})

    {
    }

    RGB ray::ChessMaterial::getColor(int recursion, Math::Point3D collisionPoint,
                                     Math::Vector3D normale, Math::Point3D camPos,
                                     const std::shared_ptr<ray::IShape> &shape,
                                     const std::shared_ptr<ray::IScene> &scene) const
    {
        Math::Vector2D pos = shape->getUVMapping(collisionPoint);
        Math::Vector3D view = {camPos.X - collisionPoint.X, camPos.Y - collisionPoint.Y, camPos.Z - collisionPoint.Z};
        view /= view.length();
        RGB color;
        int chess;

        if (recursion > REFLECTION_RECURSION_LIMIT)
            color = RGB(
                    static_cast<unsigned int>(_kd(0, 0) * 255),
                    static_cast<unsigned int>(_kd(0, 1) * 255),
                    static_cast<unsigned int>(_kd(0, 2) * 255)
                );
        else
            color = _phong.calculateColor(_kd, _ka, scene, shape, view, collisionPoint, normale, recursion);
        if (pos == Math::Vector2D{-1, -1})
            pos = getRandomCoordinates();
        else
            pos *= _chessSize;
        chess = checkerboard(
            static_cast<int>(pos.first),
            static_cast<int>(pos.second),
            1);
        if (chess == 0) {
            return {
                static_cast<unsigned int>(_minChess * color.R),
                static_cast<unsigned int>(_minChess * color.G),
                static_cast<unsigned int>(_minChess * color.B)
            };
        }
        return {
            static_cast<unsigned int>(_maxChess * color.R),
            static_cast<unsigned int>(_maxChess * color.G),
            static_cast<unsigned int>(_maxChess * color.B)
        };
    }

    void ChessMaterial::setSkybox()
    {
        _phong.setIa(1);
    }
} // ray

extern "C" ray::INode *create(const std::map<std::string, std::string> &attributes)
{
    if (attributes.find("color") == attributes.end())
        throw ray::NodeError("IMaterial: missing color attribute", "ChessMaterial.cpp");
    if (attributes.find("shadow_quality") == attributes.end())
        throw ray::NodeError("IMaterial: missing shadow_quality attribute (number between 0 and 100 for render time)", "ChessMaterial.cpp");
    if (attributes.find("ambiant_occlusion") == attributes.end())
        throw ray::NodeError("IMaterial: missing ambiant_occlusion attribute (number between 0 and 100 for render time)", "ChessMaterial.cpp");
    if (attributes.find("roughness") == attributes.end())
        throw ray::NodeError("IMaterial: missing roughness attribute (number between 0 and 1 for material roughness)", "ChessMaterial.cpp");
    if (attributes.find("chess_size") == attributes.end())
        throw ray::NodeError("IMaterial: missing chess_size attribute (number between 0 and +inf for Chessboard size)", "ChessMaterial.cpp");
    if (attributes.find("min_chess") == attributes.end())
        throw ray::NodeError("IMaterial: missing min_perlin attribute (number between 0 and 1 for Chessboard min color)", "ChessMaterial.cpp");
    if (attributes.find("max_chess") == attributes.end())
        throw ray::NodeError("IMaterial: missing max_perlin attribute (number between 0 and 1 for Chessboard max color)", "ChessMaterial.cpp");

    Maybe<RGB> color = RGB::fromStr(attributes.at("color"));
    double shadowQuality = std::stod(attributes.at("shadow_quality"));
    double ambiantOcclusion = std::stod(attributes.at("ambiant_occlusion"));
    double roughness = std::stod(attributes.at("roughness"));
    double chessSize = std::stod(attributes.at("chess_size"));
    double minChess = std::stod(attributes.at("min_chess"));
    double maxChess = std::stod(attributes.at("max_chess"));

    if (!color.has_value())
        throw ray::NodeError("IMaterial: invalid color attribute", "ChessMaterial.cpp");
    if (shadowQuality < 1 || shadowQuality > 100)
        throw ray::NodeError("IMaterial: shadow_quality must be a number between 1 and 100", "ChessMaterial.cpp");
    if (ambiantOcclusion < 0 || ambiantOcclusion > 100)
        throw ray::NodeError("IMaterial: ambiant_occlusion must be a number between 0 and 100", "ChessMaterial.cpp");
    if (roughness < 0 || roughness > 1)
        throw ray::NodeError("IMaterial: roughness must be a number between 0 and 1", "ChessMaterial.cpp");
    if (chessSize < 0)
        throw ray::NodeError("IMaterial: chess_size must be a number between 0 and 1", "ChessMaterial.cpp");
    if (minChess < 0 || minChess > 1)
        throw ray::NodeError("IMaterial: min_chess must be a number between 0 and 1", "ChessMaterial.cpp");
    if (maxChess < 0 || maxChess > 1)
        throw ray::NodeError("IMaterial: max_chess must be a number between 0 and 1", "ChessMaterial.cpp");

    ray::INode* node = new ray::ChessMaterial(color.value(), 1, shadowQuality, ambiantOcclusion, roughness, chessSize, minChess, maxChess);

    if (attributes.find("skybox") != attributes.end() && attributes.at("skybox") == "true")
        dynamic_cast<ray::ChessMaterial*>(node)->setSkybox();
    return node;
}
