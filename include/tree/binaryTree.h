#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__

#include <memory>

template<typename T, typename __Type> struct I_BINARYtrNode;
template<typename T, typename __Type> class I_BINARYtr;

//Aliases

template<typename T1, typename T2> using wPtrI_BINARYtr = std::weak_ptr<I_BINARYtrNode<T1, T2>>;
template<typename T1, typename T2> using sPtrI_BINARYtr = std::shared_ptr<I_BINARYtrNode<T1, T2>>;


template<typename T, typename __Type>
struct I_BINARYtrNode
{
    T data;
    wPtrI_BINARYtr<T, __Type> p;
    sPtrI_BINARYtr<T, __Type> left, right;
    virtual const __Type& read() const noexcept = 0;
    virtual bool write(const __Type&) = 0;
};

template<typename T, typename __Type>
class I_BINARYtr
{
public:
    virtual void inorderTreeWalk() const noexcept = 0;
    virtual void postorderTreeWalk() const noexcept = 0;
    virtual void preorderTreeWalk() const noexcept = 0;
    virtual wPtrI_BINARYtr<T, __Type> search(const T&) const noexcept = 0;
    virtual bool insert(const T&) = 0;
    virtual T remove(const wPtrI_BINARYtr<T, __Type>&) noexcept = 0;
};

#endif //__BINARY_TREE_H__