# Design Logic and Clarification

Whenever node is specified refer to it as node of a binary tree unless otherwise said so.

## Design of a Binary-Tree Node

So, Simply any Binary Tree Node 3 thing a key , pointer to it's parent and left , right child

In other words,

````
template<typename T>
struct Binary_Tree_Node
{
    T key;
    Binary_Tree_Node<T> *p , *left , *right;
}
````
or 

if using smart pointers
````
template<typename T>
struct Binary_Tree_Node
{
    T key;
    std::weak_ptr<Binary_Tree_Node<T>> p; // To avoid cyclic reference
    std::shared_ptr<Binary_Tree_Node<T>> left , right;
}
````

So, we use a data type for key weather it is bulit-in and user defined, so it must represent some kind 
of data to it type.

But now suppose, the key is just used to specify or refer a complex structure or data. We can easily do it
by taking the key itself as the whole data. But as we known writing is costly and it we are not really 
using it constly it is a *waste of space* , and When we are designing it we have to take care a lot of what
is efficient. So, to do that we assume the key are simple data type (in the sense can we easily writable) , 
and we can derive the complex data from this key using some extra infrastructre. For example, a process (which
is running instant of a program) can be represented as integer (it's ID) but the program it represents can we very
complex with a lots of things. Now, we got two choice either the key is just the ID or the whole program (which can
be representable) , let say we store the program itself as the key, we will have the whole thing at our fingertip 
but we have to write a lot of data with every step. And we want to do simply operations like pause , kill , execute etc. 
We can simply make it simply by the ID which separate it among the processes and then call a function for each task.
Ofcourse using the ID to manage process has few extra step but all the process can easily be represented and we can manage
it more efficiently. 

It is just an example each task will have it own design. 

As we want a generic node we have to assume that this is possible i.e. it should be able manage complex data
using some simple data. So, we refer the complex using another data_type (the programmer can make both same). And 
while designing it is always efficient that the CRUD (Create Read Update Delete) operations to exist as a member 
functions as they are frequrntly used.

Tl;dr we assume all CRUD operation should be a memeber function and complex data/operation can be managed using simple
data types.

### Binary Tree Node's Interface

As discussed just above, we assume all node should have CRUD operation and complex type can be represented by simple ones.

```````````
template<typename T , typename __type> // T is assumed to be simple and __type is asumed to be complex
I_BINARYtrNode
{
    virtual __type read() = 0;
    virtual bool write(__type) = 0;
}
```````````
So, above are bare neccescities to be fulfilled before we use it.

We like to expand it , so that all attributes can set using member function.

As the node has 4 attributes i.e Key , Parent , Left Child , Right Child. As future node may have other functionality
this have to bare minimum, future ones have to done by the programmer (if they needs them regularly). As the node types
can be changed as we inhertite, We can use dynamic binding but when using smart pointer it is best to provide it as 
templatee parameter and According me it better to do that, though it may make it little clumsy but the programmer can
use their own node type if they need .

````````````
template <typename T , typename __type, typename node_type>
I_BINARYtrNode
{
    //Constructors to Create
    __type read() = 0;
    bool write(__type) = 0;
    //Destructors to Delete

    virtual std::weak_ptr<node_type> get_parent() = 0;
    virtual std::weak_ptr<node_type> get_left() = 0;
    virtual std::weak_ptr<node_type> get_right() = 0;
    virtual bool set_parent(std::weak_ptr<node_type>) = 0;
    virtual bool set_left(std::weak_ptr<node_type>) = 0;
    virtual bool set_right(std::weak_ptr<node_type>) = 0;

}
````````````
We can also use dynamic binding just don't provide the node_type as the parameter and while
inherting provide itself as the node parameter in the I_BINARYtrNode.

````````
template<T, __type> Class FUTUREtrNode ;

template<T,__type>
class FUTUREtrNode : I_BINARYtrNode<T,__type,FUTUREtrNode<T,__type>> 
{ 
    //Everthing  Here
};
````````

In this Library, I will provide the node_type in the tree to it's present node not by dynamic binding.

### Binary Tree's Interface

we use similar approach as that of the node. i.e will have tree parameter T is the normal data parameter , __type is
the complex data parameter and node_type is the Node type to be provided in the class.

```````
tempale <T , __type = T , node_type>
binaryTree
{
    //All trasvering

    bool insert(T) = 0;
    T remove(std::weak_ptr<node_type>) = 0;
}
```````

### Future Binary Tree Node's Base Class

I have discussed about base classes meaning in NamingConvention.md under Base Classes section.

As disscussed we can use dynamic binding to set the attributes of future Binary Tree (other variants of Binary Tree) but it 
kinda feel uneasy if we know what kind of tree we are working with and it's not neccesary if we know what kind of tree we want
to work with. So, each future Binary Tree will be Implemented in two type Base class and Concrete.

#### Why Base Class??

As we know a node has 3 data member p , left , right are pointers to present type of node. There are two way to work with it 
Dynamic Binding or Providing node_type template parameter.

##### Dynamic Binding

In this we will have a generic function to set attribute which will implemented by future Classes. So, in this we will have a 
class we will inheriate from it as use dynamic binding to change it's attribute. Here, the unplesentness invoke as it diffucult
judge the future attribute and so we have to provide different rules to that. In this case Only concrete class suffices and
we don't need base class.

##### Providing node_type template parameter

To do this we have to implement the base class. As this will have a attribute node_type and to fulfil it we have to provide
a node let say we are argumenting the node itself, in other words

`````````
template<T , __type , node_type>
node
{
   //Code Here;
};

node<T, __type ,node<T,__type,?>> // Forms recursive and creates a problems
`````````
So, avoid this recursive problem , the class which take the node_type parameter will the Base class which will be used in future
as ...

``````````
template<T, __type, node_type> class Base_class;
template<T, __type> class Future_classes;

template<T, __type>
Future_classes : Base_class<T, __type,Future_classes<T,__type>> // no recursive problem
{
    // Proceed here;
};
``````````

**For example**, let consider we are buliding a binary tree node so it should have data , p Pointer , left and right child 
Pointers. So, now when we inheriate the node for Binary Search tree node. It will inheriate the pointer to the binary tree node
(ofcourse the allocated pointers will be of Binary Search Tree node). So, we have to manage the attribute of Binary Search Tree 
node through that and it fine as we don't add any extra data memeber to manage we can live happily but when we are implementing
suppose let say AVL tree it has a extra data member call height (which store height of the tree) , now to set it we cannot 
directly interact we have work it through dynamic binding as the pointer it inheriate is of the type Binary Tree with pointing
to a AVL tree node to manage this we have make a function like set_height() or more generic set(type,data). This is a pain as we
cannot predict the future attributes. it is better to hold the pointers of itself at the generic level. So, this solved by use
of above base class and we are providing the node_type as itself or according to your will you be expand. Now, in this base 
class approach we simply inheriate from Base class rather concrete of a base class.

In other words,

``````````````

Binary Tree Interface
        ^
        |
        |
Binary Search Tree Base Class
    ^                   ^
    |                   |
BS Concrete Class       |
                        |
                    AVL Concrete Class

``````````````
In this the concrete don't interact only the Base classes are inhertiated.

Eg.

````````
Binary Search Tree Base Class
    ^                   ^
    |                   |
    |                   |
AVL Concrete Class      |
                        |
                AVL tree Base Class
                        ^
                        |
                        |
                Future AVL trees.....

````````

This way we can focus what we use and will be used later.