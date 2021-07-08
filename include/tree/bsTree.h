#ifndef __BS_TREE_H__
#define __BS_TREE_H__

#include <memory>

template<typename T> struct BStrNode;
template<typename T> class bsTree;

template<typename T>
using wPtrBStr = std::weak_ptr<BStrNode<T>>;
template<typename T>
using sPtrBStr = std::shared_ptr<BStrNode<T>>;
template<typename T>
using c_wPtrBStr = const std::weak_ptr<BStrNode<T>>;
template<typename T>
using cwPtrBStr = std::weak_ptr<const BStrNode<T>>;
template<typename T>
using c_sPtrBStr = const std::shared_ptr<BStrNode<T>>;
template<typename T>
using csPtrBStr = std::shared_ptr<const BStrNode<T>>;

template<typename T>
struct BStrNoe
{
    T data;
    wPtrBStr<T> p;
    sPtrBStr<T> left, right;
    BStrNode(T x = T{}) : data{ x }, left{ nullptr }, right{ nullptr } { }
};

template<typename T>
class bsTree : public Tree
{
public:
};


#endif //__BS_TREE_H__