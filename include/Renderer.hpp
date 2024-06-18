/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-raytracer-robin.glaude
** File description:
** Renderer
*/

#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include "Image.hpp"
#include "RGB.hpp"
#include "math/Vector2D.hpp"

#include <iostream>
#include <exception>
#include <string>
#include <fstream>
#include <algorithm>
#include <SFML/Graphics.hpp>

namespace ray {
    class RendererException : public std::exception {
        public:
            RendererException(const std::string &message) : _message(message) {}
            const char *what() const noexcept override { return _message.c_str(); }
        private:
            std::string _message;
    };

    class Renderer {
        public:
            Renderer();
            ~Renderer();
            void getSize(const std::map<Math::Vector2D, RGB> &image);
            void renderPpmImage(Image image, std::string filename);
            void renderSfmlImage(Image &image);
            void drawPixels(Image &image);
        private:
            int _width;
            int _height;
            sf::Event _event;
            sf::RenderWindow _window;
    };
}

#endif /* !RENDERER_HPP_ */
