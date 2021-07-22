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
    using bsTree<T, __type, node_type>::NIL;
    using bsTree<T, __type, node_type>::root;
    using bsTree<T, __type, node_type>::size;
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

template<typename T, typename __type, typename node_type>
void rbTree<T, __type, node_type>::removeFixup(const wPtr<node_type>& ptr) noexcept
{
    if (!ptr.expired()) {
        auto x{ ptr.lock() };
        while ((!x->color) && x != root) {
            auto p{ x->p.lock() };
            if (x == p->left) {
                auto w{ p->right };
                // X's Sibling is RED
                if (w->color) {
                    //CASE - 1 , x's sibling is RED

                    /**
                     * @brief
                     *  x.color = black;
                     *  p.color = black;
                     *  w.color = red;
                     *
                     * @note (A) denotes Black node , (*A) denotes Red Node and (*)A) denotes ambiguous colored node.
                     *        lA means left child of A and rA means right child of A. All node will be denoted with all caps.
                     *
                     * @attention the present tree looks like
                     *
                     *                           |
                     *                           | h+1
                     *                           |
                     *                          (P)
                     *                         /   \
                     *                     h-1/     \h
                     *                       /       \
                     *                     (X)       (*W)
                     *                              /    \
                     *                            h/      \h
                     *                            /        \
                     *                          (lW)       (rW)
                     *
                     * now all properties hold, except that all children should have same black height. Which violeted at P.
                     * This can be simply retified by left rotating at p because we like adjust height to adjust the tree.
                     *
                     * @note Our aim is to rectify from above.
                     *
                     * After, left rotating at p.
                     * @attention the present tree looks like
                     *
                     *                           |
                     *                           | h (--- Overall Height)
                     *                           |
                     *                          (*W)                                   ----- 1st Level
                     *                         /   \
                     *                       h/     \h
                     *                       /       \
                     *                     (P)       (rW)                               ---- 2nd Level
                     *                    /   \
                     *                h-1/     \h
                     *                  /       \
                     *                (X)       (lW)                                     ---- 3rd Level
                     *
                     * Now the violation occur at the 3rd level, in this the sibling is black which will be delt in another
                     * case. So, let just try balance above 3rd level, as we can see the overall height was h+1, but now it
                     * is h. It can be easy rectify br making was black.
                     *
                     * @attention the present tree looks like
                     *
                     *                           |
                     *                           | h+1 (--- Overall Height)
                     *                           |
                     *                          (W)                                     ---- 1st Level
                     *                         /   \
                     *                       h/     \h
                     *                       /       \
                     *                     (P)       (rW)                               ---- 2nd Level
                     *                    /   \
                     *                h-1/     \h
                     *                  /       \
                     *                (X)       (lW)                                     ---- 3rd Level
                     */

                    w->color = false;
                    this->leftRotate(p);
                    w = p->right;
                }
                // X's Sibling is BLACK
                if (!w->right->color && !w->left->color) {
                    //CASE - 2 , x's sibling is black and both of it's children are black.
                    /**
                     * @brief
                     *  x.color = black
                     *  w.color = black
                     *  w->left.color = black
                     *  w->right.color = black
                     *  p.color = "Ambiguous"
                     *
                     * @attention the present tree looks like
                     *
                     *                           |
                     *                           | ?
                     *                           |
                     *                         (*)P)                                  ----- LEVEL 1
                     *                         /   \
                     *                     h-1/     \h
                     *                       /       \
                     *                     (X)       (W)                               ------ LEVEL 2
                     *                              /   \
                     *                            h/     \h
                     *                            /       \
                     *                          (lW)       (rW)                         ------- LEVEL 3
                     *
                     * So, the color of P is ambiguous and we will talk about it ina moment. See in LEVEL 2, the black
                     * height violation occurs. So, let try to rectify that first to do so we make W's color color
                     * red.
                     *
                     * @attention the present tree looks like
                     *
                     *                           |
                     *                           | h+1
                     *                           |
                     *                         (*)P)                                  ----- LEVEL 1
                     *                         /   \
                     *                     h-1/     \h-1
                     *                       /       \
                     *                     (X)       (*W)                               ------ LEVEL 2
                     *                              /    \
                     *                            h/      \h
                     *                            /        \
                     *                          (lW)        (rW)                         ------- LEVEL 3
                     *
                     * Now as the violation is rectified at level 2, but still the overall tree may still have violation,
                     * accordingly if P is RED or BLACK.
                     *
                     * Let, P.color = RED
                     *
                     * @attention the present tree looks like
                     *
                     *                           |
                     *                           | h-1 (previously h)
                     *                           |
                     *                          (*P)                                  ----- LEVEL 1
                     *                         /   \
                     *                     h-1/     \h-1
                     *                       /       \
                     *                     (X)       (*W)                               ------ LEVEL 2
                     *                              /    \
                     *                            h/      \h
                     *                            /        \
                     *                          (lW)        (rW)                         ------- LEVEL 3
                     *
                     * The overall height is now h-1, but before the height was h as the height of W was h, and as
                     * it is red it doesn't effect overall height. So, to rectify this it is simple we will make
                     * p as Black, it is done just at the end of loop. So, new X becomes P and the loop end.
                     *
                     * Let, P.color = BLACK
                     *
                     * @attention the present tree looks like
                     *
                     *                           |
                     *                           | h (previously h+1)
                     *                           |
                     *                          (P)                                  ----- LEVEL 1
                     *                         /   \
                     *                     h-1/     \h-1
                     *                       /       \
                     *                     (X)       (*W)                               ------ LEVEL 2
                     *                              /    \
                     *                            h/      \h
                     *                            /        \
                     *                          (lW)        (rW)                         ------- LEVEL 3
                     *
                     * so, if P is black node, the overall subtree node is decreased by 1. So, in other words
                     * it became a recursive problem. SO, the new X will P and the loop will Continue.
                     */

                    w->color = true;
                    x = p;
                } else {
                    if (!w->right->color) {

                        //CASE - 3, x's sibling is black and it's right child is black.

                        /**
                         * @brief
                         *  x.color = black
                         *  w.color = black
                         *  w->right.color = BLACK
                         *  w->left.color = RED
                         *  p.color = "Ambiguous"
                         *
                         * @attention the present tree looks like
                         *
                         *                           |
                         *                           | ?
                         *                           |
                         *                         (*)P)                                  ----- LEVEL 1
                         *                         /   \
                         *                     h-1/     \h
                         *                       /       \
                         *                     (X)       (W)                               ------ LEVEL 2
                         *                              /   \
                         *                          h-1/     \h-1
                         *                            /       \
                         *                         (*lW)      (rW)                         ------- LEVEL 3
                         *
                         * let me say this one more time, to balnce we have to rotate :) . So the simple idea is to left rotate at
                         * P as right rotating will just unbalnce more.
                         *
                         * @attention the present tree looks like after left rotating at P
                         *
                         *                           |
                         *                           | ?
                         *                           |
                         *                          (W)                                    ----- LEVEL 1
                         *                         /   \
                         *                       ?/     \h-1
                         *                       /       \
                         *                   (*)P)       (rW)                                 ----- LEVEL 2
                         *                    /   \
                         *                h-1/     \h-1
                         *                  /       \
                         *               (X)        (*lW)                                      ------- LEVEL 3
                         *
                         * So, p is ambiguous. so let make it black as lw is red.
                         *
                         * @attention the present tree looks like after left rotating at P
                         *
                         *                           |
                         *                           | ?
                         *                           |
                         *                          (W)                                    ----- LEVEL 1
                         *                         /   \
                         *                       h/     \h-1
                         *                       /       \
                         *                     (P)       (rW)                                 ----- LEVEL 2
                         *                    /   \
                         *                h-1/     \h-1
                         *                  /       \
                         *               (X)        (*lW)                                      ------- LEVEL 3
                         *
                         * Now black height is inbalanced at LEVEL 2. Let see what could we have done before to make it balanced
                         * without roatating further. So, Genral idea if so happen rW to be Red, we can instantaneously make it
                         * black without any futher problem and W would take up p's place as it's up and above from LEVEL 1
                         * has no violation. Let do that let make rW as red if it is possible to make. Which is CASE - 3
                         *
                         * @attention the present tree looks like
                         *
                         *                           |
                         *                           | ?
                         *                           |
                         *                         (*)P)                                  ----- LEVEL 1
                         *                         /   \
                         *                     h-1/     \h
                         *                       /       \
                         *                     (X)       (W)                               ------ LEVEL 2
                         *                              /   \
                         *                          h-1/     \h-1
                         *                            /       \
                         *                         (*lW)      (rW)                         ------- LEVEL 3
                         *                         /    \
                         *                     h-1/      \h-1
                         *                       /        \
                         *                     (llW)     (rlW)
                         *
                         * We can see, left child of W is red, our aim is to be make rW's position node red. We can do that
                         * by first right rotating at W because lW 's children black heights are same as the black height of
                         * right child of W, this is useful as after rotating we can make W as RED which first acheive our
                         * aim, second balance the Black height. Let first right rotate at W.
                         *
                         * @attention the present tree looks like
                         *
                         *                           |
                         *                           | ?
                         *                           |
                         *                         (*)P)                                  ----- LEVEL 1
                         *                         /   \
                         *                     h-1/     \h-1
                         *                       /       \
                         *                     (X)       (*lW)                               ------ LEVEL 2
                         *                              /   \
                         *                          h-1/     \h
                         *                            /       \
                         *                         (llW)      (W)                         ------- LEVEL 3
                         *                                    /  \
                         *                                h-1/    \h-1
                         *                                  /      \
                         *                              (rlW)      (rlW)
                         *
                         * To balance at LEVEL 3 we will make W as RED (which we want) and as now overall subtree black
                         * height is decreased by one we will have lw as black.
                         *
                         * @attention the present tree looks like
                         *
                         *                           |
                         *                           | ?
                         *                           |
                         *                         (*)P)                                  ----- LEVEL 1
                         *                         /   \
                         *                     h-1/     \h
                         *                       /       \
                         *                     (X)       (lW)                               ------ LEVEL 2
                         *                              /   \
                         *                          h-1/     \h-1
                         *                            /       \
                         *                         (llW)      (*W)                         ------- LEVEL 3
                         *                                    /  \
                         *                                h-1/    \h-1
                         *                                  /      \
                         *                              (rlW)      (rlW)
                         */

                        w->color = true;
                        w->left->color = false;
                        this->rightRotate(w);
                        w = w->p.lock();
                    }

                    //CASE - 4, x's sibling is black and w's right child is RED.

                    /**
                     * @brief
                     *  x.color = black
                     *  w.color = black
                     *  w->right.color = red
                     *  w->left.color = "Ambiguous"
                     *  p.color = "Ambiguous"
                     *
                     *
                     * @attention the present tree looks like
                     *
                     *                           |
                     *                           | ?
                     *                           |
                     *                         (*)P)                                  ----- LEVEL 1
                     *                         /   \
                     *                     h-1/     \h
                     *                       /       \
                     *                     (X)       (W)                               ------ LEVEL 2
                     *                              /   \
                     *                          h-1/     \h-1
                     *                            /       \
                     *                         (*)lW)      (*rW)                         ------- LEVEL 3
                     *
                     * @note LEVEl - 1 and above has properties withhold.
                     *
                     * As, discussed in CASE - 2, why is we are making sure right child of W is RED. Let left rotate at P.
                     *
                     * @attention the present tree looks like
                     *
                     *                           |
                     *                           | ?
                     *                           |
                     *                          (W)                                  ----- LEVEL 1
                     *                         /   \
                     *                       ?/     \h-1
                     *                       /       \
                     *                    (*)P)       (*rW)                               ------ LEVEL 2
                     *                    /    \
                     *                h-1/      \h-1
                     *                  /        \
                     *             (*)lW)        (X)                                   ------- LEVEL 3
                     *
                     * As LEVEL 1 and Up properties are withhold, so making sure that W color takes whatever p color
                     * was, it will be like p never moved.
                     *
                     * @attention the present tree looks like
                     *
                     *                           |
                     *                           | Balnced when viwed from above.
                     *                           |
                     *                         (*)W) // P's Color                       ----- LEVEL 1
                     *                         /   \
                     *                       ?/     \h-1
                     *                       /       \
                     *                    (*)P)       (*rW)                               ------ LEVEL 2
                     *                    /    \
                     *                h-1/      \h-1
                     *                  /        \
                     *             (*)lW)        (X)                                   ------- LEVEL 3
                     *
                     * As, LEVEL 3 is balanced, let focus on LEVEL 2. The problem is P is Ambiguous.So, let
                     * remove it ambiguity by black as overall black height should be h. As rW is red, we will
                     * it also black as to balance black height.
                     *
                     * @attention the present tree looks like
                     *
                     *                           |
                     *                           | Balnced when viwed from above.
                     *                           |
                     *                         (*)W) // P's Color                       ----- LEVEL 1
                     *                         /   \
                     *                       h/     \h (as rW before was RED)
                     *                       /       \
                     *                     (P)       (rW)                               ------ LEVEL 2
                     *                    /    \
                     *                h-1/      \h-1
                     *                  /        \
                     *             (*)lW)        (X)                                   ------- LEVEL 3
                     *
                     * As now the overall subtree is balanced. No need further run the loop so let make x as root.
                     */

                    w->color = p->color;
                    p->color = w->right->color = false;
                    this->leftRotate(p);
                    x = root;

                }
            } else {
                auto w{ p->left };

                //Similar Explanation as above.

                if (w->color) {
                    w->color = false;
                    this->rightRotate(p);
                    w = p->left;
                }
                if (!w->right->color && !w->left->color) {
                    w->color = true;
                    x = p;
                } else {
                    if (!w->left->color) {
                        w->color = true;
                        w->right->color = false;
                        this->leftRotate(w);
                        w = w->p.lock();
                    }
                    w->color = p->color;
                    p->color = w->left->color = false;
                    this->rightRotate(p);
                    x = root;
                }
            }
        }
        x->color = false;
    }
    return;
}

#endif //__RB_TR_NODE__