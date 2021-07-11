#ifndef __BS_TREE_H__
#define __BS_TREE_H__

#include <binaryTree.h>
#include <memory>

template<typename T, typename __type> struct BStrNode;
template<typename T, typename __type> class bsTree;

template<typename T, typename __type>
using wPtrBStrNode = std::weak_ptr<BStrNode<T, __type>>;
template<typename T, typename __type>
using sPtrBStrNode = std::shared_ptr<BStrNode<T, __type>>;
template<typename T, typename __type>
using c_wPtrBStrNode = const std::weak_ptr<BStrNode<T, __type>>;
template<typename T, typename __type>
using cwPtrBStrNode = std::weak_ptr<const BStrNode<T, __type>>;
template<typename T, typename __type>
using c_sPtrBStrNode = const std::shared_ptr<BStrNode<T, __type>>;
template<typename T, typename __type>
using csPtrBStrNode = std::shared_ptr<const BStrNode<T, __type>>;

template<typename T, typename __type>
struct BStrNode : public I_BINARYtrNode<__type>
{
    T data;
    wPtrBStrNode<T, __type> p;
    sPtrBStrNode<T, __type> left, right;
    BStrNode(T x = T{}) : data{ x }, left{ nullptr }, right{ nullptr } { }
    virtual const __type& read() const noexcept
    {
        __type temp{ data };
        return temp;
    }

    virtual bool write(const __type& x)
    {
        if (x = data)
            return true;
        return false;
    }

    virtual ~BStrNode() = default;
};

template<typename T, typename __type>
class bsTree : public I_BINARYtr
{
    sPtrBStrNode<T, __type> root;
    size_t size;
public:

};

#endif // __BS_TREE_H__