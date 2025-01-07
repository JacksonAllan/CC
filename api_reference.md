# CC API Reference

General notes:

* API macros may evaluate their first argument—the pointer to the container—multiple times, so never use expressions with side effects (e.g. `&our_containers[ ++i ]` ) for that argument. In GCC and Clang, attempting to do so will cause a compiler warning. All other arguments are only evaluated once.
* If `CC_NO_SHORT_NAMES` was declared, all API macros are prefixed with `cc_`.
* Duplicating a container handle via assignment and then operating on the duplicate will invalidate the original. Hence, only create a duplicate via assignment (including through function parameters and return values) if you have finished with the original.
* An iterator is a pointer to an element in the container or to the associated `end` (or `r_end`, if the container supports it). In the documentation below, these pointers are referred to as "pointer-iterators".
* In the documentation below, `el_ty` is the container's element type and `key_ty` is the container's key type (where applicable).

## Options

The following can be defined before including the library in any file:

```c
#define CC_NO_SHORT_NAMES
```

<dl><dd>

By default, **CC** exposes API macros without the `cc_` prefix.  
Define this flag to withhold the unprefixed names.
</dd></dl>

The following can be defined anywhere and affect all calls to API macros where the definition is visible:

```c
#define CC_REALLOC our_realloc
```

<dl><dd>

Causes API macros to use a custom `realloc` function rather than the one in the standard library.
</dd></dl>

```c
#define CC_FREE our_free
```

<dl><dd>

Causes API macros to use a custom `free` function rather than the one in the standard library.
</dd></dl>

## All containers

The following function-like macros operate on all containers:

```c
void init( <any container type> *cntr )
```

<dl><dd>

Initializes `cntr` for use.  
This call cannot fail (it does not allocate memory).
</dd></dl>

```c
bool init_clone( <any container type> *cntr, <same container type> *src )
```

<dl><dd>

Initializes `cntr` as a shallow copy of `src`.  
Returns `true`, or `false` if unsuccessful due to memory allocation failure.
</dd></dl>

```c
size_t size( <any container type> *cntr )
```

<dl><dd>

Returns the number of elements.
</dd></dl>

```c
void clear( <any container type> *cntr )
```

<dl><dd>

Erases all elements, calling the element and key types' destructors if they exist.
</dd></dl>

```c
void cleanup( <any container type> *cntr )
```

<dl><dd>

