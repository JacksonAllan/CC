# CC API Reference

> **Warning**
> API macros may evaluate their first argument - the pointer to the container - multiple times, so never use expressions with side effects (e.g. `&our_containers[ ++i ]` ) for that argument. In GCC and Clang, attempting to do so will cause a compiler warning. All other arguments are only evaluated once.

> **Note**
> * If `CC_NO_SHORT_NAMES` was declared, all API macros are prefixed with `cc_`.
> * Duplicating a container handle via assignment and then operating on the duplicate will invalidate the original. Hence, only create a duplicate via assignment (including through function parameters and return values) if you have finished with the original.
> * An iterator is a pointer to an element in the container or to the associated `end` (or `r_end`, if the container supports it). In the documentation below, these pointers are called "pointer-iterators".
> * In the documentation below, `el_ty` is the container's element type and `key_ty` is the container's key type (where applicable).

## All containers

The following macros behave the same way for all containers:

```c
void init( <any container type> *cntr )
```

Initializes `cntr` for use.  
This call cannot fail (it does not allocate memory).

```c
bool init_clone( <any container type> *cntr, <same container type> *src )
```

Initializes `cntr` as a shallow copy of `src`.  
Returns `true`, or `false` if unsuccessful due to memory allocation failure.

```c
size_t size( <any container type> *cntr )
```

Returns the number of elements.

```c
void clear( <any container type> *cntr )
```

Erases all elements, calling the element and key types' destructors if they exist.

```c
void cleanup( <any container type> *cntr )
```

