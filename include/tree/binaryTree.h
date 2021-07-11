#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__

#include <memory>

template<typename T> struct I_BINARYtrNode;
template<typename T> class I_BINARYtr;

//Aliases

template<typename T> using wPtrI_BINARYtrNode = std::weak_ptr<I_BINARYtrNode<T>>;
template<typename T> using sPtrI_BINARYtrNode = std::shared_ptr<I_BINARYtrNode<T>>;


template<typename T>
struct I_BINARYtrNode
{
    /*
    The Base idea is to have interface with CRUD (Create Read Update Delete) funtionality.
    Create and Delete will be done by Constructors and Destructors respectively.

    Note: In this context Read will to give a concise information presented by the data stored rather
          than just forwarding the data present, in similar way write will write to the data presents
          to rather than the data itself.

    This Fns read() and  write(...) need to be implemented as if some external data is manged through this->data
    i.e. T != __Type . In other words , some comprehendable data (__Type) is managed through data_type (T). Then,
    either it is to be managed externally but in this way a easier read (which read eternal data using this->data)
    to be presented as __Type. For example, A process can be represented by a integer data type , this process ID will
    be stored in this->data, but when we like to read or more properly get detailed infromation (some object) about the
    process running by the ID, we need use the this->data for more. We can implement a function which will take process
    ID and give data. In this interface, the read is to be provided as method as it is more suitble as more frequently the
    portable will not mess up. Similarly, write. This will give close knitted read and write feature, any extra functionality
    for a inheritated node can be implemented, but these two has to be provided to be concrete.
    */

    //Read : will be used Read a more concise information through the data
    virtual const T& read() const noexcept = 0;
    //Write : will be used to update the more concise information it represents ranther than changing itself 
    virtual bool write(const T&) = 0;
};

template<typename T>
class I_BINARYtr
{
public:
    virtual size_t height(wPtrI_BINARYtrNode<T>) const noexcept = 0;
    virtual void inorderTreeWalk() const noexcept = 0;
    virtual void postorderTreeWalk() const noexcept = 0;
    virtual void preorderTreeWalk() const noexcept = 0;
    virtual wPtrI_BINARYtrNode<T> search(const T&) const noexcept = 0;
    virtual bool insert(const T&) = 0;
    virtual T remove(const wPtrI_BINARYtrNode<T>&) noexcept = 0;
};

#endif //__BINARY_TREE_H__