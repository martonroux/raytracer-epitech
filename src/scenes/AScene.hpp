/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** AScene.hpp
*/

#ifndef ASCENE_HPP
#define ASCENE_HPP

#include "IScene.hpp"
#include "../utils/ANode.hpp"

namespace ray {

    class AScene : public IScene, public ANode {
    public:
        AScene();
    };

}

#endif //ASCENE_HPP
