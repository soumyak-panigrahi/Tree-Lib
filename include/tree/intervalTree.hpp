#ifndef __INTERVAL_TREE__
#define __INTERVAL_TREE__

#include "BASE_RBtrNode.hpp"
#include "rbTree.hpp"
#include <memory>

template<typename T> struct interval;
template<typename T, typename __type> struct INTERVALtrNode;
template<typename T, typename __type, typename node_type> struct intervalTree;

/** Aliases For Node Pointers (Using the Convention pointed in NamingConvention.md)
====================================================================================*/
#ifndef __POINTER_ALIASES__
#define __POINTER_ALIASES__
template<typename T> using wPtr = std::weak_ptr<T>;
template<typename T> using sPtr = std::shared_ptr<T>;
#endif //__POINTER_ALIASES__

template<typename T>
struct interval
{
    T low, high;
    unsigned char type : 2, // [ or ] - 0 and ( or ) - 1 ==> [l,h] - 00 , [l,h) - 01 , (l,h] - 10 , (l,h) - 11
        raw_6 : 6; // Can be used to some other thing....
    interval(const T& l = T{}, const T& h = T{}, unsigned char t = 0) : low{ l }, high{ h }, type{ t }, raw_6{ 0 } { }
    void forceOrder()
    {
        if (low > high) {
            auto temp{ high };
            high = low;
            low = temp;
        }
        return;
    }
    bool subSet(const interval<T>& x)
    {
        if ((x.low < low) && (high < x.high))
            return true;
        else if (((x.low == low) && (high == x.high)) && !(type == 0 && x.type == 3))
            return true;
        else if (((x.low == low) && (high < x.high)) && ((type & (1 << 1)) && !(x.type & (1 << 1))))
            return true;
        else if (((x.high == high) && (high < x.high)) && ((type & (1)) && !(x.type & (1))))
            return true;
    }
    bool superSet(const interval<T>& x)
    {
        return x.subSet(*this);
    }
    bool overlaps(const interval<T>& x) { return !((*this > x) || (*this < x)); }
    bool overlaps(const T& x) { return (x > low && x < high) || (x == low && !(type & (1 << 1))) || (x == high && !(type & 1)) }
    bool operator == (const interval<T>& x) { return ((type == x.type) && ((low == x.low) && (high == x.high))); }
    bool operator != (const interval<T>& x) { return !(*this == x); }
    bool operator > (const interval<T>& x)
    {
        return (low > x.high) || ((low == x.high) && (!(type & (1 << 1)) && (x.type & 1)));
    }
    bool operator < (const interval<T>& x)
    {
        return (high < x.low) || ((high == x.low) && ((type & 1) && !(x.type & (1 << 1))));
    }
};

template<typename T, typename __type = T>
struct INTERVALtrNode : public BASE_RBtrNode<T, __type, INTERVALtrNode<T, __type>>
{
    T max;
    interval<T> it;
    INTERVALtrNode(const T& l = T{}, const T& h = T{}, unsigned char t = 0)
        :BASE_RBtrNode<T, __type, node_type>(l), max{ h }, it(l, h, t) { }
};

template<typename T, typename __type = T, typename node_type = INTERVALtrNode<T, __type>>
class intervalTree : public rbTree<T, __Type, node_type>
{
    using rbTree<T, __type, node_type>::NIL;
    using rbTree<T, __type, node_type>::root;
    using rbTree<T, __type, node_type>::size;
public:
    intervalTree() : rbTree<T, __type, node_type>() { }
    virtual void setMax(const wPtr<node_type>&) noexcept;
    virtual void rightRotate(const wPtr<node_type>&) noexcept override;
    virtual void leftRotate(const wPtr<node_type>&) noexcept override;
    virtual wPtr<node_type> intervalInsert(const interval<T>&);
    virtual interval<T> intervalRemove(const wPtr<node_type>&) noexcept override;
    virtual wPtr<node_type> intervalSearch(const interval<T>&);
    virtual wPtr<node_type> intervalExactSearch(const interval<T>&);
};

