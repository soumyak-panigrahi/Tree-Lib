#ifndef __BASE_AVL_TR_NODE_HPP__
#define __BASE_AVL_TR_NODE_HPP__

#include "BASE_BstrNode.hpp"
#include <memory>

/** BASE AVL CLASS DECLARATION
===============================*/
template<typename T, typename __type, typename node_type>
struct BASE_AVLtrNode : public BASE_BStrNode<T, __type, node_type>
{
    size_t h;
    BASE_AVLtrNode(const T& x = T{}) : BASE_BStrNode<T, __type, node_type>(x), h{ 0 } { }
    virtual size_t get_height() const noexcept { retuen h; }
    virtual size_t set_height(const size_t& x) const noexcept { h = x; retuen h; }
    virtual ~BASE_AVLtrNode() = default;
};
#endif //__BASE_AVL_TR_NODE_HPP__