//
// Created by Guillaume Tran on 09/05/2024.
//

#include "Pixelize.hpp"

int dither(int x, int y, int scale)
{
    int ditherMatrix[4][4] = {
        {0, 8, 2, 10},
        {12, 4, 14, 6},
        {3, 11, 1, 9},
        {15, 7, 13, 5}
    };
//    int ditherMatrix[4][4] = {
//            {4, 15, 4, 15},
//            {15, 4, 15, 4},
//            {4, 15, 4, 15},
//            {15, 4, 15, 4}
//    };
//    int ditherMatrix[4][4] = {
//            {16, 0, 16, 0},
//            {16, 16, 16, 16},
//            {16, 8, 16, 0},
//            {16, 16, 16, 16}
//    };
    return ditherMatrix[x][y] * scale;
}



std::vector<RGB> parsePallet(const std::string& pallet)
{
    std::vector<RGB> colors;
    std::ifstream file(pallet);

    if (!file.is_open())
        throw std::runtime_error("Could not open file: " + pallet);
    std::string line;
    std::getline(file, line);
    std::getline(file, line);
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        unsigned r = 0;
        unsigned g = 0;
        unsigned b = 0;
        ss >> r >> g >> b;
        colors.push_back(RGB{r, g, b});
    }
    return colors;
}

Image applyPalletFilter(Image image, image_data_t imageData)
{
    if (imageData.pallet == "none"){
        std::cout << "No pallet to apply" << std::endl;
        return image;
    }
    std::vector<RGB> pallet = parsePallet(imageData.pallet);
    Image newImage;
    int i = 0;
    for (const auto& pixel : image.getMap()) {
        if (i%100000 == 0)
            std::cout << "Applying pallet: " << i << "/" << image.getMap().size() << std::endl;
        i++;
        RGB color = pixel.second;
        color = RGB{color.R + dither((int)pixel.first.first % 4, (int)pixel.first.second % 4, 1),
                    color.G + dither((int)pixel.first.first % 4, (int)pixel.first.second % 4, 1),
                    color.B + dither((int)pixel.first.first % 4, (int)pixel.first.second % 4, 1)};
        double minDist = std::numeric_limits<double>::max();
        RGB closestColor;
        for (const auto& p : pallet) {
            double dist = sqrt((color.R - p.R) * (color.R - p.R) +
                    (color.G - p.G) * (color.G - p.G) +
                    (color.B - p.B) * (color.B - p.B));
            if (dist < minDist) {
                minDist = dist;
                closestColor = p;
            }
        }
        newImage.addPixel(pixel.first, closestColor);
    }
    return newImage;
}

Image applyFilter(Image image, image_data_t imageData)
{
    if (imageData.filter == "none"){
        std::cout << "No filter to apply" << std::endl;
        return image;
    }
    Image newImage;
    for (const auto& pixel : image.getMap()) {
        RGB color = pixel.second;
        unsigned int darken = (255) / ((color.R + color.G + color.B) / 4);
        if (darken + color.B > 255)
            darken = 255 - color.B;
        color = RGB{color.R, color.G, color.B + darken};
        newImage.addPixel(pixel.first, color);
    }
    return newImage;
}