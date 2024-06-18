/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** ITransform.hpp
*/

#ifndef ITRANSFORM_HPP
#define ITRANSFORM_HPP

#include "math/MatrixN.hpp"
#include "INode.hpp"

namespace ray {

    class ITransform : virtual public ray::INode {
    public:
        virtual ~ITransform() = default;

        [[nodiscard]] virtual Math::Matrix<4, 4> getMatrix() const = 0;
    };

}

#endif
