//
// Created by Guillaume Tran on 09/05/2024.
//

#ifndef RAYTRACER_ALIASING_HPP
#define RAYTRACER_ALIASING_HPP

#include "Image.hpp"
#include "parsing.hpp"
#include "Renderer.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "utils/mainMethods.hpp"


class Aliasing {

};

Image applyAntiAliasing(Image image, image_data_t *imageData, int scale);

#endif //RAYTRACER_ALIASING_HPP
