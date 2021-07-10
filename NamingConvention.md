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

Similar convention as Node but it will be attached before,
this will be used to specify Aliases.

````
Pointer type Abbrev. (All small) + 'Ptr' + Abbrev. Name (All caps) + 'tr'
````
eg. wPtrAVLtr , uPtrBStr , sPtrRBtr etc.

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
