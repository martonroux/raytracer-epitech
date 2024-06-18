//
// Created by Guillaume Tran on 09/05/2024.
//

#ifndef RAYTRACER_PIXELIZE_HPP
#define RAYTRACER_PIXELIZE_HPP

#include "Image.hpp"
#include "parsing.hpp"
#include "Renderer.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "utils/mainMethods.hpp"

class Pixelize {

};

Image applyPalletFilter(Image image, image_data_t imageData);
Image applyFilter(Image image, image_data_t imageData);
#endif //RAYTRACER_PIXELIZE_HPP
