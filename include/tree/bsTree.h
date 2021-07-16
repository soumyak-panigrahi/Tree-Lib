#ifndef __BS_TREE_H__
#define __BS_TREE_H__

#include <binaryTree.h>
#include <memory>

/** Classes And Structs Delecratino*/
template<typename T, typename __type> struct BStrNode;
template<typename T, typename __type> class bsTree;

/** Aliases For Node Pointers (Using the Convention pointed in NamingConvention.md) */
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

/** Node Delecration */
template<typename T, typename __type>
struct BStrNode : public I_BINARYtrNode<T, __type, BStrNode<T, __type>>
{
    T data;
    wPtrBStrNode<T, __type> p;
    sPtrBStrNode<T, __type> left, right;
    BStrNode(T x = T{}) : data{ x }, left{ nullptr }, right{ nullptr } { }
    virtual const __type& read() const noexcept override
    {
        __type temp{ data };
        return temp;
    }
    virtual bool write(const __type& x) override
    {
        if (x = data)
            return true;
        return false;
    }
    virtual
        virtual ~BStrNode() = default;
};

/** bsTree Class Deleclaration*/
template<typename T, typename __type>
class bsTree : public I_BINARYtr
{
    sPtrBStrNode<T, __type> root;
    size_t size;
public:
    bsTree() : root{ nullptr }, size{ 0 } { }
    virtual size_t height(const wPtrBStrNode<T, __type>&) const noexcept;
    virtual void inorderTreeWalk() const noexcept;
    virtual void postorderTreeWalk() const noexcept;
    virtual void preorderTreeWalk() const noexcept;
    virtual wPtrBStrNode<T, __type> search(const T&) const noexcept;
    virtual bool insert(const T&);
    virtual T remove(const wPtrBStrNode<T, __type>&) noexcept;
};

#endif // __BS_TREE_H__