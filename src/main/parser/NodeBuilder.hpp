/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-raytracer-robin.glaude
** File description:
** NodeBuilder
*/

#include "NodeFactory.hpp"

#ifndef NODEBUILDER_HPP_
#define NODEBUILDER_HPP_

#include <libconfig.h++>
#include <map>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include "RGB.hpp"

typedef struct image_data_s {
    std::string filename;
    unsigned int width = 0;
    unsigned int height = 0;
    std::string pallet = "none";
    std::string filter = "none";
    unsigned int antiAliasing = 1;
} image_data_t;

namespace ray {
    class NodeBuilderException : public std::exception {
        public:
            NodeBuilderException(const std::string& message) : _message(message) {}

            const char* what() const noexcept override { return _message.c_str(); }

        private:
            std::string _message;
    };

    class NodeBuilder {
    private:
        std::map<std::string, std::shared_ptr<INode>> nodeMap;
        std::vector<std::shared_ptr<INode>> rootNodes;
        image_data_t imageData;

        static std::shared_ptr<libconfig::Config> openFile(const std::string& filename);
        static std::shared_ptr<libconfig::Config> openContent(const std::string& content);
        void parseNodes(const std::shared_ptr<libconfig::Config>& cfg);
        void buildTree(const libconfig::Setting& setting, const std::shared_ptr<INode>& parent);
        void parseHierarchy(const std::shared_ptr<libconfig::Config>& cfg);
        void parseImageData(const std::shared_ptr<libconfig::Config>& cfg);

    public:
        explicit NodeBuilder(const std::string& filename);
        explicit NodeBuilder(const std::string& content, bool isContent);

        [[nodiscard]] const std::vector<std::shared_ptr<INode>>& getRootNodes() const { return rootNodes; }
        [[nodiscard]] image_data_t getImageData() const { return imageData; }
    };
}

#endif /* !NODEBUILDER_HPP_ */
