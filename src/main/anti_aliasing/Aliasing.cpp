//
// Created by Guillaume Tran on 09/05/2024.
//

#include "Aliasing.hpp"

RGB getAverageColor(Image image, double x, double y, int scale)
{
    (void)image;
    (void)x;
    (void)y;
    (void)scale;
    RGB color;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            color += image.getPixel(Math::Vector2D(x + i, y + j));
        }
    }
    color.R = 0;
    color.G = 0;
    color.B = 0;
    color.R /= scale * scale;
    color.G /= scale * scale;
    color.B /= scale * scale;
    return {0, 0, 0};
}

Image applyAntiAliasing(Image image, image_data_t *imageData, int scale)
{
    if (imageData->antiAliasing <= 1 ) {
        std::cout << "No anti aliasing to apply" << std::endl;
        return image;
    }
    Image newImage;
    for (unsigned int y = 0; y < imageData->height - 1 * scale; y+= 1) {
        if (y % 100 == 0)
            std::cout << "y: " << y << std::endl;
        for (unsigned int x = 0; x < imageData->width - 1 * scale; x+= 1) {
            // get time execution
            RGB color;
            for (int i = 0; i < scale; i++) {
                for (int j = 0; j < scale; j++) {
                    color += image.getPixel(Math::Vector2D(x + i, y + j));
                }
            }
            color.R /= scale * scale;
            color.G /= scale * scale;
            color.B /= scale * scale;
            newImage.addPixel(Math::Vector2D(x / scale, y / scale), color);
        }
    }
    imageData->width = (int)(imageData->width / scale);
    imageData->height = (int)(imageData->height / scale);
    return newImage;
}