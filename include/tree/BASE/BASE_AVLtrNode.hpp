#ifndef __BASE_AVL_TR_NODE_HPP__
#define __BASE_AVL_TR_NODE_HPP__

#include "BASE_BstrNode.hpp"
#include <memory>

/** BASE AVL CLASS DECLARATION
===============================*/
template<typename T, typename __type, typename node_type>
struct BASE_AVLtrNode : public BASE_BStrNode<T, __type, node_type>
{

};
#endif //__BASE_AVL_TR_NODE_HPP__