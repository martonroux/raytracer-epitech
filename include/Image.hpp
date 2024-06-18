/*
** EPITECH PROJECT, 2024
** raytracer_bootstrap
** File description:
** Pixel.hpp
*/

#ifndef PIXEL_HPP
#define PIXEL_HPP

#include <map>

#include "RGB.hpp"
#include "math/Vector2D.hpp"

class Image {
private:
    std::map<Math::Vector2D, RGB> _map;

public:
    // CONSTR DESTRUCT
    Image() = default;
    ~Image() = default;

    // SETTERS
    void addPixel(Math::Vector2D pos, RGB color) { _map[pos] = color; }

    // GETTERS
    [[nodiscard]] RGB getPixel(Math::Vector2D pos) const { return _map.at(pos); }
    [[nodiscard]] std::map<Math::Vector2D, RGB> getMap() const { return _map; }
};

#endif
