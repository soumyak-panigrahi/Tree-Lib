#ifndef __BASE_RBTRNODE_HPP__
#define __BASE_RBTRNODE_HPP__

#include "BASE_BstrNode.hpp"
#include <memory>

template<typename T, typename __type, typename node_type>
struct BASE_RBtrNode : public BASE_BStrNode<T, __type, node_type>
{
    bool color;
    BASE_RBtrNode(const T& x = T{}) : BASE_BStrNode<T, __type, node_type>(x), color{ false } { }
    virtual bool set_color(bool c) noexcept { color = c; return color; }
    virtual bool get_color(bool c) const noexcept { return color; }
};

#endif //__BASE_RBTRNODE_HPP__