#ifndef __BS_TREE_H__
#define __BS_TREE_H__

#include "BASE_BstrNode.hpp"
#include <iostream>
#include <memory>

/** Classes And Structs Delecratinon
====================================*/
template<typename T, typename __type> struct BStrNode;
template<typename T, typename __type, typename node_type> class bsTree;

/** Aliases For Node Pointers (Using the Convention pointed in NamingConvention.md)
====================================================================================*/
#ifndef __POINTER_ALIASES__
#define __POINTER_ALIASES__
template<typename T> using wPtr = std::weak_ptr<T>;
template<typename T> using sPtr = std::shared_ptr<T>;
#endif //__POINTER_ALIASES__

/** Binary Search Tree 's Node Concrete class Delecration
==========================================================*/
template<typename T, typename __type>
struct BStrNode : public BASE_BStrNode<T, __type, BStrNode<T, __type>>
{
    BStrNode(T x = T{}) : BASE_BStrNode<T, __type, BStrNode<T, __type>>() { }
    virtual ~BStrNode() = default;
};

/** Binary Search Tree's Class Deleclaration
=============================================*/
template<typename T, typename __type = T, typename node_type = BStrNode<T, __type>>
class bsTree : public I_BINARYtr
{
public:
    bsTree() : NIL{ std::make_shared<node_type>() }, root{ NIL }, size{ 0 } { }
    virtual size_t height(const wPtr<node_type>&) const noexcept;
    virtual void inorderTreeWalk() const noexcept;
    virtual void postorderTreeWalk() const noexcept;
    virtual void preorderTreeWalk() const noexcept;
    virtual wPtr<node_type> search(const T&) const noexcept;
    virtual void rightRotate(const wPtr<node_type>&) noexcept;
    virtual void leftRotate(const wPtr<node_type>&) noexcept;
    virtual wPtr<node_type> treeMax() const noexcept { return this->treeMax(root); }
    virtual wPtr<node_type> treeMax(const wPtr<node_type>&) const noexcept;
    virtual wPtr<node_type> treeMin() const noexcept { return this->treeMin(root); }
    virtual wPtr<node_type> treeMin(const wPtr<node_type>&) const noexcept;
    virtual wPtr<node_type> treeSuccessor(const wPtr<node_type>&) const noexcept;
    virtual wPtr<node_type> treePredecessor(const wPtr<node_type>&) const noexcept;
    virtual bool insert(const T&);
    virtual T remove(const wPtr<node_type>&) noexcept;
protected:
    const sPtr<node_type> NIL;
    sPtr<node_type> root;
    size_t size;
    virtual void transplant(const wPtr<node_type>&, const wPtr<node_type>&) noexcept;
};

/** Binary Search Tree's Class Implementation
=============================================*/

template<typename T, typename __type, typename node_type>
void bsTree<T, __type, node_type>::preorderTreeWalk() const noexcept
{
    auto x{ root }, prev{ x->p.lock() };
    while (x != NIL) {
        if (prev == x->p.lock()) {
            std::cout << x->data << " ";
            prev = x;
            if (x->left != NIL)
                x = x->left;
            else if (x->right != NIL)
                x = x->right;
            else
                x = x->p.lock();
        } else if (x->left == prev && x->right) {
            prev = x;
            x = x->right;
        } else {
            prev = x;
            x = x->p;
        }
    }
    return;
}

template<typename T, typename __type, typename node_type>
void bsTree<T, __type, node_type>::inorderTreeWalk() const noexcept
{
    auto x{ root }, prev{ x->p.lock() };
    while (x != NIL) {
        if (prev == x->p.lock && x->left) {
            prev = x;
            x = x->left;
        } else if ((prev == x->p.lock()) || (prev == x->left)) {
            std::cout << x->data << " ";
            prev = x;
            if (x->right != NIL)
                x = x->right;
            else
                x = x->p.lock();
        } else {
            prev = x;
            x = x->p.lock()
        }
    }
    return;
}

template<typename T, typename __type, typename node_type>
void bsTree<T, __type, node_type>::postorderTreeWalk() const noexcept
{
    auto x{ root }, prev{ x->p.lock() };
    while (x != NIL) {
        if (prev == x->p.lock && x->right) {
            prev = x;
            x = x->right;
        } else if ((prev == x->p.lock()) || (prev == x->right)) {
            std::cout << x->data << " ";
            prev = x;
            if (x->left != NIL)
                x = x->left;
            else
                x = x->p.lock();
        } else {
            prev = x;
            x = x->p.lock()
        }
    }
    return;
}

template<typename T, typename __type, typename node_type>
size_t bsTree<T, __type, node_type>::height(const wPtr<node_type>& ptr) const noexcept
{
    if (!ptr.expired || ptr.lock() == NIL) {
        auto x{ ptr.lock() };
        auto lh{ this->height(x->left) }, rh{ this->height(x->right) };
        if (lh > rh)
            return lh + 1;
        else
            return rh + 1;
    }
    return 0;
}

