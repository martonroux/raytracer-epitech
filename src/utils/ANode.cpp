/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-raytracer-robin.glaude
** File description:
** ANode
*/

#include "ANode.hpp"

void ray::ANode::addChild(std::shared_ptr<INode> child)
{
    if (child == nullptr)
        return;
    child->setParent(shared_from_this());
    children.push_back(child);
}

void ray::ANode::removeChild(std::shared_ptr<INode> child)
{
    if (child == nullptr)
        return;
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

void ray::ANode::setParent(std::shared_ptr<INode> parent)
{
    _parent = parent;
}

void ray::ANode::setType(ray::type type)
{
    ANode::type = type;
}

void ray::ANode::setName(const std::string &name)
{
    _name = name;
}


std::shared_ptr<ray::INode> ray::ANode::getShared()
{
    return shared_from_this();
}

void ray::ANode::initValues()
{
}

std::shared_ptr<ray::INode> ray::ANode::getChild(int index) const
{
    if (index < 0 || index >= static_cast<int>(children.size()))
        return nullptr;
    return children[index];
}

std::shared_ptr<ray::INode> ray::ANode::getParent() const
{
    return _parent;
}

ray::type ray::ANode::getType() const
{
    return type;
}

std::string ray::ANode::getName() const
{
    return _name;
}
