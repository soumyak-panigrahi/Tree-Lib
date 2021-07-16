#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__

#include <memory>

template<typename T, typename __type, typename node_type> struct I_BINARYtrNode;
template<typename T, typename __type, typename node_type> class I_BINARYtr;

/** Aliases
===========*/
#ifndef __POINTER_ALIASES__
#define __POINTER_ALIASES__
template<typename T> using wPtr = std::weak_ptr<T>;
template<typename T> using sPtr = std::shared_ptr<T>;
#endif //__POINTER_ALIASES__

/** Binary Tree Node Interface
===============================*/
template<typename T, typename __type, typename node_type>
struct I_BINARYtrNode
{
    virtual const __type read() const noexcept = 0;
    virtual bool write(const __type&) = 0;

    virtual bool set_data(const T&) = 0;
    virtual T get_data() const noexcept = 0;

    virtual std::weak_ptr<node_type> get_parent() const noexcept = 0;
    virtual bool set_parent(std::weak_ptr<node_type>&) = 0;

    virtual std::weak_ptr<node_type> get_left() const noexcept = 0;
    virtual bool set_left(std::weak_ptr<node_type>&) = 0;

    virtual std::weak_ptr<node_type> get_right() const noexcept = 0;
    virtual bool set_right(std::weak_ptr<node_type>&) = 0;
};


/** Binary Tree Interface
==========================*/
template<typename T, typename __type, typename node_type>
class I_BINARYtr
{
public:
    virtual size_t height(const std::weak_ptr<node_type>&) const noexcept = 0;
    virtual void inorderTreeWalk() const noexcept = 0;
    virtual void postorderTreeWalk() const noexcept = 0;
    virtual void preorderTreeWalk() const noexcept = 0;
    virtual std::weak_ptr<node_type> search(const T&) const noexcept = 0;
    virtual bool insert(const T&) = 0;
    virtual T remove(std::weak_ptr<node_type>&) noexcept = 0;
};

#endif //__BINARY_TREE_H__