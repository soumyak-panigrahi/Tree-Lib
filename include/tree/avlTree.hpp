#ifndef __AVL_TREE_HPP__
#define __AVL_TREE_HPP__

#include "BASE_AVLtrNode.hpp"
#include "bsTree.hpp"
#include <memory>

/** Classes And Structs Delecratinon
====================================*/
template<typename T, typename __type> struct AVLtrNode;
template<typename T, typename __type, typename node_type> class avlTree;

/** Aliases For Node Pointers (Using the Convention pointed in NamingConvention.md)
====================================================================================*/
#ifndef __POINTER_ALIASES__
#define __POINTER_ALIASES__
template<typename T> using wPtr = std::weak_ptr<T>;
template<typename T> using sPtr = std::shared_ptr<T>;
#endif //__POINTER_ALIASES__

/** AVL Tree 's Node Concrete Class Declaration
========================================================*/
template<typename T, typename __type>
struct AVLtrNode : public BASE_AVLtrNode<T, __type, AVLtrNode<T, __type>>
{
    AVLtrNode(const T& x = T{}) : BASE_AVLtrNode<T, __type, AVLtrNode<T, __type>>(x) { }
    virtual ~AVLtrNode() = default;
};

/** AVL Tree Class Declaration
===============================*/
template<typename T, typename __type = T, typename node_type = AVLtrNode<T, __type>>
class avlTree : public bsTree<T, __type, node_type>
{
    using bsTree<T, __type, node_type>::NIL;
    using bsTree<T, __type, node_type>::root;
    using bsTree<T, __type, node_type>::size;
public:
    rbTree() : bsTree<T, __type, node_type>() { }
    virtual wPtr<node_type> insert(const T&) override;
    virtual T remove(const wPtr<node_type>&) noexcept override;
protected:
    virtual void balance(const wPtr<node_type>&) noexcept;
    virtual void fixUp(const wPtr<node_type>&) noexcept;
};

/** AVL Tree Class Implementation
=========================================*/

template<typename T, typename __type, typename node_type>
wPtr<node_type> avlTree<T, __type, node_type>::insert(const T& k)
{
    auto ptr = bsTree<T, __type, node_type>::insert(k);
    this->fixUp(ptr);
    return ptr;
}

template<typename T, typename __type, typename node_type>
T avlTree<T, __type, node_type>::remove(const wPtr<node_type>& ptr) noexcept
{
    if (!ptr.expired()) {
        auto x{ ptr.lock }, y{ x };
        if (x->left == NIL)
            y = x->right;
        else if (x->right == NIL)
            y = x->left;
        else {
            y = this->treeMin(x->right);
            y->h = x->h;
            y = y->right;
        }
        auto removed_data = bsTree<T, __type, node_type>::remove(ptr);
        this->fixUp(y);
        return removed_data;
    }
    return T{};
}

template<typename T, typename __type, typename node_type>
void avlTree<T, __type, node_type>::balance(const wPtr<node_type>& ptr) noexcept
{
    if (!ptr.expired()) {
        auto x{ ptr.lock() };
        if (x != NIL) {
            if ((x->left->h > x->right->h) && ((x->left->h - x->right->h) == 2)) {
                /**
                 * @brief In this balancing condition is not satisfied i.e height difference in 1 or 0.
                 *
                 * @note In the the height difference is 2. lA is left child of A and rA right child of A
                 *
                 * @attention The tree looks like
                 *                                   |
                 *                                   |h+3
                 *                                   |
                 *                                  (X)
                 *                                 /    \
                 *                             h+2/      \h
                 *                               /        \
                 *                            (lX)        (rX)
                 *
                 * So, now we have to right rotate at X, as to balance the height. Before we do that, we have to consider, lX
                 * children height.
                 *
                 * let rlX.h > llX.h
                 *
                 * @attention The tree looks like
                 *                                   |
                 *                                   |h+3
                 *                                   |
                 *                                  (X)
                 *                                 /    \
                 *                             h+2/      \h
                 *                               /        \
                 *                            (lX)        (rX)
                 *                           /    \
                 *                         h/      \h+1
                 *                         /        \
                 *                      (llX)        (rlX)
                 *
                 * @note both cannot be equal as the balance is appiled by inserting a node.
                 *
                 * Now, suppose we right rotate at X.
                 *
                 * @attention The tree looks like
                 *                                   |
                 *                                   |h+3
                 *                                   |
                 *                                  (lX)
                 *                                 /    \
                 *                               h/      \h+2
                 *                               /        \
                 *                           (llX)        (X)
                 *                                       /   \
                 *                                   h+1/     \h
                 *                                     /       \
                 *                                 (rlX)        (rX)
                 *
                 *
                 * So, AVL tree is still unbalanced. So, the conclusion is the right child of lX be the smaller than it's left.
                 * And it can be easily done by left rotating at lX, as to like to increase left sub tree height.
                 *
                 * @attention The tree looks like
                 *
                 *                                  (X)
                 *                                 /    \
                 *                             h+2/      \h
                 *                               /        \
                 *                           (rlX)        (rX)
                 *                           /    \
                 *                       h+1/      \h
                 *                         /        \
                 *                      (lX)        (rrlX)
                 *                     /
                 *                   h/
                 *                   /
                 *               (llX)
                 *
                 * Now, right rotating will suffice to be balaced.
                 *
                 * @attention The tree looks like
                 *                                   |
                 *                                   |h+2
                 *                                   |
                 *                                 (rlX)
                 *                                 /    \
                 *                             h+1/      \h+1
                 *                               /        \
                 *                            (lX)        (X) // Height of X decreased by 2.
                 *                                       /   \
                 *                                     h/     \h
                 *                                     /       \
                 *                               (rrlX)         (rX)
                 */

                if (x->left->right->h > x->left->left->h) {
                    x->left->h--;
                    x->left->right->h++;
                    this->leftRotate(x->left);
                }
                x->h -= 2;
                this->rightRotate(x);

            } else if ((x->right->h > x->left->h) && ((x->right->h - x->left->h) == 2)) {

                //Similar Explanation as above

                if (x->right->left->h > x->right->right->h) {
                    x->right->h--;
                    x->right->left->h++;
                    this->rightRotate(x->right);
                }
                x->h -= 2;
                this->leftRotate(x);
            }
            if (x->left == NIL && x->right == NIL)
                x->h = 0; // As NIL is not considered as child.
        }
    }
    return;
}

template<typename T, typename __type, typename node_type>
void avlTree<T, __type, node_type>::fixUp(const wPtr<node_type>& ptr) noexcept
{
    if (!ptr.expired()) {
        auto x{ ptr.lock() }, p{ x->p.lock() }, w{ p };
        while (p != NIL) {
            if (p->left == x)
                w = p->right;
            else
                w = p->left;
            p->h = (w->h > x->h ? w->h : x->h) + 1;
            size_t h_diff = w->h > x->h ? (w->h - x->h) : (x->h - w->h);
            if (h_diff == 2)
                this->balance(p);
            x = p;
            p = x->p.lock();
        }
    }
    return;
}

#endif //__AVL_TREE_HPP__