template<typename T, typename __type, typename node_type>
void intervalTree<T, __type, node_type>::rightRotate(const wPtr<node_type>& ptr) noexcept
{
    if (!ptr.expired()) {
        rbTree<T, __type, node_type>::rightRotate(ptr);
        auto x{ ptr.lock() };
        this->setMax(x);
        this->setMax(x->p);
    }
    return;
}

template<typename T, typename __type, typename node_type>
void intervalTree<T, __type, node_type>::leftRotate(const wPtr<node_type>& ptr) noexcept
{
    if (!ptr.expired()) {
        rbTree<T, __type, node_type>::leftRotate(ptr);
        auto x{ ptr.lock() };
        this->setMax(x);
        this->setMax(x->p);
    }
    return;
}

template<typename T, typename __type, typename node_type>
void intervalTree<T, __type, node_type>::setMax(const wPtr<node_type>& ptr) noexcept
{
    if (!ptr.expired()) {
        auto x{ ptr.lock() };
        if ((x->it->high > x->left->max) && (x->it->high > x->right->max))
            x->max = x->it->high;
        else if ((x->left->max > x->right->max) && (x->left->max > x->it->high))
            x->max = x->left->max;
        else
            x->max = x->right->max;
    }
    return;
}

template<typename T, typename __type, typename node_type>
wPtr<node_type> intervalTree<T, __type, node_type>::intervalInsert(const interval<T>& i)
{
    auto x{ root }, y{ root };
    i.forceOrder();
    auto max{ i.high }, val{ i.low };
    while (x != NIL) {
        y = x;
        x->max = ((max > x->max) ? max : x->max);
        if (val < x->data)
            x = x->left;
        else
            x = x->right;
    }
    auto z = std::make_shared<node_type>();
    if (!z)
        return wPtr<node_type>();
    z->data = val;
    z->max = max;
    z->it = i;
    z->p = y;
    z->right = z->left = NIL;
    if (y == NIL)
        root = z;
    else if (val < y->data)
        y->left = z;
    else
        y->right = z;
    z->color = true;
    size++;
    rbTree<T, __type, node_type>::insertFixup(z);
    return z;
}

template<typename T, typename __type, typename node_type>
interval<T> intervalTree<T, __type, node_type>::intervalRemove(const wPtr<node_type>& ptr) noexcept
{
    if (!ptr.expired()) {
        auto x{ ptr.lock() }, y{ x }, z{ x };
        bool y_orig_color{ y->color };
        auto removed_data = x->it;
        if (x != NIL) {
            if (x->left == NIL) {
                z = x->right;
                z->color = x->color;
                this->transplant(x, x->right);
            } else if (x->right == NIL) {
                z = x->left;
                z->color = x->color;
                this->transplant(x, x->left)
            } else {
                y = this->treeMin(x->right);
                y_orig_data = y->color;
                z = y->right;
                if (y->p.lock() != x) {
                    this->transplant(y, y->right);
                    y->left = x->left;
                    x->left->p = y;
                }
                this->transplant(x, y);
                y->right = x->right;
                x->right->p = y;
                y->color = x->color;
            }
            y = z->p.lock();
            while (y == NIL) {
                this->setMax(y);
                y = y->p.lock();
            }
            if (!y_orig_color)
                rbTree<T, __type, node_type>::removeFixup(z);
            size--;
        }
        return removed_data;
    }
    return interval<T>();
}

template<typename T, typename __type, typename node_type>
wPtr<node_type> intervalTree<T, __type, node_type>::intervalSearch(const interval<T>& i)
{
    auto x{ root };
    i.forceOrder();
    while (x != NIL && !(x->it.overlaps(i))) {
        if (x->left != NIL && x->left->max > i->low)
            x = x->left;
        else
            x = x->right;
    }
    return x;
}

template<typename T, typename __type, typename node_type>
wPtr<node_type> intervalTree<T, __type, node_type>::intervalExactSearch(const interval<T>& i)
{
    auto x{ root };
    i.forceOrder();
    while (x != NIL && (x->it != i)) {
        if (x->data > i.low && (x->left != NIL && x->left->max > i.high))
            x = x->left;
        else if (x->data < i.low && (x->right != NIL && x->right->max > i.high))
            x = x->right
        else
            x = NIL;
    }
    return x;
}

#endif //__INTERVAL_TREE__