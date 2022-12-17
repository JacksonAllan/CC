# CC.H API Reference

> **Warning**
> API macros may evaluate their first argument - the pointer to the container - multiple times, so never use expressions with side effects (e.g. `&our_containers[ ++i ]` ) for that argument. In GCC and Clang, attempting to do so will cause a compiler warning. All other arguments are only evaluated once.

* If `CC_NO_SHORT_NAMES` was declared, all API macros are prefixed with `cc_`.

* Duplicating a container handle via assignment and then operating on the duplicate will invalidate the original. Hence, only create a duplicate via assignment (including through function parameters and return values) if you have finished with the original.

* An iterator is a pointer to an element in the container or to the associated `end` (or `r_end`, if the container supports it). In the documentation below, these pointers are called "pointer-iterators".

* In the documentation below, `el_ty` is the container's element type and `key_ty` is the container's key type (where applicable).

### All containers:

The following macros behave the same way for all containers:

```c
void init( <any container type> *cntr )
```

Initializes the container for use.  
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

Erases all elements, calling the element and key types' destructors where applicable.

```c
void cleanup( <any container type> *cntr )
```

Erases all elements (calling the element and key types' destructors where applicable), frees any other memory associated with the container, and initializes the container for reuse.

```c
for_each( <any container type> *cntr, i_name )
```

Creates a loop iterating over all elements from first to last.  
This macro declares a pointer-iterator (`el_ty *`) named `i_name`.  
It is equivalent to `for( el_ty* i_name = first( cntr ); i_name != end( cntr ); i_name = next( cntr, i_name ) )` and should be followed by the loop body.

### Vector

(a dynamic array that stores elements in contiguous memory)

```c
vec( el_ty ) cntr
```

Declares an uninitialized vector named cntr.

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
Returns a pointer-iterator to the element after the erased element, or an end pointer-iterator if there is no subsequent element.

```c
el_ty *erase_n( vec( el_ty ) *cntr, size_t i, size_t n )
```

Erases n elements beginning at index `i`, calling the element type's destructor, if it exists, for each erased element.  
Returns a pointer-iterator to the element after the erased elements, or an end pointer/iterator if there is no subsequent element.

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

Returns a pointer-iterator to the element after the element pointed to by `i`, or an end pointer-iterator if `i` points to the last element.

> **Note**
> Vector pointer-iterators (including end) are invalidated by any API calls that cause memory reallocation.
