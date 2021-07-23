#ifndef __SPLAY_TREE_HPP__
#define __SPLAY_TREE_HPP__

#include "bsTree.hpp"
#include <memory>

/** Aliases For Node Pointers (Using the Convention pointed in NamingConvention.md)
====================================================================================*/
#ifndef __POINTER_ALIASES__
#define __POINTER_ALIASES__
template<typename T> using wPtr = std::weak_ptr<T>;
template<typename T> using sPtr = std::shared_ptr<T>;
#endif //__POINTER_ALIASES__

/** Splay Tree's Class Declaration
==================================*/
template<typename T, typename  __type = T, typename node_type = BStrNode<T, __type>>
class splayTree : public bsTree<T, __type, node_type>
{
    using bsTree<T, __type, node_type>::NIL;
    using bsTree<T, __type, node_type>::root;
    using bsTree<T, __type, node_type>::size;
public:
    splayTree() : bsTree<T, __type, node_type>() { }
    virtual void splay(const wPtr<node_type>&) noexcept;
    virtual wPtr<node_type> insert(const T&) override;
};

template<typename T, typename  __type, typename node_type>
void splayTree<T, __type, node_type>::splay(const wPtr<node_type>& ptr) noexcept
{
    if (!ptr.expired()) {
        auto x{ ptr.lock() }, p{ x->p.lock() };
        while (p != NIL) {
            if (p->left == x)
                this->rightRotate(p);
            else
                this->leftRotate(p);
            p = x->p.lock();
        }
    }
    return;
}

template<typename T, typename  __type, typename node_type>
wPtr<node_type> splayTree<T, __type, node_type>::insert(const T& k)
{
    auto x = bsTree<T, __type, node_type>::insert(k);
    this->splay(x);
    return x;
}

#endif //__SPLAY_TREE_HPP__