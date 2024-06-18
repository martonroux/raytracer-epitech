/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-raytracer-robin.glaude
** File description:
** INode
*/

#ifndef INODE_HPP_
#define INODE_HPP_

#include <memory>

namespace ray {

    enum class type {
        UNDEFINED = 0,
        SCENE = 1,
        SHAPE = 2,
        LIGHT = 3,
        CAMERA = 4,
        MATERIAL = 5,
        TRANSFORM = 6,
    };

    class INode : public std::enable_shared_from_this<INode> {
    public:
        virtual ~INode() = default;

        // SETTERS
        virtual void addChild(std::shared_ptr<INode> node) = 0;
        virtual void removeChild(std::shared_ptr<INode> node) = 0;
        virtual void setParent(std::shared_ptr<INode> node) = 0;
        virtual void setType(ray::type type) = 0;

        [[nodiscard]] virtual std::shared_ptr<INode> getShared() = 0;

        virtual void initValues() = 0;

        // GETTERS
        [[nodiscard]] virtual std::shared_ptr<INode> getChild(int index) const = 0;
        [[nodiscard]] virtual std::shared_ptr<INode> getParent() const = 0;
        [[nodiscard]] virtual type getType() const = 0;
    };
}

#endif /* !INODE_HPP_ */