Erases all elements (calling the element and key types' destructors if they exist), frees any other memory associated with the container, and initializes the container for reuse.

```c
for_each( <any container type> *cntr, i_name )
```

Creates a loop iterating over all elements from first to last.  
This macro declares a pointer-iterator (`el_ty *`) named `i_name`.  
It is equivalent to `for( el_ty *i_name = first( cntr ); i_name != end( cntr ); i_name = next( cntr, i_name ) )` and should be followed by the loop body.

## Vector

A `vec` is a dynamic array that stores elements in contiguous memory.

```c
vec( el_ty ) cntr
```

Declares an uninitialized vector named `cntr`.

```c
size_t cap( vec( el_ty ) *cntr )
```

Returns the current capacity.

```c
bool reserve( vec( el_ty ) *cntr, size_t n )
```

Ensures that the the capacity is large enough to support `n` elements.  
Returns `true`, or `false` if unsuccessful due to memory allocation failure.

```c
bool resize( vec( el_ty ) *cntr, size_t n )
```

Sets the number of elements to `n`.  
If `n` is above the current size, the new elements are uninitialized.  
If `n` is below the current size, the element type's destructor (if it exists) is called for each erased element.  
Returns `true`, or `false` if unsuccessful due to memory allocation failure.

```c
bool shrink( vec( el_ty ) *cntr )
```

Shrinks the capacity to the current size.  
Returns `true`, or `false` if unsuccessful due to memory allocation failure.

```c
el_ty *get( vec( el_ty ) *cntr, size_t i )
```

Returns an a pointer-iterator to the element at index `i`.

```c
el_ty *push( vec( el_ty ) *cntr, el_ty el )
```

Inserts `el` at the end of the vector.  
Returns a pointer-iterator to the new element, or `NULL` in the case of memory allocation failure.

```c
el_ty *push_n( vec( el_ty ) *cntr, el_ty *els, size_t n )
```

Inserts `n` elements from array `els` at the end of the vector.  
Returns a pointer-iterator to the first new element, or `NULL` in the case of memory allocation failure.

```c
el_ty *insert( vec( el_ty ) *cntr, size_t i, el_ty el )
```

Inserts `el` at index `i`.  
Returns a pointer-iterator to the new element, or `NULL` in the case of memory allocation failure.

```c
el_ty *insert_n( vec( el_ty ) *cntr, size_t i, el_ty *els, size_t n )
```

Inserts `n` elements from array els at index `i`.  
Returns a pointer-iterator to the first new element, or `NULL` in the case of memory allocation failure.

```c
el_ty *erase( vec( el_ty ) *cntr, size_t i )
```

Erases the element at index `i`, calling the element type's destructor if it exists.  
Returns a pointer-iterator to the element after the erased element, or an `end` pointer-iterator if there is no subsequent element.

```c
el_ty *erase_n( vec( el_ty ) *cntr, size_t i, size_t n )
```

Erases `n` elements beginning at index `i`, calling the element type's destructor, if it exists, for each erased element.  
Returns a pointer-iterator to the element after the erased elements, or an `end` pointer-iterator if there is no subsequent element.

```c
el_ty *end( vec( el_ty ) *cntr )
```

Returns an `end` pointer-iterator.  
This call is synonymous with `get( cntr, size( cntr ) )`.

```c
el_ty *first( vec( el_ty ) *cntr )
```

Returns an pointer-iterator to the first element, or an `end` pointer-iterator if the vector is empty.  
This call is synonymous with `get( cntr, 0 )`.

```c
el_ty *last( vec( el_ty ) *cntr )
```

Returns a pointer-iterator to the last element.  
This call is synonymous with `get( cntr, size( cntr ) - 1 )`.  
It assumes that at the vector is not empty.

```c
el_ty *next( vec( el_ty ) *cntr, el_ty *i )
```

Returns a pointer-iterator to the element after the element pointed to by `i`, or an `end` pointer-iterator if `i` points to the last element.

> **Note**
> Vector pointer-iterators (including `end`) are invalidated by any API calls that cause memory reallocation.

## List

A `list` is a doubly linked list with sentinels.

```c
list( el_ty ) cntr
```

Declares an uninitialized list named `cntr`.

```c
el_ty *insert( list( el_ty ) *cntr, el_ty *i, el_ty el )
```

Inserts `el` before pointer-iterator `i`.  
Returns a pointer-iterator to the new element, or `NULL` in the case of memory allocation failure.

```c
el_ty *push( list( el_ty ) *cntr, el_ty el )
```

Inserts `el` at the end of the list.  
Returns a pointer-iterator to the new element, or `NULL` in the case of memory allocation failure.  
This call is synonymous with `insert( cntr, end( cntr ), el )`.

```c
el_ty *erase( list( el_ty ) *cntr, el_ty *i )
```

Erases element pointed to by pointer-iterator `i`, calling the element type's destructor if it exists.  
Returns a pointer-iterator to the element after `i`, or an `end` pointer-iterator if `i` was the last element.

```c
bool splice( list( el_ty ) *cntr, el_ty *i, list( el_ty ) src, el_ty *src_i )
```

Removes element pointed to by pointer-iterator `src_i` from `src` and inserts it before the element pointed to by pointer-iterator `i` in `cntr`.  
Returns `true`, or `false` if unsuccessful.  
This call only allocates memory, and therefore can only fail, if the list has not had any element inserted, pushed, or spliced into it since it was initialized.

```c
el_ty *first( list( el_ty ) *cntr )
```

Returns a pointer-iterator to the first element, or an `end` pointer-iterator if the list is empty.

```c
el_ty *last( list( el_ty ) *cntr )
```

Returns a pointer-iterator to the last element, or an `r_end` pointer-iterator if the list is empty.

```c
el_ty *r_end( list( el_ty ) *cntr )
```

Returns an `r_end` (reverse end) pointer-iterator for the list.  
`r_end` acts as a sentinel node.

```c
el_ty *end( list( el_ty ) *cntr )
```

Returns an `end` pointer-iterator for the list.  
`end` acts as a sentinel node.

```c
el_ty *next( list( el_ty ) *cntr, el_ty *i )
```

Returns a pointer-iterator to the element after the one pointed to by `i`.  
If `i` points to the last element, the return value is an `end` pointer-iterator.  
If `i` points to `r_end`, the return value is a pointer-iterator to the first element, or an `end` pointer-iterator if the list is empty.

```c
el_ty *prev( list( el_ty ) *cntr, el_ty *i )
```

Returns a pointer-iterator to the element before the one pointed to by `i`.  
If `i` points to the first element, the return value is an `r_end` pointer-iterator.  
If `i` points to `end`, then the return value is a pointer-iterator to the last element, or an `r_end` pointer-iterator if the list is empty.

```c
r_for_each( list( el_ty ) *cntr, i_name )
```

Creates a loop iterating over all elements from last to first.  
This macro declares an `el_ty *` pointer-iterator named `i_name`.  
It is equivalent to `for( el_ty *i_name = last( cntr ); i_name != r_end( cntr ); i_name = prev( cntr, i_name ) )` and should be followed by the body of the loop.

> **Note**
> List pointer-iterators (including `r_end` and `end`) are not invalidated by any API calls besides `init` and `cleanup`, unless they point to erased elements.

### Map

A `map` is an unordered container associating elements with keys, implemented as a Robin Hood hash table.

```c
map( key_ty, el_ty ) cntr
```

Declares an uninitialized map named `cntr`.  
`key_ty` must be a type, or alias for a type, for which comparison and hash functions have been defined.  
This requirement is enforced internally such that neglecting it causes a compiler error.  
For types with in-built comparison and hash functions, and for details on how to declare new comparison and hash functions, see _Destructor, comparison, and hash functions and custom max load factors_ below.

```c
size_t cap( map( key_ty, el_ty ) *cntr )
```

Returns the current capacity, i.e. bucket count.  
Note that the number of elements a map can support without rehashing is not its capacity but its capacity multiplied by the max load factor associated with its key type.

```c
bool reserve( map( key_ty, el_ty ) *cntr, size_t n )
```

Ensures that the capacity is large enough to support `n` elements without rehashing.  
Returns `true`, or `false` if unsuccessful due to memory allocation failure.

```c
bool shrink( map( key_ty, el_ty ) *cntr )
```

Shrinks the capacity to best accommodate the current size.  
Returns `true`, or `false` if unsuccessful due to memory allocation failure.

```c
el_ty *insert( map( key_ty, el_ty ) *cntr, key_ty key, el_ty el )
```

Inserts element `el` with the specified key.  
If an element with the same key already exists, the existing element is replaced.  
Returns a pointer-iterator to the new element, or `NULL` in the case of memory allocation failure.  
If adding one element would violate the map's max load factor, failure can occur even if it already contains the key.

```c
el_ty *get( map( key_ty, el_ty ) *cntr, key_ty key )
```

Returns a pointer-iterator to the element with the specified key, or `NULL` if no such element exists.

```c
el_ty *get_or_insert( map( key_ty, el_ty ) *cntr, key_ty key, el_ty el )
```

Inserts element `el` if no element with the specified key already exist.  
Returns a pointer-iterator to the new element if it was inserted, or a pointer-iterator to the existing element with the same key, or `NULL` in the case of memory allocation failure.  
If adding one element would violate the map's max load factor, failure can occur even if it already contains the key.  
Determine whether an element was inserted by comparing the map's size before and after the call.

```c
const key_ty *key_for( map( key_ty, el_ty ) *cntr, el_ty *i )
```

Returns a `const` pointer to the key for the element pointed to by pointer-iterator `i`.

```c
bool erase( map( key_ty, el_ty ) *cntr, key_ty key )
```

Erases the element with the specified key, if it exists.  
Returns `true` if an element was erased, or `false` if no such element exists.

```c
void erase_itr( map( key_ty, el_ty ) *cntr, el_ty *i )
```

Erases the element pointed to by pointer-iterator `i`.

```c
el_ty *first( map( key_ty, el_ty ) *cntr )
```

Returns a pointer-iterator to the first element, or an `end` pointer-iterator if the map is empty.

```c
el_ty *last( map( key_ty, el_ty ) *cntr )
```

Returns a pointer-iterator to the last element, or an `r_end` pointer-iterator if the map is empty.

```c
el_ty *r_end( map( key_ty, el_ty ) *cntr )
```

Returns an `r_end` (reverse end) pointer-iterator for the map.

```c
el_ty *end( map( key_ty, el_ty ) *cntr )
```

Returns an `end` pointer-iterator for the map.

```c
el_ty *next( map( key_ty, el_ty ) *cntr, el_ty *i )
```

Returns a pointer-iterator to the element after the one pointed to by `i`.  
If `i` points to the last element, the value returned is an `end` pointer-iterator.  
If `i` points to `r_end`, the value returned points to the first element, or is an `end` pointer-iterator if the map is empty.

```c
el_ty *prev( map( key_ty, el_ty ) *cntr, el_ty *i )
```

Returns a pointer-iterator to the element before the one pointed to by `i`.  
If `i` points to the first element, the value returned is an `r_end` pointer-iterator.  
If `i` points to `end`, then the value returned points to the last element, or is an `r_end` pointer-iterator if the map is empty.

```c
for_each( map( key_ty, el_ty ) *cntr, key_ptr_name, i_name )
```

Creates a loop iterating over all elements from first to last, with easy access to the corresponding keys.  
This macro declares a pointer to the key (`const key_ty *`) named `key_ptr_name` and a pointer-iterator (`el_ty *`) named `i_name`.  
It should be followed by the body of the loop.

```c
r_for_each( map( key_ty, el_ty ) *cntr, i_name )
```

Creates a loop iterating over all elements from last to first.  
This macro declares an `el_ty *` pointer-iterator named `i_name`.  
It is equivalent to `for( el_ty *i_name = last( cntr ); i_name != r_end( cntr ); i_name = prev( cntr, i_name ) )` and should be followed by the body of the loop.

```c
r_for_each( map( key_ty, el_ty ) *cntr, key_ptr_name, i_name )
```

Creates a loop iterating over all elements from last to first, with easy access to the corresponding keys.  
This macro declares a pointer to the key (`const key_ty *`) named `key_ptr_name` and a pointer-iterator (`el_ty *`) named `i_name`.  
It should be followed by the body of the loop.

> **Note**
> Map pointer-iterators (including `r_end` and `end`) may be invalidated by any API calls that cause memory reallocation.

## Set

A `set` is a Robin Hood hash table for elements without a separate key.

```c
set( el_ty ) cntr
```

Declares an uninitialized set named `cntr`.  
`el_ty` must be a type, or alias for a type, for which comparison and hash functions have been defined.  
This requirement is enforced internally such that neglecting it causes a compiler error.  
For types with in-built comparison and hash functions, and for details on how to declare new comparison and hash functions, see _Destructor, comparison, and hash functions and custom max load factors_ below.

```c
size_t cap( set( el_ty ) *cntr )
```

Returns the current capacity, i.e. bucket count.  
Note that the number of elements a set can support without rehashing is not its capacity but its capacity multiplied by the max load factor associated with its key type.

```c
bool reserve( set( el_ty ) *cntr, size_t n )
```

Ensures that the capacity is large enough to support `n` elements without rehashing.  
Returns `true`, or `false` if unsuccessful due to memory allocation failure.

```c
bool shrink( set( el_ty ) *cntr )
```

Shrinks the capacity to best accommodate the current size.  
Returns `true`, or `false` if unsuccessful due to memory allocation failure.

```c
el_ty *insert( set( el_ty ) *cntr, el_ty el )
```

Inserts element `el`.  
If the element already exists, the existing element is replaced.  
Returns a pointer-iterator to the new element, or `NULL` in the case of memory allocation failure.  
Note that if adding one element would violate the set's max load factor, failure can occur even if it already contains `el`.

```c
el_ty *get( set( el_ty ) *cntr, el_ty el )
```

Returns a pointer-iterator to element `el`, or `NULL` if no such element exists.

```c
el_ty *get_or_insert( set( el_ty ) *cntr, el_ty el )
```

Inserts element `el` if it does not already exist.  
Returns a pointer-iterator to the new element if it was inserted, or a pointer-iterator to the existing element, or `NULL` in the case of memory allocation failure.  
If adding one element would violate the set's max load factor, failure can occur even if it already contains the element.  
Determine whether an element was inserted by comparing the set's size before and after the call.

```c
bool erase( set( el_ty ) *cntr, el_ty el )
```

Erases the element `el`, if it exists.  
Returns `true` if an element was erased, or `false` if no such element exists.

```c
el_ty *first( set( el_ty ) *cntr )
```

Returns a pointer-iterator to the first element, or an `end` pointer-iterator if the set is empty.

```c
el_ty *last( set( el_ty ) *cntr )
```

Returns a pointer-iterator to the last element, or an `r_end` pointer-iterator if the set is empty.

```c
el_ty *r_end( set( el_ty ) *cntr )
```

Returns an `r_end` (reverse end) pointer-iterator for the set.

```c
el_ty *end( set( el_ty ) *cntr )
```

Returns an `end` pointer-iterator for the set.

```c
el_ty *next( set( el_ty ) *cntr, el_ty *i )
```

Returns a pointer-iterator to the element after the one pointed to by `i`.  
If `i` points to the last element, the pointer-iterator returned is an `end` pointer-iterator.  
If `i` points to `r_end`, then the pointer-iterator returned points to the first element, or is an `end` pointer-iterator if the set is empty.

```c
el_ty *prev( set( el_ty ) *cntr, el_ty *i )
```

Returns a pointer-iterator to the element before the one pointed to by `i`.  
If `i` points to the first element, the return value is an `r_end` pointer-iterator.  
If `i` points to `end`, then the pointer-iterator returned points to the last element, or is an `r_end` pointer-iterator if the set is empty.

```c
r_for_each( set( el_ty ) *cntr, i_name )
```

Creates a loop iterating over all elements from last to first.  
This macro declares an `el_ty *` pointer-iterator named `i_name`.  
It is equivalent to `for( el_ty *i_name = last( cntr ); i_name != r_end( cntr ); i_name = prev( cntr, i_name ) )` and should be followed by the body of the loop.

> **Note**
> Set pointer-iterators (including `r_end` and `end`) may be invalidated by any API calls that cause memory reallocation.

## Destructor, comparison, and hash functions and custom max load factors

This part of the API allows the user to define custom destructor, comparison, and hash functions and max load factors for a type.  
Once these functions are defined, any container using that type for its elements or keys will call them automatically.  
Once the max load factor is defined, any map using the type for its key and any set using the type for its elements will use the defined load factor to determine when rehashing is necessary.

```c
#define CC_DTOR ty, { function body }
#include "cc.h"
```

Defines a destructor for type `ty`.  
The signature of the function is `void ( ty val )`.

```c
#define CC_CMPR ty, { function body }
#include "cc.h"
```

Defines a comparison function for type `ty`.  
The signature of the function is `int ( ty val_1, ty val_2 )`.  
The function should return `0` if `val_1` and `val_2` are equal, a negative integer if `val_1` is less than `val_2`, and a positive integer if `val_1` is more than `val_2`.

```c
#define CC_HASH ty, { function body }
#include "cc.h"
```

Defines a hash function for type `ty`.  
The signature of the function is `size_t ( ty val )`.  
The function should return the hash of `val`.

```c
#define CC_LOAD ty, max_load_factor
```

Defines the max load factor for type `ty`.  
`max_load_factor` should be a `float` or `double` between `0.0` and `1.0`.  
The default max load factor is `0.8`.

Trivial example:

```c
typedef struct { int x; } my_type;
#define CC_DTOR my_type, { printf( "!%d\n", val.x ); }
#define CC_CMPR my_type, { return ( val_1.x > val_2.x ) - ( val_1.x < val_2.x ); }
#define CC_HASH my_type, { return val.x * 2654435761ull; }
#define CC_LOAD my_type, 0.5
#include "cc.h"
```

> **Note**
> * These functions are inline and have static scope, so you need to either redefine them in each translation unit from which they should be called or (preferably) define them in a shared header. For structs or unions, a sensible place to define them would be immediately after the definition of the struct or union.
> * Only one destructor, comparison, or hash function or max load factor should be defined by the user for each type.
> * Including `cc.h` in these cases does not include the full header, so you still need to include it separately at the top of your files.
> * In-built comparison and hash functions are already defined for the following types: `char`, `unsigned char`, `signed char`, `unsigned short`, `short`, `unsigned int`, `int`, `unsigned long`, `long`, `unsigned long long`, `long long`, `size_t`, and `char *` (a `NULL`-terminated string). Defining a comparsion or hash function for one of these types will overwrite the in-built function.
