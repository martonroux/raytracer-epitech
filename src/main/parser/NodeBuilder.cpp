/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** NodeBuilder.cpp
*/

#include "NodeBuilder.hpp"

namespace ray {

    std::shared_ptr<libconfig::Config> NodeBuilder::openFile(const std::string& filename)
    {
        std::shared_ptr<libconfig::Config> cfg = std::make_shared<libconfig::Config>();
        try {
            cfg->readFile(filename.c_str());
        } catch (const libconfig::ParseException &pex) {
            std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                      << " - " << pex.getError() << std::endl;
        }
        return cfg;
    }

    std::shared_ptr<libconfig::Config> NodeBuilder::openContent(const std::string &content)
    {
        std::string tempFilename = "temp.cfg";
        std::ofstream tempFile(tempFilename);

        size_t lastChar = content.find_last_not_of('\n');
        std::string trimmedContent = content.substr(0, lastChar + 1);

        tempFile << trimmedContent;
        tempFile.close();

        auto cfg = std::make_shared<libconfig::Config>();
        try {
            cfg->readFile(tempFilename.c_str());
        } catch (const libconfig::ParseException& e) {
            throw NodeBuilderException("Failed to parse configuration: " + std::string(e.what()));
        }

        std::remove(tempFilename.c_str());

        return cfg;
    }

    void NodeBuilder::parseNodes(const std::shared_ptr<libconfig::Config>& cfg)
    {
        const libconfig::Setting& nodes = cfg->lookup("nodes");
        for (int i = 0; i < nodes.getLength(); ++i) {
            const libconfig::Setting& node = nodes[i];
            std::string id;
            std::string typePath;
            node.lookupValue("id", id);
            node.lookupValue("type", typePath);

            if (nodeMap.find(id) != nodeMap.end())
                throw NodeBuilderException("Duplicate node ID: " + id);

            // propriétés
            std::map<std::string, std::string> properties;
            try {
                const libconfig::Setting& props = node["properties"];
                for (int j = 0; j < props.getLength(); ++j) {
                    const std::string propName = props[j].getName();
                    properties[propName] = props[j].c_str();
                }
            } catch (const libconfig::SettingNotFoundException &nfex) {
                std::cerr << "Setting not found: " << nfex.getPath() << std::endl;
            } catch (const libconfig::SettingTypeException &tex) {
                std::cerr << "Type error in setting: " << tex.getPath() << std::endl;
            } catch (const std::exception &ex) {
                std::cerr << "General exception: " << ex.what() << std::endl;
            }

            auto createdNode = NodeFactory<INode>::create("plugins/" + typePath, properties);
            if (!createdNode) {
                throw NodeBuilderException("Failed to create node for ID " + id);
            }
            nodeMap[id] = createdNode;
        }
    }

    void NodeBuilder::buildTree(const libconfig::Setting &setting,
        const std::shared_ptr<INode>& parent)
    {
        std::string id;
        setting.lookupValue("id", id);
        if (nodeMap.find(id) == nodeMap.end()) {
            throw NodeBuilderException("Node ID not found: " + id + ". "
                "Keep in mind that nodes can only be used ONCE in the hierarchy, otherwise they "
                "will have multiple parents, which is impossible with our structure.");
        }
        auto node = nodeMap[id];

        nodeMap.erase(id);

        if (parent != nullptr) {
            parent->addChild(node);
        } else {
            rootNodes.push_back(node);
        }

        if (setting.exists("children")) {
            int count = setting["children"].getLength();
            for (int i = 0; i < count; ++i) {
                buildTree(setting["children"][i], node);
            }
        }
    }

    void NodeBuilder::parseHierarchy(const std::shared_ptr<libconfig::Config>& cfg)
    {
        const libconfig::Setting& configs = cfg->lookup("config");
        for (int i = 0; i < configs.getLength(); i++)
            buildTree(configs[i], nullptr);
    }

    void NodeBuilder::parseImageData(
        const std::shared_ptr<libconfig::Config> &cfg)
    {
        if (!cfg->exists("image"))
            throw NodeBuilderException("NodeBuilder: \"image\" block is missing in the configuration file.");
        const libconfig::Setting& image = cfg->lookup("image");

        if (image.getLength() != 1)
            throw NodeBuilderException("NodeBuilder: \"image\" block is wrongly formatted / empty.");

        image[0].lookupValue("file", imageData.filename);
        image[0].lookupValue("width", imageData.width);
        image[0].lookupValue("height", imageData.height);
        image[0].lookupValue("pallet", imageData.pallet);
        image[0].lookupValue("filter", imageData.filter);
        image[0].lookupValue("antiAliasing", imageData.antiAliasing);
        if (imageData.antiAliasing > 1) {
            imageData.width *= imageData.antiAliasing;
            imageData.height *= imageData.antiAliasing;
        }
    }

    NodeBuilder::NodeBuilder(const std::string &filename)
    {
        std::shared_ptr<libconfig::Config> cfg = openFile(filename);

        parseNodes(cfg);
        parseHierarchy(cfg);
        parseImageData(cfg);
    }

    NodeBuilder::NodeBuilder(const std::string &str, bool isContent)
    {
        std::shared_ptr<libconfig::Config> cfg;
        if (isContent) {
            cfg = openContent(str);
        } else {
            cfg = openFile(str);
        }

        parseNodes(cfg);
        parseHierarchy(cfg);
        parseImageData(cfg);
    }
}
