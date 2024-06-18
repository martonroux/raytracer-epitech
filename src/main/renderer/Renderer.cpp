/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-raytracer-robin.glaude
** File description:
** Renderer
*/

#include "Renderer.hpp"

ray::Renderer::Renderer()
{
    _width = 0;
    _height = 0;
}

ray::Renderer::~Renderer()
{
}

void ray::Renderer::getSize(const std::map<Math::Vector2D, RGB> &image)
{
    if (image.empty())
        throw std::runtime_error("Image map is empty");
    for (const auto& pair : image) {
        _width = std::max(_width, (int)pair.first.first);
        _height = std::max(_height, (int)pair.first.second);
    }
}

void ray::Renderer::renderPpmImage(Image image, std::string filename)
{
    auto color = RGB(0, 0, 0);
    std::ofstream file(filename);
    if (!file.is_open())
        throw RendererException("Can't open file");
    this->getSize(image.getMap());
    file << "P3\n";
    file << _width << " " << _height << "\n";
    file << "255\n";
    for (int i = 0; i < _height; i++) {
        for (int j = 0; j < _width; j++) {
            color = image.getPixel(Math::Vector2D(j, i));
            file << color.R << " " << color.G << " " << color.B;
            if (j != _width - 1)
                file << "\t";
        }
        file << "\n";
    }
    if (file.fail())
        throw std::runtime_error("Failed to write to file: " + filename);
    file.close();
}

void ray::Renderer::renderSfmlImage(Image &image)
{
    this->getSize(image.getMap());
    if (!_window.isOpen())
        _window.create(sf::VideoMode(_width, _height), "Raytracer", sf::Style::Close);
    _window.clear();
    this->drawPixels(image);
    _window.display();
    while (_window.isOpen()) {
        while (_window.pollEvent(_event)) {
            if (_event.type == sf::Event::Closed)
                _window.close();
            if (_event.type == sf::Event::KeyPressed && _event.key.code == sf::Keyboard::Escape)
                _window.close();
        }
    }
}

void ray::Renderer::drawPixels(Image &image)
{
    for (int i = 0; i < _height; i++) {
        for (int j = 0; j < _width; j++) {
            auto color = image.getPixel(Math::Vector2D(j, i));
            sf::RectangleShape pixel(sf::Vector2f(1, 1));
            pixel.setFillColor(sf::Color(color.R, color.G, color.B));
            pixel.setPosition(j, i);
            _window.draw(pixel);
        }
    }
}
