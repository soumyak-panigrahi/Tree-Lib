#ifndef __RB_TR_NODE__
#define __RB_TR_NODE__

#include "BASE_RBtrNode.hpp"
#include "bsTree.hpp"

/** Classes And Structs Delecratinon
====================================*/
template<typename T, typename __type> struct RBtrNode;
template<typename T, typename __type, typename node_type> class rbTree;

/** Aliases For Node Pointers (Using the Convention pointed in NamingConvention.md)
====================================================================================*/
#ifndef __POINTER_ALIASES__
#define __POINTER_ALIASES__
template<typename T> using wPtr = std::weak_ptr<T>;
template<typename T> using sPtr = std::shared_ptr<T>;
#endif //__POINTER_ALIASES__

/** Red - Black Tree 's Node Concrete Class Declaration
========================================================*/
template<typename T, typename __type>
struct RBtrNode : public BASE_RBtrNode<T, __type, RBtrNode<T, __type>>
{
    RBtrNode(const T& x = T{}) : BASE_RBtrNode<T, __type, RBtrNode<T, __type>>(x) { }
    virtual ~RBtrNode() = default;
};

/** Red - Black Tree Class Declaration
=======================================*/
template<typename T, typename __type = T, typename node_type = RBtrNode<T, __type>>
class rbTree : public bsTree<T, __type, node_type>
{
public:
    rbTree() : bsTree<T, __type, node_type>() { }
    virtual wPtr<node_type> insert(const T&) override;
    virtual T remove(const wPtr<node_type>&) noexcept override;
protected:
    virtual void insertFixup(const wPtr<node_type>&) noexcept;
    virtual void removeFixup(const wPtr<node_type>&) noexcept;
};

/** Red - Black Tree Class Implementation
=========================================*/
template<typename T, typename __type, typename node_type>
wPtr<node_type> rbTree<T, __type, node_type>::insert(const T& x)
{
    auto x = (bsTree<T, __type, node_type>::insert(x)).lock();
    if (x) {
        x->color = true;
        this->insertFixup(x);
    }
    return x;
}

