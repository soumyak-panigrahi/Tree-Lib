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
    virtual wPtr<node_type> intervalSearch(const interval<T>&);
};

#endif //__INTERVAL_TREE__