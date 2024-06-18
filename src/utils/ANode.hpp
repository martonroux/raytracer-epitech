/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-raytracer-robin.glaude
** File description:
** ANode
*/

#ifndef B_OOP_400_MPL_4_1_RAYTRACER_ROBIN_GLAUDE_ANODE_HPP
#define B_OOP_400_MPL_4_1_RAYTRACER_ROBIN_GLAUDE_ANODE_HPP

#include "../../include/INode.hpp"
#include <iostream>
#include <utility>
#include <vector>
#include <exception>
#include <algorithm>

namespace ray {

    class NodeError : public std::exception {
        std::string _msg;
        std::string _where;
    public:
        NodeError(std::string msg, std::string where) : _msg(std::move(msg)), _where(std::move(where)) {}
        const char * what() const noexcept override { return _msg.c_str(); }
        const char * where() const noexcept { return _where.c_str(); }
    };

    class ANode : virtual public INode {
    private:
        std::shared_ptr<ray::INode> _parent;
        ray::type type;
        std::vector<std::shared_ptr<ray::INode>> children;
        std::string _name;

    protected:
        explicit ANode(ray::type type) : type(type) {}

    public:
        virtual ~ANode() = default;

        // SETTERS
        void addChild(std::shared_ptr<INode> child) override;
        void removeChild(std::shared_ptr<INode> child) override;
        void setParent(std::shared_ptr<INode> parent) override;
        void setType(ray::type type) override;
        void setName(const std::string &name);

        [[nodiscard]] std::shared_ptr<INode> getShared() override;

        void initValues() override;

        // GETTERS
        [[nodiscard]] std::shared_ptr<ray::INode> getChild(int index) const override;
        [[nodiscard]] std::shared_ptr<ray::INode> getParent() const override;
        [[nodiscard]] ray::type getType() const override;
        [[nodiscard]] std::string getName() const;
    };
}

#endif //B_OOP_400_MPL_4_1_RAYTRACER_ROBIN_GLAUDE_ANODE_HPP