template<typename T, typename __type, typename node_type>
void rbTree<T, __type, node_type>::insertFixup(const wPtr<node_type>& ptr) noexcept
{
    if (!ptr.expired()) {
        auto x{ ptr.lock() }, p{ x->p.lock() };
        while (p->color && p != NIL) {
            auto pp{ p->p.lock() };
            if (p == pp->left) {
                auto w = pp->right;

                if (w->color) {
                    // CASE - 1, if x's Uncle is RED

                    /**
                     * @brief x = inserted node , p = parent of x , pp is parent of p and w = sibling of p or uncle of x
                     *
                     * @brief x.color = red , p.color = red , w.color = red , pp.color = black
                     *
                     * @note (A) means black node and (*A) means red black and (*)A) means ambiguous colored node.
                     *       \ and / these are used to indicate left or right child, if | is used the child type is
                     *        not needed. In 2nd level always black height is indicated
                     *
                     * @attention The tree looks like
                     *
                     *                                  (PP)
                     *                                 /    \
                     *                               h/      \h
                     *                               /        \
                     *                            (*P)         (*W)
                     *                              |
                     *                             h|
                     *                              |
                     *                            (*X)
                     *
                     *  So, the property violated is Red node should have only black children. But note black can have both.
                     *  therefore to improve that we can make p as black but now the black height will increase.
                     *
                     * @attention The tree looks like
                     *                                   |
                     *                                   | h+1
                     *                                   |
                     *                                  (PP)
                     *                                 /    \
                     *                             h+1/      \h
                     *                               /        \
                     *                             (P)         (*W)
                     *                              |
                     *                             h|
                     *                              |
                     *                            (*X)
                     * Now, the black Height property i.e, all children should have same black height (no of black node in the
                     * path) is violated. So, to do that we will have W also as black.
                     *
                     * @attention The tree looks like
                     *                                   |
                     *                                   | h+2
                     *                                   |
                     *                                  (PP)
                     *                                 /    \
                     *                             h+1/      \h+1
                     *                               /        \
                     *                             (P)         (W)
                     *                              |
                     *                             h|
                     *                              |
                     *                            (*X)
                     *
                     * Although P and W black become same but the black height of overall tree is increased by 1 (fron h+1 to
                     * h+2), so simply we will make PP as red as none of the properties are violated in below level and the
                     * overall black height will decrease by 1.
                     *
                     * @attention The tree looks like
                     *                                   |
                     *                                   | h+1
                     *                                   |
                     *                                 (*PP)
                     *                                 /    \
                     *                             h+1/      \h+1
                     *                               /        \
                     *                             (P)         (W)
                     *                              |
                     *                             h|
                     *                              |
                     *                            (*X)
                     *
                     * So, the red -black tree is holded with PP as root but if it is sub tree, the violation now can occur at
                     * PP as it is changed from black to red, in other words if become a recursive problem with violation
                     * at PP. therefore, we make the new X as PP and P as new X's parent and continue the loop.
                     */

                    p->color = w->color = false;
                    pp->color = true;
                    x = pp;
                    p = x->p.lock();

                } else {
                    // CASE - 2 and CASE - 3, if x's Uncle is black.
                    if (x == p->right) {
                        // CASE - 2

                        /**
                         * @brief x = inserted node , p is x's parent node , pp is x's grandparent node and w is x's uncle node
                         *        x.color = RED , p.color = RED , pp.color = BLACK and w.color = BLACK
                         *
                         * @note This is a auxilary case, we try to convert this to CASE - 3.
                         *
                         * @attention The tree looks like
                         *
                         *                                  (PP)
                         *                                 /    \
                         *                               h/      \h
                         *                               /        \
                         *                            (*P)         (W)
                         *                           /    \
                         *                         h/      \h
                         *                         /        \
                         *                       (lP)        (*X)
                         *
                         * So, let approach like we did in CASE - 1 , so there is violation in p and x. Hence, change p to be
                         * black but now in this case W is black so there is a inbalance of black height , after we make P to be
                         * black. Therefore, that approach can't happen. Simply, idea is balancing the height after p is black.
                         * that can happen through rotations. First let us make p to be black.
                         *
                         * @attention The tree looks like
                         *                                   |
                         *                                   | h+1
                         *                                   |
                         *                                  (PP)
                         *                                 /    \
                         *                             h+1/      \h
                         *                               /        \
                         *                             (P)         (W)
                         *                           /    \
                         *                         h/      \h
                         *                         /        \
                         *                       (lP)        (*X)
                         *
                         * So, now to balance we have right Rotate at PP as PP's left tree black height is more.
                         *
                         * @attention The tree looks like
                         *                                   |
                         *                                   | h+1
                         *                                   |
                         *                                  (P)
                         *                                 /    \
                         *                               h/      \h+1
                         *                               /        \
                         *                            (lP)         (PP)
                         *                                        /    \
                         *                                      h/      \h
                         *                                      /        \
                         *                                    (*X)       (W)
                         *
                         * So, now the Black Height of left tree of p is h but right h+1, but we cannot make PP as red,
                         * because X is already Red, which will again violation red children are black property.
                         *
                         * @remark Hence, the conclusion is X has to be left child to right Roataion to work at original PP.
                         * So, we will do that first that is in CASE - 2, we will make new X to be the left child of new P.
                         * Then we do the above in CASE - 3. So, we seen that it will work.
                         *
                         * Let Start with CASE - 2, working.
                         *
                         * @attention The tree looks like
                         *                                   |
                         *                                   |h+1
                         *                                   |
                         *                                  (PP)
                         *                                 /    \
                         *                               h/      \h
                         *                               /        \
                         *                            (*P)         (W)
                         *                           /    \
                         *                         h/      \h
                         *                         /        \
                         *                       (lP)        (*X)
                         *
                         * Now our aim is to make the left child of P's position to be black and without distrubing the blavk
                         * height. As, the violation only occurs at P and X. And in CASE - 1 we try make sure the lower tree
                         * of X is not violating any properties.
                         *
                         * @note left child of node A will be denoted to lA and similary rA.
                         *
                         * @attention The tree looks like
                         *                                   |
                         *                                   |h+1
                         *                                   |
                         *                                  (PP)
                         *                                 /    \
                         *                               h/      \h
                         *                               /        \
                         *                            (*P)         (W)
                         *                           /    \
                         *                         h/      \h
                         *                         /        \
                         *                       (lP)        (*X)
                         *                                  /    \
                         *                                h/      \h
                         *                                /        \
                         *                              (lX)       (rX)
                         *
                         * We are able to say Lower levels of X hold all properties as in CASE - 1 , we try to do that.
                         *
                         * Now, we want X's position node to black. It can be done by two ways.
                         *
                         * First, left or right rotate at X
                         *
                         * @attention The tree looks like after right rotate at X
                         *                                   |
                         *                                   |h+1
                         *                                   |
                         *                                  (PP)
                         *                                 /    \
                         *                               h/      \h
                         *                               /        \
                         *                            (*P)         (W)
                         *                           /    \
                         *                         h/      \h
                         *                         /        \
                         *                       (lP)        (lX)
                         *                                  /    \
                         *                              h-1/      \h
                         *                                /        \
                         *                              ...       (*X)
                         *                                       /    \
                         *                                   h-1/      \h
                         *                                     /        \
                         *                                   ...         (rX)
                         *
                         * But you can see it is messing up the already balanced Red - black tree. So, this can't we done.
                         *
                         * @remark At any point don't try to mess with X.
                         *
                         * Second, we will left rotate at P
                         *
                         * @attention The tree looks like
                         *                                   |
                         *                                   |h+1
                         *                                   |
                         *                                  (PP)
                         *                                 /    \
                         *                               h/      \h
                         *                               /        \
                         *                            (*X)         (W)
                         *                           /    \
                         *                         h/      \h
                         *                         /        \
                         *                      (*P)        (rX)
                         *                     /    \
                         *                   h/      \h
                         *                   /        \
                         *                 (lP)       (lX)
                         *
                         * Now, this acheives two thing. First, it doesn't mess up the black height and it make new P's
                         * right child as black and new X's is left child of new P.
                         */

                        this->leftRotate(p);
                        x = p;
                        p = x->p.lock();
                        pp = p->p.lock();
                    }

                    // CASE - 3

                    /**
                     * @brief x.color = red , p.color = red , w.color = black.
                     *
                     * @note in CASE - 2 we try the tree similar the below one
                     *
                     * @attention The tree looks like
                     *                                   |
                     *                                   | h+1
                     *                                   |
                     *                                  (PP)
                     *                                 /    \
                     *                               h/      \h
                     *                               /        \
                     *                            (*P)         (W)
                     *                           /    \
                     *                         h/      \h
                     *                         /        \
                     *                      (*X)        (rP)
                     *
                     * As we already talked in CASE - 2, rotaion will be used to balance as the changing p as black
                     * mess up thr black height property. Let first make p as black.
                     *
                     * @attention The tree looks like
                     *                                   |
                     *                                   | h+1
                     *                                   |
                     *                                  (PP)
                     *                                 /    \
                     *                             h+1/      \h
                     *                               /        \
                     *                             (P)         (W)
                     *                           /    \
                     *                         h/      \h
                     *                         /        \
                     *                      (*X)        (rP)
                     *
                     * Now, black height of P tree is h+1 and W is h. So, right rotating will still maintain the overall height
                     * of it as h+1 as PP tree height is h+1. Let first do that.
                     *
                     * @attention The tree looks like
                     *                                   |
                     *                                   | h+1
                     *                                   |
                     *                                  (P)
                     *                                 /   \
                     *                               h/     \h+1
                     *                               /       \
                     *                            (*X)        (PP)
                     *                                       /   \
                     *                                     h/      \h
                     *                                     /        \
                     *                                  (rP)        (W)
                     *
                     * so, now the right black height is h+1 and it can be easily retified by making PP to be red, as both of it
                     * children are black it can be done (we discussed in CASE- 2, why right child of P has to black as to
                     * fulfill the above stated condition).
                     *
                     * @attention The tree looks like
                     *                                   |
                     *                                   | h+1
                     *                                   |
                     *                                  (P)
                     *                                 /   \
                     *                               h/     \h
                     *                               /       \
                     *                            (*X)       (*PP)
                     *                                       /   \
                     *                                     h/      \h
                     *                                     /        \
                     *                                  (rP)        (W)
                     *
                     * Finally, the whole tree is balanced. So, the loop will end.
                     */

                    pp->color = true;
                    p->color = false;
                    this->rightRotate(pp);
                }
            } else {
                w = pp->left;
                // Explanation of all these are similar to above.
                if (w->color) {
                    w->color = p->color = false;
                    pp->color = true;
                    x = pp;
                    p = x->p.lock();
                } else {
                    if (x == x->left) {
                        this->rightRotate(p);
                        x = p;
                        p = x->p.lock();
                        pp = p->p.lock();
                    }
                    pp->color = true;
                    p->color = false;
                    this->leftRotate(pp);
                }
            }
        }
        root->color = false; // to preserve the root have to be black property.
        return;
    }
}

template<typename T, typename __type, typename node_type>
T rbTree<T, __type, node_type>::remove(const wPtr<node_type>& ptr) noexcept
{
    if (!ptr.expired()) {
        auto y = ptr.lock();
        if (x != NIL) {
            auto z{ y }, x{ y };
            bool y_org_color = y->color;
            if (z->left == NIL) {
                x = z->right;
                this->transplant(z, z->right);
            } else if (x->right == NIL) {
                x = z->left;
                this->transplant(z, z->left);
            } else {
                y = this->treeMin(z->right);
                y_org_color = y->color;
                x = y->right;
                if (y->p.lock() != z) {
                    this->transplant(y, y->right);
                    y->right = z->right;
                    z->right->p = y;
                }
                this->transplant(z, y);
                y->left = z->left;
                z->left->p = y;
                y->color = z->color;
            }
            if (!y_org_color)
                this->removeFixup(x);
            size--;
            return z->data;
        }
    }
    return T{};
}

#endif //__RB_TR_NODE__