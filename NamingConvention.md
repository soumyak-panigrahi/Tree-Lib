# Tree

We will use common Abbreviation for different *Tree* name.

Common one which are composed of group of words (or Mine) :-

* Binary Search - BS
    * Red-Black - RB
    * Adelson-Velsky and Landis - AVL

if length of it's usual name is 5 or less, it will be used
eg:- tree will be as it is , treap (Tree + heap) will be used as it is and even heap or splay.

first, we will separate naming the main class and it affiliated class (or struct or anything) 

## Main Class

*Note :- + is used to concate*

This will be simply , the abbreviated name (or proper name if it is small enough) in all small and the word `Tree`.
````
 Abb. Name(all small) + Tree
````
eg. avlTree , rbTree , bsTree etc.

## Affiliated Classes and other

this include the tree Node , Alias to utilities such as pointers.

In this library, we will throughout use some common abbreviation.

* Tree - TR
* Pointer - PTR
* Parent - P
* Height - H
* Left Child simply - left
* right Child simply - right

In context to Smart pointer, the type abbrev.

* Unique - U
* Shared - S
* Weak - W

`Node` will be used at end of the class, to specify it as Node.

### Node

It will be composed of the Abbrev. Name (or proper) in all caps with `tr` and `Node` at the end

````
Abbrev. Name (all caps) + 'tr' + 'Node'
````
eg. BStrNode , RBtrNode , AVLtrNode etc.

### Pointer 

Similar convention as Node but it will be attached before, this will be used to specify Aliases.
Note, the pointer is for the node ranther than tree itself. We have omitted 'Node' at the end
as we assumed the user will make it pointer implementation if he wants.

````
Pointer type Abbrev. (All small) + 'Ptr' + Abbrev. Name (All caps) + 'tr'
````
eg. wPtrAVLtr , uPtrBStr , sPtrRBtr etc.

If it is to node added 'Node' at end

````
Pointer type Abbrev. (All small) + 'Ptr' + Abbrev. Name (All caps) + 'tr' + 'Node'
````
eg. wPtrAVLtrNode , sPtrBStrNode etc.
#### Micro Specification to Pointers

Smart pointer can be of diiferent type, note we take shared_ptr\<int\> as our refrence , it applicable to any type of pointer and data type.

So, mainly we will try do two micro specification, i.e. pointer pointing to is const or pointer pointing is const
accordingly , `const shared_ptr<int>` and `shared_ptr<const int>`. And the combo of both `const shared_ptr<const int>`

and sub specification is about is it L-value refrence or R-value reference. i.e. `shared_ptr<int>&` and `shared_ptr<int>&&`

So, LR refrence can be done easily , therefore no aliases.

The convention is `const shared_ptr` == `c_sPtr` and `shared_ptr<const int>` == `csPtr`

## Interfaces

In interfaces we need to be as robust as possible, so we try to include full tree name or Abbrev. name
whichever that doesn't lead to confusin. To identify as interface we include 'I' (capital i) at the start
then in small either full or Abbrev. then at the end 'Tree'

Type 1 :-
````
'I' + Full or Abbrev. (in small) + 'Tree'
````
eg. IrbTree , IbsTree , IbinaryTree , IavlTree etc.

or 

Type 2 :-
`````
'I_' + Node convention (don't add the word Node at the end)
`````
eg. I_RBtr , I_BStr , I_AVLtr , I_BINARYtr , etc.

Note:- In this library Type 2 convention will be used. 

Similarly, we will also use these if we need to implement it's node and/or ptr.

eg. I_RBtrNode , wPtrI_RBtr

## Base Classes

**To just be on the same page**, I like to differenciate between Interfaces and Base Classes

In Interface, we like define give guidelines to followed when the programmer is writing a class which is derived
from the Interface, in the sense when we defining a class is a type of the Interface it must follow all the guideline
it to be used otherwise it can't be used. In short, it is just guideline nothing more or less. So, we implement the guideline
as it varies case to case. But interface can also contain the data memeber which are not object specific i.e static data type,
and even method. 

So , Interface == Guidelines + Universal Data + Universal Method 

Note:- In C++ , universal means *static* type. In this Library we represent it by prefixing **I_**.

In base class, it not neccesayily only consist of guideline and can also contain implementation and data according to the 
need. So, in a way it is not totally generic (ofcourse all of these can be overridden) but it is useful if we doing same work 
for a lot of diffrent classes. In other words it consist of a Interface and a Implementation.

So, Base Classes == Interface + Implementation

Note:- In this Library we represent it by prefixing **BASE_**.

## Bitwise Usage

Now, a program to be most efficient, it have to take care of each bit to it's full potential. Let see how struct or class
padding few btype as roundoff the size to be more accessable. As bit are not addressable, a bool type take 1 byte as only
bytes are addressable. As if we think spacewise 7 bits are wasted as bool value requires only 1 bit. So, we use bit fields
to store these extra bits in another data member as to access it.

Note :- bit field only work in a struct or class, you cannot have it outside. As, a struct or class can be padding to be
        addressable. And these bit fields doesn't have address.

Ofcourse we cannot get each bit efficency, because it is upto compiler to pad it to be addressable. Our aim if we can make
out these extra bit we can use it later or can be inherited for argumenting. Sometime, the order in which we write the data
member matter to it's size.For example, if the size is greater than a byte and less than 4 it will be padded to be 4 and
it less than 8 padded to be 8 and so on. You can alway test and squeeze out these padded data and use it for later. For
more about [alignment](https://en.wikipedia.org/wiki/Data_structure_alignment) and [squeezing out padding](https://docs.microsoft.com/en-us/cpp/preprocessor/pack)

So, we use for the extra bits which we know will be stored in `raw_N` where N is unused bits and make sure you either make it 
**public or protected** you know why.

tl; dr if extra bits left store it in `raw_N` and make it public or protected. 