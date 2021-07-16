#ifndef __BASE_BINARY_SEARCH_TREE_NODE_H__
#define __BASE_BINARY_SEARCH_TREE_NODE_H__

#include "binaryTree.h"
#include <memory>

/** Aliases
===========*/
#ifndef __POINTER_ALIASES__
#define __POINTER_ALIASES__
template<typename T> using wPtr = std::weak_ptr<T>;
template<typename T> using sPtr = std::shared_ptr<T>;
#endif //__POINTER_ALIASES__

/** Binary Search Tree Base Class
==================================*/
template<typename T, typename __type, typename node_type>
struct BASE_BStrNode : public I_BINARYtrNode<T, __type, node_type>
{
    T data;
    std::weak_ptr<node_type> p;
    std::shared_ptr<node_type> left, right;

    BASE_BStrNode(const T& x = T{}) : data{ x }, left{ nullptr }, right{ nullptr } { }

    virtual const __type read() const noexcept { __type temp{ data }; return temp }
    virtual bool write(const __type& x) { data = x; return true; }

    virtual bool set_data(const T&);
    virtual T get_data() const noexcept { return data; }

    virtual std::weak_ptr<node_type> get_parent() const noexcept { return p; }
    virtual bool set_parent(std::weak_ptr<node_type>&);

    virtual std::weak_ptr<node_type> get_left() const noexcept { return left; }
    virtual bool set_left(std::weak_ptr<node_type>&);

    virtual std::weak_ptr<node_type> get_right() const noexcept { return right; }
    virtual bool set_right(std::weak_ptr<node_type>&);

    ~BASE_BStrNode() = default;
};

/* Binary Search Tree Base Class Implementation
===============================================*/

template<typename T, typename __type, typename node_type>
bool BASE_BStrNode<T, __type, node_type>::set_data(const T& x)
{
    data = x;
    return true;
}

template<typename T, typename __type, typename node_type>
bool BASE_BStrNode<T, __type, node_type>::set_parent(std::weak_ptr<node_type>& ptr)
{
    if (!ptr.expired()) {
        p = ptr;
        return true;
    }
    return false;
}

template<typename T, typename __type, typename node_type>
bool BASE_BStrNode<T, __type, node_type>::set_left(std::weak_ptr<node_type>& ptr)
{
    if (!ptr.expired()) {
        left = ptr;
        return true;
    }
    return false;
}

template<typename T, typename __type, typename node_type>
bool BASE_BStrNode<T, __type, node_type>::set_right(std::weak_ptr<node_type>& ptr)
{
    if (!ptr.expired()) {
        right = ptr;
        return true;
    }
    return false;
}


#endif //__BASE_BINARY_SEARCH_TREE_NODE_H__