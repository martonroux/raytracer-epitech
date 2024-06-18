/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-raytracer-robin.glaude
** File description:
** parsing
*/

#ifndef PARSING_HPP_
#define PARSING_HPP_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <memory>
#include <libconfig.h++>
#include "INode.hpp"
#include "../src/main/parser/NodeFactory.hpp"
#include "../src/main/parser/NodeBuilder.hpp"

namespace ray {
    class CoreException : public std::exception {
        public:
            CoreException(const std::string& message) : _message(message) {}
            const char* what() const noexcept override { return _message.c_str(); }
        private:
            std::string _message;
        };
}

#endif /* !PARSING_HPP_ */
