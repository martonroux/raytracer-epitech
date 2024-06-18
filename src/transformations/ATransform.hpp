/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** ATransform.hpp
*/

#ifndef ATRANSFORM_HPP
#define ATRANSFORM_HPP

#include "ITransform.hpp"
#include "../utils/ANode.hpp"

namespace ray {

    class ATransform : public ITransform, public ANode {
    protected:
        Math::Matrix<4, 4> _matrix;

        [[nodiscard]] static Math::Matrix<4, 4> getIdentityMatrix();

        ATransform();

    public:
        virtual ~ATransform() = default;

        [[nodiscard]] Math::Matrix<4, 4> getMatrix() const override { return _matrix; }
    };

} // ray

#endif //ATRANSFORM_HPP