template<typename T, typename __type, typename node_type>
wPtr<node_type> bsTree<T, __type, node_type>::search(const T& k) const noexcept
{
    auto x{ root };
    while (x != NIL && x->data != k) {
        if (k > x->data)
            x = x->right;
        else
            x = x->left;
    }
    return x;
}

template<typename T, typename __type, typename node_type>
wPtr<node_type> bsTree<T, __type, node_type>::treeMax(const wPtr<node_type>& ptr) const noexcept
{
    if (!ptr.expired()) {
        auto x{ ptr.lock() };
        while (x->right != NIL)
            x = x->right;
        return x;
    }
    return ptr;
}

template<typename T, typename __type, typename node_type>
wPtr<node_type> bsTree<T, __type, node_type>::treeMin(const wPtr<node_type>& ptr) const noexcept
{
    if (!ptr.expired()) {
        auto x{ ptr.lock() }
            while (x->left != NIL)
                x = x->left;
        return x;
    }
    return ptr;
}

template<typename T, typename __type, typename node_type>
wPtr<node_type> bsTree<T, __type, node_type>::treeSuccessor(const wPtr<node_type>& ptr) const noexcept
{
    if (!ptr.expired()) {
        auto x{ ptr.lock() };
        if (x->right != NIL)
            return this->treeMin(x);
        else {
            auto p{ x->p.lock() };
            while (p != NIL) {
                if (p->left == x)
                    return p;
                x = p;
                p = x->p.lock();
            }
        }
    }
    return ptr;
}

template<typename T, typename __type, typename node_type>
wPtr<node_type> bsTree<T, __type, node_type>::treePredecessor(const wPtr<node_type>& ptr) const noexcept
{
    if (!ptr.expired()) {
        auto x{ ptr.lock() };
        if (x->left != NIL)
            return this->treeMax(x->left);
        else {
            auto p{ x->p.lock() };
            while (p != NIL) {
                if (p->right == x)
                    return p;
                x = p;
                p = x->p.lock();
            }
        }
    }
    return ptr;
}

template<typename T, typename __type, typename node_type>
bool bsTree<T, __type, node_type>::insert(const T& k)
{
    auto x{ root };
    auto y{ x };
    while (x != NIL) {
        y = x;
        if (k > x->data)
            x = x->right;
        else
            x = x->left;
    }
    auto z = std::make_shared<node_type>();
    if (!z)
        return false;
    z->data = k;
    z->left = z->right = NIL;
    z->p = y;
    if (root == NIL)
        root = z;
    else if (k > y->data)
        y->right = z;
    else
        y->left = z;
    size++;
    return true;
}

template<typename T, typename __type, typename node_type>
void bsTree<T, __type, node_type>::transplant(const wPtr<node_type>& dest, const wPtr<node_type>& src) noexcept
{
    if (!dest.expired() && !src.expired()) {
        auto u{ dest.lock() }, v{ src.lock() };
        auto p{ u->p.lock() };
        if (p == NIL)
            root = v;
        else if (p->left == u)
            p->left = v;
        else
            p->right = v;
        if (v != NIL)
            v->p = p;
    }
    return;
}

template<typename T, typename __type, typename node_type>
T bsTree<T, __type, node_type>::remove(const wPtr<node_type>& ptr) noexcept
{
    if (!ptr.expired()) {
        auto x{ ptr.lock() };
        if (x->left == NIL)
            this->transplant(x, x->right);
        else if (x->right == NIL)
            this->transplant(x, x->left);
        else {
            auto y{ this->treeMin(x->right).lock() };
            auto p{ y->p.lock() };
            if (p != x) {
                this->transplant(y, y->right);
                y->right = x->right;
                y->right->p = y;
            }
            this->transplant(x, y);
            y->left = x->left;
            y->left->p = y;
        }
        size--;
        return x->data:
    }
    return T{};
}

template<typename T, typename __type, typename node_type>
void bsTree<T, __type, node_type>::leftRotate(const wPtr<node_type>& ptr) noexcept
{
    if (!ptr.expired()) {
        auto x{ ptr.lock() }, y{ x->right };
        if (x != NIL && y != NIL) {
            x->right = y->left;
            if (y->left != NIL)
                y->left->p = x;
            auto p{ x->p.lock() };
            if (p != NIL) {
                if (p->left == x)
                    p->left = y;
                else
                    p->right = y;
            } else
                root = y;
            y->p = x->p;
            y->right = x;
            x->p = y;
        }
    }
    return;
}

template<typename T, typename __type, typename node_type>
void bsTree<T, __type, node_type>::rightRotate(const wPtr<node_type>& ptr) noexcept
{
    if (!ptr.expired()) {
        auto x{ ptr.lock() }, y{ x->left };
        if (x != NIL && y != NIL) {
            x->left = y->right;
            if (y->right != NIL)
                y->left->p = x;
            auto p{ x->p.lock() };
            if (p != NIL) {
                if (p->left == x)
                    p->left = y;
                else
                    p->right = y;
            } else
                root = y;
            y->p = x->p;
            y->left = x;
            x->p = y;
        }
    }
    return;
}

#endif // __BS_TREE_H__