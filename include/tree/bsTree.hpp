#ifndef __BS_TREE_H__
#define __BS_TREE_H__

#include "BASE_BstrNode.hpp"
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
template<typename T, typename __type, typename node_type = BStrNode<T, __type>>
class bsTree : public I_BINARYtr
{
protected:
    sPtr<node_type> root;
    size_t size;
public:
    bsTree() : root{ nullptr }, size{ 0 } { }
    virtual size_t height(const wPtr<node_type>&) const noexcept;
    virtual void inorderTreeWalk() const noexcept;
    virtual void postorderTreeWalk() const noexcept;
    virtual void preorderTreeWalk() const noexcept;
    virtual wPtr<node_type> search(const T&) const noexcept;
    virtual bool insert(const T&);
    virtual T remove(const wPtr<node_type>&) noexcept;
};

/** Binary Search Tree's Class Implementation
=============================================*/

#endif // __BS_TREE_H__