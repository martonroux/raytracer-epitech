/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** ACamera.hpp
*/

#ifndef ACAMERA_HPP
#define ACAMERA_HPP

#include "ICamera.hpp"
#include "../utils/ANode.hpp"

namespace ray {
    class ACamera : public ANode, public ICamera {
    protected:
        ACamera() : ANode(ray::type::CAMERA) {}

    public:
        virtual ~ACamera() = default;
    };
}


#endif //ACAMERA_HPP
