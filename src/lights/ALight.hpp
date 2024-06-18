/*
** EPITECH PROJECT, 2024
** raytracer_bootstrap
** File description:
** ALight.hpp
*/

#ifndef ALIGHT_HPP
#define ALIGHT_HPP

#include "ILight.hpp"
#include "../utils/ANode.hpp"

namespace ray {

    class ALight : public ILight, public ANode {
    protected:
        RGB _color;

        explicit ALight(RGB color) : ANode(type::LIGHT), _color(color) {}

    public:
        [[nodiscard]] RGB getColor() const override;
    };

}

#endif //ALIGHT_HPP