Erases all elements (calling the element and key types' destructors if they exist), frees any other memory associated with the container, and initializes the container for reuse.
</dd></dl>

```c
el_ty *first( <any container type> *cntr )
```

<dl><dd>

Returns a pointer-iterator to the first element, or an `end` pointer-iterator if the container is empty.
</dd></dl>

```c
el_ty *next( <any container type> *cntr, el_ty *i )
```

<dl><dd>

Returns a pointer-iterator to the element after the one pointed to by `i`.  
If `i` points to the last element, the return value is an `end` pointer-iterator.  
If `i` points to `r_end`, the return value is a pointer-iterator to the first element, or an `end` pointer-iterator if the container is empty.
</dd></dl>

```c
el_ty *end( <any container type> *cntr )
```

<dl><dd>

Returns an `end` pointer-iterator for the container.
</dd></dl>

```c
for_each( <any container type> *cntr, i_name )
```

<dl><dd>

Creates a loop iterating over all elements from first to last.  
This macro declares a pointer-iterator (`el_ty *`) named `i_name`.  
It is equivalent to `for( el_ty *i_name = first( cntr ); i_name != end( cntr ); i_name = next( cntr, i_name ) )` and should be followed by the loop body.
</dd></dl>

## Vector

A `vec` is a dynamic array that stores elements in contiguous memory.

Vector pointer-iterators (including `end`) are invalidated by any API calls that cause memory reallocation.

The following function-like macros operate on vectors:

```c
vec( el_ty ) cntr
```

<dl><dd>

Declares an uninitialized vector named `cntr`.
</dd></dl>

```c
size_t cap( vec( el_ty ) *cntr )
```

<dl><dd>

Returns the current capacity.
</dd></dl>

```c
bool reserve( vec( el_ty ) *cntr, size_t n )
```

<dl><dd>

Ensures that the the capacity is large enough to accommodate `n` elements.  
Returns `true`, or `false` if unsuccessful due to memory allocation failure.
</dd></dl>

```c
bool resize( vec( el_ty ) *cntr, size_t n )
```

<dl><dd>

Sets the number of elements to `n`.  
If `n` is above the current size, the new elements are uninitialized.  
If `n` is below the current size, the element type's destructor (if it exists) is called for each erased element.  
Returns `true`, or `false` if unsuccessful due to memory allocation failure.
</dd></dl>

```c
bool shrink( vec( el_ty ) *cntr )
```

<dl><dd>

Shrinks the capacity to the current size.  
Returns `true`, or `false` if unsuccessful due to memory allocation failure.
</dd></dl>

```c
el_ty *get( vec( el_ty ) *cntr, size_t i )
```

<dl><dd>

Returns a pointer-iterator to the element at index `i`.
</dd></dl>

```c
el_ty *push( vec( el_ty ) *cntr, el_ty el )
```

<dl><dd>

Inserts `el` at the end of the vector.  
Returns a pointer-iterator to the new element, or `NULL` in the case of memory allocation failure.
</dd></dl>

```c
el_ty *push_n( vec( el_ty ) *cntr, el_ty *els, size_t n )
```

<dl><dd>

Inserts `n` elements from array `els` at the end of the vector.  
Returns a pointer-iterator to the first new element, or `NULL` in the case of memory allocation failure.
</dd></dl>

```c
el_ty *insert( vec( el_ty ) *cntr, size_t i, el_ty el )
```

<dl><dd>

Inserts `el` at index `i`.  
Returns a pointer-iterator to the new element, or `NULL` in the case of memory allocation failure.
</dd></dl>

```c
el_ty *insert_n( vec( el_ty ) *cntr, size_t i, el_ty *els, size_t n )
```

<dl><dd>

Inserts `n` elements from array `els` at index `i`.  
Returns a pointer-iterator to the first new element, or `NULL` in the case of memory allocation failure.
</dd></dl>

```c
el_ty *erase( vec( el_ty ) *cntr, size_t i )
```

<dl><dd>

Erases the element at index `i`, calling the element type's destructor if it exists.  
Returns a pointer-iterator to the element after the erased element, or an `end` pointer-iterator if there is no subsequent element.
</dd></dl>

```c
el_ty *erase_n( vec( el_ty ) *cntr, size_t i, size_t n )
```

<dl><dd>

Erases `n` elements beginning at index `i`, calling the element type's destructor, if it exists, for each erased element.  
Returns a pointer-iterator to the element after the erased elements, or an `end` pointer-iterator if there is no subsequent element.
</dd></dl>

```c
el_ty *last( vec( el_ty ) *cntr )
```

<dl><dd>

Returns a pointer-iterator to the last element.  
This call is synonymous with `get( cntr, size( cntr ) - 1 )` and assumes that at the vector is not empty.
</dd></dl>

## List

A `list` is a doubly linked list.

List pointer-iterators (including `r_end` and `end`) are not invalidated by any API calls besides `init` and `cleanup`, unless they point to erased elements.

The following function-like macros operate on lists:

```c
list( el_ty ) cntr
```

<dl><dd>

Declares an uninitialized list named `cntr`.
</dd></dl>

```c
el_ty *insert( list( el_ty ) *cntr, el_ty *i, el_ty el )
```

<dl><dd>

Inserts `el` before pointer-iterator `i`.  
Returns a pointer-iterator to the new element, or `NULL` in the case of memory allocation failure.
</dd></dl>

```c
el_ty *push( list( el_ty ) *cntr, el_ty el )
```

<dl><dd>

Inserts `el` at the end of the list.  
Returns a pointer-iterator to the new element, or `NULL` in the case of memory allocation failure.  
This call is synonymous with `insert( cntr, end( cntr ), el )`.
</dd></dl>

```c
el_ty *erase( list( el_ty ) *cntr, el_ty *i )
```

<dl><dd>

Erases the element pointed to by pointer-iterator `i`, calling the element type's destructor if it exists.  
Returns a pointer-iterator to the element after `i`, or an `end` pointer-iterator if `i` was the last element.
</dd></dl>

```c
bool splice( list( el_ty ) *cntr, el_ty *i, list( el_ty ) src, el_ty *src_i )
```

<dl><dd>

Removes the element pointed to by pointer-iterator `src_i` from `src` and inserts it before the element pointed to by pointer-iterator `i` in `cntr`.  
Returns `true`, or `false` if unsuccessful.  
This call only allocates memory, and therefore can only fail, if the list has not had any element inserted, pushed, or spliced into it since it was initialized.
</dd></dl>

```c
el_ty *last( list( el_ty ) *cntr )
```

<dl><dd>

Returns a pointer-iterator to the last element, or an `r_end` pointer-iterator if the list is empty.
</dd></dl>

```c
el_ty *prev( list( el_ty ) *cntr, el_ty *i )
```

<dl><dd>

Returns a pointer-iterator to the element before the one pointed to by `i`.  
If `i` points to the first element, the return value is an `r_end` pointer-iterator.  
If `i` points to `end`, then the return value is a pointer-iterator to the last element, or an `r_end` pointer-iterator if the list is empty.
</dd></dl>

```c
el_ty *r_end( list( el_ty ) *cntr )
```

<dl><dd>

Returns an `r_end` (reverse end) pointer-iterator for the list.
</dd></dl>

```c
r_for_each( list( el_ty ) *cntr, i_name )
```

<dl><dd>

Creates a loop iterating over all elements from last to first.  
This macro declares an `el_ty *` pointer-iterator named `i_name`.  
It is equivalent to `for( el_ty *i_name = last( cntr ); i_name != r_end( cntr ); i_name = prev( cntr, i_name ) )` and should be followed by the body of the loop.
</dd></dl>

##  Map

A `map` is an unordered associative container mapping elements to keys, implemented as a hybrid open-addressing, chained hash table.

Map pointer-iterators (including `end`) may be invalidated by any API calls that cause memory reallocation.

The following function-like macros operate on maps:

```c
map( key_ty, el_ty ) cntr
```

<dl><dd>

Declares an uninitialized map named `cntr`.  
`key_ty` must be a type, or alias for a type, for which comparison and hash functions have been defined (this requirement is enforced internally such that neglecting it causes a compiler error).  
For types with in-built comparison and hash functions, and for details on how to declare new comparison and hash functions, see *Destructor, comparison, and hash functions and custom max load factors* below.
</dd></dl>

```c
size_t cap( map( key_ty, el_ty ) *cntr )
```

<dl><dd>

Returns the current capacity, i.e. bucket count.  
Note that the number of elements a map can accommodate without rehashing is not its capacity but its capacity multiplied by the max load factor associated with its key type.
</dd></dl>

```c
bool reserve( map( key_ty, el_ty ) *cntr, size_t n )
```

<dl><dd>

Ensures that the capacity is large enough to accommodate `n` elements without rehashing.  
Returns `true`, or `false` if unsuccessful due to memory allocation failure.
</dd></dl>

```c
bool shrink( map( key_ty, el_ty ) *cntr )
```

<dl><dd>

Shrinks the capacity to best accommodate the current size.  
Returns `true`, or `false` if unsuccessful due to memory allocation failure.
</dd></dl>

```c
el_ty *insert( map( key_ty, el_ty ) *cntr, key_ty key, el_ty el )
```

<dl><dd>

Inserts element `el` with the specified key.  
If an element with the same key already exists, the existing element is replaced.  
Returns a pointer-iterator to the new element, or `NULL` in the case of memory allocation failure.
</dd></dl>

```c
el_ty *get( map( key_ty, el_ty ) *cntr, key_ty key )
```

<dl><dd>

Returns a pointer-iterator to the element with the specified key, or `NULL` if no such element exists.
</dd></dl>

```c
el_ty *get_or_insert( map( key_ty, el_ty ) *cntr, key_ty key, el_ty el )
```

<dl><dd>

Inserts element `el` if no element with the specified key already exists.  
Returns a pointer-iterator to the new element if it was inserted, or a pointer-iterator to the existing element with the same key, or `NULL` in the case of memory allocation failure.  
Determine whether an element was inserted by comparing the map's size before and after the call.
</dd></dl>

```c
const key_ty *key_for( map( key_ty, el_ty ) *cntr, el_ty *i )
```

<dl><dd>

Returns a `const` pointer to the key for the element pointed to by pointer-iterator `i`.
</dd></dl>

```c
bool erase( map( key_ty, el_ty ) *cntr, key_ty key )
```

<dl><dd>

Erases the element with the specified key, if it exists.  
Returns `true` if an element was erased, or `false` if no such element exists.
</dd></dl>

```c
el_ty *erase_itr( map( key_ty, el_ty ) *cntr, el_ty *i )
```

<dl><dd>

Erases the element pointed to by pointer-iterator `i`.  
Returns a pointer-iterator to the next element in the map, or an `end` pointer-iterator if the erased element was the last one.
</dd></dl>

```c
for_each( map( key_ty, el_ty ) *cntr, key_ptr_name, i_name )
```

<dl><dd>

Creates a loop iterating over all elements from first to last, with easy access to the corresponding keys.  
This macro declares a pointer to the key (`const key_ty *`) named `key_ptr_name` and a pointer-iterator (`el_ty *`) named `i_name`.  
It should be followed by the body of the loop.
</dd></dl>

## Set

A `set` is an unordered associative container for elements without a separate key, implemented as a hybrid open-addressing, chained hash table.

Set pointer-iterators (including `end`) may be invalidated by any API calls that cause memory reallocation.

The following function-like macros operate on sets:

```c
set( el_ty ) cntr
```

<dl><dd>

Declares an uninitialized set named `cntr`.  
`el_ty` must be a type, or alias for a type, for which comparison and hash functions have been defined (this requirement is enforced internally such that neglecting it causes a compiler error).  
For types with in-built comparison and hash functions, and for details on how to declare new comparison and hash functions, see *Destructor, comparison, and hash functions and custom max load factors* below.
</dd></dl>

```c
size_t cap( set( el_ty ) *cntr )
```

<dl><dd>

Returns the current capacity, i.e. bucket count.  
Note that the number of elements a set can accommodate without rehashing is not its capacity but its capacity multiplied by the max load factor associated with its key type.
</dd></dl>

```c
bool reserve( set( el_ty ) *cntr, size_t n )
```

<dl><dd>

Ensures that the capacity is large enough to accommodate `n` elements without rehashing.  
Returns `true`, or `false` if unsuccessful due to memory allocation failure.
</dd></dl>

```c
bool shrink( set( el_ty ) *cntr )
```

<dl><dd>

Shrinks the capacity to best accommodate the current size.  
Returns `true`, or `false` if unsuccessful due to memory allocation failure.
</dd></dl>

```c
el_ty *insert( set( el_ty ) *cntr, el_ty el )
```

<dl><dd>

Inserts element `el`.  
If the element already exists, the existing element is replaced.  
Returns a pointer-iterator to the new element, or `NULL` in the case of memory allocation failure.
</dd></dl>

```c
el_ty *get( set( el_ty ) *cntr, el_ty el )
```

<dl><dd>

Returns a pointer-iterator to element `el`, or `NULL` if no such element exists.
</dd></dl>

```c
el_ty *get_or_insert( set( el_ty ) *cntr, el_ty el )
```

<dl><dd>

Inserts element `el` if it does not already exist.  
Returns a pointer-iterator to the new element if it was inserted, or a pointer-iterator to the existing element, or `NULL` in the case of memory allocation failure.  
Determine whether an element was inserted by comparing the set's size before and after the call.
</dd></dl>

```c
bool erase( set( el_ty ) *cntr, el_ty el )
```

<dl><dd>

Erases the element `el`, if it exists.  
Returns `true` if an element was erased, or `false` if no such element exists.
</dd></dl>

```c
el_ty *erase_itr( set( el_ty ) *cntr, el_ty *i )
```

<dl><dd>

Erases the element pointed to by pointer-iterator `i`.  
Returns a pointer-iterator to the next element in the set, or an `end` pointer-iterator if the erased element was the last one.
</dd></dl>

## Ordered map

An `omap` is an ordered associative container mapping elements to keys, implemented as a red-black tree.

Ordered map pointer-iterators (including `r_end` and `end`) are not invalidated by any API calls besides `init` and `cleanup`, unless they point to erased elements.

The following function-like macros operate on ordered maps:

```c
omap( key_ty, el_ty ) cntr
```

<dl><dd>

Declares an uninitialized ordered map named `cntr`.  
`key_ty` must be a type, or alias for a type, for which a comparison function has been defined (this requirement is enforced internally such that neglecting it causes a compiler error).  
For types with in-built comparison functions, and for details on how to declare new comparison functions, see *Destructor, comparison, and hash functions and custom max load factors* below.
</dd></dl>

```c
el_ty *insert( omap( key_ty, el_ty ) *cntr, key_ty key, el_ty el )
```

<dl><dd>

Inserts element `el` with the specified key.  
If an element with the same key already exists, the existing element is replaced.  
Returns a pointer-iterator to the new element, or `NULL` in the case of memory allocation failure.
</dd></dl>

```c
el_ty *get( omap( key_ty, el_ty ) *cntr, key_ty key )
```

<dl><dd>

Returns a pointer-iterator to the element with the specified key, or `NULL` if no such element exists.
</dd></dl>

```c
el_ty *get_or_insert( omap( key_ty, el_ty ) *cntr, key_ty key, el_ty el )
```

<dl><dd>

Inserts element `el` if no element with the specified key already exist.  
Returns a pointer-iterator to the new element if it was inserted, or a pointer-iterator to the existing element with the same key, or `NULL` in the case of memory allocation failure.  
Determine whether an element was inserted by comparing the ordered map's size before and after the call.
</dd></dl>

```c
const key_ty *key_for( omap( key_ty, el_ty ) *cntr, el_ty *i )
```

<dl><dd>

Returns a `const` pointer to the key for the element pointed to by pointer-iterator `i`.
</dd></dl>

```c
bool erase( omap( key_ty, el_ty ) *cntr, key_ty key )
```

<dl><dd>

Erases the element with the specified key, if it exists.  
Returns `true` if an element was erased, or `false` if no such element exists.
</dd></dl>

```c
el_ty *erase_itr( omap( key_ty, el_ty ) *cntr, el_ty *i )
```

<dl><dd>

Erases the element pointed to by pointer-iterator `i`.  
Returns a pointer-iterator to the next element in the ordered map, or an end pointer-iterator if the erased element was the last one.
</dd></dl>

```c
el_ty *first( omap( key_ty, el_ty ) *cntr, key_ty key )
```

<dl><dd>

Returns a pointer-iterator to the first element with a key greater than or equal to the specified key, or an `end` pointer-iterator if no such element exists.
</dd></dl>

```c
el_ty *last( omap( key_ty, el_ty ) *cntr )
```

<dl><dd>

Returns a pointer-iterator to the last element, or an `r_end` pointer-iterator if the ordered map is empty.
</dd></dl>

```c
el_ty *last( omap( key_ty, el_ty ) *cntr, key_ty key )
```

<dl><dd>

Returns a pointer-iterator to the last element with a key less than or equal to the specified key, or an `r_end` pointer-iterator if the ordered map is empty.
</dd></dl>

```c
el_ty *prev( omap( key_ty, el_ty ) *cntr, el_ty *i )
```

<dl><dd>

Returns a pointer-iterator to the element before the one pointed to by `i`.  
If `i` points to the first element, the value returned is an `r_end` pointer-iterator.  
If `i` points to `end`, then the value returned points to the last element, or is an `r_end` pointer-iterator if the ordered map is empty.
</dd></dl>

```c
el_ty *r_end( omap( key_ty, el_ty ) *cntr )
```

<dl><dd>

Returns an `r_end` (reverse end) pointer-iterator for the ordered map.
</dd></dl>

```c
for_each( omap( key_ty, el_ty ) *cntr, key_ptr_name, i_name )
```

<dl><dd>

Creates a loop iterating over all elements from first to last, with easy access to the corresponding keys.  
This macro declares a pointer to the key (`const key_ty *`) named `key_ptr_name` and a pointer-iterator (`el_ty *`) named `i_name`.  
It should be followed by the body of the loop.
</dd></dl>

```c
r_for_each( omap( key_ty, el_ty ) *cntr, i_name )
```

<dl><dd>

Creates a loop iterating over all elements from last to first.  
This macro declares an `el_ty *` pointer-iterator named `i_name`.  
It is equivalent to `for( el_ty *i_name = last( cntr ); i_name != r_end( cntr ); i_name = prev( cntr, i_name ) )` and should be followed by the body of the loop.
</dd></dl>

```c
r_for_each( omap( key_ty, el_ty ) *cntr, key_ptr_name, i_name )
```

<dl><dd>

Creates a loop iterating over all elements from last to first, with easy access to the corresponding keys.  
This macro declares a pointer to the key (`const key_ty *`) named `key_ptr_name` and a pointer-iterator (`el_ty *`) named `i_name`.  
It should be followed by the body of the loop.
</dd></dl>

## Ordered set

An `oset` is an ordered associative container for elements without a separate key, implemented as a red-black tree.

Ordered set pointer-iterators (including `r_end` and `end`) may be invalidated by any API calls that cause memory reallocation.

The following function-like macros operate on ordered sets:

```c
oset( el_ty ) cntr
```

<dl><dd>

Declares an uninitialized ordered set named `cntr`.  
`el_ty` must be a type, or alias for a type, for which a comparison function has been defined (this requirement is enforced internally such that neglecting it causes a compiler error).  
For types with in-built comparison functions, and for details on how to declare new comparison functions, see *Destructor, comparison, and hash functions and custom max load factors* below.
</dd></dl>

```c
el_ty *insert( oset( el_ty ) *cntr, el_ty el )
```

<dl><dd>

Inserts element `el`.  
If the element already exists, the existing element is replaced.  
Returns a pointer-iterator to the new element, or `NULL` in the case of memory allocation failure.
</dd></dl>

```c
el_ty *get( oset( el_ty ) *cntr, el_ty el )
```

<dl><dd>

Returns a pointer-iterator to element `el`, or `NULL` if no such element exists.
</dd></dl>

```c
el_ty *get_or_insert( oset( el_ty ) *cntr, el_ty el )
```

<dl><dd>

Inserts element `el` if it does not already exist.  
Returns a pointer-iterator to the new element if it was inserted, or a pointer-iterator to the existing element, or `NULL` in the case of memory allocation failure.  
Determine whether an element was inserted by comparing the ordered set's size before and after the call.
</dd></dl>

```c
bool erase( oset( el_ty ) *cntr, el_ty el )
```

<dl><dd>

Erases the element `el`, if it exists.  
Returns `true` if an element was erased, or `false` if no such element exists.
</dd></dl>

```c
el_ty *erase_itr( oset( el_ty ) *cntr, el_ty *i )
```

<dl><dd>

Erases the element pointed to by pointer-iterator `i`.  
Returns a pointer-iterator to the next element in the ordered set, or an `end` pointer-iterator if the erased element was the last one.
</dd></dl>

```c
el_ty *first( oset( key_ty, el_ty ) *cntr, el_ty el )
```

<dl><dd>

Returns a pointer-iterator to the first element greater than or equal to `el`, or an `end` pointer-iterator if no such element exists.
</dd></dl>

```c
el_ty *last( oset( el_ty ) *cntr )
```

<dl><dd>

Returns a pointer-iterator to the last element, or an `r_end` pointer-iterator if the ordered set is empty.
</dd></dl>

```c
el_ty *last( oset( key_ty, el_ty ) *cntr, el_ty el )
```

<dl><dd>

Returns a pointer-iterator to the last element less than or equal to `el`, or an `end` pointer-iterator if no such element exists.
</dd></dl>

```c
el_ty *prev( oset( el_ty ) *cntr, el_ty *i )
```

<dl><dd>

Returns a pointer-iterator to the element before the one pointed to by `i`.  
If `i` points to the first element, the return value is an `r_end` pointer-iterator.  
If `i` points to `end`, then the pointer-iterator returned points to the last element, or is an `r_end` pointer-iterator if the ordered set is empty.
</dd></dl>

```c
el_ty *r_end( oset( el_ty ) *cntr )
```

<dl><dd>

Returns an `r_end` (reverse end) pointer-iterator for the ordered set.
</dd></dl>

```c
r_for_each( oset( el_ty ) *cntr, i_name )
```

<dl><dd>

Creates a loop iterating over all elements from last to first.  
This macro declares an `el_ty *` pointer-iterator named `i_name`.  
It is equivalent to `for( el_ty *i_name = last( cntr ); i_name != r_end( cntr ); i_name = prev( cntr, i_name ) )` and should be followed by the body of the loop.
</dd></dl>

## Destructor, comparison, and hash functions and custom max load factors

This part of the API allows the user to define custom destructor, comparison, and hash functions and max load factors for a type.

Once these functions are defined, any container using that type for its elements or keys will call them automatically.

Once the max load factor is defined, any map using the type for its keys and any set using the type for its elements will use the defined load factor to determine when rehashing is necessary.

```c
#define CC_DTOR ty, { function body }
#include "cc.h"
```

<dl><dd>

Defines a destructor for type `ty`.  
The signature of the function is `void ( ty val )`.
</dd></dl>

```c
#define CC_CMPR ty, { function body }
#include "cc.h"
```

<dl><dd>

Defines a comparison function for type `ty`.  
The signature of the function is `int ( ty val_1, ty val_2 )`.  
The function should return `0` if `val_1` and `val_2` are equal, a negative integer if `val_1` is less than `val_2`, and a positive integer if `val_1` is greater than `val_2`.
</dd></dl>

```c
#define CC_HASH ty, { function body }
#include "cc.h"
```

<dl><dd>

Defines a hash function for type `ty`.  
The signature of the function is `size_t ( ty val )`.  
The function should return the hash of `val`.
</dd></dl>

```c
#define CC_LOAD ty, max_load_factor
```

<dl><dd>

Defines the max load factor for type `ty`.  
`max_load_factor` should be a `float` or `double` between `0.0` and `1.0`.  
The default max load factor is `0.9`.
</dd></dl>

Trivial example:

```c
typedef struct { int x; } our_type;
#define CC_DTOR our_type, { printf( "!%d\n", val.x ); }
#define CC_CMPR our_type, { return val_1.x < val_2.x ? -1 : val_1.x > val_2.x; }
#define CC_HASH our_type, { return val.x * 2654435761ull; }
#define CC_LOAD our_type, 0.5
#include "cc.h"
```

Notes:
* These functions are `inline` and have `static` scope, so you need to either redefine them in each translation unit from which they should be called or (preferably) define them in a shared header. For structs or unions, a sensible place to define them is immediately after the definition of the struct or union.
* Only one destructor, comparison, or hash function or max load factor should be defined by the user for each type.
* Including `cc.h` in these cases does not include the full header, so you still need to include it separately at the top of your files.
* In-built comparison and hash functions are already defined for the following types: `char`, `unsigned char`, `signed char`, `unsigned short`, `short`, `unsigned int`, `int`, `unsigned long`, `long`, `unsigned long long`, `long long`, `size_t`, and null-terminated strings (`char *` and `const char *`). Defining a comparison or hash function for one of these types will overwrite the in-built function.