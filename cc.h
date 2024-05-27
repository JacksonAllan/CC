/*----------------------------------------- CC: CONVENIENT CONTAINERS v1.1.1 -------------------------------------------

This library provides usability-oriented generic containers (vectors, linked lists, unordered maps, and unordered sets).

Features:

- Fully generic API (no need to specify element or key type except when first declaring a container).
- No need to pre-declare container types per element or key/element pair.
- Type safety.
- User-defined destructor, comparison, and hash functions associated with element and key types.
- Handles memory allocation failure.
- Single header.
- Compiles in C and C++.

Requires C23, or C11 and compiler support for __typeof__, or C++11.

Tested with GCC, MinGW, and Clang.

#including the library:

  Place this at the top of your file/s:

    #include "cc.h"

  The following can be #defined before #including the library in any file:

    #define CC_NO_SHORT_NAMES
      By default, CC exposes API macros without the "cc_" prefix.
      Define this flag to withhold the unprefixed names.

  The following can be #defined anywhere and affect all calls to API macros where the definition is visible:
  
    #define CC_REALLOC our_realloc
      Causes API macros to use a custom realloc function rather than the one in the standard library.

    #define CC_FREE our_free
      Causes API macros to use a custom free function rather than the one in the standard library.

API:

  General notes:

  - API macros may evaluate their first argument - the pointer to the container - multiple times, so never use
    expressions with side effects (e.g. &our_containers[ ++i ] ) for that argument. In GCC and Clang, attempting to do
    so will cause a compiler warning. All other arguments are only evaluated once.
  - If CC_NO_SHORT_NAMES was declared, all API macros are prefixed with "cc_".
  - Duplicating a container handle via assignment and then operating on the duplicate will invalidate the original.
    Hence, only create a duplicate via assignment (including through function parameters and return values) if you have
    finished with the original.
  - An iterator is a pointer to an element in the container or to the associated end (or r_end, if the container
    supports it). In the documentation below, these pointers are referred to as "pointer-iterators".
  - In the documentation below, el_ty is the container's element type and key_ty is the container's key type (where
    applicable).

  All containers:

    The following macros behave the same way for all containers:

    void init( <any container type> *cntr )

      Initializes cntr for use.
      This call cannot fail (it does not allocate memory).

    bool init_clone( <any container type> *cntr, <same container type> *src )

      Initializes cntr as a shallow copy of src.
      Returns true, or false if unsuccessful due to memory allocation failure.

    size_t size( <any container type> *cntr )

      Returns the number of elements.

    void clear( <any container type> *cntr )

      Erases all elements, calling the element and key types' destructors if they exist.

    void cleanup( <any container type> *cntr )

      Erases all elements (calling the element and key types' destructors if they exist), frees any other memory
      associated with the container, and initializes the container for reuse.

    for_each( <any container type> *cntr, i_name )

      Creates a loop iterating over all elements from first to last.
      This macro declares a pointer-iterator (el_ty *) named i_name.
      It is equivalent to
        for( el_ty *i_name = first( cntr ); i_name != end( cntr ); i_name = next( cntr, i_name ) )
      and should be followed by the loop body.

  Vector (a dynamic array that stores elements in contiguous memory):

    vec( el_ty ) cntr

      Declares an uninitialized vector named cntr.

    size_t cap( vec( el_ty ) *cntr )

      Returns the current capacity.

    bool reserve( vec( el_ty ) *cntr, size_t n )

      Ensures that the the capacity is large enough to accommodate n elements.
      Returns true, or false if unsuccessful due to memory allocation failure.

    bool resize( vec( el_ty ) *cntr, size_t n )

      Sets the number of elements to n.
      If n is above the current size, the new elements are uninitialized.
      If n is below the current size, the element type's destructor (if it exists) is called for each erased element.
      Returns true, or false if unsuccessful due to memory allocation failure.

    bool shrink( vec( el_ty ) *cntr )

      Shrinks the capacity to the current size.
      Returns true, or false if unsuccessful due to memory allocation failure.

    el_ty *get( vec( el_ty ) *cntr, size_t i )

      Returns an a pointer-iterator to the element at index i.

    el_ty *push( vec( el_ty ) *cntr, el_ty el )

      Inserts el at the end of the vector.
      Returns a pointer-iterator to the new element, or NULL in the case of memory allocation failure.

    el_ty *push_n( vec( el_ty ) *cntr, el_ty *els, size_t n )

      Inserts n elements from array els at the end of the vector.
      Returns a pointer-iterator to the first new element, or NULL in the case of memory allocation failure.

    el_ty *insert( vec( el_ty ) *cntr, size_t i, el_ty el )

      Inserts el at index i.
      Returns a pointer-iterator to the new element, or NULL in the case of memory allocation failure.

    el_ty *insert_n( vec( el_ty ) *cntr, size_t i, el_ty *els, size_t n )

      Inserts n elements from array els at index i.
      Returns a pointer-iterator to the first new element, or NULL in the case of memory allocation failure.

    el_ty *erase( vec( el_ty ) *cntr, size_t i )

      Erases the element at index i, calling the element type's destructor if it exists.
      Returns a pointer-iterator to the element after the erased element, or an end pointer-iterator if there
      is no subsequent element.

    el_ty *erase_n( vec( el_ty ) *cntr, size_t i, size_t n )

      Erases n elements beginning at index i, calling the element type's destructor, if it exists, for each
      erased element.
      Returns a pointer-iterator to the element after the erased elements, or an end pointer-iterator if there is no
      subsequent element.

    el_ty *end( vec( el_ty ) *cntr )

      Returns an end pointer-iterator.
      This call is synonymous with get( cntr, size( cntr ) ).

    el_ty *first( vec( el_ty ) *cntr )

      Returns an pointer-iterator to the first element, or an end pointer-iterator if the vector is empty.
      This call is synonymous with get( cntr, 0 ).

    el_ty *last( vec( el_ty ) *cntr )

      Returns a pointer-iterator to the last element.
      This call is synonymous with get( cntr, size( cntr ) - 1 ).
      It assumes that at the vector is not empty.

   el_ty *next( vec( el_ty ) *cntr, el_ty *i )

      Returns a pointer-iterator to the element after the element pointed to by i, or an end pointer-iterator if i
      points to the last element.

    Notes:
    - Vector pointer-iterators (including end) are invalidated by any API calls that cause memory reallocation.

  List (a doubly linked list with sentinels):

    list( el_ty ) cntr

      Declares an uninitialized list named cntr.

    el_ty *insert( list( el_ty ) *cntr, el_ty *i, el_ty el )

      Inserts el before pointer-iterator i.
      Returns a pointer-iterator to the new element, or NULL in the case of memory allocation failure.

    el_ty *push( list( el_ty ) *cntr, el_ty el )

      Inserts el at the end of the list.
      Returns a pointer-iterator to the new element, or NULL in the case of memory allocation failure.
      This call is synonymous with insert( cntr, end( cntr ), el ).

    el_ty *erase( list( el_ty ) *cntr, el_ty *i )

      Erases element pointed to by pointer-iterator i, calling the element type's destructor if it exists.
      Returns a pointer-iterator to the element after i, or an end pointer-iterator if i was the last element.

    bool splice( list( el_ty ) *cntr, el_ty *i, list( el_ty ) src, el_ty *src_i )

      Removes element pointed to by pointer-iterator src_i from src and inserts it before the element pointed to by
      pointer-iterator i in cntr.
      Returns true, or false if unsuccessful.
      This call only allocates memory, and therefore can only fail, if the list has not had any element inserted,
      pushed, or spliced into it since it was initialized.

    el_ty *first( list( el_ty ) *cntr )

      Returns a pointer-iterator to the first element, or an end pointer-iterator if the list is empty.

    el_ty *last( list( el_ty ) *cntr )

      Returns a pointer-iterator to the last element, or an r_end pointer-iterator if the list is empty.

    el_ty *r_end( list( el_ty ) *cntr )

      Returns an r_end (reverse end) pointer-iterator for the list. r_end acts as a sentinel node.

    el_ty *end( list( el_ty ) *cntr )

      Returns an end pointer-iterator for the list. End acts as a sentinel node.

    el_ty *next( list( el_ty ) *cntr, el_ty *i )

      Returns a pointer-iterator to the element after the one pointed to by i.
      If i points to the last element, the return value is an end pointer-iterator.
      If i points to r_end, the return value is a pointer-iterator to the first element, or an end pointer-iterator if
      the list is empty.

    el_ty *prev( list( el_ty ) *cntr, el_ty *i )

      Returns a pointer-iterator to the element before the one pointed to by i.
      If i points to the first element, the return value is an r_end pointer-iterator.
      If i points to end, then the return value is a pointer-iterator to the last element, or an r_end pointer-iterator
      if the list is empty.

    r_for_each( list( el_ty ) *cntr, i_name )

      Creates a loop iterating over all elements from last to first.
      This macro declares an el_ty * pointer-iterator named i_name.
      It is equivalent to
        for( el_ty *i_name = last( cntr ); i_name != r_end( cntr ); i_name = prev( cntr, i_name ) )
      and should be followed by the body of the loop.

    Notes:
    - List pointer-iterators (including r_end and end) are not invalidated by any API calls besides init and cleanup,
      unless they point to erased elements.

  Map (an unordered associative container mapping elements to keys, implemented as a hybird open-addressing, chained
  hash table):

    map( key_ty, el_ty ) cntr

      Declares an uninitialized map named cntr.
      For types with in-built comparison and hash functions, and for details on how to declare new comparison and hash
      functions, see "Destructor, comparison, and hash functions and custom max load factors" below.

    size_t cap( map( key_ty, el_ty ) *cntr )

      Returns the current capacity, i.e. bucket count.
      Note that the number of elements a map can accommodate without rehashing is not its capacity but its capacity
      multiplied by the max load factor associated with its key type.

    bool reserve( map( key_ty, el_ty ) *cntr, size_t n )

      Ensures that the capacity is large enough to accommodate n elements without rehashing.
      Returns true, or false if unsuccessful due to memory allocation failure.

    bool shrink( map( key_ty, el_ty ) *cntr )

      Shrinks the capacity to best accommodate the current size.
      Returns true, or false if unsuccessful due to memory allocation failure.

    el_ty *insert( map( key_ty, el_ty ) *cntr, key_ty key, el_ty el )

      Inserts element el with the specified key.
      If an element with the same key already exists, the existing element is replaced.
      Returns a pointer-iterator to the new element, or NULL in the case of memory allocation failure.

    el_ty *get( map( key_ty, el_ty ) *cntr, key_ty key )

      Returns a pointer-iterator to the element with the specified key, or NULL if no such element exists.

    el_ty *get_or_insert( map( key_ty, el_ty ) *cntr, key_ty key, el_ty el )

      Inserts element el if no element with the specified key already exist.
      Returns a pointer-iterator to the new element if it was inserted, or a pointer-iterator to the existing
      element with the same key, or NULL in the case of memory allocation failure.
      Determine whether an element was inserted by comparing the map's size before and after the call.

    const key_ty *key_for( map( key_ty, el_ty ) *cntr, el_ty *i )

      Returns a const pointer to the key for the element pointed to by pointer-iterator i.

    bool erase( map( key_ty, el_ty ) *cntr, key_ty key )

      Erases the element with the specified key, if it exists.
      Returns true if an element was erased, or false if no such element exists.

    el_ty *erase_itr( map( key_ty, el_ty ) *cntr, el_ty *i )

      Erases the element pointed to by pointer-iterator i.
      Returns a pointer-iterator to the next element in the map, or an end pointer-iterator if the erased element was
      the last one.

    el_ty *first( map( key_ty, el_ty ) *cntr )

      Returns a pointer-iterator to the first element, or an end pointer-iterator if the map is empty.

    el_ty *last( map( key_ty, el_ty ) *cntr )

      Returns a pointer-iterator to the last element, or an r_end pointer-iterator if the map is empty.

    el_ty *r_end( map( key_ty, el_ty ) *cntr )

      Returns an r_end (reverse end) pointer-iterator for the map.

    el_ty *end( map( key_ty, el_ty ) *cntr )

      Returns an end pointer-iterator for the map.

    el_ty *next( map( key_ty, el_ty ) *cntr, el_ty *i )

      Returns a pointer-iterator to the element after the one pointed to by i.
      If i points to the last element, the value returned is an end pointer-iterator.
      If i points to r_end, the value returned points to the first element, or is an end pointer-iterator if the map is
      empty.

    el_ty *prev( map( key_ty, el_ty ) *cntr, el_ty *i )

      Returns a pointer-iterator to the element before the one pointed to by i.
      If i points to the first element, the value returned is an r_end pointer-iterator.
      If i points to end, then the value returned points to the last element, or is an r_end pointer-iterator if the map
      is empty.

    for_each( map( key_ty, el_ty ) *cntr, key_ptr_name, i_name )

      Creates a loop iterating over all elements from first to last, with easy access to the corresponding keys.
      This macro declares a pointer to the key (const key_ty *) named key_ptr_name and a pointer-iterator (el_ty *)
      named i_name.
      It should be followed by the body of the loop.

    r_for_each( map( key_ty, el_ty ) *cntr, i_name )

      Creates a loop iterating over all elements from last to first.
      This macro declares an el_ty * pointer-iterator named i_name.
      It is equivalent to
        for( el_ty *i_name = last( cntr ); i_name != r_end( cntr ); i_name = prev( cntr, i_name ) )
      and should be followed by the body of the loop.

    r_for_each( map( key_ty, el_ty ) *cntr, key_ptr_name, i_name )

      Creates a loop iterating over all elements from last to first, with easy access to the corresponding keys.
      This macro declares a pointer to the key (const key_ty *) named key_ptr_name and a pointer-iterator (el_ty *)
      named i_name.
      It should be followed by the body of the loop.

    Notes:
    - Map pointer-iterators (including r_end and end) may be invalidated by any API calls that cause memory
      reallocation.

  Set (an unordered associative container for elements without a separate key, implemented as a hybird open-addressing,
  chained hash table):

    set( el_ty ) cntr

      Declares an uninitialized set named cntr.
      For types with in-built comparison and hash functions, and for details on how to declare new comparison and hash
      functions, see "Destructor, comparison, and hash functions and custom max load factors" below.

    size_t cap( set( el_ty ) *cntr )

      Returns the current capacity, i.e. bucket count.
      Note that the number of elements a set can accommodate without rehashing is not its capacity but its capacity
      multiplied by the max load factor associated with its key type.

    bool reserve( set( el_ty ) *cntr, size_t n )

      Ensures that the capacity is large enough to accommodate n elements without rehashing.
      Returns true, or false if unsuccessful due to memory allocation failure.

    bool shrink( set( el_ty ) *cntr )

      Shrinks the capacity to best accommodate the current size.
      Returns true, or false if unsuccessful due to memory allocation failure.

    el_ty *insert( set( el_ty ) *cntr, el_ty el )

      Inserts element el.
      If the element already exists, the existing element is replaced.
      Returns a pointer-iterator to the new element, or NULL in the case of memory allocation failure.

    el_ty *get( set( el_ty ) *cntr, el_ty el )

      Returns a pointer-iterator to element el, or NULL if no such element exists.

    el_ty *get_or_insert( set( el_ty ) *cntr, el_ty el )

      Inserts element el if it does not already exist.
      Returns a pointer-iterator to the new element if it was inserted, or a pointer-iterator to the existing element,
      or NULL in the case of memory allocation failure.
      Determine whether an element was inserted by comparing the set's size before and after the call.

    bool erase( set( el_ty ) *cntr, el_ty el )

      Erases the element el, if it exists.
      Returns true if an element was erased, or false if no such element exists.

    el_ty *erase_itr( set( el_ty ) *cntr, el_ty *i )

      Erases the element pointed to by pointer-iterator i.
      Returns an pointer-iterator to the next element in the set, or an end pointer-iterator if the erased element was
      the last one.

    el_ty *first( set( el_ty ) *cntr )

      Returns a pointer-iterator to the first element, or an end pointer-iterator if the set is empty.

    el_ty *last( set( el_ty ) *cntr )

      Returns a pointer-iterator to the last element, or an r_end pointer-iterator if the set is empty.

    el_ty *r_end( set( el_ty ) *cntr )

      Returns an r_end (reverse end) pointer-iterator for the set.

    el_ty *end( set( el_ty ) *cntr )

      Returns an end pointer-iterator for the set.

    el_ty *next( set( el_ty ) *cntr, el_ty *i )

      Returns a pointer-iterator to the element after the one pointed to by i.
      If i points to the last element, the pointer-iterator returned is an end pointer-iterator.
      If i points to r_end, then the pointer-iterator returned points to the first element, or is an end
      pointer-iterator if the set is empty.

    el_ty *prev( set( el_ty ) *cntr, el_ty *i )

      Returns a pointer-iterator to the element before the one pointed to by i.
      If i points to the first element, the return value is an r_end pointer-iterator.
      If i points to end, then the pointer-iterator returned points to the last element, or is an r_end pointer-iterator
      if the set is empty.

    r_for_each( set( el_ty ) *cntr, i_name )

      Creates a loop iterating over all elements from last to first.
      This macro declares an el_ty * pointer-iterator named i_name.
      It is equivalent to
        for( el_ty *i_name = last( cntr ); i_name != r_end( cntr ); i_name = prev( cntr, i_name ) )
      and should be followed by the body of the loop.

    Notes:
    - Set pointer-iterators (including r_end and end) may be invalidated by any API calls that cause memory
      reallocation.

  Destructor, comparison, and hash functions and custom max load factors:

    This part of the API allows the user to define custom destructor, comparison, and hash functions and max load
    factors for a type.
    Once these functions are defined, any container using that type for its elements or keys will call them
    automatically.
    Once the max load factor is defined, any map using the type for its key and any set using the type for its elements
    will use the defined load factor to determine when rehashing is necessary.

    #define CC_DTOR ty, { function body }
    #include "cc.h"

      Defines a destructor for type ty.
      The signature of the function is void ( ty val ).

    #define CC_CMPR ty, { function body }
    #include "cc.h"

      Defines a comparison function for type ty.
      The signature of the function is int ( ty val_1, ty val_2 ).
      The function should return 0 if val_1 and val_2 are equal, a negative integer if val_1 is less than val_2, and a
      positive integer if val_1 is greater than val_2.

    #define CC_HASH ty, { function body }
    #include "cc.h"

      Defines a hash function for type ty.
      The signature of the function is size_t ( ty val ).
      The function should return the hash of val.

    #define CC_LOAD ty, max_load_factor

      Defines the max load factor for type ty.
      max_load_factor should be a float or double between 0.0 and 1.0.
      The default max load factor is 0.8.
    
    Trivial example:

      typedef struct { int x; } our_type;
      #define CC_DTOR our_type, { printf( "!%d\n", val.x ); }
      #define CC_CMPR our_type, { return val_1.x < val_2.x ? -1 : val_1.x > val_2.x; }
      #define CC_HASH our_type, { return val.x * 2654435761ull; }
      #define CC_LOAD our_type, 0.5
      #include "cc.h"

    Notes:
    - These functions are inline and have static scope, so you need to either redefine them in each translation unit
      from which they should be called or (preferably) define them in a shared header. For structs or unions, a sensible
      place to define them would be immediately after the definition of the struct or union.
    - Only one destructor, comparison, or hash function or max load factor should be defined by the user for each type.
    - #including cc.h in these cases does not #include the full header, so you still need to #include it separately
      at the top of your files.
    - In-built comparison and hash functions are already defined for the following types: char, unsigned char, signed
      char, unsigned short, short, unsigned int, int, unsigned long, long, unsigned long long, long long, size_t, and
      char * (a NULL-terminated string). Defining a comparsion or hash function for one of these types will overwrite
      the in-built function.

Version history:

  27/05/2024 1.1.1: Fixed a bug in map and set that could theoretically cause a crash on rehash (triggerable in testing
                    using cc_shrink with a maximum load factor significantly higher than 1.0).
  18/03/2024 1.1.0: Replaced the Robin Hood implementations of map and set with Verstable implementations.
                    Added branch-prediction optimizations.
                    Improved documentation.
  23/01/2024 1.0.4: Fixed critical bug causing undefined behavior upon iteration of empty maps with nonzero capacities.
                    Fixed formatting inconsistencies and improved code comments.
  04/05/2023 1.0.3: Completed refractor that reduces compile speed by approximately 53% in C with GCC.
                    This was achieved by:
                    - Reducing the number of _Generic expressions in API calls by extracting multiple key-related data
                      from a single expression).
                    - Reducing the work done inside _Generic expressions (this seemed to be a bottleneck).
                    - Changing the API macro function-selection mechanism so that only the function itself varies based
                      on container type (all parallel functions now use the same parameter interface) and each macro
                      contains only one argument list (unused arguments are discarded).
                    Also introduced performance improvements into maps and sets, changed the default integer hash
                    functions to more robust ones, and corrected a bug that could cause map and set probe-length
                    integers to be unaligned for small elements and keys.
  04/02/2023 1.0.2: Fixed bug preventing custom hash table load factors from taking effect when CC_LOAD is defined in
                    isolation of CC_HASH, CC_CMPR, or CC_DTOR.
                    Made minor adjustment to code comments and documentation so that they are more consistent.
  27/01/2023 1.0.1: Made minor corrections to code comments.
  26/12/2022 1.0.0: Initial release.

License (MIT):

  Copyright (c) 2022-2024 Jackson L. Allan

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
  documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
  persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
  Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
  WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
  OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#if !defined( CC_DTOR ) && !defined( CC_CMPR ) && !defined( CC_HASH ) && !defined( CC_LOAD )/*------------------------*/
/*                                                                                                                    */
/*                                                REGULAR HEADER MODE                                                 */
/*                                                                                                                    */
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef CC_NO_SHORT_NAMES
#define vec( ... )           cc_vec( __VA_ARGS__ )
#define list( ... )          cc_list( __VA_ARGS__ )
#define map( ... )           cc_map( __VA_ARGS__ )
#define set( ... )           cc_set( __VA_ARGS__ )
#define init( ... )          cc_init( __VA_ARGS__ )
#define init_clone( ... )    cc_init_clone( __VA_ARGS__ )
#define size( ... )          cc_size( __VA_ARGS__ )
#define cap( ... )           cc_cap( __VA_ARGS__ )
#define reserve( ... )       cc_reserve( __VA_ARGS__ )
#define resize( ... )        cc_resize( __VA_ARGS__ )
#define shrink( ... )        cc_shrink( __VA_ARGS__ )
#define insert( ... )        cc_insert( __VA_ARGS__ )
#define insert_n( ... )      cc_insert_n( __VA_ARGS__ )
#define get_or_insert( ... ) cc_get_or_insert( __VA_ARGS__ )
#define push( ... )          cc_push( __VA_ARGS__ )
#define push_n( ... )        cc_push_n( __VA_ARGS__ )
#define splice( ... )        cc_splice( __VA_ARGS__ )
#define get( ... )           cc_get( __VA_ARGS__ )
#define key_for( ... )       cc_key_for( __VA_ARGS__ )
#define erase( ... )         cc_erase( __VA_ARGS__ )
#define erase_n( ... )       cc_erase_n( __VA_ARGS__ )
#define erase_itr( ... )     cc_erase_itr( __VA_ARGS__ )
#define clear( ... )         cc_clear( __VA_ARGS__ ) 
#define cleanup( ... )       cc_cleanup( __VA_ARGS__ )
#define first( ... )         cc_first( __VA_ARGS__ )
#define last( ... )          cc_last( __VA_ARGS__ )
#define r_end( ... )         cc_r_end( __VA_ARGS__ )
#define end( ... )           cc_end( __VA_ARGS__ )
#define next( ... )          cc_next( __VA_ARGS__ )
#define prev( ... )          cc_prev( __VA_ARGS__ )
#define for_each( ... )      cc_for_each( __VA_ARGS__ )
#define r_for_each( ... )    cc_r_for_each( __VA_ARGS__ )
#endif

#ifndef CC_H
#define CC_H

#include <limits.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
#include <type_traits>
#endif

/*--------------------------------------------------------------------------------------------------------------------*/
/*                                                    Preliminary                                                     */
/*--------------------------------------------------------------------------------------------------------------------*/

// _Static_assert alternative that can be used inside an expression.
#define CC_STATIC_ASSERT( xp ) (void)sizeof( char[ (xp) ? 1 : -1 ] )

// CC_MAKE_LVAL_COPY macro for making an addressable temporary copy of a variable or expression.
// The copy is valid until at least the end of full expression surrounding the macro call.
// In C, this is accomplished using a compound literal.
// In C++, we use rvalue reference magic.
// This is used to pass pointers to elements and keys (which the user may have provided as rvalues) into container
// functions.
#ifdef __cplusplus
template<class ty> ty& cc_unmove( ty&& var ) { return var; }
#define CC_MAKE_LVAL_COPY( ty, xp ) cc_unmove( (ty)( xp ) )
#else
#define CC_MAKE_LVAL_COPY( ty, xp ) *( ty[ 1 ] ){ xp }
#endif

// Macro used primarily for silencing unused-expression warnings for macros that return cast pointers.
// This issue seems to affect Clang in particular.
// GCC, on the other hand, seems to accept that pointer casts may be redundant.
#ifdef __cplusplus
template<typename ty_1, typename ty_2> ty_1 cc_maybe_unused( ty_2 xp ){ return (ty_1)xp; }
#define CC_CAST_MAYBE_UNUSED( ty, xp ) cc_maybe_unused<ty>( xp )
#else
#define CC_CAST_MAYBE_UNUSED( ty, xp ) ( ( ty ){ 0 } = ( (ty)( xp ) ) )
#endif

// Ensures inlining if possible.
// TODO: Adding MSVC attribute once that compiler supports typeof (C23).
#ifdef __GNUC__
#define CC_ALWAYS_INLINE __attribute__((always_inline))
#else
#define CC_ALWAYS_INLINE
#endif

// Branch optimization macros.
#ifdef __GNUC__
#define CC_LIKELY( xp )   __builtin_expect( (bool)( xp ), true )
#define CC_UNLIKELY( xp ) __builtin_expect( (bool)( xp ), false )
#else
#define CC_LIKELY( xp )   ( xp )
#define CC_UNLIKELY( xp ) ( xp )
#endif

// CC_IF_THEN_CAST_TY_1_ELSE_CAST_TY_2 is the same as above, except that it selects the type to which to cast based on
// a condition.
// This is necessary because some API macros (e.g. cc_erase) return either a pointer-iterator or a bool depending on the
// container type.

#ifdef __cplusplus

template<bool cond, class ty_1, class ty_2, class xp_ty, typename std::enable_if<cond, bool>::type = true> \
ty_1 cc_if_then_cast_ty_1_else_cast_ty_2( xp_ty xp ){ return (ty_1)xp; }                                   \

template<bool cond, class ty_1, class ty_2, class xp_ty, typename std::enable_if<!cond, bool>::type = true> \
ty_2 cc_if_then_cast_ty_1_else_cast_ty_2( xp_ty xp ){ return (ty_2)xp; }                                    \

#define CC_IF_THEN_CAST_TY_1_ELSE_CAST_TY_2( cond, ty_1, ty_2, xp ) \
cc_if_then_cast_ty_1_else_cast_ty_2<cond, ty_1, ty_2>( xp )         \

#else

#define CC_IF_THEN_CAST_TY_1_ELSE_CAST_TY_2( cond, ty_1, ty_2, xp ) \
CC_CAST_MAYBE_UNUSED(                                               \
  CC_TYPEOF_XP(                                                     \
    _Generic( (char (*)[ 1 + (bool)( cond ) ]){ 0 },                \
        char (*)[ 1 ]: (ty_2){ 0 },                                 \
        char (*)[ 2 ]: (ty_1){ 0 }                                  \
    )                                                               \
  ),                                                                \
  ( xp )                                                            \
)                                                                   \

#endif

// In GCC and Clang, we can generate a warning if the user passes an expression that may have side effects as the first
// argument of API macros.
// If the expression could have side effects, the compiler will be unable to resolve a comparison of it with itself at
// compile time, which we can check using __builtin_constant_p.
// The warning itself is generated via a division by zero.
// This macro may produce false positives (e.g. for &our_containers[ our_func() ] where our_func is a pure function that
// always returns the same value), but that is a reasonable price to pay for more macro safety.

#ifdef __GNUC__

#define CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr )                                                            \
(void)(                                                                                                   \
  "WARNING: CONTAINER ARGUMENT MAY HAVE DUPLICATED SIDE EFFECTS" &&1/__builtin_constant_p((cntr)==(cntr)) \
)                                                                                                         \

#else
#define CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ) (void)0
#endif

// Typeof for expressions and abstract declarators.
#ifdef __cplusplus
#define CC_TYPEOF_XP( xp ) std::decay<std::remove_reference<decltype( xp )>::type>::type
#define CC_TYPEOF_TY( ty ) std::decay<std::remove_reference<decltype( std::declval<ty>() )>::type>::type
#else
// TODO: Add C23 check once C23 is supported by major compilers.
#define CC_TYPEOF_XP( xp ) __typeof__( xp )
#define CC_TYPEOF_TY( ty ) __typeof__( ty )
#endif

// CC_SELECT_ON_NUM_ARGS macro for overloading API macros based on the number of arguments.
#define CC_CAT_2_( a, b ) a##b
#define CC_CAT_2( a, b ) CC_CAT_2_( a, b )
#define CC_N_ARGS_( _1, _2, _3, _4, _5, _6, n, ... ) n
#define CC_N_ARGS( ... ) CC_N_ARGS_( __VA_ARGS__, _6, _5, _4, _3, _2, _1, x )
#define CC_SELECT_ON_NUM_ARGS( func, ... ) CC_CAT_2( func, CC_N_ARGS( __VA_ARGS__ ) )( __VA_ARGS__ )

// If the user has defined CC_REALLOC and CC_FREE, then CC_GET_REALLOC and CC_GET_FREE are replaced with those macros.
// Otherwise, they are replaced by realloc and free from the standard library.
#define CC_ARG_2_( _1, _2, ... ) _2
#define CC_ARG_2( ... ) CC_ARG_2_( __VA_ARGS__ )
#define CC_REALLOC_COMMA ,
#define CC_REALLOC_FN CC_ARG_2( CC_CAT_2( CC_REALLOC, _COMMA ) realloc, CC_REALLOC, )
#define CC_FREE_COMMA ,
#define CC_FREE_FN CC_ARG_2( CC_CAT_2( CC_FREE, _COMMA ) free, CC_FREE, )

// Macro used with CC_STATIC_ASSERT to provide type safety in cc_init_clone and cc_splice calls.
#ifdef __cplusplus
#define CC_IS_SAME_TY( a, b ) std::is_same<CC_TYPEOF_XP( a ), CC_TYPEOF_XP( b )>::value
#else
#define CC_IS_SAME_TY( a, b ) _Generic( (a), CC_TYPEOF_XP( b ): true, default: false )
#endif

// Macro for handling unused parameters in most container functions that plug directly into API macros.
// These functions must provide a standardized signature across containers.
// The compiler should optimize away unused parameters anyway, but we can nevertheless mark them as redundant.
#ifdef __cplusplus
#define CC_UNUSED( ty, name ) ty
#else
#ifdef __GNUC__
#define CC_UNUSED( ty, name ) __attribute__((unused)) ty name
#else
#define CC_UNUSED( ty, name ) ty name
#endif
#endif

// Some functions that must return true or false must return the value in the form of a pointer.
// This is because they are paired in ternary expressions inside API macros with other functions for other containers
// that return a pointer (primarily cc_erase).
// While any suitably aligned pointer - e.g. the container handle - would do, we declare a global cc_dummy_true_ptr for
// the sake of code readability.
static max_align_t cc_dummy_true;
static void *cc_dummy_true_ptr = &cc_dummy_true;

// Default max load factor for maps and sets.
#define CC_DEFAULT_LOAD 0.9

// Types for comparison, hash, destructor, realloc, and free functions.
// These are only for internal use as user-provided comparison, hash, and destructor functions have different signatures
// (see above documentation).
typedef bool ( *cc_cmpr_fnptr_ty )( void *, void * );
typedef size_t ( *cc_hash_fnptr_ty )( void * );
typedef void ( *cc_dtor_fnptr_ty )( void * );
typedef void *( *cc_realloc_fnptr_ty )( void *, size_t );
typedef void ( *cc_free_fnptr_ty )( void * );

// Container ids to identify container type at compile-time.
#define CC_VEC  1
#define CC_LIST 2
#define CC_MAP  3
#define CC_SET  4

// Produces the underlying function pointer type for a given element/key type pair.
#define CC_MAKE_BASE_FNPTR_TY( el_ty, key_ty ) CC_TYPEOF_TY( CC_TYPEOF_TY( el_ty ) (*)( CC_TYPEOF_TY( key_ty )* ) )

// Produces the container handle type for a given element type, key type, and container id.
// In other words, this macro creates a pointer that carries all the information needed to identify and operate on a
// container at compile time: el_ty (*(*)[ cntr_id ])( key_ty * ).
// That is a pointer to an array of function pointers whose signature is el_ty ( key_ty * ).
#define CC_MAKE_CNTR_TY( el_ty, key_ty, cntr_id ) CC_TYPEOF_TY( CC_MAKE_BASE_FNPTR_TY( el_ty, key_ty )(*)[ cntr_id ] )

// API macros for declaring containers.
// The key type is the type used to look up elements, even in non-associative containers.
// This simplifies API macros by eliminating the need to treat non-associative containers as special cases.

#define cc_vec( el_ty )         CC_MAKE_CNTR_TY( el_ty, size_t, CC_VEC ) // Vector key type is size_t.

#define cc_list( el_ty )        CC_MAKE_CNTR_TY( el_ty, void *, CC_LIST ) // List key is a pointer-iterator.

#define cc_map( key_ty, el_ty ) CC_MAKE_CNTR_TY(                                                          \
                                  el_ty,                                                                  \
                                  key_ty,                                                                 \
                                  CC_MAP * ( (                                                            \
                                    /* Compiler error if key type lacks comparison and hash functions. */ \
                                    CC_HAS_CMPR( key_ty ) && CC_HAS_HASH( key_ty ) &&                     \
                                    /* Compiler error if bucket layout constraints are violated. */       \
                                    CC_SATISFIES_LAYOUT_CONSTRAINTS( key_ty, el_ty )                      \
                                  ) ? 1 : -1 )                                                            \
                                )                                                                         \

#define cc_set( el_ty )         CC_MAKE_CNTR_TY(                                                                 \
                                  /* As set simply wraps map, we use el_ty as both the element and key types. */ \
                                  /* This allows minimal changes to map function arguments to make sets work. */ \
                                  el_ty,                                                                         \
                                  el_ty,                                                                         \
                                  CC_SET * ( (                                                                   \
                                    /* Compiler error if key type lacks comparison and hash functions. */        \
                                    CC_HAS_CMPR( el_ty ) && CC_HAS_HASH( el_ty ) &&                              \
                                    /* Compiler error if bucket layout constraints are violated. */              \
                                    CC_SATISFIES_LAYOUT_CONSTRAINTS( el_ty, el_ty )                              \
                                  ) ? 1 : -1 )                                                                   \
                                )                                                                                \

// Retrieves a container's id (e.g. CC_VEC) from its handle.
#define CC_CNTR_ID( cntr ) ( sizeof( *cntr ) / sizeof( **cntr ) )

// Retrieves a container's element type from its handle.
#define CC_EL_TY( cntr ) CC_TYPEOF_XP( (**cntr)( NULL ) )

// Easy access to a container's element and key sizes.
#define CC_EL_SIZE( cntr )  sizeof( CC_EL_TY( cntr ) )

// CC_KEY_TY macros for retrieving a container's key type from its handle (i.e. from the argument of the base function
// pointer).
#ifdef __cplusplus // For C++, this can be done easily with the help of a template function.

template<typename el_ty, typename key_ty>
key_ty cc_key_ty( el_ty (*)( key_ty * ) )
{
  key_ty dummy;
  return dummy;
}

#define CC_KEY_TY( cntr ) CC_TYPEOF_XP( cc_key_ty( **cntr ) )

#else // For C, we need to use _Generic trickery to match the base function pointer type with a key type previously
      // coupled with a comparison function.

#define CC_KEY_TY_SLOT( n, arg ) CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( arg ), cc_cmpr_##n##_ty ): ( cc_cmpr_##n##_ty ){ 0 },
#define CC_KEY_TY( cntr )                                                                         \
CC_TYPEOF_XP(                                                                                     \
  _Generic( (**cntr),                                                                             \
    CC_FOR_EACH_CMPR( CC_KEY_TY_SLOT, cntr )                                                      \
    default: _Generic( (**cntr),                                                                  \
      CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), char ):               ( char ){ 0 },               \
      CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned char ):      ( unsigned char ){ 0 },      \
      CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), signed char ):        ( signed char ){ 0 },        \
      CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned short ):     ( unsigned short ){ 0 },     \
      CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), short ):              ( short ){ 0 },              \
      CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned int ):       ( unsigned int ){ 0 },       \
      CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), int ):                ( int ){ 0 },                \
      CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned long ):      ( unsigned long ){ 0 },      \
      CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), long ):               ( long ){ 0 },               \
      CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned long long ): ( unsigned long long ){ 0 }, \
      CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), long long ):          ( long long ){ 0 },          \
      CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), cc_maybe_size_t ):    ( size_t ){ 0 },             \
      CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), char * ):             ( char * ){ 0 },             \
      CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), void * ):             ( void * ){ 0 },             \
      default: (char){ 0 } /* Nothing. */                                                         \
    )                                                                                             \
  )                                                                                               \
)                                                                                                 \

#endif

// The functions associated with some containers require extra information about how elements and/or keys and other data
// are laid out in memory.
// Specifically, maps - and by extension sets - need information about their bucket layouts, which depend on their
// element and/or key types (a future implementation of ordered maps and sets will require similar information).
// This data is formed by extracting the key size and alignment and passing it, along with the element size and
// alignment and the container type id, into the cc_layout function, which returns a uint64_t describing the layout.
// The key size and alignment are inferred via a _Generic macro that looks up the key type based on the default
// and user-supplied comparison functions, since all containers that require layout information also require a
// comparison function.
// Although this mechanism is convoluted, it has proven to compile much faster than other approaches (e.g. calculating
// the layout inside the _Generic macro).
// The compiler should optimize the entire mechanism into compile-time constants.

// The layout for a map bucket is:
//   +------------+----+------------+----+
//   |     #1     | #2 |     #3     | #4 |
//   +------------+----+------------+----+
//   #1 Element.
//   #2 Element padding to key_ty alignment.
//   #3 Key.
//   #4 Key padding to the larger of el_ty and key_ty alignments.

// For sets, the layout collapses to just:
//   +------------+
//   |     #1     |
//   +------------+
//   #1 Element.

// The layout data passed into a container function is a uint64_t composed of a uint32_t denoting the key size, a
// uint16_t denoting the padding after the element, and a uint16_t denoting the padding after the key.
// The reason that a uint64_t, rather than a struct, is used is that GCC seems to have trouble properly optimizing the
// passing of the struct - even if only 8 bytes - into some container functions (specifically cc_map_insert), apparently
// because it declines to pass by register.

// Macro for ensuring valid layout on container declaration.
// Since the key size occupies four bytes and the padding values each occupy two bytes, the key size must be <=
// UINT32_MAX (about 4.3GB) and the alignment of the element and key must be <= UINT16_MAX + 1 (i.e. 65536).
// It unlikely that these constraints would be violated in practice, but we can check anyway.
#define CC_SATISFIES_LAYOUT_CONSTRAINTS( key_ty, el_ty )                                                        \
( sizeof( key_ty ) <= UINT32_MAX && alignof( el_ty ) <= UINT16_MAX + 1 && alignof( key_ty ) <= UINT16_MAX + 1 ) \

// Macros and functions for constructing a map bucket layout.

#define CC_PADDING( size, align ) ( ( ~(size) + 1 ) & ( (align) - 1 ) )
#define CC_MAX( a, b ) ( (a) > (b) ? (a) : (b) )

#define CC_MAP_EL_PADDING( el_size, key_align ) \
CC_PADDING( el_size, key_align )                \

#define CC_MAP_KEY_PADDING( el_size, el_align, key_size, key_align )                                      \
CC_PADDING( el_size + CC_MAP_EL_PADDING( el_size, key_align ) + key_size, CC_MAX( el_align, key_align ) ) \

// Struct for conveying key-related information from the _Generic macro into the function below.
typedef struct
{
  uint64_t size;
  uint64_t align;
} cc_key_details_ty;

// Function for creating the uint64_t layout descriptor.
// This function must be inlined in order for layout calculations to be optimized into a compile-time constant.
static inline CC_ALWAYS_INLINE uint64_t cc_layout(
  size_t cntr_id,
  uint64_t el_size,
  uint64_t el_align,
  cc_key_details_ty key_details
)
{
  if( cntr_id == CC_MAP )
    return
      key_details.size                                                                   |
      CC_MAP_EL_PADDING( el_size, key_details.align )                              << 32 |
      CC_MAP_KEY_PADDING( el_size, el_align, key_details.size, key_details.align ) << 48;

  if( cntr_id == CC_SET )
    return el_size;

  return 0; // Other container types don't require layout data.
}

// Macros for extracting data from a uint64_t layout descriptor.

#define CC_KEY_SIZE( layout ) (uint32_t)( layout )

#define CC_KEY_OFFSET( el_size, layout ) ( (el_size) + (uint16_t)( layout >> 32 ) )

#define CC_BUCKET_SIZE( el_size, layout )                                                \
( CC_KEY_OFFSET( el_size, layout ) + (uint32_t)( layout ) + (uint16_t)( layout >> 48 ) ) \

// Return type for all functions that could reallocate a container's memory.
// It contains a new container handle (the pointer may have changed to due reallocation) and an additional pointer whose
// purpose depends on the function.
// For functions that insert elements, that pointer points to the newly inserted elements, or NULL in the case of
// allocation failure.
// For other functions, that pointer is a dummy pointer that evaluates to true, or NULL in the case of allocation
// failure (i.e. its only purpose is to convey whether the operation was successful).
// The struct is aligned to max_align_t because the container handle must temporarily point to it (see
// CC_POINT_HNDL_TO_ALLOCING_FN_RESULT below).
typedef struct
{
  alignas( max_align_t )
  void *new_cntr;
  void *other_ptr;
} cc_allocing_fn_result_ty;

// Helper function for creating a cc_allocing_fn_result_ty in one line.
static inline cc_allocing_fn_result_ty cc_make_allocing_fn_result( void *new_cntr, void *other_ptr )
{
  cc_allocing_fn_result_ty result;
  result.new_cntr = new_cntr;
  result.other_ptr = other_ptr;
  return result;
}

// Performs memcpy and returns ptr.
// This is used in conjunction with cc_allocing_fn_result_ty (see below).
static inline void *cc_memcpy_and_return_ptr( void *dest, void *src, size_t size, void *ptr )
{
  memcpy( dest, src, size );
  return ptr;
}

// All macros that call functions that could cause memory reallocation do two essential things to circumvent limitations
// of ISO C (such as the inability to declare variables in expressions and _Thread_local linking issues).
// Firstly, they temporarily set the container handle to point to a temporary cc_insert_result object returned by the
// call to the function (see CC_MAKE_LVAL_COPY above).
#define CC_POINT_HNDL_TO_ALLOCING_FN_RESULT( cntr, fn_call )                           \
  cntr = (CC_TYPEOF_XP( cntr ))&CC_MAKE_LVAL_COPY( cc_allocing_fn_result_ty, fn_call ) \

// Secondly, they call cc_memcpy_and_return_ptr to restore the correct container handle (a new pointer in the case of
// reallocation) and return a pointer, either to any new elements or to signify success/failure, to the user.
// (In the latter case, that pointer is cast to bool in the API macro before being passed to the user.)
// Without this call, we would be unable to access the new elements/success-or-failure pointer stored in the
// cc_allocing_fn_result_ty after restoring the correct container handle.
// Note that outside the function, a temporary container handle is created from the new handle in the
// cc_allocing_fn_result_ty so that the later (void *) is properly converted to the correct handle type.
// This new, correctly typed handle is then memcpy-ed over the user-supplied handle inside the function.
#define CC_FIX_HNDL_AND_RETURN_OTHER_PTR( cntr )                                              \
cc_memcpy_and_return_ptr(                                                                     \
  &cntr,                                                                                      \
  &CC_MAKE_LVAL_COPY( CC_TYPEOF_XP( cntr ), ( (cc_allocing_fn_result_ty *)cntr )->new_cntr ), \
  sizeof( cntr ),                                                                             \
  ( (cc_allocing_fn_result_ty *)cntr )->other_ptr                                             \
)                                                                                             \

// Functions to find the first and last non-zero uint16_t in a uint64_t.
// These functions are used when we scan four buckets at a time while iterating over maps and sets.
// They rely on compiler intrinsics if possible.

#if defined( __GNUC__ ) && ULLONG_MAX == 0xFFFFFFFFFFFFFFFF

// The compiler will optimize away the endian check at -O1 and above.
static inline bool cc_is_little_endian( void )
{
  const uint16_t endian_checker = 0x0001;
  return *(const char *)&endian_checker;
}

static inline int cc_first_nonzero_uint16( uint64_t a )
{
  if( cc_is_little_endian() )
    return __builtin_ctzll( a ) / 16;
  
  return __builtin_clzll( a ) / 16;
}

static inline int cc_last_nonzero_uint16( uint64_t a )
{
  if( cc_is_little_endian() )
    return __builtin_clzll( a ) / 16;
  
  return __builtin_ctzll( a ) / 16;
}

#else

static inline int cc_first_nonzero_uint16( uint64_t a )
{
  int result = 0;

  uint32_t half;
  memcpy( &half, &a, sizeof( uint32_t ) );
  if( !half )
    result += 2;
  
  uint16_t quarter;
  memcpy( &quarter, (char *)&a + result * sizeof( uint16_t ), sizeof( uint16_t ) );
  if( !quarter )
    result += 1;
  
  return result;
}

static inline int cc_last_nonzero_uint16( uint64_t a )
{
  int result = 3;

  uint32_t half;
  memcpy( &half, (char *)&a + sizeof( uint32_t ), sizeof( uint32_t ) );
  if( !half )
    result -= 2;
  
  uint16_t quarter;
  memcpy( &quarter, (char *)&a + result * sizeof( uint16_t ), sizeof( uint16_t ) );
  if( !quarter )
    result -= 1;
  
  return 3 - result;
}

#endif

/*--------------------------------------------------------------------------------------------------------------------*/
/*                                                      Vector                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/

// Vector header.
typedef struct
{
  alignas( max_align_t )
  size_t size;
  size_t cap;
} cc_vec_hdr_ty;

// Global placeholder for vector with no allocated storage.
// In the case of vectors, the placeholder allows us to avoid checking for a NULL container handle inside functions.
static const cc_vec_hdr_ty cc_vec_placeholder = { 0, 0 };

// Easy header access function.
static inline cc_vec_hdr_ty *cc_vec_hdr( void *cntr )
{
  return (cc_vec_hdr_ty *)cntr;
}

static inline size_t cc_vec_size( void *cntr )
{
  return cc_vec_hdr( cntr )->size;
}

static inline size_t cc_vec_cap( void *cntr )
{
  return cc_vec_hdr( cntr )->cap;
}

static inline bool cc_vec_is_placeholder( void *cntr )
{
  return cc_vec_hdr( cntr )->cap == 0;
}

// Returns a pointer-iterator to the element at a specified index.
static inline void *cc_vec_get(
  void *cntr,
  void *key, // Pointer to a size_t index.
  size_t el_size,
  CC_UNUSED( uint64_t, layout ),
  CC_UNUSED( cc_hash_fnptr_ty, hash ),
  CC_UNUSED( cc_cmpr_fnptr_ty, cmpr )
)
{
  return (char *)cntr + sizeof( cc_vec_hdr_ty ) + el_size * *(size_t *)key;
}

// Ensures that the capacity is large enough to accommodate n elements without reallocation.
// Returns a cc_allocing_fn_result_ty containing the new handle and a pointer that evaluates to true if the operation
// was successful.
static inline cc_allocing_fn_result_ty cc_vec_reserve(
  void *cntr,
  size_t n,
  size_t el_size,
  CC_UNUSED( uint64_t, layout ),
  CC_UNUSED( cc_hash_fnptr_ty, hash ),
  CC_UNUSED( double, max_load ),
  cc_realloc_fnptr_ty realloc_,
  CC_UNUSED( cc_free_fnptr_ty, free_ )
)
{
  if( cc_vec_cap( cntr ) >= n )
    return cc_make_allocing_fn_result( cntr, cc_dummy_true_ptr );

  bool is_placeholder = cc_vec_is_placeholder( cntr );

  cc_vec_hdr_ty *new_cntr = (cc_vec_hdr_ty *)realloc_(
    is_placeholder ? NULL : cntr,
    sizeof( cc_vec_hdr_ty ) + el_size * n
  );

  if( CC_UNLIKELY( !new_cntr ) )
    return cc_make_allocing_fn_result( cntr, NULL );

  if( is_placeholder )
    new_cntr->size = 0;

  new_cntr->cap = n;
  return cc_make_allocing_fn_result( new_cntr, cc_dummy_true_ptr );
}

// Inserts elements at the specified index.
// Returns a cc_allocing_fn_result_ty containing the new handle and a pointer-iterator to the newly inserted elements.
// If the underlying storage needed to be expanded and an allocation failure occurred, or if n is zero, the latter
// pointer will be NULL.
static inline cc_allocing_fn_result_ty cc_vec_insert_n(
  void *cntr,
  size_t index,
  void *els,
  size_t n,
  size_t el_size,
  cc_realloc_fnptr_ty realloc_
)
{
  if( n == 0 )
    return cc_make_allocing_fn_result( cntr, NULL );

  if( cc_vec_size( cntr ) + n > cc_vec_cap( cntr ) )
  {
    size_t cap = cc_vec_cap( cntr );
    if( !cap )
      cap = 2;

    while( cap < cc_vec_size( cntr ) + n )
      cap *= 2;

    cc_allocing_fn_result_ty result = cc_vec_reserve(
      cntr,
      cap,
      el_size,
      0,        // Dummy.
      NULL,     // Dummy.
      0.0,      // Dummy.
      realloc_,
      NULL      // Dummy.
    );
    if( CC_UNLIKELY( !result.other_ptr ) )
      return result;

    cntr = result.new_cntr;
  }

  char *new_els = (char *)cntr + sizeof( cc_vec_hdr_ty ) + el_size * index;
  memmove( new_els + n * el_size, new_els, el_size * ( cc_vec_hdr( cntr )->size - index ) );
  memcpy( new_els, els, el_size * n );
  cc_vec_hdr( cntr )->size += n;

  return cc_make_allocing_fn_result( cntr, new_els );
}

static inline cc_allocing_fn_result_ty cc_vec_insert(
  void *cntr,
  void *el,
  void *key, // Pointer to size_t index.
  CC_UNUSED( bool, replace ),
  size_t el_size,
  CC_UNUSED( uint64_t, layout ),
  CC_UNUSED( cc_hash_fnptr_ty, hash ),
  CC_UNUSED( cc_cmpr_fnptr_ty, cmpr ),
  CC_UNUSED( double, max_load ),
  CC_UNUSED( cc_dtor_fnptr_ty, el_dtor ),
  CC_UNUSED( cc_dtor_fnptr_ty, key_dtor ),
  cc_realloc_fnptr_ty realloc_,
  CC_UNUSED( cc_free_fnptr_ty, free_ )
)
{
  return cc_vec_insert_n( cntr, *(size_t *)key, el, 1, el_size, realloc_ );
}

static inline cc_allocing_fn_result_ty cc_vec_push_n(
  void *cntr,
  void *els,
  size_t n,
  size_t el_size,
  cc_realloc_fnptr_ty realloc_
)
{
  return cc_vec_insert_n( cntr, cc_vec_size( cntr ), els, n, el_size, realloc_ );
}

static inline cc_allocing_fn_result_ty cc_vec_push(
  void *cntr,
  void *el,
  size_t el_size,
  cc_realloc_fnptr_ty realloc_
)
{
  return cc_vec_push_n( cntr, el, 1, el_size, realloc_ );
}

// Erases n elements at the specified index.
// Returns a pointer-iterator to the element after the erased elements, or an end pointer-iterator if there is no
// subsequent element.
static inline void *cc_vec_erase_n(
  void *cntr,
  size_t index,
  size_t n,
  size_t el_size,
  cc_dtor_fnptr_ty el_dtor
)
{
  if( n == 0 )
    return (char *)cntr + sizeof( cc_vec_hdr_ty ) + el_size * index;

  if( el_dtor )
    for( size_t j = 0; j < n; ++j )
      el_dtor( (char *)cntr + sizeof( cc_vec_hdr_ty ) + el_size * ( index + j ) );

  memmove(
    (char *)cntr + sizeof( cc_vec_hdr_ty ) + el_size * index,
    (char *)cntr + sizeof( cc_vec_hdr_ty ) + el_size * ( index + n ),
    ( cc_vec_hdr( cntr )->size - n - index ) * el_size
  );

  cc_vec_hdr( cntr )->size -= n;
  return (char *)cntr + sizeof( cc_vec_hdr_ty ) + el_size * index;
}

// Shrinks the vector's capacity to its current size.
// Returns a cc_allocing_fn_result_ty containing the new container handle and a pointer that evaluates to true if the
// operation was successful or false in the case of allocation failure.
static inline void *cc_vec_erase(
  void *cntr,
  void *key, // Pointer to size_t index.
  size_t el_size,
  CC_UNUSED( uint64_t, layout ),
  CC_UNUSED( cc_hash_fnptr_ty, hash ),
  CC_UNUSED( cc_cmpr_fnptr_ty, cmpr ),
  cc_dtor_fnptr_ty el_dtor,
  CC_UNUSED( cc_dtor_fnptr_ty, key_dtor ),
  CC_UNUSED( cc_free_fnptr_ty, free_ )
)
{
  return cc_vec_erase_n( cntr, *(size_t *)key, 1, el_size, el_dtor );
}

// Sets the number of elements in the vector.
// If n is below the current size, then the destructor is called for all erased elements.
// In this case, the vector's capacity is not changed.
// If n is above the current size, the new elements are uninitialized.
// Returns a cc_allocing_fn_result_ty containing new container handle and a pointer that evaluates to true if the
// operation was successful or false in the case of allocation failure.
static inline cc_allocing_fn_result_ty cc_vec_resize(
  void *cntr,
  size_t n,
  size_t el_size,
  cc_dtor_fnptr_ty el_dtor,
  cc_realloc_fnptr_ty realloc_
)
{
  // No resize necessary (also handles placeholder).
  if( n == cc_vec_size( cntr ) )
    return cc_make_allocing_fn_result( cntr, cc_dummy_true_ptr );

  // Downsizing.
  if( n < cc_vec_size( cntr ) )
  {
    cc_vec_erase_n( cntr, n, cc_vec_size( cntr ) - n, el_size, el_dtor );
    return cc_make_allocing_fn_result( cntr, cc_dummy_true_ptr );
  }

  // Up-sizing.
  cc_allocing_fn_result_ty result = cc_vec_reserve(
    cntr,
    n,
    el_size,
    0,        // Dummy.
    NULL,     // Dummy.
    0.0,      // Dummy.
    realloc_,
    NULL      // Dummy.
  );
  if( CC_UNLIKELY( !result.other_ptr ) )
    return result;

  cc_vec_hdr( result.new_cntr )->size = n;

  return result;
}

static inline cc_allocing_fn_result_ty cc_vec_shrink(
  void *cntr,
  size_t el_size,
  CC_UNUSED( uint64_t, layout ),
  CC_UNUSED( cc_hash_fnptr_ty, hash ),
  CC_UNUSED( double, max_load ),
  cc_realloc_fnptr_ty realloc_,
  cc_free_fnptr_ty free_
)
{
  if( cc_vec_size( cntr ) == cc_vec_cap( cntr ) ) // Also handles placeholder.
    return cc_make_allocing_fn_result( cntr, cc_dummy_true_ptr );

  if( cc_vec_size( cntr ) == 0 )
  {
    // Restore placeholder.
    free_( cntr );
    return cc_make_allocing_fn_result( (void *)&cc_vec_placeholder, cc_dummy_true_ptr );
  }

  cc_vec_hdr_ty *new_cntr = (cc_vec_hdr_ty *)realloc_( cntr, sizeof( cc_vec_hdr_ty ) + el_size * cc_vec_size( cntr ) );
  if( CC_UNLIKELY( !new_cntr ) )
    return cc_make_allocing_fn_result( cntr, NULL );

  cc_vec_hdr( new_cntr )->cap = cc_vec_size( new_cntr );
  return cc_make_allocing_fn_result( new_cntr, cc_dummy_true_ptr );
}

// Initializes a shallow copy of the source vector.
// The capacity of the new vector is the size of the source vector, not its capacity.
// Returns a pointer to the copy, or NULL in the case of allocation failure.
// The return value is cast to bool in the corresponding macro.
static inline void *cc_vec_init_clone(
  void *src,
  size_t el_size,
  CC_UNUSED( uint64_t, layout ),
  cc_realloc_fnptr_ty realloc_,
  CC_UNUSED( cc_free_fnptr_ty, free_ )
)
{
  if( cc_vec_size( src ) == 0 )
    return (void *)&cc_vec_placeholder;

  cc_allocing_fn_result_ty result = cc_vec_resize(
    (void *)&cc_vec_placeholder,
    cc_vec_size( src ),
    el_size,
    NULL, // Destructor unused.
    realloc_
  );

  if( CC_UNLIKELY( !result.other_ptr ) )
    return NULL;

  memcpy(
    (char *)result.new_cntr + sizeof( cc_vec_hdr_ty ),
    (char *)src + sizeof( cc_vec_hdr_ty ),
    el_size * cc_vec_size( src )
  );

  return result.new_cntr;
}

// Erases all elements, calling their destructors if necessary.
static inline void cc_vec_clear(
  void *cntr,
  size_t el_size,
  CC_UNUSED( uint64_t, layout ),
  cc_dtor_fnptr_ty el_dtor,
  CC_UNUSED( cc_dtor_fnptr_ty, key_dtor ),
  CC_UNUSED( cc_free_fnptr_ty, free_ )
)
{
  cc_vec_erase_n( cntr, 0, cc_vec_size( cntr ), el_size, el_dtor );
}

// Clears the vector and frees its memory if it is not a placeholder.
static inline void cc_vec_cleanup(
  void *cntr,
  size_t el_size,
  CC_UNUSED( uint64_t, layout ),
  cc_dtor_fnptr_ty el_dtor,
  CC_UNUSED( cc_dtor_fnptr_ty, key_dtor ),
  cc_free_fnptr_ty free_
)
{
  cc_vec_clear(
    cntr,
    el_size,
    0,       // Dummy.
    el_dtor,
    NULL,    // Dummy.
    NULL     // Dummy.
  );

  if( !cc_vec_is_placeholder( cntr ) )
    free_( cntr );
}

static inline void *cc_vec_end(
  void *cntr,
  size_t el_size,
  CC_UNUSED( uint64_t, layout )
)
{
  return (char *)cntr + sizeof( cc_vec_hdr_ty ) + el_size * cc_vec_size( cntr );
}

static inline void *cc_vec_next(
  CC_UNUSED( void *, cntr ),
  void *itr,
  size_t el_size,
  CC_UNUSED( uint64_t, layout )
)
{
  return (char *)itr + el_size;
}

static inline void *cc_vec_first(
  void *cntr,
  CC_UNUSED( size_t, el_size ),
  CC_UNUSED( uint64_t, layout )
)
{
  return (char *)cntr + sizeof( cc_vec_hdr_ty );
}

static inline void *cc_vec_last(
  void *cntr,
  size_t el_size,
  CC_UNUSED( uint64_t, layout )
)
{
  return (char *)cntr + sizeof( cc_vec_hdr_ty ) + el_size * ( cc_vec_size( cntr ) - 1 );
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*                                                        List                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/

// List is implemented as a doubly linked list with sentinel nodes.

// Node header.
typedef struct cc_listnode_hdr_ty
{
  alignas( max_align_t )
  struct cc_listnode_hdr_ty *prev;
  struct cc_listnode_hdr_ty *next;
} cc_listnode_hdr_ty;

// List header.
// It does not need to be aligned to alignof( max_align_t ) because no memory is allocated after the header.
typedef struct
{
  size_t size;
  cc_listnode_hdr_ty r_end;
  cc_listnode_hdr_ty end;
} cc_list_hdr_ty;

// Placeholder for a list with no allocated header.
// The main purpose this serves is to provide every list with stable r_end and end pointer-iterators across translation
// units and irrespective of whether any memory has been allocated for its header.
// Every list initially (after cc_init) points to this placeholder, which differs across translation units, and is then
// associated with that placeholder until cc_cleanup is called.
// Calls to cc_r_end and cc_end on a list return pointers to the associated placeholder's r_end and end nodes even after
// a header has been dynamically allocated for it.
// The way this works is that the placeholder's r_end.prev pointer points to the placeholder's r_end (i.e. a circular
// link), and ditto for end.next.
// Meanwhile, a list with an allocated header has it's r_end.prev pointer point to the placeholder's r_end, and ditto
// for it's end.next pointer.
// Therefore cc_list_hdr( cntr )->r_end.prev always produces the associated placeholder's r_end, and ditto for
// cc_list_hdr( cntr )->end.next.
// Unfortunately, this means that r_end and end must be handled as special cases during inserts, splices, and iteration.
static const cc_list_hdr_ty cc_list_placeholder = {
  0,
  {
    (cc_listnode_hdr_ty *)&cc_list_placeholder.r_end, // Circular link.
    (cc_listnode_hdr_ty *)&cc_list_placeholder.end
  },
  {
    (cc_listnode_hdr_ty *)&cc_list_placeholder.r_end,
    (cc_listnode_hdr_ty *)&cc_list_placeholder.end    // Circular link.
  }
};

// Easy access to the list header.
static inline cc_list_hdr_ty *cc_list_hdr( void *cntr )
{
  return (cc_list_hdr_ty *)cntr;
}

// Easy access to a list node header from a pointer-iterator.
static inline cc_listnode_hdr_ty *cc_listnode_hdr( void *itr )
{
  return (cc_listnode_hdr_ty *)( (char *)itr - sizeof( cc_listnode_hdr_ty ) );
}

// Easy access to a pointer-iterator from a pointer to a list node header.
static inline void *cc_list_el( void *hdr )
{
  return (char *)hdr + sizeof( cc_listnode_hdr_ty );
}

static inline bool cc_list_is_placeholder( void *cntr )
{
  return cc_list_hdr( cntr )->r_end.prev == &cc_list_hdr( cntr )->r_end;
}

static inline size_t cc_list_size( void *cntr )
{
  return cc_list_hdr( cntr )->size;
}

static inline void *cc_list_r_end( void *cntr )
{
  return cc_list_el( cc_list_hdr( cntr )->r_end.prev );
}

static inline void *cc_list_end(
  void *cntr,
  CC_UNUSED( size_t, el_size ),
  CC_UNUSED( uint64_t, layout )
)
{
  return cc_list_el( cc_list_hdr( cntr )->end.next );
}

static inline void *cc_list_prev(
  void *cntr,
  void *itr,
  CC_UNUSED( size_t, el_size ),
  CC_UNUSED( uint64_t, layout )
)
{
  cc_listnode_hdr_ty *prev = cc_listnode_hdr( itr )->prev;

  // If itr is r_end, then we need to decrement the pointer-iterator once more to ensure that the returned
  // pointer-iterator is the r_end of the placeholder originally associated with the list.
  if( prev == &cc_list_hdr( cntr )->r_end )
    prev = prev->prev;

  return cc_list_el( prev );
}

static inline void *cc_list_next(
  void *cntr,
  void *itr,
  CC_UNUSED( size_t, el_size ),
  CC_UNUSED( uint64_t, layout )
)
{
  cc_listnode_hdr_ty *next = cc_listnode_hdr( itr )->next;

  // See the comment in cc_list_prev above.
  if( next == &cc_list_hdr( cntr )->end )
    next = next->next;

  return cc_list_el( next );
}

static inline void *cc_list_first(
  void *cntr,
  CC_UNUSED( size_t, el_size ),
  CC_UNUSED( uint64_t, layout )
)
{
  return cc_list_next(
    cntr,
    cc_list_el( &cc_list_hdr( cntr )->r_end ),
    0, // Dummy.
    0  // Dummy.
  );
}

static inline void *cc_list_last(
  void *cntr,
  CC_UNUSED( size_t, el_size ),
  CC_UNUSED( uint64_t, layout )
)
{
  return cc_list_prev(
    cntr,
    cc_list_el( &cc_list_hdr( cntr )->end ),
    0, // Dummy.
    0  // Dummy.
  );
}

// Allocates a header for a list that is currently a placeholder.
// Returns the new container handle, or NULL in the case of allocation failure.
static inline void *cc_list_alloc_hdr(
  void *cntr,
  cc_realloc_fnptr_ty realloc_
)
{
  cc_list_hdr_ty *new_cntr = (cc_list_hdr_ty *)realloc_( NULL, sizeof( cc_list_hdr_ty ) );
  if( CC_UNLIKELY( !new_cntr ) )
    return NULL;

  new_cntr->r_end.next = &new_cntr->end;
  new_cntr->end.prev = &new_cntr->r_end;

  // Link the new header's r_end and end with the original placeholder's r_end and end.
  new_cntr->r_end.prev = &cc_list_hdr( cntr )->r_end;
  new_cntr->end.next = &cc_list_hdr( cntr )->end;

  new_cntr->size = 0; 
  return new_cntr;
}

// Attaches a node to the list before the node pointed to by the specified pointer-iterator.
static inline void cc_list_attach(
  void *cntr,
  void *itr,
  cc_listnode_hdr_ty *node
)
{
  // Handle r_end and end pointer-iterators as a special case.
  // We need to convert the pointer-iterator from the global placeholder's r_end or end to the local r_end or end.
  if( itr == cc_list_r_end( cntr ) )
    itr = cc_list_el( &cc_list_hdr( cntr )->r_end );
  else if( itr == cc_list_end( cntr, 0 /* Dummy */, 0 /* Dummy */ ) )
    itr = cc_list_el( &cc_list_hdr( cntr )->end );

  // Link node.
  node->next = cc_listnode_hdr( itr );
  node->prev = node->next->prev;
  node->next->prev = node;
  node->prev->next = node;
}

// Inserts an element into the list before the node pointed to by a given pointer-iterator.
// Returns a cc_allocing_fn_result_ty containing the new container handle and a pointer-iterator to the newly inserted
// element (or NULL in the case of allocation failure).
static inline cc_allocing_fn_result_ty cc_list_insert(
  void *cntr,
  void *el,
  void *key, // Pointer to void pointer-interator.
  CC_UNUSED( bool, replace ),
  size_t el_size,
  CC_UNUSED( uint64_t, layout ),
  CC_UNUSED( cc_hash_fnptr_ty, hash ),
  CC_UNUSED( cc_cmpr_fnptr_ty, cmpr ),
  CC_UNUSED( double, max_load ),
  CC_UNUSED( cc_dtor_fnptr_ty, el_dtor ),
  CC_UNUSED( cc_dtor_fnptr_ty, key_dtor ),
  cc_realloc_fnptr_ty realloc_,
  CC_UNUSED( cc_free_fnptr_ty, free_ )
)
{
  if( cc_list_is_placeholder( cntr ) )
  {
    void *new_cntr = cc_list_alloc_hdr( cntr, realloc_ );
    if( CC_UNLIKELY( !new_cntr ) )
      return cc_make_allocing_fn_result( cntr, NULL );

    cntr = new_cntr;
  }

  cc_listnode_hdr_ty *new_node = (cc_listnode_hdr_ty *)realloc_( NULL, sizeof( cc_listnode_hdr_ty ) + el_size );
  if( CC_UNLIKELY( !new_node ) )
    return cc_make_allocing_fn_result( cntr, NULL );

  memcpy( cc_list_el( new_node ), el, el_size );

  // Handle r_end and end pointer-iterators as a special case.
  // We need to convert the pointer-iterator from the associated placeholder's r_end or end to the local r_end or end.
  if( *(void **)key == cc_list_r_end( cntr ) )
    *(void **)key = cc_list_el( &cc_list_hdr( cntr )->r_end );
  else if( *(void **)key == cc_list_end( cntr, 0 /* Dummy */, 0 /* Dummy */ ) )
    *(void **)key = cc_list_el( &cc_list_hdr( cntr )->end );

  cc_list_attach( cntr, *(void **)key, new_node );

  ++cc_list_hdr( cntr )->size;

  return cc_make_allocing_fn_result( cntr, cc_list_el( new_node ) );  
}

static inline cc_allocing_fn_result_ty cc_list_push(
  void *cntr,
  void *el,
  size_t el_size,
  cc_realloc_fnptr_ty realloc_
)
{
  return cc_list_insert(
    cntr,
    el,
    &CC_MAKE_LVAL_COPY( void *, cc_list_end( cntr, 0 /* Dummy */, 0 /* Dummy */ ) ),
    false,    // Dummy.
    el_size,
    0,        // Dummy.
    NULL,     // Dummy.
    NULL,     // Dummy.
    0.0,      // Dummy.
    NULL,     // Dummy.
    NULL,     // Dummy.
    realloc_,
    NULL      // Dummy.
  );
}

// Erases the element pointed to by a given pointer-iterator and returns a pointer-iterator to next element (or end if
// the element was the last element in the list).
static inline void *cc_list_erase(
  void *cntr,
  void *key, // Pointer to void pointer-interator.
  CC_UNUSED( size_t, el_size ),
  CC_UNUSED( uint64_t, layout ),
  CC_UNUSED( cc_hash_fnptr_ty, hash ),
  CC_UNUSED( cc_cmpr_fnptr_ty, cmpr ),
  cc_dtor_fnptr_ty el_dtor,
  CC_UNUSED( cc_dtor_fnptr_ty, key_dtor ),
  cc_free_fnptr_ty free_
)
{
  cc_listnode_hdr_ty *hdr = cc_listnode_hdr( *(void **)key );
  cc_listnode_hdr_ty *next = hdr->next;
  hdr->prev->next = next;
  next->prev = hdr->prev;
  
  if( el_dtor )
    el_dtor( *(void **)key );

  free_( hdr );
  --cc_list_hdr( cntr )->size;

  // If next is end, we need to make sure we're returning the associated placeholder's end.
  if( next == &cc_list_hdr( cntr )->end )
    next = next->next;

  return cc_list_el( next );
}

// Removes the element pointed to by pointer-iterator src_itr from the source list and attaches it to the list before
// pointer-iterator itr.
// Although this function never allocates memory for the element/node itself, it must allocate the list's header if the
// list is currently a placeholder.
// Returns a cc_allocing_fn_result_ty containing the new container handle and a pointer that evaluates to true if the
// operation was successful or false in the case of allocation failure.
static inline cc_allocing_fn_result_ty cc_list_splice(
  void *cntr,
  void *itr,
  void *src,
  void *src_itr,
  cc_realloc_fnptr_ty realloc_
)
{
  if( cc_list_is_placeholder( cntr ) )
  {
    void *new_cntr = cc_list_alloc_hdr( cntr, realloc_ );
    if( CC_UNLIKELY( !new_cntr ) )
      return cc_make_allocing_fn_result( cntr, NULL );

    cntr = new_cntr;
  }

  cc_listnode_hdr( src_itr )->prev->next = cc_listnode_hdr( src_itr )->next;
  cc_listnode_hdr( src_itr )->next->prev = cc_listnode_hdr( src_itr )->prev;
  cc_list_attach( cntr, itr, cc_listnode_hdr( src_itr ) );

  --cc_list_hdr( src )->size;
  ++cc_list_hdr( cntr )->size;

  return cc_make_allocing_fn_result( cntr, cc_dummy_true_ptr );
}

// Initializes a shallow copy of the source list.
// This requires allocating memory for every node, as well as for the list's header unless src is a placeholder.
// Returns a pointer to the copy, or NULL in the case of allocation failure.
// That return value is cast to bool in the corresponding macro.
static inline void *cc_list_init_clone(
  void *src,
  size_t el_size,
  CC_UNUSED( uint64_t, layout ),
  cc_realloc_fnptr_ty realloc_,
  cc_free_fnptr_ty free_
)
{
  cc_allocing_fn_result_ty result = { (void *)&cc_list_placeholder, cc_dummy_true_ptr };

  for(
    void *i = cc_list_first( src, 0 /* Dummy */, 0 /* Dummy */ );
    i != cc_list_end( src, 0 /* Dummy */, 0 /* Dummy */ );
    i = cc_list_next( src, i, 0 /* Dummy */, 0 /* Dummy */ )
  )
  {
    result = cc_list_insert(
      result.new_cntr,
      i,
      &CC_MAKE_LVAL_COPY(
        void *,
        cc_list_end( result.new_cntr, 0 /* Dummy */, 0 /* Dummy */ )
      ),
      false,    // Dummy.
      el_size,
      0,        // Dummy.
      NULL,     // Dummy.
      NULL,     // Dummy.
      0.0,      // Dummy.
      NULL,     // Dummy.
      NULL,     // Dummy.
      realloc_,
      NULL      // Dummy.
    );

    if( !result.other_ptr )
    {
      // Erase incomplete clone without invoking destructors.
      
      void *j = cc_list_first( result.new_cntr, 0 /* Dummy */, 0 /* Dummy */ );
      while( j != cc_list_end( result.new_cntr, 0 /* Dummy */, 0 /* Dummy */ ) )
      {
        void *next = cc_list_next( result.new_cntr, j, 0 /* Dummy */, 0 /* Dummy */ );
        free_( cc_listnode_hdr( j ) );
        j = next;
      }

      if( !cc_list_is_placeholder( result.new_cntr ) )
        free_( result.new_cntr );

      return NULL;
    }
  }

  return result.new_cntr;
}

// Erases all elements, calling their destructors if necessary.
static inline void cc_list_clear(
  void *cntr,
  CC_UNUSED( size_t, el_size ),
  CC_UNUSED( uint64_t, layout ),
  cc_dtor_fnptr_ty el_dtor,
  CC_UNUSED( cc_dtor_fnptr_ty, key_dtor ),
  cc_free_fnptr_ty free_
)
{
  while( cc_list_first( cntr, 0 /* Dummy */, 0 /* Dummy */ ) != cc_list_end( cntr, 0 /* Dummy */, 0 /* Dummy */ ) )
    cc_list_erase(
      cntr,
      &CC_MAKE_LVAL_COPY( void *, cc_list_first( cntr, 0 /* Dummy */, 0 /* Dummy */ ) ),
      0,       // Dummy.
      0,       // Dummy.
      NULL,    // Dummy.
      NULL,    // Dummy.
      el_dtor,
      NULL,    // Dummy.
      free_
    );
}

// Erases all elements, calling their destructors if necessary, and frees the memory for the list's header if it is not
// a placeholder.
static inline void cc_list_cleanup(
  void *cntr,
  CC_UNUSED( size_t, el_size ),
  CC_UNUSED( uint64_t, layout ),
  cc_dtor_fnptr_ty el_dtor,
  CC_UNUSED( cc_dtor_fnptr_ty, key_dtor ),
  cc_free_fnptr_ty free_
)
{
  cc_list_clear( cntr, 0 /* Dummy */, 0 /* Dummy */, el_dtor, NULL /* Dummy */, free_ );

  if( !cc_list_is_placeholder( cntr ) )
    free_( cntr );
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*                                                        Map                                                         */
/*--------------------------------------------------------------------------------------------------------------------*/

// cc_map implements Verstable (https://github.com/JacksonAllan/Verstable) within the constraints of the CC API.
// As explained in its documentation, Verstable is an open-addressing hash table using quadratic probing and the
// following additions:
//
// - All keys that hash (i.e. "belong") to the same bucket (their "home bucket") are linked together by an 11-bit
//   integer specifying the quadratic displacement, relative to that bucket, of the next key in the chain.
//
// - If a chain of keys exists for a given bucket, then it always begins at that bucket. To maintain this policy, a
//   1-bit flag is used to mark whether the key occupying a bucket belongs there. When inserting a new key, if the
//   bucket it belongs to is occupied by a key that does not belong there, then the occupying key is evicted and the new
//   key takes the bucket.
//
// - A 4-bit fragment of each key's hash code is also stored.
//
// - The aforementioned metadata associated with each bucket (the 4-bit hash fragment, the 1-bit flag, and the 11-bit
//   link to the next key in the chain) are stored together in a uint16_t array rather than in the bucket alongside the
//   key and (optionally) the value.
//
// One way to conceptualize this scheme is as a chained hash table in which overflowing keys are stored not in separate
// memory allocations but in otherwise unused buckets. In this regard, it shares similarities with Malte Skarupke's
// Bytell hash table (https://www.youtube.com/watch?v=M2fKMP47slQ) and traditional "coalesced hashing".
//
// Advantages of this scheme include:
//
// - Fast lookups impervious to load factor: If the table contains any key belonging to the lookup key's home bucket,
//   then that bucket contains the first in a traversable chain of all keys belonging to it. Hence, only the home bucket
//   and other buckets containing keys belonging to it are ever probed. Moreover, the stored hash fragments allow
//   skipping most non-matching keys in the chain without accessing the actual buckets array or calling the (potentially
//   expensive) key comparison function.
//
// - Fast insertions: Insertions are faster than they are in other schemes that move keys around (e.g. Robin Hood)
//   because they only move, at most, one existing key.
//
// - Fast, tombstone-free deletions: Deletions, which usually require tombstones in quadratic-probing hash tables, are
//   tombstone-free and only move, at most, one existing key.
//
// - Fast iteration: The separate metadata array allows keys in sparsely populated tables to be found without incurring
//   the frequent cache misses that would result from traversing the buckets array.
//
// Adapting Verstable to the CC API necessitates some compromises, including:
//
// - Slower iteration: As a pointer-iterator in CC cannot store a pointer to the metadatum associated with the bucket to
//   which it points, the metadatum's address must be calculated every iteration.
//
// - Slower iterator-based erasure: As CC pointer-iterators cannot store a key's home bucket, CC must rehash the key
//   being deleted in cases wherein Verstable does not need to.

// Masks for manipulating and extracting data from a bucket's uint16_t metadatum.
#define CC_MAP_EMPTY               0x0000
#define CC_MAP_HASH_FRAG_MASK      0xF000 // 0b1111000000000000.
#define CC_MAP_IN_HOME_BUCKET_MASK 0x0800 // 0b0000100000000000.
#define CC_MAP_DISPLACEMENT_MASK   0x07FF // 0b0000011111111111, also denotes the displacement limit. Set to CC_LOAD to
                                          // 1.0 to test proper handling of encroachment on the displacement limit
                                          // during inserts.

// Extracts a hash fragment from a uint64_t hash code.
// We take the highest four bits so that keys that map (via modulo) to the same bucket have distinct hash fragments.
static inline uint16_t cc_hash_frag( size_t hash )
{
  return ( hash >> ( sizeof( size_t ) - sizeof( uint16_t ) ) * CHAR_BIT ) & CC_MAP_HASH_FRAG_MASK;
}

// Standard quadratic probing formula that guarantees that all buckets are visited when the bucket count is a power of
// two (at least in theory, because the displacement limit could terminate the search early when the bucket count is
// high).
static inline size_t cc_quadratic( uint16_t displacement )
{
  return ( (size_t)displacement * displacement + displacement ) / 2;
}

#define CC_MAP_MIN_NONZERO_BUCKET_COUNT 8 // Must be a power of two.

// Map header.
typedef struct
{
  alignas( max_align_t )
  size_t size;
  size_t cap_mask; // Rather than storing the capacity (i.e. bucket count) directly, we store the bit mask used to
                   // reduce a hash code or displacement-derived bucket index to the buckets array, i.e. the capacity
                   // minus one.
                   // Consequently, a zero bucket count (i.e. a placeholder map) constitutes a special case, represented
                   // by all bits unset (i.e. zero).
  uint16_t *metadata; // As described above, each metadatum consists of a 4-bit hash-code fragment (X), a 1-bit flag
                      // indicating whether the key in this bucket begins a chain associated with the bucket (Y), and
                      // an 11-bit value indicating the quadratic displacement of the next key in the chain (Z):
                      // XXXXYZZZZZZZZZZZ.
                      // The metadata array lives in the same allocation as the header and buckets array, but we
                      // store a pointer to it to avoid constantly recalculating its offset.
} cc_map_hdr_ty;

// In the case of maps, this placeholder allows us to avoid checking for a NULL handle inside functions.
// Setting the placeholder's metadata pointer to point to a CC_MAP_EMPTY placeholder, rather than NULL, allows us to
// avoid checking for a zero bucket count during insertion and lookup.
static const uint16_t cc_map_placeholder_metadatum = CC_MAP_EMPTY;
static const cc_map_hdr_ty cc_map_placeholder = { 0, 0x0000000000000000ull, (uint16_t *)&cc_map_placeholder_metadatum };

static inline cc_map_hdr_ty *cc_map_hdr( void *cntr )
{
  return (cc_map_hdr_ty *)cntr;
}

static inline size_t cc_map_size( void *cntr )
{
  return cc_map_hdr( cntr )->size;
}

static inline size_t cc_map_cap( void *cntr )
{
  // If the capacity is zero, cap_mask will be zero, not the capacity minus one.
  // We account for this special case by adding (bool)cap_mask rather than one.
  return cc_map_hdr( cntr )->cap_mask + (bool)cc_map_hdr( cntr )->cap_mask;
}

static inline bool cc_map_is_placeholder( void *cntr )
{
  return !cc_map_hdr( cntr )->cap_mask;
}

static inline void *cc_map_el(
  void *cntr,
  size_t bucket,
  size_t el_size,
  uint64_t layout
)
{
  return (char *)cntr + sizeof( cc_map_hdr_ty ) + CC_BUCKET_SIZE( el_size, layout ) * bucket;
}

static inline void *cc_map_key(
  void *cntr,
  size_t bucket,
  size_t el_size,
  uint64_t layout
)
{
  return (char *)cc_map_el( cntr, bucket, el_size, layout ) + CC_KEY_OFFSET( el_size, layout );
}

static inline void *cc_map_key_for(
  void *itr,
  size_t el_size,
  uint64_t layout
)
{
  return (char *)itr + CC_KEY_OFFSET( el_size, layout );
}

static inline size_t cc_map_bucket_index_from_itr( void *cntr, void *itr, size_t el_size, uint64_t layout )
{
  return ( (char *)itr - (char *)cc_map_el( cntr, 0, el_size, layout ) ) / CC_BUCKET_SIZE( el_size, layout );
}

static inline size_t cc_map_min_cap_for_n_els(
  size_t n,
  double max_load
)
{
  if( n == 0 )
    return 0;

  // Round up to a power of two.
  size_t cap = CC_MAP_MIN_NONZERO_BUCKET_COUNT;
  while( n > cap * max_load )
    cap *= 2;

  return cap;
}

// Calculates the metadata array offset and total allocation size for a map with a given non-zero capacity.
// The data is organized in memory as follows:
//   +--------+-----------------------------+-----+----------------+--------+
//   | Header |           Buckets           | Pad |    Metadata    | Excess |
//   +--------+-----------------------------+-----+----------------+--------+
// The metadata array requires four excess elements to ensure that iteration functions, which read four metadata at a
// time, never read beyond the end of it.
static inline void cc_map_allocation_details(
  size_t cap,
  size_t el_size,
  uint64_t layout,
  size_t *metadata_offset,
  size_t *allocation_size
)
{
  size_t buckets_size = CC_BUCKET_SIZE( el_size, layout ) * cap;
  *metadata_offset = sizeof( cc_map_hdr_ty ) + buckets_size + CC_PADDING( buckets_size, alignof( uint16_t ) );
  *allocation_size = *metadata_offset + sizeof( uint16_t ) * ( cap + 4 );
}

// Finds the earliest empty bucket in which a key-element pair belonging to home_bucket can be placed, assuming that
// home_bucket is already occupied.
// The reason to begin the search at home_bucket, rather than the end of the existing chain, is that key-element pairs
// deleted from other chains might have freed up buckets that could fall in this chain before the final key.
// Returns true if an empty bucket within the range of the displacement limit was found, in which case the final two
// pointer arguments contain the index of the empty bucket and its quadratic displacement from home_bucket.

static inline bool cc_map_find_first_empty(
  void *cntr,
  size_t home_bucket,
  size_t *empty,
  uint16_t *displacement
)
{
  *displacement = 1;
  size_t linear_dispacement = 1;

  while( true )
  {
    *empty = ( home_bucket + linear_dispacement ) & cc_map_hdr( cntr )->cap_mask;
    if( cc_map_hdr( cntr )->metadata[ *empty ] == CC_MAP_EMPTY )
      return true;

    if( CC_UNLIKELY( ++*displacement == CC_MAP_DISPLACEMENT_MASK ) )
      return false;

    linear_dispacement += *displacement;
  }
}

// Finds the key-element pair in the chain beginning in home_bucket after which to link a new key-element pair with
// displacement_to_empty quadratic displacement and returns the index of the bucket containing that key-element pair.
// Although the new key-element pair could simply be linked to the end of the chain, keeping the chain ordered by
// displacement theoretically improves cache locality during lookups.
static inline size_t cc_map_find_insert_location_in_chain(
  void *cntr,
  size_t home_bucket,
  uint16_t displacement_to_empty
)
{
  size_t candidate = home_bucket;
  while( true )
  {
    uint16_t displacement = cc_map_hdr( cntr )->metadata[ candidate ] & CC_MAP_DISPLACEMENT_MASK;

    if( displacement > displacement_to_empty )
      return candidate;

    candidate = ( home_bucket + cc_quadratic( displacement ) ) & cc_map_hdr( cntr )->cap_mask;
  }
}

// Frees up a bucket occupied by a key-element pair not belonging there so that a new key-element pair belonging there
// can be placed there as the beginning of a new chain.
// This requires:
// - Finding the previous key-element pair in the chain to which the occupying key-element pair belongs by rehashing the
//   key and traversing the chain.
// - Disconnecting the key-element pair from the chain.
// - Finding the appropriate empty bucket to which to move the key-element pair.
// - Moving the key-element pair to the empty bucket.
// - Re-linking the key-element pair to the chain.
// Returns true if the eviction succeeded, or false if no empty bucket to which to evict the occupying key-element pair
// could be found within the displacement limit.
static inline bool cc_map_evict(
  void *cntr,
  size_t bucket,
  size_t el_size,
  uint64_t layout,
  cc_hash_fnptr_ty hash
)
{
  // Find the previous key-element pair in chain.
  size_t home_bucket = hash( cc_map_key( cntr, bucket, el_size, layout ) ) & cc_map_hdr( cntr )->cap_mask;
  size_t prev = home_bucket;
  while( true )
  {
    size_t next = ( home_bucket + cc_quadratic( cc_map_hdr( cntr )->metadata[ prev ] & CC_MAP_DISPLACEMENT_MASK ) ) &
      cc_map_hdr( cntr )->cap_mask;

    if( next == bucket )
      break;

    prev = next;
  }

  // Disconnect the key-element pair from chain.
  cc_map_hdr( cntr )->metadata[ prev ] = ( cc_map_hdr( cntr )->metadata[ prev ] & ~CC_MAP_DISPLACEMENT_MASK ) |
    ( cc_map_hdr( cntr )->metadata[ bucket ] & CC_MAP_DISPLACEMENT_MASK );

  // Find the empty bucket to which to move the key-element pair.
  size_t empty;
  uint16_t displacement;
  if( CC_UNLIKELY( !cc_map_find_first_empty( cntr, home_bucket, &empty, &displacement ) ) )
    return false;

  // Find the key-element pair in the chain after which to link the moved key-element pair.
  prev = cc_map_find_insert_location_in_chain( cntr, home_bucket, displacement );

  // Move the key and element.
  memcpy(
    cc_map_el( cntr, empty, el_size, layout ),
    cc_map_el( cntr, bucket, el_size, layout ),
    CC_BUCKET_SIZE( el_size, layout )
  );

  // Re-link the key-element pair to the chain from its new bucket.
  cc_map_hdr( cntr )->metadata[ empty ] = ( cc_map_hdr( cntr )->metadata[ bucket ] & CC_MAP_HASH_FRAG_MASK ) |
    ( cc_map_hdr( cntr )->metadata[ prev ] & CC_MAP_DISPLACEMENT_MASK );
  cc_map_hdr( cntr )->metadata[ prev ] = ( cc_map_hdr( cntr )->metadata[ prev ] & ~CC_MAP_DISPLACEMENT_MASK ) |
    displacement;

  return true;
}

// Inserts a key-element pair, optionally replacing the existing key-element pair containing the same key if it exists.
// There are two main cases that must be handled:
// - If the key-element pair's home bucket is empty or occupied by a key-element pair that does not belong there, then
//   the key-element pair is inserted there, evicting the occupying key-element pair if there is one.
// - Otherwise, the chain of key-element pairs beginning at the home bucket is traversed in search of a matching key.
//   If none is found, then the new key-element pair is inserted at the earliest available bucket, per quadratic probing
//   from the home bucket, and then linked to the chain in a manner that maintains its quadratic order.
// The replace argument tells the function whether to replace an existing key-element pair.
// If replace is true, the function returns a pointer-iterator to the inserted element, or NULL if the key-element pair
// was not inserted because of the max load factor or displacement limit constraints.
// If replace is false, then the return value is as described above, except that if the key already exists, the function
// returns a pointer-iterator to the associated element.
static inline void *cc_map_insert_raw(
  void *cntr,
  void *el,
  void *key,
  bool replace,
  size_t el_size,
  uint64_t layout,
  double max_load,
  cc_hash_fnptr_ty hash,
  cc_cmpr_fnptr_ty cmpr,
  cc_dtor_fnptr_ty el_dtor,
  cc_dtor_fnptr_ty key_dtor
)
{
  size_t key_hash = hash( key );
  uint16_t hashfrag = cc_hash_frag( key_hash );
  size_t home_bucket = key_hash & cc_map_hdr( cntr )->cap_mask;

  // Case 1: The home bucket is empty or contains a key-element pair that doesn't belong there.
  // This case also implicitly handles the case of a zero-capacity placeholder map, since home_bucket will be zero and
  // metadata[ 0 ] will be the empty placeholder metadatum.
  // In that scenario, the zero cap_mask triggers the below load-factor check.
  if( !( cc_map_hdr( cntr )->metadata[ home_bucket ] & CC_MAP_IN_HOME_BUCKET_MASK ) )
  {
    // Load-factor check.
    if( CC_UNLIKELY( cc_map_hdr( cntr )->size + 1 > max_load * cc_map_cap( cntr ) ) )
      return NULL;

    // Vacate the home bucket if it contains a key-element pair.
    if(
      cc_map_hdr( cntr )->metadata[ home_bucket ] != CC_MAP_EMPTY &&
      CC_UNLIKELY( !cc_map_evict( cntr, home_bucket, el_size, layout, hash ) )
    )
      return NULL;

    memcpy( cc_map_key( cntr, home_bucket, el_size, layout ), key, CC_KEY_SIZE( layout ) );
    memcpy( cc_map_el( cntr, home_bucket, el_size, layout ), el, el_size );
    cc_map_hdr( cntr )->metadata[ home_bucket ] = hashfrag | CC_MAP_IN_HOME_BUCKET_MASK | CC_MAP_DISPLACEMENT_MASK;

    ++cc_map_hdr( cntr )->size;

    return cc_map_el( cntr, home_bucket, el_size, layout );
  }

  // Case 2: The home bucket contains the beginning of a chain.

  // Check the existing chain.
  size_t bucket = home_bucket;
  while( true )
  {
    if(
      ( cc_map_hdr( cntr )->metadata[ bucket ] & CC_MAP_HASH_FRAG_MASK ) == hashfrag &&
      CC_LIKELY( cmpr( cc_map_key( cntr, bucket, el_size, layout ), key ) )
    )
    {
      if( replace )
      {
        if( key_dtor )
          key_dtor( cc_map_key( cntr, bucket, el_size, layout ) );

        if( el_dtor )
          el_dtor( cc_map_el( cntr, bucket, el_size, layout ) );

        memcpy( cc_map_key( cntr, bucket, el_size, layout ), key, CC_KEY_SIZE( layout ) );
        memcpy( cc_map_el( cntr, bucket, el_size, layout ), el, el_size );
      }

      return cc_map_el( cntr, bucket, el_size, layout );
    }

    uint16_t displacement = cc_map_hdr( cntr )->metadata[ bucket ] & CC_MAP_DISPLACEMENT_MASK;
    if( displacement == CC_MAP_DISPLACEMENT_MASK )
      break;

    bucket = ( home_bucket + cc_quadratic( displacement ) ) & cc_map_hdr( cntr )->cap_mask;
  }

  // Load-factor check.
  if( CC_UNLIKELY( cc_map_hdr( cntr )->size + 1 > max_load * cc_map_cap( cntr ) ) )
    return NULL;

  // Find the earliest empty bucket, per quadratic probing.
  size_t empty;
  uint16_t displacement;
  if( CC_UNLIKELY( !cc_map_find_first_empty( cntr, home_bucket, &empty, &displacement ) ) )
    return NULL;

  size_t prev = cc_map_find_insert_location_in_chain( cntr, home_bucket, displacement );

  // Insert the new key and element in the empty bucket and link it to the chain.

  memcpy( cc_map_key( cntr, empty, el_size, layout ), key, CC_KEY_SIZE( layout ) );
  memcpy( cc_map_el( cntr, empty, el_size, layout ), el, el_size );

  cc_map_hdr( cntr )->metadata[ empty ] = hashfrag | ( cc_map_hdr( cntr )->metadata[ prev ] & CC_MAP_DISPLACEMENT_MASK
    );
  cc_map_hdr( cntr )->metadata[ prev ] = ( cc_map_hdr( cntr )->metadata[ prev ] & ~CC_MAP_DISPLACEMENT_MASK ) |
    displacement;

  ++cc_map_hdr( cntr )->size;

  return cc_map_el( cntr, empty, el_size, layout );
}

// Inserts a key-element pair, assuming that the key does not already exist and that the map's capacity is large enough
// to accommodate it without violating the load factor constraint.
// These conditions are met during map resizing and rehashing.
// This function is the same as cc_map_insert_raw, except that no load-factor check or check of the existing chain is
// performed.
// It returns a pointer-iterator to the inserted element, or NULL if the key-element pair was not inserted because of
// the max load factor or displacement limit constraints.
static inline void *cc_map_reinsert(
  void *cntr,
  void *el,
  void *key,
  size_t el_size,
  uint64_t layout,
  cc_hash_fnptr_ty hash
)
{
  size_t key_hash = hash( key );
  uint16_t hashfrag = cc_hash_frag( key_hash );
  size_t home_bucket = key_hash & cc_map_hdr( cntr )->cap_mask;

  if( !( cc_map_hdr( cntr )->metadata[ home_bucket ] & CC_MAP_IN_HOME_BUCKET_MASK ) )
  {
    if(
      cc_map_hdr( cntr )->metadata[ home_bucket ] != CC_MAP_EMPTY &&
      CC_UNLIKELY( !cc_map_evict( cntr, home_bucket, el_size, layout, hash ) )
    )
      return NULL;

    memcpy( cc_map_key( cntr, home_bucket, el_size, layout ), key, CC_KEY_SIZE( layout ) );
    memcpy( cc_map_el( cntr, home_bucket, el_size, layout ), el, el_size );
    cc_map_hdr( cntr )->metadata[ home_bucket ] = hashfrag | CC_MAP_IN_HOME_BUCKET_MASK | CC_MAP_DISPLACEMENT_MASK;

    ++cc_map_hdr( cntr )->size;

    return cc_map_el( cntr, home_bucket, el_size, layout );
  }

  size_t empty;
  uint16_t displacement;
  if( CC_UNLIKELY( !cc_map_find_first_empty( cntr, home_bucket, &empty, &displacement ) ) )
    return NULL;

  size_t prev = cc_map_find_insert_location_in_chain( cntr, home_bucket, displacement );

  memcpy( cc_map_key( cntr, empty, el_size, layout ), key, CC_KEY_SIZE( layout ) );
  memcpy( cc_map_el( cntr, empty, el_size, layout ), el, el_size );

  cc_map_hdr( cntr )->metadata[ empty ] = hashfrag | ( cc_map_hdr( cntr )->metadata[ prev ] & CC_MAP_DISPLACEMENT_MASK
    );
  cc_map_hdr( cntr )->metadata[ prev ] = ( cc_map_hdr( cntr )->metadata[ prev ] & ~CC_MAP_DISPLACEMENT_MASK ) |
    displacement;

  ++cc_map_hdr( cntr )->size;

  return cc_map_el( cntr, empty, el_size, layout );
}

// Creates a rehashed duplicate of cntr with capacity cap.
// Assumes that cap is a power of two large enough to accommodate all key-element pairs without violating the max load
// factor.
// Returns a pointer to the duplicate, or NULL in the case of allocation failure.
// As this function is called very rarely in cc_map_insert and cc_map_get_or_insert, ideally it should not be inlined
// into those functions.
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes" // Silence warning about combining noinline with static inline.
__attribute__((noinline))
#endif
static inline void *cc_map_make_rehash(
  void *cntr,
  size_t cap,
  size_t el_size,
  uint64_t layout,
  cc_hash_fnptr_ty hash,
  cc_realloc_fnptr_ty realloc_,
  cc_free_fnptr_ty free_
)
{
  // The attempt to resize and rehash must occur inside a loop that incrementally doubles the target bucket count
  // because a failure could theoretically occur at any load factor due to the displacement limit.
  while( true )
  {
    size_t metadata_offset;
    size_t allocation_size;
    cc_map_allocation_details( cap, el_size, layout, &metadata_offset, &allocation_size );

    cc_map_hdr_ty *new_cntr = (cc_map_hdr_ty *)realloc_( NULL, allocation_size );
    if( CC_UNLIKELY( !new_cntr ) )
      return NULL;

    new_cntr->size = 0;
    new_cntr->cap_mask = cap - 1;
    new_cntr->metadata = (uint16_t *)( (char *)new_cntr + metadata_offset );

    memset( new_cntr->metadata, 0x00, ( cap + 4 ) * sizeof( uint16_t ) );

    // Iteration stopper at the end of the actual metadata array (i.e. the first of the four excess metadata).
    new_cntr->metadata[ cap ] = 0x01;

    for( size_t i = 0; i < cc_map_cap( cntr ); ++i )
      if( cc_map_hdr( cntr )->metadata[ i ] != CC_MAP_EMPTY )
      {
        void *key = cc_map_key( cntr, i, el_size, layout );
        if( CC_UNLIKELY( !cc_map_reinsert(            
          new_cntr,
          cc_map_el( cntr, i, el_size, layout ),
          key,
          el_size,
          layout,
          hash
        ) ) )
          break;
      }

    // If a key could not be reinserted due to the displacement limit, double the bucket count and retry.
    if( CC_UNLIKELY( new_cntr->size < cc_map_size( cntr ) ) )
    {
      free_( new_cntr );
      cap *= 2;
      continue;
    }

    return new_cntr;
  }
}
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

// Reserves capacity such that the map can accommodate n key-element pairs without reallocation (i.e. without violating
// the max load factor).
// Returns a cc_allocing_fn_result_ty containing the new container handle and a pointer that evaluates to true if the
// operation was successful or false in the case of allocation failure.
static inline cc_allocing_fn_result_ty cc_map_reserve(
  void *cntr,
  size_t n,
  size_t el_size,
  uint64_t layout,
  cc_hash_fnptr_ty hash,
  double max_load,
  cc_realloc_fnptr_ty realloc_,
  cc_free_fnptr_ty free_
)
{
  size_t cap = cc_map_min_cap_for_n_els( n, max_load );

  if( cc_map_cap( cntr ) >= cap )
    return cc_make_allocing_fn_result( cntr, cc_dummy_true_ptr );

  void *new_cntr = cc_map_make_rehash(
    cntr,
    cap,
    el_size,
    layout,
    hash,
    realloc_,
    free_
  );
  if( CC_UNLIKELY( !new_cntr ) )
    return cc_make_allocing_fn_result( cntr, NULL );

  if( !cc_map_is_placeholder( cntr ) )
    free_( cntr );

  return cc_make_allocing_fn_result( new_cntr, cc_dummy_true_ptr );
}

// Inserts a key-element pair.
// If replace is true, then the new key-element pair replaces any existing key-element pair containing the same key.
// This function wraps cc_map_insert_raw in a loop that handles growing and rehashing the table if a new key-element
// pair cannot be inserted because of the max load factor or displacement limit constraints.
// Returns a cc_allocing_fn_result_ty containing the new container handle and a pointer-iterator to the newly inserted
// element, or to the existing element with the matching key if replace is false.
// If the underlying storage needed to be expanded and an allocation failure occurred, the latter pointer will be NULL.
static inline cc_allocing_fn_result_ty cc_map_insert(
  void *cntr,
  void *el,
  void *key,
  bool replace,
  size_t el_size,
  uint64_t layout,
  cc_hash_fnptr_ty hash,
  cc_cmpr_fnptr_ty cmpr,
  double max_load,
  cc_dtor_fnptr_ty el_dtor,
  cc_dtor_fnptr_ty key_dtor,
  cc_realloc_fnptr_ty realloc_,
  cc_free_fnptr_ty free_
)
{
  while( true )
  {
    void *itr = cc_map_insert_raw(
      cntr,
      el,
      key,
      replace,
      el_size,
      layout,
      max_load,
      hash,
      cmpr,
      el_dtor,
      key_dtor
    );

    if( CC_LIKELY( itr ) )
      return cc_make_allocing_fn_result( cntr, itr );

    void *new_cntr = cc_map_make_rehash(
      cntr,
      cc_map_hdr( cntr )->cap_mask ? cc_map_cap( cntr ) * 2 : CC_MAP_MIN_NONZERO_BUCKET_COUNT,
      el_size,
      layout,
      hash,
      realloc_,
      free_
    );

    if( CC_UNLIKELY( !new_cntr ) )
      return cc_make_allocing_fn_result( cntr, NULL );

    if( !cc_map_is_placeholder( cntr ) )
        free_( cntr );

    cntr = new_cntr;
  }
}

static inline void *cc_map_get(
  void *cntr,
  void *key,
  size_t el_size,
  uint64_t layout,
  cc_hash_fnptr_ty hash,
  cc_cmpr_fnptr_ty cmpr
)
{
  size_t key_hash = hash( key );
  size_t home_bucket = key_hash & cc_map_hdr( cntr )->cap_mask;

  // If the home bucket is empty or contains a key-element pair that does not belong there, then our key does not exist.
  // This check also implicitly handles the case of a zero bucket count, since home_bucket will be zero and
  // metadata[ 0 ] will be the empty placeholder.
  if( !( cc_map_hdr( cntr )->metadata[ home_bucket ] & CC_MAP_IN_HOME_BUCKET_MASK ) )
    return NULL;

  // Traverse the chain of key-element pairs belonging to the home bucket.
  uint16_t hashfrag = cc_hash_frag( key_hash );
  size_t bucket = home_bucket;
  while( true )
  {
    if(
      ( cc_map_hdr( cntr )->metadata[ bucket ] & CC_MAP_HASH_FRAG_MASK ) == hashfrag &&
      CC_LIKELY( cmpr( cc_map_key( cntr, bucket, el_size, layout ), key ) )
    )
      return cc_map_el( cntr, bucket, el_size, layout );

    uint16_t displacement = cc_map_hdr( cntr )->metadata[ bucket ] & CC_MAP_DISPLACEMENT_MASK;
    if( displacement == CC_MAP_DISPLACEMENT_MASK )
      return NULL;

    bucket = ( home_bucket + cc_quadratic( displacement ) ) & cc_map_hdr( cntr )->cap_mask;
  }
}

// For maps, the container handle doubles up as r_end.
static inline void *cc_map_r_end( void *cntr )
{
  return cntr;
}

static inline void *cc_map_end(
  void *cntr,
  size_t el_size,
  uint64_t layout
)
{
  return cc_map_el( cntr, cc_map_cap( cntr ), el_size, layout );
}

// Finds the first occupied bucket at or after the bucket pointed to by itr.
// This function scans four buckets at a time, ideally using intrinsics.
static inline void *cc_map_leap_forward( void *cntr, void *itr, size_t el_size, uint64_t layout )
{
  uint16_t *itr_metadatum = cc_map_hdr( cntr )->metadata + cc_map_bucket_index_from_itr( cntr, itr, el_size, layout );

  while( true )
  {
    uint64_t metadata;
    memcpy( &metadata, itr_metadatum, sizeof( uint64_t ) );
    if( metadata )
      return (char *)itr + CC_BUCKET_SIZE( el_size, layout ) * cc_first_nonzero_uint16( metadata );

    itr = (char *)itr + CC_BUCKET_SIZE( el_size, layout ) * 4;
    itr_metadatum += 4;
  }
}

// Finds the first occupied bucket before the bucket pointed to by itr.
// This function also scans four buckets at a time, ideally using intrinsics.
// However, because the r_end pointer-iterator, unlike end, constitutes a special case, this function is less efficient
// cc_map_leap_forward.
static inline void *cc_map_leap_backward( void *cntr, void *itr, size_t el_size, uint64_t layout )
{
  size_t bucket = cc_map_bucket_index_from_itr( cntr, itr, el_size, layout );

  while( true )
  {
    if( bucket < 4 )
    {
      while( bucket-- )
        if( cc_map_hdr( cntr )->metadata[ bucket ] )
          return cc_map_el( cntr, bucket, el_size, layout );

      return cc_map_r_end( cntr );
    }

    if( cc_map_hdr( cntr )->metadata == &cc_map_placeholder_metadatum )
      __builtin_unreachable();

    uint64_t metadatum;
    memcpy( &metadatum, cc_map_hdr( cntr )->metadata + bucket - 4, sizeof( uint64_t ) );
    if( metadatum )
      return cc_map_el( cntr, bucket - 1 - cc_last_nonzero_uint16( metadatum ), el_size, layout );

    bucket -= 4;
  }
}

static inline void *cc_map_first(
  void *cntr,
  size_t el_size,
  uint64_t layout
)
{
  void *itr = cc_map_el( cntr, 0, el_size, layout );

  if( !cc_map_hdr( cntr )->cap_mask )
    return itr;

  return cc_map_leap_forward( cntr, itr, el_size, layout );
}

static inline void *cc_map_last(
  void *cntr,
  size_t el_size,
  uint64_t layout
)
{
  return cc_map_leap_backward( cntr, cc_map_end( cntr, el_size, layout ), el_size, layout );
}

static inline void *cc_map_prev(
  void *cntr,
  void *itr,
  size_t el_size,
  uint64_t layout
)
{
  return cc_map_leap_backward( cntr, itr, el_size, layout );
}

static inline void *cc_map_next(
  void *cntr,
  void *itr,
  size_t el_size,
  uint64_t layout
)
{
  itr = (char *)itr + CC_BUCKET_SIZE( el_size, layout );
  return cc_map_leap_forward( cntr, itr, el_size, layout );
}

// Erases the key-element pair in the specified bucket.
// The erasure always occurs at the end of the chain to which the key-element pair belongs.
// If the key-element pair to be erased is not the last in the chain, it is swapped with the last so that erasure occurs
// at the end.
// This helps keep a chain's key-element pairs close to their home bucket for the sake of cache locality.
// Returns true if, in the case of iteration from first to end, cc_map_next should now be called on the pointer-iterator
// to find the next key-element pair.
// This return value is necessary because at the iterator location, the erasure could result in an empty bucket, a
// bucket containing a moved key-element pair already visited during the iteration, or a bucket containing a moved
// key-element pair not yet visited.
static inline bool cc_map_erase_raw(
  void *cntr,
  size_t erase_bucket,
  size_t home_bucket, // SIZE_MAX if unknown.
  size_t el_size,
  size_t layout,
  cc_hash_fnptr_ty hash,
  cc_dtor_fnptr_ty el_dtor,
  cc_dtor_fnptr_ty key_dtor
)
{
  --cc_map_hdr( cntr )->size;

  // Case 1: The key-element pair is the only one in its chain, so just remove it.
  if(
    cc_map_hdr( cntr )->metadata[ erase_bucket ] & CC_MAP_IN_HOME_BUCKET_MASK &&
    ( cc_map_hdr( cntr )->metadata[ erase_bucket ] & CC_MAP_DISPLACEMENT_MASK ) == CC_MAP_DISPLACEMENT_MASK
  )
  {
    if( el_dtor )
      el_dtor( cc_map_el( cntr, erase_bucket, el_size, layout ) );
    if( key_dtor )
      key_dtor( cc_map_key( cntr, erase_bucket, el_size, layout ) );

    cc_map_hdr( cntr )->metadata[ erase_bucket ] = CC_MAP_EMPTY;
    return true;
  }

  // Case 2 and 3 require that we know the key-element pair's home bucket.
  if( home_bucket == SIZE_MAX )
  {
    if( cc_map_hdr( cntr )->metadata[ erase_bucket ] & CC_MAP_IN_HOME_BUCKET_MASK )
      home_bucket = erase_bucket;
    else
      home_bucket = hash( cc_map_key( cntr, erase_bucket, el_size, layout ) ) & cc_map_hdr( cntr )->cap_mask;
  }

  if( el_dtor )
    el_dtor( cc_map_el( cntr, erase_bucket, el_size, layout ) );
  if( key_dtor )
    key_dtor( cc_map_key( cntr, erase_bucket, el_size, layout ) );

  // Case 2: The key-element pair is the last in a chain containing multiple key-element pairs.
  // Traverse the chain from the beginning and find the penultimate key-element pair.
  // Then disconnect the key-element pair and erase.
  if( ( cc_map_hdr( cntr )->metadata[ erase_bucket ] & CC_MAP_DISPLACEMENT_MASK ) == CC_MAP_DISPLACEMENT_MASK )
  {
    size_t bucket = home_bucket;
    while( true )
    {
      uint16_t displacement = cc_map_hdr( cntr )->metadata[ bucket ] & CC_MAP_DISPLACEMENT_MASK;
      size_t next = ( home_bucket + cc_quadratic( displacement ) ) & cc_map_hdr( cntr )->cap_mask;
      if( next == erase_bucket )
      {
        cc_map_hdr( cntr )->metadata[ bucket ] |= CC_MAP_DISPLACEMENT_MASK;
        cc_map_hdr( cntr )->metadata[ erase_bucket ] = CC_MAP_EMPTY;
        return true;
      }

      bucket = next;
    }
  }

  // Case 3: The chain has multiple key-element pairs, and the key-element pair is not the last one.
  // Traverse the chain from the key-element pair to be erased and find the last and penultimate key-element pairs.
  // Disconnect the last key-element pair from the chain, and swap it with the key-element pair to erase.
  size_t bucket = erase_bucket;
  while( true )
  {
    size_t prev = bucket;
    bucket = ( home_bucket + cc_quadratic( cc_map_hdr( cntr )->metadata[ bucket ] & CC_MAP_DISPLACEMENT_MASK ) ) &
      cc_map_hdr( cntr )->cap_mask;

    if( ( cc_map_hdr( cntr )->metadata[ bucket ] & CC_MAP_DISPLACEMENT_MASK ) == CC_MAP_DISPLACEMENT_MASK )
    {
      memcpy(
        cc_map_el( cntr, erase_bucket, el_size, layout ),
        cc_map_el( cntr, bucket, el_size, layout ),
        CC_BUCKET_SIZE( el_size, layout )
      );

      cc_map_hdr( cntr )->metadata[ erase_bucket ] = ( cc_map_hdr( cntr )->metadata[ erase_bucket ] &
        ~CC_MAP_HASH_FRAG_MASK ) | ( cc_map_hdr( cntr )->metadata[ bucket ] & CC_MAP_HASH_FRAG_MASK );

      cc_map_hdr( cntr )->metadata[ prev ] |= CC_MAP_DISPLACEMENT_MASK;
      cc_map_hdr( cntr )->metadata[ bucket ] = CC_MAP_EMPTY;

      // Whether an pointer-iterator pointing to erase_bucket should be advanced depends on whether the key-element pair
      // moved to the erase_bucket came from before or after that bucket.
      // In the former case, the iteration would already have hit the moved key-element pair, so the pointer-iterator
      // should still be advanced.
      if( bucket > erase_bucket )
        return false;

      return true;
    }
  }
}

// Erases the key-element pair pointed to by itr and returns a pointer-iterator to the next key-element pair in the
// table.
// This function must be inlined to ensure that the compiler optimizes away the cc_map_next call if the returned
// pointer-iterator is discarded.
static inline CC_ALWAYS_INLINE void *cc_map_erase_itr(
  void *cntr,
  void *itr,
  size_t el_size,
  uint64_t layout,
  cc_hash_fnptr_ty hash,
  cc_dtor_fnptr_ty el_dtor,
  cc_dtor_fnptr_ty key_dtor
)
{
  size_t bucket = cc_map_bucket_index_from_itr( cntr, itr, el_size, layout );

  if( cc_map_erase_raw( cntr, bucket, SIZE_MAX, el_size, layout, hash, el_dtor, key_dtor ) )
    return cc_map_next( cntr, itr, el_size, layout );

  return itr;
}

// Erases the key-element pair containing the specified key, if it exists.
// Returns a pointer that evaluates to true if a key-element pair was erased, or else is NULL.
// This pointer is eventually cast to bool by the cc_erase API macro.
static inline void *cc_map_erase(
  void *cntr,
  void *key,
  size_t el_size,
  uint64_t layout,
  cc_hash_fnptr_ty hash,
  cc_cmpr_fnptr_ty cmpr,
  cc_dtor_fnptr_ty el_dtor,
  cc_dtor_fnptr_ty key_dtor,
  CC_UNUSED( cc_free_fnptr_ty, free_ )
)
{
  size_t key_hash = hash( key );
  size_t home_bucket = key_hash & cc_map_hdr( cntr )->cap_mask;

  if( !( cc_map_hdr( cntr )->metadata[ home_bucket ] & CC_MAP_IN_HOME_BUCKET_MASK ) )
    return NULL;

  uint16_t hashfrag = cc_hash_frag( key_hash );
  size_t bucket = home_bucket;
  while( true )
  {
    if(
      ( cc_map_hdr( cntr )->metadata[ bucket ] & CC_MAP_HASH_FRAG_MASK ) == hashfrag &&
      CC_LIKELY( cmpr( cc_map_key( cntr, bucket, el_size, layout ), key ) )
    )
    {
      cc_map_erase_raw( cntr, bucket, home_bucket, el_size, layout, hash, el_dtor, key_dtor );
      return &cc_dummy_true;
    }

    uint16_t displacement = cc_map_hdr( cntr )->metadata[ bucket ] & CC_MAP_DISPLACEMENT_MASK;
    if( displacement == CC_MAP_DISPLACEMENT_MASK )
      return NULL;

    bucket = ( home_bucket + cc_quadratic( displacement ) ) & cc_map_hdr( cntr )->cap_mask;
  }
}

// Shrinks the map's capacity to the minimum possible without violating the max load factor associated with the key
// type.
// If shrinking is necessary, then a complete rehash occurs.
// Returns a cc_allocing_fn_result_ty containing the new container handle and a pointer that evaluates to true if the
// operation was successful or false in the case of allocation failure.
static inline cc_allocing_fn_result_ty cc_map_shrink(
  void *cntr,
  size_t el_size,
  uint64_t layout,
  cc_hash_fnptr_ty hash,
  double max_load,
  cc_realloc_fnptr_ty realloc_,
  cc_free_fnptr_ty free_
)
{
  size_t cap = cc_map_min_cap_for_n_els( cc_map_size( cntr ), max_load );

  if( cap == cc_map_cap( cntr ) ) // Shrink unnecessary.
    return cc_make_allocing_fn_result( cntr, cc_dummy_true_ptr );

  if( cap == 0 ) // Restore placeholder.
  {
    if( !cc_map_is_placeholder( cntr ) )
      free_( cntr );

    return cc_make_allocing_fn_result( (void *)&cc_map_placeholder, cc_dummy_true_ptr );
  }

  void *new_cntr = cc_map_make_rehash(
    cntr,
    cap,
    el_size,
    layout,
    hash,
    realloc_,
    free_
  );
  if( CC_UNLIKELY( !new_cntr ) )
    return cc_make_allocing_fn_result( cntr, NULL );

  if( !cc_map_is_placeholder( cntr ) )
    free_( cntr );

  return cc_make_allocing_fn_result( new_cntr, cc_dummy_true_ptr );
}

// Initializes a shallow copy of the source map.
// The capacity of the copy is the same as the capacity of the source map, unless the source map is empty, in which case
// the copy is a placeholder.
// Hence, this function does no rehashing.
// Returns a pointer to the copy, or NULL in the case of allocation failure.
// That return value is cast to bool in the corresponding API macro.
static inline void *cc_map_init_clone(
  void *src,
  size_t el_size,
  uint64_t layout,
  cc_realloc_fnptr_ty realloc_,
  CC_UNUSED( cc_free_fnptr_ty, free_ )
)
{
  if( cc_map_size( src ) == 0 ) // Also handles placeholder.
    return (void *)&cc_map_placeholder;

  size_t buckets_size = CC_BUCKET_SIZE( el_size, layout ) * cc_map_cap( src );
  size_t metadata_offset = sizeof( cc_map_hdr_ty ) + buckets_size + CC_PADDING( buckets_size, alignof( uint16_t ) );
  size_t allocation_size = metadata_offset + sizeof( uint16_t ) * ( cc_map_cap( src ) + 4 );

  cc_map_hdr_ty *new_cntr = (cc_map_hdr_ty*)realloc_( NULL, allocation_size );
  if( CC_UNLIKELY( !new_cntr ) )
    return NULL;

  memcpy( new_cntr, src, allocation_size );
  new_cntr->metadata = (uint16_t *)( (char *)new_cntr + metadata_offset );

  return new_cntr;
}

// Erases all key-element pairs, calling the destructors for the key and element types if necessary, without changing
// the map's capacity.
static inline void cc_map_clear(
  void *cntr,
  size_t el_size,
  uint64_t layout,
  cc_dtor_fnptr_ty el_dtor,
  cc_dtor_fnptr_ty key_dtor,
  CC_UNUSED( cc_free_fnptr_ty, free_ )
)
{
  if( cc_map_size( cntr ) == 0 ) // Also handles placeholder.
    return;

  for( size_t bucket = 0; bucket < cc_map_cap( cntr ); ++bucket )
    if( cc_map_hdr( cntr )->metadata[ bucket ] )
    {
      if( key_dtor )
        key_dtor( cc_map_key( cntr, bucket, el_size, layout ) );

      if( el_dtor )
        el_dtor( cc_map_el( cntr, bucket, el_size, layout ) );

      cc_map_hdr( cntr )->metadata[ bucket ] = CC_MAP_EMPTY;
    }

  cc_map_hdr( cntr )->size = 0;
}

// Clears the map and frees its memory if is not a placeholder.
static inline void cc_map_cleanup(
  void *cntr,
  size_t el_size,
  uint64_t layout,
  cc_dtor_fnptr_ty el_dtor,
  cc_dtor_fnptr_ty key_dtor,
  cc_free_fnptr_ty free_
)
{
  cc_map_clear( cntr, el_size, layout, key_dtor, el_dtor, NULL /* Dummy */ );

  if( !cc_map_is_placeholder( cntr ) )
    free_( cntr );
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*                                                        Set                                                         */
/*--------------------------------------------------------------------------------------------------------------------*/

// A set is implemented as a map where the key and element are combined into one space in memory.
// Hence, it reuses the functions for map, except:
// - The key offset inside the bucket is zero.
//   This is handled at the API-macro level via the cc_layout function and associated macros.
// - The element size passed into map functions is zero in order to avoid double memcpy-ing.

static inline size_t cc_set_size( void *cntr )
{
  return cc_map_size( cntr );
}

static inline size_t cc_set_cap( void *cntr )
{
  return cc_map_cap( cntr );
}

static inline cc_allocing_fn_result_ty cc_set_reserve(
  void *cntr,
  size_t n,
  CC_UNUSED( size_t, el_size ),
  uint64_t layout,
  cc_hash_fnptr_ty hash,
  double max_load,
  cc_realloc_fnptr_ty realloc_,
  cc_free_fnptr_ty free_
)
{
  return cc_map_reserve( cntr, n, 0 /* Zero element size */, layout, hash, max_load, realloc_, free_ );
}

static inline cc_allocing_fn_result_ty cc_set_insert(
  void *cntr,
  void *key,
  bool replace,
  uint64_t layout,
  cc_hash_fnptr_ty hash,
  cc_cmpr_fnptr_ty cmpr,
  double max_load,
  cc_dtor_fnptr_ty el_dtor,
  cc_realloc_fnptr_ty realloc_,
  cc_free_fnptr_ty free_
)
{
  return cc_map_insert(
    cntr,
    cntr,     // Dummy pointer for element as memcpy-ing to a NULL pointer is undefined behavior even when size is zero.
    key,
    replace,
    0,        // Zero element size.
    layout,
    hash,
    cmpr,
    max_load,
    el_dtor,
    NULL,     // Only one destructor.
    realloc_,
    free_
  );
}

static inline void *cc_set_get(
  void *cntr,
  void *key,
  CC_UNUSED( size_t, el_size ),
  uint64_t layout,
  cc_hash_fnptr_ty hash,
  cc_cmpr_fnptr_ty cmpr
)
{
  return cc_map_get( cntr, key, 0 /* Zero element size */, layout, hash, cmpr );
}

static inline void *cc_set_erase_itr(
  void *cntr,
  void *itr,
  CC_UNUSED( size_t, el_size ),
  uint64_t layout,
  cc_hash_fnptr_ty hash,
  cc_dtor_fnptr_ty el_dtor,
  CC_UNUSED( cc_dtor_fnptr_ty, key_dtor )
)
{
  return cc_map_erase_itr(
    cntr,
    itr,
    0,       // Zero element size.
    layout,
    hash,
    el_dtor,
    NULL     // Only one destructor.
  );
}

static inline void *cc_set_erase(
  void *cntr,
  void *key,
  CC_UNUSED( size_t, el_size ),
  uint64_t layout,
  cc_hash_fnptr_ty hash,
  cc_cmpr_fnptr_ty cmpr,
  cc_dtor_fnptr_ty el_dtor,
  CC_UNUSED( cc_dtor_fnptr_ty, key_dtor ),
  CC_UNUSED( cc_free_fnptr_ty, free_ )
)
{
  return cc_map_erase(
    cntr,
    key,
    0,       // Zero element size.
    layout,
    hash,
    cmpr,
    el_dtor,
    NULL,    // Only one destructor.
    NULL     // Dummy.
  );
}

static inline cc_allocing_fn_result_ty cc_set_shrink(
  void *cntr,
  CC_UNUSED( size_t, el_size ),
  uint64_t layout,
  cc_hash_fnptr_ty hash,
  double max_load,
  cc_realloc_fnptr_ty realloc_,
  cc_free_fnptr_ty free_
)
{
  return cc_map_shrink( cntr, 0 /* Zero element size */, layout, hash, max_load, realloc_, free_ );
}

static inline void *cc_set_init_clone(
  void *src,
  CC_UNUSED( size_t, el_size ),
  uint64_t layout,
  cc_realloc_fnptr_ty realloc_,
  CC_UNUSED( cc_free_fnptr_ty, free_ )
)
{
  return cc_map_init_clone( src, /* Zero element size */ 0, layout, realloc_, NULL /* Dummy */ );
}

static inline void cc_set_clear(
  void *cntr,
  CC_UNUSED( size_t, el_size ),
  uint64_t layout,
  cc_dtor_fnptr_ty el_dtor,
  CC_UNUSED( cc_dtor_fnptr_ty, key_dtor ),
  CC_UNUSED( cc_free_fnptr_ty, free_ )
)
{
  cc_map_clear( cntr, 0 /* Zero element size */, layout, el_dtor, NULL /* Only one destructor */, NULL /* Dummy */ );
}

static inline void cc_set_cleanup(
  void *cntr,
  CC_UNUSED( size_t, el_size ),
  uint64_t layout,
  void ( *el_dtor )( void * ),
  CC_UNUSED( cc_dtor_fnptr_ty, key_dtor ),
  void ( *free_ )( void * ))
{
  cc_map_cleanup( cntr, 0 /* Zero element size */, layout, el_dtor, NULL /* Only one destructor */, free_ );
}

static inline void *cc_set_r_end( void *cntr )
{
  return cc_map_r_end( cntr );
}

static inline void *cc_set_end(
  void *cntr,
  CC_UNUSED( size_t, el_size ),
  uint64_t layout
)
{
  return cc_map_end( cntr, /* Zero element size */ 0, layout );
}

static inline void *cc_set_first(
  void *cntr,
  CC_UNUSED( size_t, el_size ),
  uint64_t layout
)
{
  return cc_map_first( cntr, /* Zero element size */ 0, layout );
}

static inline void *cc_set_last(
  void *cntr,
  CC_UNUSED( size_t, el_size ),
  uint64_t layout
)
{
  return cc_map_last( cntr, /* Zero element size */ 0, layout );
}

static inline void *cc_set_prev(
  void *cntr,
  void *itr,
  CC_UNUSED( size_t, el_size ),
  uint64_t layout
)
{
  return cc_map_prev( cntr, itr, /* Zero element size */ 0, layout );
}

static inline void *cc_set_next(
  void *cntr,
  void *itr,
  CC_UNUSED( size_t, el_size ),
  uint64_t layout
)
{
  return cc_map_next( cntr, itr, /* Zero element size */ 0, layout );
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*                                                        API                                                         */
/*--------------------------------------------------------------------------------------------------------------------*/

#define cc_init( cntr )                                                                \
(                                                                                      \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                              \
  CC_STATIC_ASSERT(                                                                    \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ||                                                \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ||                                                \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ||                                                \
    CC_CNTR_ID( *(cntr) ) == CC_SET                                                    \
  ),                                                                                   \
  *(cntr) = (                                                                          \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ? (CC_TYPEOF_XP( *(cntr) ))&cc_vec_placeholder  : \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ? (CC_TYPEOF_XP( *(cntr) ))&cc_list_placeholder : \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ? (CC_TYPEOF_XP( *(cntr) ))&cc_map_placeholder  : \
                          /* CC_SET */ (CC_TYPEOF_XP( *(cntr) ))&cc_map_placeholder    \
  ),                                                                                   \
  (void)0                                                                              \
)                                                                                      \

#define cc_size( cntr )                               \
(                                                     \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),             \
  CC_STATIC_ASSERT(                                   \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ||               \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ||               \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ||               \
    CC_CNTR_ID( *(cntr) ) == CC_SET                   \
  ),                                                  \
  /* Function select */                               \
  (                                                   \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ? cc_vec_size  : \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ? cc_list_size : \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ? cc_map_size  : \
                          /* CC_SET */ cc_set_size    \
  )                                                   \
  /* Function arguments */                            \
  (                                                   \
    *(cntr)                                           \
  )                                                   \
)                                                     \

#define cc_cap( cntr )                              \
(                                                   \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),           \
  CC_STATIC_ASSERT(                                 \
    CC_CNTR_ID( *(cntr) ) == CC_VEC ||              \
    CC_CNTR_ID( *(cntr) ) == CC_MAP ||              \
    CC_CNTR_ID( *(cntr) ) == CC_SET                 \
  ),                                                \
  /* Function select */                             \
  (                                                 \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ? cc_vec_cap : \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ? cc_map_cap : \
                          /* CC_SET */ cc_set_cap   \
  )                                                 \
  /* Function arguments */                          \
  (                                                 \
    *(cntr)                                         \
  )                                                 \
)                                                   \

#define cc_reserve( cntr, n )                                                                \
(                                                                                            \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                                    \
  CC_STATIC_ASSERT(                                                                          \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ||                                                      \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ||                                                      \
    CC_CNTR_ID( *(cntr) ) == CC_SET                                                          \
  ),                                                                                         \
  CC_POINT_HNDL_TO_ALLOCING_FN_RESULT(                                                       \
    *(cntr),                                                                                 \
    /* Function select */                                                                    \
    (                                                                                        \
      CC_CNTR_ID( *(cntr) ) == CC_VEC  ? cc_vec_reserve :                                    \
      CC_CNTR_ID( *(cntr) ) == CC_MAP  ? cc_map_reserve :                                    \
                            /* CC_SET */ cc_set_reserve                                      \
    )                                                                                        \
    /* Function arguments */                                                                 \
    (                                                                                        \
      *(cntr),                                                                               \
      n,                                                                                     \
      CC_EL_SIZE( *(cntr) ),                                                                 \
      CC_LAYOUT( *(cntr) ),                                                                  \
      CC_KEY_HASH( *(cntr) ),                                                                \
      CC_KEY_LOAD( *(cntr) ),                                                                \
      CC_REALLOC_FN,                                                                         \
      CC_FREE_FN                                                                             \
    )                                                                                        \
  ),                                                                                         \
  CC_CAST_MAYBE_UNUSED( CC_EL_TY( *(cntr) ) *, CC_FIX_HNDL_AND_RETURN_OTHER_PTR( *(cntr) ) ) \
)                                                                                            \

#define cc_insert( ... ) CC_SELECT_ON_NUM_ARGS( cc_insert, __VA_ARGS__ )

#define cc_insert_2( cntr, key )                                                             \
(                                                                                            \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                                    \
  CC_STATIC_ASSERT( CC_CNTR_ID( *(cntr) ) == CC_SET ),                                       \
  CC_POINT_HNDL_TO_ALLOCING_FN_RESULT(                                                       \
    *(cntr),                                                                                 \
    cc_set_insert(                                                                           \
      *(cntr),                                                                               \
      &CC_MAKE_LVAL_COPY( CC_KEY_TY( *(cntr) ), (key) ),                                     \
      true,                                                                                  \
      CC_LAYOUT( *(cntr) ),                                                                  \
      CC_KEY_HASH( *(cntr) ),                                                                \
      CC_KEY_CMPR( *(cntr) ),                                                                \
      CC_KEY_LOAD( *(cntr) ),                                                                \
      CC_EL_DTOR( *(cntr) ),                                                                 \
      CC_REALLOC_FN,                                                                         \
      CC_FREE_FN                                                                             \
    )                                                                                        \
  ),                                                                                         \
  CC_CAST_MAYBE_UNUSED( CC_EL_TY( *(cntr) ) *, CC_FIX_HNDL_AND_RETURN_OTHER_PTR( *(cntr) ) ) \
)                                                                                            \

#define cc_insert_3( cntr, key, el )                                                         \
(                                                                                            \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                                    \
  CC_STATIC_ASSERT(                                                                          \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ||                                                      \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ||                                                      \
    CC_CNTR_ID( *(cntr) ) == CC_MAP                                                          \
  ),                                                                                         \
  CC_POINT_HNDL_TO_ALLOCING_FN_RESULT(                                                       \
    *(cntr),                                                                                 \
    /* Function select */                                                                    \
    (                                                                                        \
      CC_CNTR_ID( *(cntr) ) == CC_VEC  ? cc_vec_insert  :                                    \
      CC_CNTR_ID( *(cntr) ) == CC_LIST ? cc_list_insert :                                    \
                            /* CC_MAP */ cc_map_insert                                       \
    )                                                                                        \
    /* Function arguments */                                                                 \
    (                                                                                        \
      *(cntr),                                                                               \
      &CC_MAKE_LVAL_COPY( CC_EL_TY( *(cntr) ), (el) ),                                       \
      &CC_MAKE_LVAL_COPY( CC_KEY_TY( *(cntr) ), (key) ),                                     \
      true,                                                                                  \
      CC_EL_SIZE( *(cntr) ),                                                                 \
      CC_LAYOUT( *(cntr) ),                                                                  \
      CC_KEY_HASH( *(cntr) ),                                                                \
      CC_KEY_CMPR( *(cntr) ),                                                                \
      CC_KEY_LOAD( *(cntr) ),                                                                \
      CC_EL_DTOR( *(cntr) ),                                                                 \
      CC_KEY_DTOR( *(cntr) ),                                                                \
      CC_REALLOC_FN,                                                                         \
      CC_FREE_FN                                                                             \
    )                                                                                        \
  ),                                                                                         \
  CC_CAST_MAYBE_UNUSED( CC_EL_TY( *(cntr) ) *, CC_FIX_HNDL_AND_RETURN_OTHER_PTR( *(cntr) ) ) \
)                                                                                            \

#define cc_insert_n( cntr, index, els, n )                                                   \
(                                                                                            \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                                    \
  CC_STATIC_ASSERT( CC_CNTR_ID( *(cntr) ) == CC_VEC ),                                       \
  CC_POINT_HNDL_TO_ALLOCING_FN_RESULT(                                                       \
    *(cntr),                                                                                 \
    cc_vec_insert_n( *(cntr), (index), (els), (n), CC_EL_SIZE( *(cntr) ), CC_REALLOC_FN )    \
  ),                                                                                         \
  CC_CAST_MAYBE_UNUSED( CC_EL_TY( *(cntr) ) *, CC_FIX_HNDL_AND_RETURN_OTHER_PTR( *(cntr) ) ) \
)                                                                                            \

#define cc_push( cntr, el )                                                                  \
(                                                                                            \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                                    \
  CC_STATIC_ASSERT(                                                                          \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ||                                                      \
    CC_CNTR_ID( *(cntr) ) == CC_LIST                                                         \
  ),                                                                                         \
  CC_POINT_HNDL_TO_ALLOCING_FN_RESULT(                                                       \
    *(cntr),                                                                                 \
    /* Function select */                                                                    \
    (                                                                                        \
      CC_CNTR_ID( *(cntr) ) == CC_VEC  ?  cc_vec_push  :                                     \
                            /* CC_LIST */ cc_list_push                                       \
    )                                                                                        \
    /* Function arguments */                                                                 \
    (                                                                                        \
      *(cntr),                                                                               \
      &CC_MAKE_LVAL_COPY( CC_EL_TY( *(cntr) ), (el) ),                                       \
      CC_EL_SIZE( *(cntr) ),                                                                 \
      CC_REALLOC_FN                                                                          \
    )                                                                                        \
  ),                                                                                         \
  CC_CAST_MAYBE_UNUSED( CC_EL_TY( *(cntr) ) *, CC_FIX_HNDL_AND_RETURN_OTHER_PTR( *(cntr) ) ) \
)                                                                                            \

#define cc_push_n( cntr, els, n )                                                            \
(                                                                                            \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                                    \
  CC_STATIC_ASSERT( CC_CNTR_ID( *(cntr) ) == CC_VEC ),                                       \
  CC_POINT_HNDL_TO_ALLOCING_FN_RESULT(                                                       \
    *(cntr),                                                                                 \
    cc_vec_push_n( *(cntr), (els), (n), CC_EL_SIZE( *(cntr) ), CC_REALLOC_FN )               \
  ),                                                                                         \
  CC_CAST_MAYBE_UNUSED( CC_EL_TY( *(cntr) ) *, CC_FIX_HNDL_AND_RETURN_OTHER_PTR( *(cntr) ) ) \
)                                                                                            \

#define cc_get_or_insert( ... ) CC_SELECT_ON_NUM_ARGS( cc_get_or_insert, __VA_ARGS__ )

#define cc_get_or_insert_2( cntr, key )                                                      \
(                                                                                            \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                                    \
  CC_STATIC_ASSERT( CC_CNTR_ID( *(cntr) ) == CC_SET ),                                       \
  CC_POINT_HNDL_TO_ALLOCING_FN_RESULT(                                                       \
    *(cntr),                                                                                 \
    cc_set_insert(                                                                           \
      *(cntr),                                                                               \
      &CC_MAKE_LVAL_COPY( CC_KEY_TY( *(cntr) ), (key) ),                                     \
      false,                                                                                 \
      CC_LAYOUT( *(cntr) ),                                                                  \
      CC_KEY_HASH( *(cntr) ),                                                                \
      CC_KEY_CMPR( *(cntr) ),                                                                \
      CC_KEY_LOAD( *(cntr) ),                                                                \
      CC_EL_DTOR( *(cntr) ),                                                                 \
      CC_REALLOC_FN,                                                                         \
      CC_FREE_FN                                                                             \
    )                                                                                        \
  ),                                                                                         \
  CC_CAST_MAYBE_UNUSED( CC_EL_TY( *(cntr) ) *, CC_FIX_HNDL_AND_RETURN_OTHER_PTR( *(cntr) ) ) \
)                                                                                            \

#define cc_get_or_insert_3( cntr, key, el )                                                  \
(                                                                                            \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                                    \
  CC_STATIC_ASSERT( CC_CNTR_ID( *(cntr) ) == CC_MAP ),                                       \
  CC_POINT_HNDL_TO_ALLOCING_FN_RESULT(                                                       \
    *(cntr),                                                                                 \
    cc_map_insert(                                                                           \
      *(cntr),                                                                               \
      &CC_MAKE_LVAL_COPY( CC_EL_TY( *(cntr) ), (el) ),                                       \
      &CC_MAKE_LVAL_COPY( CC_KEY_TY( *(cntr) ), (key) ),                                     \
      false,                                                                                 \
      CC_EL_SIZE( *(cntr) ),                                                                 \
      CC_LAYOUT( *(cntr) ),                                                                  \
      CC_KEY_HASH( *(cntr) ),                                                                \
      CC_KEY_CMPR( *(cntr) ),                                                                \
      CC_KEY_LOAD( *(cntr) ),                                                                \
      CC_EL_DTOR( *(cntr) ),                                                                 \
      CC_KEY_DTOR( *(cntr) ),                                                                \
      CC_REALLOC_FN,                                                                         \
      CC_FREE_FN                                                                             \
    )                                                                                        \
  ),                                                                                         \
  CC_CAST_MAYBE_UNUSED( CC_EL_TY( *(cntr) ) *, CC_FIX_HNDL_AND_RETURN_OTHER_PTR( *(cntr) ) ) \
)                                                                                            \

#define cc_get( cntr, key )                              \
(                                                        \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                \
  CC_STATIC_ASSERT(                                      \
    CC_CNTR_ID( *(cntr) ) == CC_VEC ||                   \
    CC_CNTR_ID( *(cntr) ) == CC_MAP ||                   \
    CC_CNTR_ID( *(cntr) ) == CC_SET                      \
  ),                                                     \
  CC_CAST_MAYBE_UNUSED(                                  \
    CC_EL_TY( *(cntr) ) *,                               \
    /* Function select */                                \
    (                                                    \
      CC_CNTR_ID( *(cntr) ) == CC_VEC  ? cc_vec_get :    \
      CC_CNTR_ID( *(cntr) ) == CC_MAP  ? cc_map_get :    \
                            /* CC_SET */ cc_set_get      \
    )                                                    \
    /* Function arguments */                             \
    (                                                    \
      *(cntr),                                           \
      &CC_MAKE_LVAL_COPY( CC_KEY_TY( *(cntr) ), (key) ), \
      CC_EL_SIZE( *(cntr) ),                             \
      CC_LAYOUT( *(cntr) ),                              \
      CC_KEY_HASH( *(cntr) ),                            \
      CC_KEY_CMPR( *(cntr) )                             \
    )                                                    \
  )                                                      \
)                                                        \

#define cc_key_for( cntr, itr )                                          \
(                                                                        \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                \
  CC_STATIC_ASSERT( CC_CNTR_ID( *(cntr) ) == CC_MAP ),                   \
  CC_CAST_MAYBE_UNUSED(                                                  \
    const CC_KEY_TY( *(cntr) ) *,                                        \
    cc_map_key_for( (itr), CC_EL_SIZE( *(cntr) ), CC_LAYOUT( *(cntr) ) ) \
  )                                                                      \
)                                                                        \

#define cc_erase( cntr, key )                                           \
(                                                                       \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                               \
  CC_STATIC_ASSERT(                                                     \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ||                                 \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ||                                 \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ||                                 \
    CC_CNTR_ID( *(cntr) ) == CC_SET                                     \
  ),                                                                    \
  CC_IF_THEN_CAST_TY_1_ELSE_CAST_TY_2(                                  \
    CC_CNTR_ID( *(cntr) ) == CC_MAP || CC_CNTR_ID( *(cntr) ) == CC_SET, \
    bool,                                                               \
    CC_EL_TY( *(cntr) ) *,                                              \
    /* Function select */                                               \
    (                                                                   \
      CC_CNTR_ID( *(cntr) ) == CC_VEC  ? cc_vec_erase  :                \
      CC_CNTR_ID( *(cntr) ) == CC_LIST ? cc_list_erase :                \
      CC_CNTR_ID( *(cntr) ) == CC_MAP  ? cc_map_erase  :                \
                            /* CC_SET */ cc_set_erase                   \
    )                                                                   \
    /* Function arguments */                                            \
    (                                                                   \
      *(cntr),                                                          \
      &CC_MAKE_LVAL_COPY( CC_KEY_TY( *(cntr) ), (key) ),                \
      CC_EL_SIZE( *(cntr) ),                                            \
      CC_LAYOUT( *(cntr) ),                                             \
      CC_KEY_HASH( *(cntr) ),                                           \
      CC_KEY_CMPR( *(cntr) ),                                           \
      CC_EL_DTOR( *(cntr) ),                                            \
      CC_KEY_DTOR( *(cntr) ),                                           \
      CC_FREE_FN                                                        \
    )                                                                   \
  )                                                                     \
)                                                                       \

#define cc_erase_n( cntr, index, n )                                                      \
(                                                                                         \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                                 \
  CC_STATIC_ASSERT( CC_CNTR_ID( *(cntr) ) == CC_VEC ),                                    \
  CC_CAST_MAYBE_UNUSED(                                                                   \
    CC_EL_TY( *(cntr) ) *,                                                                \
    cc_vec_erase_n( *(cntr), (index), (n), CC_EL_SIZE( *(cntr) ), CC_EL_DTOR( *(cntr) ) ) \
  )                                                                                       \
)                                                                                         \

#define cc_erase_itr( cntr, itr )                         \
(                                                         \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                 \
  CC_STATIC_ASSERT(                                       \
    CC_CNTR_ID( *(cntr) ) == CC_MAP ||                    \
    CC_CNTR_ID( *(cntr) ) == CC_SET                       \
  ),                                                      \
  /* Function select */                                   \
  (                                                       \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ? cc_map_erase_itr : \
                          /* CC_SET */ cc_set_erase_itr   \
  )                                                       \
  /* Function arguments */                                \
  (                                                       \
    *(cntr),                                              \
    (itr),                                                \
    CC_EL_SIZE( *(cntr) ),                                \
    CC_LAYOUT( *(cntr) ),                                 \
    CC_KEY_HASH( *(cntr) ),                               \
    CC_EL_DTOR( *(cntr) ),                                \
    CC_KEY_DTOR( *(cntr) )                                \
  )                                                       \
)                                                         \

#define cc_splice( cntr, itr, src, src_itr )                                \
(                                                                           \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                   \
  CC_STATIC_ASSERT( CC_CNTR_ID( *(cntr) ) == CC_LIST ),                     \
  CC_STATIC_ASSERT( CC_IS_SAME_TY( (cntr), (src) ) ),                       \
  CC_POINT_HNDL_TO_ALLOCING_FN_RESULT(                                      \
    *(cntr),                                                                \
    cc_list_splice( *(cntr), (itr), *(src), src_itr, CC_REALLOC_FN )        \
  ),                                                                        \
  CC_CAST_MAYBE_UNUSED( bool, CC_FIX_HNDL_AND_RETURN_OTHER_PTR( *(cntr) ) ) \
)                                                                           \

#define cc_resize( cntr, n )                                                                   \
(                                                                                              \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                                      \
  CC_STATIC_ASSERT( CC_CNTR_ID( *(cntr) ) == CC_VEC ),                                         \
  CC_POINT_HNDL_TO_ALLOCING_FN_RESULT(                                                         \
    *(cntr),                                                                                   \
    cc_vec_resize( *(cntr), (n), CC_EL_SIZE( *(cntr) ), CC_EL_DTOR( *(cntr) ), CC_REALLOC_FN ) \
  ),                                                                                           \
  CC_CAST_MAYBE_UNUSED( bool, CC_FIX_HNDL_AND_RETURN_OTHER_PTR( *(cntr) ) )                    \
)                                                                                              \

#define cc_shrink( cntr )                                                   \
(                                                                           \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                   \
  CC_STATIC_ASSERT(                                                         \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ||                                     \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ||                                     \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ||                                     \
    CC_CNTR_ID( *(cntr) ) == CC_SET                                         \
  ),                                                                        \
  CC_POINT_HNDL_TO_ALLOCING_FN_RESULT(                                      \
    *(cntr),                                                                \
    /* Function select */                                                   \
    (                                                                       \
      CC_CNTR_ID( *(cntr) ) == CC_VEC  ? cc_vec_shrink :                    \
      CC_CNTR_ID( *(cntr) ) == CC_MAP  ? cc_map_shrink :                    \
                            /* CC_SET */ cc_set_shrink                      \
    )                                                                       \
    /* Function arguments */                                                \
    (                                                                       \
      *(cntr),                                                              \
      CC_EL_SIZE( *(cntr) ),                                                \
      CC_LAYOUT( *(cntr) ),                                                 \
      CC_KEY_HASH( *(cntr) ),                                               \
      CC_KEY_LOAD( *(cntr) ),                                               \
      CC_REALLOC_FN,                                                        \
      CC_FREE_FN                                                            \
    )                                                                       \
  ),                                                                        \
  CC_CAST_MAYBE_UNUSED( bool, CC_FIX_HNDL_AND_RETURN_OTHER_PTR( *(cntr) ) ) \
)                                                                           \

#define cc_init_clone( cntr, src )                            \
(                                                             \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                     \
  CC_STATIC_ASSERT(                                           \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ||                       \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ||                       \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ||                       \
    CC_CNTR_ID( *(cntr) ) == CC_SET                           \
  ),                                                          \
  CC_STATIC_ASSERT( CC_IS_SAME_TY( *(cntr), *(src) ) ),       \
  CC_CAST_MAYBE_UNUSED(                                       \
    bool,                                                     \
    *(cntr) = (CC_TYPEOF_XP( *(cntr) ))                       \
    /* Function select */                                     \
    (                                                         \
      CC_CNTR_ID( *(cntr) ) == CC_VEC  ? cc_vec_init_clone  : \
      CC_CNTR_ID( *(cntr) ) == CC_LIST ? cc_list_init_clone : \
      CC_CNTR_ID( *(cntr) ) == CC_MAP  ? cc_map_init_clone  : \
                            /* CC_SET */ cc_set_init_clone    \
    )                                                         \
    /* Function arguments */                                  \
    (                                                         \
      *(src),                                                 \
      CC_EL_SIZE( *(cntr) ),                                  \
      CC_LAYOUT( *(cntr) ),                                   \
      CC_REALLOC_FN,                                          \
      CC_FREE_FN                                              \
    )                                                         \
  )                                                           \
)                                                             \

#define cc_clear( cntr )                               \
(                                                      \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),              \
  CC_STATIC_ASSERT(                                    \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ||                \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ||                \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ||                \
    CC_CNTR_ID( *(cntr) ) == CC_SET                    \
  ),                                                   \
  /* Function select */                                \
  (                                                    \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ? cc_vec_clear  : \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ? cc_list_clear : \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ? cc_map_clear  : \
                          /* CC_SET */ cc_set_clear    \
  )                                                    \
  /* Function arguments */                             \
  (                                                    \
    *(cntr),                                           \
    CC_EL_SIZE( *(cntr) ),                             \
    CC_LAYOUT( *(cntr) ),                              \
    CC_EL_DTOR( *(cntr) ),                             \
    CC_KEY_DTOR( *(cntr) ),                            \
    CC_FREE_FN                                         \
  )                                                    \
)                                                      \

#define cc_cleanup( cntr )                               \
(                                                        \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                \
  CC_STATIC_ASSERT(                                      \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ||                  \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ||                  \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ||                  \
    CC_CNTR_ID( *(cntr) ) == CC_SET                      \
  ),                                                     \
  /* Function select */                                  \
  (                                                      \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ? cc_vec_cleanup  : \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ? cc_list_cleanup : \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ? cc_map_cleanup  : \
                          /* CC_SET */ cc_set_cleanup    \
  )                                                      \
  /* Function arguments */                               \
  (                                                      \
    *(cntr),                                             \
    CC_EL_SIZE( *(cntr) ),                               \
    CC_LAYOUT( *(cntr) ),                                \
    CC_EL_DTOR( *(cntr) ),                               \
    CC_KEY_DTOR( *(cntr) ),                              \
    CC_FREE_FN                                           \
  ),                                                     \
  cc_init( cntr )                                        \
)                                                        \

#define cc_r_end( cntr )                                 \
(                                                        \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                \
  CC_STATIC_ASSERT(                                      \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ||                  \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ||                  \
    CC_CNTR_ID( *(cntr) ) == CC_SET                      \
  ),                                                     \
  CC_CAST_MAYBE_UNUSED(                                  \
    CC_EL_TY( *(cntr) ) *,                               \
    /* Function select */                                \
    (                                                    \
      CC_CNTR_ID( *(cntr) ) == CC_LIST ? cc_list_r_end : \
      CC_CNTR_ID( *(cntr) ) == CC_MAP  ? cc_map_r_end  : \
                            /* CC_SET */ cc_set_r_end    \
    )                                                    \
    /* Function arguments */                             \
    (                                                    \
      *(cntr)                                            \
    )                                                    \
  )                                                      \
)                                                        \

#define cc_end( cntr )                                 \
(                                                      \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),              \
  CC_STATIC_ASSERT(                                    \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ||                \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ||                \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ||                \
    CC_CNTR_ID( *(cntr) ) == CC_SET                    \
  ),                                                   \
  CC_CAST_MAYBE_UNUSED(                                \
    CC_EL_TY( *(cntr) ) *,                             \
    /* Function select */                              \
    (                                                  \
      CC_CNTR_ID( *(cntr) ) == CC_VEC  ? cc_vec_end  : \
      CC_CNTR_ID( *(cntr) ) == CC_LIST ? cc_list_end : \
      CC_CNTR_ID( *(cntr) ) == CC_MAP  ? cc_map_end  : \
                            /* CC_SET */ cc_set_end    \
    )                                                  \
    /* Function arguments */                           \
    (                                                  \
      *(cntr),                                         \
      CC_EL_SIZE( *(cntr) ),                           \
      CC_LAYOUT( *(cntr) )                             \
    )                                                  \
  )                                                    \
)                                                      \

#define cc_first( cntr )                                 \
(                                                        \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                \
  CC_STATIC_ASSERT(                                      \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ||                  \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ||                  \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ||                  \
    CC_CNTR_ID( *(cntr) ) == CC_SET                      \
  ),                                                     \
  CC_CAST_MAYBE_UNUSED(                                  \
    CC_EL_TY( *(cntr) ) *,                               \
    /* Function select */                                \
    (                                                    \
      CC_CNTR_ID( *(cntr) ) == CC_VEC  ? cc_vec_first  : \
      CC_CNTR_ID( *(cntr) ) == CC_LIST ? cc_list_first : \
      CC_CNTR_ID( *(cntr) ) == CC_MAP  ? cc_map_first  : \
                            /* CC_SET */ cc_set_first    \
    )                                                    \
    /* Function arguments */                             \
    (                                                    \
      *(cntr),                                           \
      CC_EL_SIZE( *(cntr) ),                             \
      CC_LAYOUT( *(cntr) )                               \
    )                                                    \
  )                                                      \
)                                                        \

#define cc_last( cntr )                                 \
(                                                       \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),               \
  CC_STATIC_ASSERT(                                     \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ||                 \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ||                 \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ||                 \
    CC_CNTR_ID( *(cntr) ) == CC_SET                     \
  ),                                                    \
  CC_CAST_MAYBE_UNUSED(                                 \
    CC_EL_TY( *(cntr) ) *,                              \
    /* Function select */                               \
    (                                                   \
      CC_CNTR_ID( *(cntr) ) == CC_VEC  ? cc_vec_last  : \
      CC_CNTR_ID( *(cntr) ) == CC_LIST ? cc_list_last : \
      CC_CNTR_ID( *(cntr) ) == CC_MAP  ? cc_map_last  : \
                            /* CC_SET */ cc_set_last    \
    )                                                   \
    /* Function arguments */                            \
    (                                                   \
      *(cntr),                                          \
      CC_EL_SIZE( *(cntr) ),                            \
      CC_LAYOUT( *(cntr) )                              \
    )                                                   \
  )                                                     \
)                                                       \

#define cc_next( cntr, itr )                            \
(                                                       \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),               \
  CC_STATIC_ASSERT(                                     \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ||                 \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ||                 \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ||                 \
    CC_CNTR_ID( *(cntr) ) == CC_SET                     \
  ),                                                    \
  CC_CAST_MAYBE_UNUSED(                                 \
    CC_EL_TY( *(cntr) ) *,                              \
    /* Function select */                               \
    (                                                   \
      CC_CNTR_ID( *(cntr) ) == CC_VEC  ? cc_vec_next  : \
      CC_CNTR_ID( *(cntr) ) == CC_LIST ? cc_list_next : \
      CC_CNTR_ID( *(cntr) ) == CC_MAP  ? cc_map_next  : \
                            /* CC_SET */ cc_set_next    \
    )                                                   \
    /* Function arguments */                            \
    (                                                   \
      *(cntr),                                          \
      (itr),                                            \
      CC_EL_SIZE( *(cntr) ),                            \
      CC_LAYOUT( *(cntr) )                              \
    )                                                   \
  )                                                     \
)                                                       \

#define cc_prev( cntr, itr )                            \
(                                                       \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),               \
  CC_STATIC_ASSERT(                                     \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ||                 \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ||                 \
    CC_CNTR_ID( *(cntr) ) == CC_SET                     \
  ),                                                    \
  CC_CAST_MAYBE_UNUSED(                                 \
    CC_EL_TY( *(cntr) ) *,                              \
    /* Function select */                               \
    (                                                   \
      CC_CNTR_ID( *(cntr) ) == CC_LIST ? cc_list_prev : \
      CC_CNTR_ID( *(cntr) ) == CC_MAP  ? cc_map_prev  : \
                            /* CC_SET */ cc_set_prev    \
    )                                                   \
    /* Function arguments */                            \
    (                                                   \
      *(cntr),                                          \
      (itr),                                            \
      CC_EL_SIZE( *(cntr) ),                            \
      CC_LAYOUT( *(cntr) )                              \
    )                                                   \
  )                                                     \
)                                                       \

#define cc_for_each( ... ) CC_SELECT_ON_NUM_ARGS( cc_for_each, __VA_ARGS__ )

#define cc_for_each_2( cntr, i_name )                                                                                 \
  for( CC_EL_TY( *(cntr) ) *i_name = cc_first( cntr ); i_name != cc_end( cntr ); i_name = cc_next( (cntr), i_name ) ) \

#define cc_for_each_3( cntr, key_ptr_name, i_name )                                                                   \
  for( CC_EL_TY( *(cntr) ) *i_name = cc_first( cntr ); i_name != cc_end( cntr ); i_name = cc_next( (cntr), i_name ) ) \
    for( const CC_KEY_TY( *(cntr) ) *key_ptr_name = cc_key_for( (cntr), i_name ); key_ptr_name; key_ptr_name = NULL ) \

#define cc_r_for_each( ... ) CC_SELECT_ON_NUM_ARGS( cc_r_for_each, __VA_ARGS__ )

#define cc_r_for_each_2( cntr, i_name )                                                                                \
  for( CC_EL_TY( *(cntr) ) *i_name = cc_last( cntr ); i_name != cc_r_end( cntr ); i_name = cc_prev( (cntr), i_name ) ) \

#define cc_r_for_each_3( cntr, key_ptr_name, i_name )                                                                  \
  for( CC_EL_TY( *(cntr) ) *i_name = cc_last( cntr ); i_name != cc_r_end( cntr ); i_name = cc_prev( (cntr), i_name ) ) \
    for( const CC_KEY_TY( *(cntr) ) *key_ptr_name = cc_key_for( (cntr), i ); key_ptr_name; key_ptr_name = NULL )       \

/*--------------------------------------------------------------------------------------------------------------------*/
/*                         Destructor, comparison, and hash functions and custom load factors                         */
/*--------------------------------------------------------------------------------------------------------------------*/

// Octal counters that support up to 511 of each function type and 511 load factors.
#define CC_N_DTORS_D1 0 // D1 = digit 1, i.e. the least significant digit.
#define CC_N_DTORS_D2 0
#define CC_N_DTORS_D3 0
#define CC_N_CMPRS_D1 0
#define CC_N_CMPRS_D2 0
#define CC_N_CMPRS_D3 0
#define CC_N_HASHS_D1 0
#define CC_N_HASHS_D2 0
#define CC_N_HASHS_D3 0
#define CC_N_LOADS_D1 0
#define CC_N_LOADS_D2 0
#define CC_N_LOADS_D3 0

#define CC_CAT_3_( a, b, c ) a##b##c
#define CC_CAT_3( a, b, c ) CC_CAT_3_( a, b, c )
#define CC_CAT_4_( a, b, c, d ) a##b##c##d
#define CC_CAT_4( a, b, c, d ) CC_CAT_4_( a, b, c, d )

// Macros that provide the current value of each counter as a three-digit octal number preceded by 0.
// These numbers are used to form unique type and function names to plug into CC_EL_DTOR, CC_KEY_DTOR, CC_KEY_CMPR, etc.
#define CC_N_DTORS CC_CAT_4( 0, CC_N_DTORS_D3, CC_N_DTORS_D2, CC_N_DTORS_D1 )
#define CC_N_CMPRS CC_CAT_4( 0, CC_N_CMPRS_D3, CC_N_CMPRS_D2, CC_N_CMPRS_D1 )
#define CC_N_HASHS CC_CAT_4( 0, CC_N_HASHS_D3, CC_N_HASHS_D2, CC_N_HASHS_D1 )
#define CC_N_LOADS CC_CAT_4( 0, CC_N_LOADS_D3, CC_N_LOADS_D2, CC_N_LOADS_D1 )

// CC_FOR_EACH_XXX macros that call macro m with the first argument n, where n = [0, counter XXX ),
// and the second argument arg.
// These are used to construct _Generic or ternary expressions that select the correct function (or load factor) for a
// container's key or element type (see below).

#define CC_CALL_MACRO( m, arg, n ) m( n, arg )
#define CC_R1_0( m, arg, d3, d2 )
#define CC_R1_1( m, arg, d3, d2 ) CC_CALL_MACRO( m, arg, CC_CAT_4( 0, d3, d2, 0 ) )
#define CC_R1_2( m, arg, d3, d2 ) CC_CALL_MACRO( m, arg, CC_CAT_4( 0, d3, d2, 1 ) ) CC_R1_1( m, arg, d3, d2 )
#define CC_R1_3( m, arg, d3, d2 ) CC_CALL_MACRO( m, arg, CC_CAT_4( 0, d3, d2, 2 ) ) CC_R1_2( m, arg, d3, d2 )
#define CC_R1_4( m, arg, d3, d2 ) CC_CALL_MACRO( m, arg, CC_CAT_4( 0, d3, d2, 3 ) ) CC_R1_3( m, arg, d3, d2 )
#define CC_R1_5( m, arg, d3, d2 ) CC_CALL_MACRO( m, arg, CC_CAT_4( 0, d3, d2, 4 ) ) CC_R1_4( m, arg, d3, d2 )
#define CC_R1_6( m, arg, d3, d2 ) CC_CALL_MACRO( m, arg, CC_CAT_4( 0, d3, d2, 5 ) ) CC_R1_5( m, arg, d3, d2 )
#define CC_R1_7( m, arg, d3, d2 ) CC_CALL_MACRO( m, arg, CC_CAT_4( 0, d3, d2, 6 ) ) CC_R1_6( m, arg, d3, d2 )
#define CC_R1_8( m, arg, d3, d2 ) CC_CALL_MACRO( m, arg, CC_CAT_4( 0, d3, d2, 7 ) ) CC_R1_7( m, arg, d3, d2 )
#define CC_R2_0( m, arg, d3 )
#define CC_R2_1( m, arg, d3 ) CC_R1_8( m, arg, d3, 0 )
#define CC_R2_2( m, arg, d3 ) CC_R1_8( m, arg, d3, 1 ) CC_R2_1( m, arg, d3 )
#define CC_R2_3( m, arg, d3 ) CC_R1_8( m, arg, d3, 2 ) CC_R2_2( m, arg, d3 )
#define CC_R2_4( m, arg, d3 ) CC_R1_8( m, arg, d3, 3 ) CC_R2_3( m, arg, d3 )
#define CC_R2_5( m, arg, d3 ) CC_R1_8( m, arg, d3, 4 ) CC_R2_4( m, arg, d3 )
#define CC_R2_6( m, arg, d3 ) CC_R1_8( m, arg, d3, 5 ) CC_R2_5( m, arg, d3 )
#define CC_R2_7( m, arg, d3 ) CC_R1_8( m, arg, d3, 6 ) CC_R2_6( m, arg, d3 )
#define CC_R2_8( m, arg, d3 ) CC_R1_8( m, arg, d3, 7 ) CC_R2_7( m, arg, d3 )
#define CC_R3_0( m, arg )
#define CC_R3_1( m, arg ) CC_R2_8( m, arg, 0 )
#define CC_R3_2( m, arg ) CC_R2_8( m, arg, 1 ) CC_R3_1( m, arg )
#define CC_R3_3( m, arg ) CC_R2_8( m, arg, 2 ) CC_R3_2( m, arg )
#define CC_R3_4( m, arg ) CC_R2_8( m, arg, 3 ) CC_R3_3( m, arg )
#define CC_R3_5( m, arg ) CC_R2_8( m, arg, 4 ) CC_R3_4( m, arg )
#define CC_R3_6( m, arg ) CC_R2_8( m, arg, 5 ) CC_R3_5( m, arg )
#define CC_R3_7( m, arg ) CC_R2_8( m, arg, 6 ) CC_R3_6( m, arg )

#define CC_FOR_OCT_COUNT( m, arg, d3, d2, d1 ) \
CC_CAT_2( CC_R1_, d1 )( m, arg, d3, d2 )       \
CC_CAT_2( CC_R2_, d2 )( m, arg, d3 )           \
CC_CAT_2( CC_R3_, d3 )( m, arg )               \

#define CC_FOR_EACH_DTOR( m, arg ) CC_FOR_OCT_COUNT( m, arg, CC_N_DTORS_D3, CC_N_DTORS_D2, CC_N_DTORS_D1 )
#define CC_FOR_EACH_CMPR( m, arg ) CC_FOR_OCT_COUNT( m, arg, CC_N_CMPRS_D3, CC_N_CMPRS_D2, CC_N_CMPRS_D1 )
#define CC_FOR_EACH_HASH( m, arg ) CC_FOR_OCT_COUNT( m, arg, CC_N_HASHS_D3, CC_N_HASHS_D2, CC_N_HASHS_D1 )
#define CC_FOR_EACH_LOAD( m, arg ) CC_FOR_OCT_COUNT( m, arg, CC_N_LOADS_D3, CC_N_LOADS_D2, CC_N_LOADS_D1 )

// Macros for inferring the destructor, comparison, or hash function or load factor associated with a container's
// key or element type, as well as for determining whether a comparison or hash function exists for a type and inferring
// certain map function arguments in bulk (argument packs) from the key type.
// In C, we use the CC_FOR_EACH_XXXX macros above to create _Generic expressions that select the correct user-defined
// function or load factor for the container's key or element types.
// For comparison and hash functions, the list of user-defined functions is followed by a nested _Generic statement
// containing the default functions for integer types and NULL-terminated strings, which allows the user to overwrite
// the default functions.
// In C++, we use the same macros combined with std::is_same and ternary expressions to emulate _Generic statements.
// Unfortunately, the behavior is not identical because the C++ versions won't generate compile errors if the user
// defines multiple functions (e.g. multiple destructors) for the same type.
// Hence, it is up to the user to make sure they are not doing that if they are compiling in C++.

#ifdef __cplusplus

#define CC_EL_DTOR_SLOT( n, arg ) std::is_same<arg, cc_dtor_##n##_ty>::value ? cc_dtor_##n##_fn :
#define CC_EL_DTOR( cntr )                              \
(                                                       \
  CC_FOR_EACH_DTOR( CC_EL_DTOR_SLOT, CC_EL_TY( cntr ) ) \
  (cc_dtor_fnptr_ty)NULL                                \
)                                                       \

#define CC_KEY_DTOR_SLOT( n, arg )                           \
std::is_same<                                                \
  CC_TYPEOF_XP(**arg),                                       \
  CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( arg ), cc_dtor_##n##_ty ) \
>::value ? cc_dtor_##n##_fn :                                \

#define CC_KEY_DTOR( cntr )                  \
(                                            \
  CC_FOR_EACH_DTOR( CC_KEY_DTOR_SLOT, cntr ) \
  (cc_dtor_fnptr_ty)NULL                     \
)                                            \

#define CC_KEY_CMPR_SLOT( n, arg )                                                                      \
std::is_same<CC_TYPEOF_XP(**arg), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( arg ), cc_cmpr_##n##_ty ) >::value ? \
  cc_cmpr_##n##_fn_select                                                                             : \

#define CC_KEY_CMPR( cntr )                                                                                  \
(                                                                                                            \
  CC_FOR_EACH_CMPR( CC_KEY_CMPR_SLOT, cntr )                                                                 \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), char )>::value               ? \
    cc_cmpr_char_select                                                                                    : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned char )>::value      ? \
    cc_cmpr_unsigned_char_select                                                                           : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), signed char )>::value        ? \
    cc_cmpr_signed_char_select                                                                             : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned short )>::value     ? \
    cc_cmpr_unsigned_short_select                                                                          : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), short )>::value              ? \
    cc_cmpr_short_select                                                                                   : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned int )>::value       ? \
    cc_cmpr_unsigned_int_select                                                                            : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), int )>::value                ? \
    cc_cmpr_int_select                                                                                     : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned long )>::value      ? \
    cc_cmpr_unsigned_long_select                                                                           : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), long )>::value               ? \
    cc_cmpr_long_select                                                                                    : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned long long )>::value ? \
    cc_cmpr_unsigned_long_long_select                                                                      : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), long long )>::value          ? \
    cc_cmpr_long_long_select                                                                               : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), size_t )>::value             ? \
    cc_cmpr_size_t_select                                                                                  : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), char * )>::value             ? \
    cc_cmpr_c_string_select                                                                                : \
  cc_cmpr_dummy_select                                                                                       \
)( CC_CNTR_ID( cntr ) )                                                                                      \

#define CC_KEY_HASH_SLOT( n, arg )                           \
std::is_same<                                                \
  CC_TYPEOF_XP(**arg),                                       \
  CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( arg ), cc_hash_##n##_ty ) \
>::value ? cc_hash_##n##_fn :                                \

#define CC_KEY_HASH( cntr )                                                                                  \
(                                                                                                            \
  CC_FOR_EACH_HASH( CC_KEY_HASH_SLOT, cntr )                                                                 \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), char )>::value               ? \
    cc_hash_char                                                                                           : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned char )>::value      ? \
    cc_hash_unsigned_char                                                                                  : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), signed char )>::value        ? \
    cc_hash_signed_char                                                                                    : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned short )>::value     ? \
    cc_hash_unsigned_short                                                                                 : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), short )>::value              ? \
    cc_hash_short                                                                                          : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned int )>::value       ? \
    cc_hash_unsigned_int                                                                                   : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), int )>::value                ? \
    cc_hash_int                                                                                            : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned long )>::value      ? \
    cc_hash_unsigned_long                                                                                  : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), long )>::value               ? \
    cc_hash_long                                                                                           : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned long long )>::value ? \
    cc_hash_unsigned_long_long                                                                             : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), long long )>::value          ? \
    cc_hash_long_long                                                                                      : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), size_t )>::value             ? \
    cc_hash_size_t                                                                                         : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), char * )>::value             ? \
    cc_hash_c_string                                                                                       : \
  (cc_hash_fnptr_ty)NULL                                                                                     \
)                                                                                                            \

#define CC_HAS_CMPR_SLOT( n, arg ) std::is_same<arg, cc_cmpr_##n##_ty>::value ? true :
#define CC_HAS_CMPR( ty )                              \
(                                                      \
  std::is_same<ty, char>::value               ? true : \
  std::is_same<ty, unsigned char>::value      ? true : \
  std::is_same<ty, signed char>::value        ? true : \
  std::is_same<ty, unsigned short>::value     ? true : \
  std::is_same<ty, signed short>::value       ? true : \
  std::is_same<ty, unsigned int>::value       ? true : \
  std::is_same<ty, signed int>::value         ? true : \
  std::is_same<ty, unsigned long>::value      ? true : \
  std::is_same<ty, signed long>::value        ? true : \
  std::is_same<ty, unsigned long long>::value ? true : \
  std::is_same<ty, signed long long>::value   ? true : \
  std::is_same<ty, size_t>::value             ? true : \
  std::is_same<ty, char *>::value             ? true : \
  CC_FOR_EACH_CMPR( CC_HAS_CMPR_SLOT, ty )             \
  false                                                \
)                                                      \

#define CC_HAS_HASH_SLOT( n, arg ) std::is_same<arg, cc_hash_##n##_ty>::value ? true :
#define CC_HAS_HASH( ty )                              \
(                                                      \
  std::is_same<ty, char>::value               ? true : \
  std::is_same<ty, unsigned char>::value      ? true : \
  std::is_same<ty, signed char>::value        ? true : \
  std::is_same<ty, unsigned short>::value     ? true : \
  std::is_same<ty, signed short>::value       ? true : \
  std::is_same<ty, unsigned int>::value       ? true : \
  std::is_same<ty, signed int>::value         ? true : \
  std::is_same<ty, unsigned long>::value      ? true : \
  std::is_same<ty, signed long>::value        ? true : \
  std::is_same<ty, unsigned long long>::value ? true : \
  std::is_same<ty, signed long long>::value   ? true : \
  std::is_same<ty, size_t>::value             ? true : \
  std::is_same<ty, char *>::value             ? true : \
  CC_FOR_EACH_HASH( CC_HAS_HASH_SLOT, ty )             \
  false                                                \
)                                                      \

#define CC_KEY_LOAD_SLOT( n, arg )                           \
std::is_same<                                                \
  CC_TYPEOF_XP(**arg),                                       \
  CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( arg ), cc_load_##n##_ty ) \
>::value ? cc_load_##n##_val :                               \

#define CC_KEY_LOAD( cntr )                  \
(                                            \
  CC_FOR_EACH_LOAD( CC_KEY_LOAD_SLOT, cntr ) \
  CC_DEFAULT_LOAD                            \
)                                            \

#define CC_LAYOUT( cntr )                                                        \
cc_layout(                                                                       \
  CC_CNTR_ID( cntr ),                                                            \
  CC_EL_SIZE( cntr ),                                                            \
  alignof( CC_EL_TY( cntr ) ),                                                   \
  cc_key_details_ty{ sizeof( CC_KEY_TY( cntr ) ), alignof( CC_KEY_TY( cntr ) ) } \
)                                                                                \

#else

#define CC_EL_DTOR_SLOT( n, arg ) cc_dtor_##n##_ty: cc_dtor_##n##_fn,
#define CC_EL_DTOR( cntr )             \
_Generic( (CC_EL_TY( cntr )){ 0 },     \
  CC_FOR_EACH_DTOR( CC_EL_DTOR_SLOT, ) \
  default: (cc_dtor_fnptr_ty)NULL      \
)                                      \

#define CC_KEY_DTOR_SLOT( n, arg ) CC_MAKE_BASE_FNPTR_TY( arg, cc_dtor_##n##_ty ): cc_dtor_##n##_fn,
#define CC_KEY_DTOR( cntr )                              \
_Generic( (**cntr),                                      \
  CC_FOR_EACH_DTOR( CC_KEY_DTOR_SLOT, CC_EL_TY( cntr ) ) \
  default: (cc_dtor_fnptr_ty)NULL                        \
)                                                        \

#define CC_KEY_CMPR_SLOT( n, arg ) CC_MAKE_BASE_FNPTR_TY( arg, cc_cmpr_##n##_ty ): cc_cmpr_##n##_fn_select,
#define CC_KEY_CMPR( cntr )                                                                           \
_Generic( (**cntr),                                                                                   \
  CC_FOR_EACH_CMPR( CC_KEY_CMPR_SLOT, CC_EL_TY( cntr ) )                                              \
  default: _Generic( (**cntr),                                                                        \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), char ):               cc_cmpr_char_select,               \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned char ):      cc_cmpr_unsigned_char_select,      \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), signed char ):        cc_cmpr_signed_char_select,        \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned short ):     cc_cmpr_unsigned_short_select,     \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), short ):              cc_cmpr_short_select,              \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned int ):       cc_cmpr_unsigned_int_select,       \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), int ):                cc_cmpr_int_select,                \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned long ):      cc_cmpr_unsigned_long_select,      \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), long ):               cc_cmpr_long_select,               \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned long long ): cc_cmpr_unsigned_long_long_select, \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), long long ):          cc_cmpr_long_long_select,          \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), cc_maybe_size_t ):    cc_cmpr_size_t_select,             \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), char * ):             cc_cmpr_c_string_select,           \
    default: cc_cmpr_dummy_select                                                                     \
  )                                                                                                   \
)( CC_CNTR_ID( cntr ) )                                                                               \

#define CC_KEY_HASH_SLOT( n, arg ) CC_MAKE_BASE_FNPTR_TY( arg, cc_hash_##n##_ty ): cc_hash_##n##_fn,
#define CC_KEY_HASH( cntr )                                                                    \
_Generic( (**cntr),                                                                            \
  CC_FOR_EACH_HASH( CC_KEY_HASH_SLOT, CC_EL_TY( cntr ) )                                       \
  default: _Generic( (**cntr),                                                                 \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), char ):               cc_hash_char,               \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned char ):      cc_hash_unsigned_char,      \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), signed char ):        cc_hash_signed_char,        \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned short ):     cc_hash_unsigned_short,     \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), short ):              cc_hash_short,              \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned int ):       cc_hash_unsigned_int,       \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), int ):                cc_hash_int,                \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned long ):      cc_hash_unsigned_long,      \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), long ):               cc_hash_long,               \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned long long ): cc_hash_unsigned_long_long, \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), long long ):          cc_hash_long_long,          \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), cc_maybe_size_t ):    cc_hash_size_t,             \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), char * ):             cc_hash_c_string,           \
    default: (cc_hash_fnptr_ty)NULL                                                            \
  )                                                                                            \
)                                                                                              \

#define CC_HAS_CMPR_SLOT( n, arg ) cc_cmpr_##n##_ty: true,
#define CC_HAS_CMPR( ty )               \
_Generic( (ty){ 0 },                    \
  CC_FOR_EACH_CMPR( CC_HAS_CMPR_SLOT, ) \
  default: _Generic( (ty){ 0 },         \
    char:               true,           \
    unsigned char:      true,           \
    signed char:        true,           \
    unsigned short:     true,           \
    short:              true,           \
    unsigned int:       true,           \
    int:                true,           \
    unsigned long:      true,           \
    long:               true,           \
    unsigned long long: true,           \
    long long:          true,           \
    cc_maybe_size_t:    true,           \
    char *:             true,           \
    default:            false           \
  )                                     \
)                                       \

#define CC_HAS_HASH_SLOT( n, arg ) cc_hash_##n##_ty: true,
#define CC_HAS_HASH( ty )               \
_Generic( (ty){ 0 },                    \
  CC_FOR_EACH_HASH( CC_HAS_HASH_SLOT, ) \
  default: _Generic( (ty){ 0 },         \
    char:               true,           \
    unsigned char:      true,           \
    signed char:        true,           \
    unsigned short:     true,           \
    short:              true,           \
    unsigned int:       true,           \
    int:                true,           \
    unsigned long:      true,           \
    long:               true,           \
    unsigned long long: true,           \
    long long:          true,           \
    cc_maybe_size_t:    true,           \
    char *:             true,           \
    default:            false           \
  )                                     \
)                                       \

#define CC_KEY_LOAD_SLOT( n, arg ) CC_MAKE_BASE_FNPTR_TY( arg, cc_load_##n##_ty ): cc_load_##n##_val,
#define CC_KEY_LOAD( cntr )                              \
_Generic( (**cntr),                                      \
  CC_FOR_EACH_LOAD( CC_KEY_LOAD_SLOT, CC_EL_TY( cntr ) ) \
  default: CC_DEFAULT_LOAD                               \
)                                                        \

#define CC_KEY_DETAILS_SLOT( n, arg )                                               \
CC_MAKE_BASE_FNPTR_TY( arg, cc_cmpr_##n##_ty ):                                     \
  ( cc_key_details_ty ){ sizeof( cc_cmpr_##n##_ty ), alignof( cc_cmpr_##n##_ty ) }, \

#define CC_KEY_DETAILS( cntr )                                                              \
_Generic( (**cntr),                                                                         \
  CC_FOR_EACH_CMPR( CC_KEY_DETAILS_SLOT, CC_EL_TY( cntr ) )                                 \
  default: _Generic( (**cntr),                                                              \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), char ):                                        \
      ( cc_key_details_ty ){ sizeof( char ), alignof( char ) },                             \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned char ) :                              \
      ( cc_key_details_ty ){ sizeof( unsigned char ), alignof( unsigned char ) },           \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), signed char ) :                                \
      ( cc_key_details_ty ){ sizeof( signed char ), alignof( signed char ) },               \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned short ) :                             \
      ( cc_key_details_ty ){ sizeof( unsigned short ), alignof( unsigned short ) },         \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), short ) :                                      \
      ( cc_key_details_ty ){ sizeof( short ), alignof( short ) },                           \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned int ) :                               \
      ( cc_key_details_ty ){ sizeof( unsigned int ), alignof( unsigned int ) },             \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), int ) :                                        \
      ( cc_key_details_ty ){ sizeof( int ), alignof( int ) },                               \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned long ):                               \
      ( cc_key_details_ty ){ sizeof( unsigned long ), alignof( unsigned long ) },           \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), long ):                                        \
      ( cc_key_details_ty ){ sizeof( long ), alignof( long ) },                             \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned long long ):                          \
      ( cc_key_details_ty ){ sizeof( unsigned long long ), alignof( unsigned long long ) }, \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), long long ):                                   \
      ( cc_key_details_ty ){ sizeof( long long ), alignof( long long ) },                   \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), cc_maybe_size_t ):                             \
      ( cc_key_details_ty ){ sizeof( cc_maybe_size_t ), alignof( cc_maybe_size_t ) },       \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), char * ):                                      \
      ( cc_key_details_ty ){ sizeof( char * ), alignof( char * ) },                         \
    default: ( cc_key_details_ty ){ 0 }                                                     \
  )                                                                                         \
)                                                                                           \

#define CC_LAYOUT( cntr )                                                                                \
cc_layout( CC_CNTR_ID( cntr ), CC_EL_SIZE( cntr ), alignof( CC_EL_TY( cntr ) ), CC_KEY_DETAILS( cntr ) ) \

#endif

// Macros for extracting the type and function body or load factor from user-defined DTOR, CMPR, HASH, and LOAD macros. 
#define CC_1ST_ARG_( _1, ... )    _1
#define CC_1ST_ARG( ... )         CC_1ST_ARG_( __VA_ARGS__ )
#define CC_OTHER_ARGS_( _1, ... ) __VA_ARGS__
#define CC_OTHER_ARGS( ... )      CC_OTHER_ARGS_( __VA_ARGS__ )

// Default hash and comparison functions for fundamental types.

// Integer types.

static inline size_t cc_hash_uint64( uint64_t val )
{
#if SIZE_MAX == 0xFFFFFFFFFFFFFFFF || SIZE_MAX == 0xFFFFFFFF // 64-bit or 32-bit size_t.
  // Fast-hash: https://jonkagstrom.com/bit-mixer-construction
  //            https://code.google.com/archive/p/fast-hash
  val ^= val >> 23;
  val *= 0x2127599BF4325C37ULL;
  val ^= val >> 47;
#if SIZE_MAX == 0xFFFFFFFFFFFFFFFF
  return val;
#elif SIZE_MAX == 0xFFFFFFFF
  return val - ( val >> 32 );
#endif
#else // Unknown size_t, fall back on Knuth.
  return val * 2654435761ULL;
#endif
}

// Comparison functionality for each type is split between an equality function and a less-than function.
// A select function returns a pointer to the relevant of these two functions based on the container type.
// This bifurcation, instead of one unified three-way comparison function, allows the most optimized comparison to
// always be used without increasing the number of _Generic statements inside API macros or complicating the API for
// defining custom comparison functions (see "DEFINING DESTRUCTOR, COMPARISON, OR HASH FUNCTION OR LOAD FACTOR MODE"
// below for more details).
// Note that the less-than functions are currently unused, but they will be necessary for future tree-based containers
// and sort functions for vectors and lists.

#define CC_DEFAULT_INTEGER_CMPR_HASH_FUNCTIONS( ty, name )                                        \
                                                                                                  \
static inline bool cc_cmpr_##name##_less( void *void_val_1, void *void_val_2 )                    \
{                                                                                                 \
  return *(ty *)void_val_1 < *(ty *)void_val_2;                                                   \
}                                                                                                 \
                                                                                                  \
static inline bool cc_cmpr_##name##_equal( void *void_val_1, void *void_val_2 )                   \
{                                                                                                 \
  return *(ty *)void_val_1 == *(ty *)void_val_2;                                                  \
}                                                                                                 \
                                                                                                  \
static inline CC_ALWAYS_INLINE cc_cmpr_fnptr_ty cc_cmpr_##name##_select( size_t cntr_id )         \
{                                                                                                 \
  return cntr_id == CC_MAP || cntr_id == CC_SET ? cc_cmpr_##name##_equal : cc_cmpr_##name##_less; \
}                                                                                                 \
                                                                                                  \
static inline size_t cc_hash_##name( void *void_val )                                             \
{                                                                                                 \
  return cc_hash_uint64( (uint64_t)*(ty *)void_val );                                             \
}                                                                                                 \

CC_DEFAULT_INTEGER_CMPR_HASH_FUNCTIONS( char, char )
CC_DEFAULT_INTEGER_CMPR_HASH_FUNCTIONS( unsigned char, unsigned_char )
CC_DEFAULT_INTEGER_CMPR_HASH_FUNCTIONS( signed char, signed_char )
CC_DEFAULT_INTEGER_CMPR_HASH_FUNCTIONS( unsigned short, unsigned_short )
CC_DEFAULT_INTEGER_CMPR_HASH_FUNCTIONS( short, short )
CC_DEFAULT_INTEGER_CMPR_HASH_FUNCTIONS( unsigned int, unsigned_int )
CC_DEFAULT_INTEGER_CMPR_HASH_FUNCTIONS( int, int )
CC_DEFAULT_INTEGER_CMPR_HASH_FUNCTIONS( unsigned long, unsigned_long )
CC_DEFAULT_INTEGER_CMPR_HASH_FUNCTIONS( long, long )
CC_DEFAULT_INTEGER_CMPR_HASH_FUNCTIONS( unsigned long long, unsigned_long_long )
CC_DEFAULT_INTEGER_CMPR_HASH_FUNCTIONS( long long, long_long )

// size_t could be an alias for a fundamental integer type or a distinct type.
// Hence, in C we have to handle it as a special case so that it doesn't clash with another type in _Generic statements.
// If size_t is an alias, cc_maybe_size_t will be a dummy type used in no other context.
// Otherwise, cc_maybe_size_t will be an alias for size_t.

#ifndef __cplusplus

typedef struct { char nothing; } cc_size_t_dummy;

typedef CC_TYPEOF_XP(
  _Generic( (size_t){ 0 },
    unsigned short:     (cc_size_t_dummy){ 0 },
    short:              (cc_size_t_dummy){ 0 },
    unsigned int:       (cc_size_t_dummy){ 0 },
    int:                (cc_size_t_dummy){ 0 },
    unsigned long:      (cc_size_t_dummy){ 0 },
    long:               (cc_size_t_dummy){ 0 },
    unsigned long long: (cc_size_t_dummy){ 0 },
    long long:          (cc_size_t_dummy){ 0 },
    default:            (size_t){ 0 }
  )
) cc_maybe_size_t;

#endif

CC_DEFAULT_INTEGER_CMPR_HASH_FUNCTIONS( size_t, size_t )

// Null-terminated C strings.
// We use FNV-1a because newer, faster alternatives that process word-sized chunks require prior knowledge of the
// string's length.

static inline bool cc_cmpr_c_string_less( void *void_val_1, void *void_val_2 )
{
  return strcmp( *(char **)void_val_1, *(char **)void_val_2 ) < 0;
}

static inline bool cc_cmpr_c_string_equal( void *void_val_1, void *void_val_2 )
{
  return strcmp( *(char **)void_val_1, *(char **)void_val_2 ) == 0;
}

static inline cc_cmpr_fnptr_ty cc_cmpr_c_string_select( size_t cntr_id )                          \
{                                                                                                 \
  return cntr_id == CC_MAP || cntr_id == CC_SET ? cc_cmpr_c_string_equal : cc_cmpr_c_string_less; \
}                                                                                                 \

#if SIZE_MAX == 0xFFFFFFFF // 32-bit size_t.

static inline size_t cc_hash_c_string( void *void_val )
{
    char *val = *(char **)void_val;
    size_t hash = 0x01000193;
    while( *val )
      hash = ( (unsigned char)*val++ ^ hash ) * 0x811C9DC5;

    return hash;
}

#elif SIZE_MAX == 0xFFFFFFFFFFFFFFFF // 64-bit size_t.

static inline size_t cc_hash_c_string( void *void_val )
{
    char *val = *(char **)void_val;
    size_t hash = 0xCBF29CE484222325;
    while( *val )
      hash = ( (unsigned char)*val++ ^ hash ) * 0x100000001B3;

    return hash;
}

#else // Strange size_t.

static inline size_t cc_hash_c_string( void *void_val )
{
    char *val = *(char **)void_val;
    size_t hash = 0;
    while( *val )
        hash = hash * 131 + (unsigned char)*val++;

    return hash;
}

#endif

// Dummy for containers with no comparison function.
static inline CC_ALWAYS_INLINE cc_cmpr_fnptr_ty cc_cmpr_dummy_select( CC_UNUSED( size_t, cntr_id ) )
{
  return NULL;
}

#endif

#else/*---------------------------------------------------------------------------------------------------------------*/
/*                                                                                                                    */
/*                       DEFINING DESTRUCTOR, COMPARISON, OR HASH FUNCTION OR LOAD FACTOR MODE                        */
/*                                                                                                                    */
/*--------------------------------------------------------------------------------------------------------------------*/

#ifdef CC_DTOR

// Convert the user-defined CC_DTOR macro into a cc_dtor_XXXX_ty and cc_dtor_XXXX_fn pair that can be plugged into the
// CC_GET_EL_DTOR and CC_GET_KEY_DTOR macros above.

typedef CC_TYPEOF_TY( CC_1ST_ARG( CC_DTOR ) ) CC_CAT_3( cc_dtor_, CC_N_DTORS, _ty );

static inline void CC_CAT_3( cc_dtor_, CC_N_DTORS, _fn )( void *void_val )
{
  CC_CAT_3( cc_dtor_, CC_N_DTORS, _ty ) val = *(CC_CAT_3( cc_dtor_, CC_N_DTORS, _ty ) *)void_val;
  CC_OTHER_ARGS( CC_DTOR )
}

// Increment DTOR counter.
#if CC_N_DTORS_D1 == 0
#undef CC_N_DTORS_D1
#define CC_N_DTORS_D1 1
#elif CC_N_DTORS_D1 == 1
#undef CC_N_DTORS_D1
#define CC_N_DTORS_D1 2
#elif CC_N_DTORS_D1 == 2
#undef CC_N_DTORS_D1
#define CC_N_DTORS_D1 3
#elif CC_N_DTORS_D1 == 3
#undef CC_N_DTORS_D1
#define CC_N_DTORS_D1 4
#elif CC_N_DTORS_D1 == 4
#undef CC_N_DTORS_D1
#define CC_N_DTORS_D1 5
#elif CC_N_DTORS_D1 == 5
#undef CC_N_DTORS_D1
#define CC_N_DTORS_D1 6
#elif CC_N_DTORS_D1 == 6
#undef CC_N_DTORS_D1
#define CC_N_DTORS_D1 7
#elif CC_N_DTORS_D1 == 7
#undef CC_N_DTORS_D1
#define CC_N_DTORS_D1 0
#if CC_N_DTORS_D2 == 0
#undef CC_N_DTORS_D2
#define CC_N_DTORS_D2 1
#elif CC_N_DTORS_D2 == 1
#undef CC_N_DTORS_D2
#define CC_N_DTORS_D2 2
#elif CC_N_DTORS_D2 == 2
#undef CC_N_DTORS_D2
#define CC_N_DTORS_D2 3
#elif CC_N_DTORS_D2 == 3
#undef CC_N_DTORS_D2
#define CC_N_DTORS_D2 4
#elif CC_N_DTORS_D2 == 4
#undef CC_N_DTORS_D2
#define CC_N_DTORS_D2 5
#elif CC_N_DTORS_D2 == 5
#undef CC_N_DTORS_D2
#define CC_N_DTORS_D2 6
#elif CC_N_DTORS_D2 == 6
#undef CC_N_DTORS_D2
#define CC_N_DTORS_D2 7
#elif CC_N_DTORS_D2 == 7
#undef CC_N_DTORS_D2
#define CC_N_DTORS_D2 0
#if CC_N_DTORS_D3 == 0
#undef CC_N_DTORS_D3
#define CC_N_DTORS_D3 1
#elif CC_N_DTORS_D3 == 1
#undef CC_N_DTORS_D3
#define CC_N_DTORS_D3 2
#elif CC_N_DTORS_D3 == 2
#undef CC_N_DTORS_D3
#define CC_N_DTORS_D3 3
#elif CC_N_DTORS_D3 == 3
#undef CC_N_DTORS_D3
#define CC_N_DTORS_D3 4
#elif CC_N_DTORS_D3 == 4
#undef CC_N_DTORS_D3
#define CC_N_DTORS_D3 5
#elif CC_N_DTORS_D3 == 5
#undef CC_N_DTORS_D3
#define CC_N_DTORS_D3 6
#elif CC_N_DTORS_D3 == 6
#undef CC_N_DTORS_D3
#define CC_N_DTORS_D3 7
#elif CC_N_DTORS_D3 == 7
#error Sorry, the number of destructor functions is limited to 511.
#endif
#endif
#endif

#undef CC_DTOR
#endif

#ifdef CC_CMPR

typedef CC_TYPEOF_TY( CC_1ST_ARG( CC_CMPR ) ) CC_CAT_3( cc_cmpr_, CC_N_CMPRS, _ty );

// For user-defined comparison functions, the user provides a three-way comparison (see the API documentation).
// The library then defines less-than and equals functions that call the three-way comparison function.
// This allows the compiler to optimize the comparison for best performance in both cases without burdening the user
// with having to define two separate comparison functions for a single type.

static inline CC_ALWAYS_INLINE int CC_CAT_3( cc_cmpr_, CC_N_CMPRS, _fn_three_way )( void *void_val_1, void *void_val_2 )
{
  CC_CAT_3( cc_cmpr_, CC_N_CMPRS, _ty ) val_1 = *(CC_CAT_3( cc_cmpr_, CC_N_CMPRS, _ty ) *)void_val_1;
  CC_CAT_3( cc_cmpr_, CC_N_CMPRS, _ty ) val_2 = *(CC_CAT_3( cc_cmpr_, CC_N_CMPRS, _ty ) *)void_val_2;
  CC_OTHER_ARGS( CC_CMPR )
}

static inline bool CC_CAT_3( cc_cmpr_, CC_N_CMPRS, _fn_less )( void *void_val_1, void *void_val_2 )
{
  return CC_CAT_3( cc_cmpr_, CC_N_CMPRS, _fn_three_way )( void_val_1, void_val_2 ) < 0;
}

static inline bool CC_CAT_3( cc_cmpr_, CC_N_CMPRS, _fn_equals )( void *void_val_1, void *void_val_2 )
{
  return CC_CAT_3( cc_cmpr_, CC_N_CMPRS, _fn_three_way )( void_val_1, void_val_2 ) == 0;
}

static inline CC_ALWAYS_INLINE cc_cmpr_fnptr_ty CC_CAT_3( cc_cmpr_, CC_N_CMPRS, _fn_select )( size_t cntr_id )
{
  return cntr_id == CC_MAP || cntr_id == CC_SET ?
    CC_CAT_3( cc_cmpr_, CC_N_CMPRS, _fn_equals )
    :
    CC_CAT_3( cc_cmpr_, CC_N_CMPRS, _fn_less );
}

#if CC_N_CMPRS_D1 == 0
#undef CC_N_CMPRS_D1
#define CC_N_CMPRS_D1 1
#elif CC_N_CMPRS_D1 == 1
#undef CC_N_CMPRS_D1
#define CC_N_CMPRS_D1 2
#elif CC_N_CMPRS_D1 == 2
#undef CC_N_CMPRS_D1
#define CC_N_CMPRS_D1 3
#elif CC_N_CMPRS_D1 == 3
#undef CC_N_CMPRS_D1
#define CC_N_CMPRS_D1 4
#elif CC_N_CMPRS_D1 == 4
#undef CC_N_CMPRS_D1
#define CC_N_CMPRS_D1 5
#elif CC_N_CMPRS_D1 == 5
#undef CC_N_CMPRS_D1
#define CC_N_CMPRS_D1 6
#elif CC_N_CMPRS_D1 == 6
#undef CC_N_CMPRS_D1
#define CC_N_CMPRS_D1 7
#elif CC_N_CMPRS_D1 == 7
#undef CC_N_CMPRS_D1
#define CC_N_CMPRS_D1 0
#if CC_N_CMPRS_D2 == 0
#undef CC_N_CMPRS_D2
#define CC_N_CMPRS_D2 1
#elif CC_N_CMPRS_D2 == 1
#undef CC_N_CMPRS_D2
#define CC_N_CMPRS_D2 2
#elif CC_N_CMPRS_D2 == 2
#undef CC_N_CMPRS_D2
#define CC_N_CMPRS_D2 3
#elif CC_N_CMPRS_D2 == 3
#undef CC_N_CMPRS_D2
#define CC_N_CMPRS_D2 4
#elif CC_N_CMPRS_D2 == 4
#undef CC_N_CMPRS_D2
#define CC_N_CMPRS_D2 5
#elif CC_N_CMPRS_D2 == 5
#undef CC_N_CMPRS_D2
#define CC_N_CMPRS_D2 6
#elif CC_N_CMPRS_D2 == 6
#undef CC_N_CMPRS_D2
#define CC_N_CMPRS_D2 7
#elif CC_N_CMPRS_D2 == 7
#undef CC_N_CMPRS_D2
#define CC_N_CMPRS_D2 0
#if CC_N_CMPRS_D3 == 0
#undef CC_N_CMPRS_D3
#define CC_N_CMPRS_D3 1
#elif CC_N_CMPRS_D3 == 1
#undef CC_N_CMPRS_D3
#define CC_N_CMPRS_D3 2
#elif CC_N_CMPRS_D3 == 2
#undef CC_N_CMPRS_D3
#define CC_N_CMPRS_D3 3
#elif CC_N_CMPRS_D3 == 3
#undef CC_N_CMPRS_D3
#define CC_N_CMPRS_D3 4
#elif CC_N_CMPRS_D3 == 4
#undef CC_N_CMPRS_D3
#define CC_N_CMPRS_D3 5
#elif CC_N_CMPRS_D3 == 5
#undef CC_N_CMPRS_D3
#define CC_N_CMPRS_D3 6
#elif CC_N_CMPRS_D3 == 6
#undef CC_N_CMPRS_D3
#define CC_N_CMPRS_D3 7
#elif CC_N_CMPRS_D3 == 7
#error Sorry, the number of comparison functions is limited to 511.
#endif
#endif
#endif

#undef CC_CMPR
#endif

#ifdef CC_HASH

typedef CC_TYPEOF_TY( CC_1ST_ARG( CC_HASH ) ) CC_CAT_3( cc_hash_, CC_N_HASHS, _ty );

static inline size_t CC_CAT_3( cc_hash_, CC_N_HASHS, _fn )( void *void_val )
{
  CC_CAT_3( cc_hash_, CC_N_HASHS, _ty ) val = *(CC_CAT_3( cc_hash_, CC_N_HASHS, _ty ) *)void_val;
  CC_OTHER_ARGS( CC_HASH )
}

#if CC_N_HASHS_D1 == 0
#undef CC_N_HASHS_D1
#define CC_N_HASHS_D1 1
#elif CC_N_HASHS_D1 == 1
#undef CC_N_HASHS_D1
#define CC_N_HASHS_D1 2
#elif CC_N_HASHS_D1 == 2
#undef CC_N_HASHS_D1
#define CC_N_HASHS_D1 3
#elif CC_N_HASHS_D1 == 3
#undef CC_N_HASHS_D1
#define CC_N_HASHS_D1 4
#elif CC_N_HASHS_D1 == 4
#undef CC_N_HASHS_D1
#define CC_N_HASHS_D1 5
#elif CC_N_HASHS_D1 == 5
#undef CC_N_HASHS_D1
#define CC_N_HASHS_D1 6
#elif CC_N_HASHS_D1 == 6
#undef CC_N_HASHS_D1
#define CC_N_HASHS_D1 7
#elif CC_N_HASHS_D1 == 7
#undef CC_N_HASHS_D1
#define CC_N_HASHS_D1 0
#if CC_N_HASHS_D2 == 0
#undef CC_N_HASHS_D2
#define CC_N_HASHS_D2 1
#elif CC_N_HASHS_D2 == 1
#undef CC_N_HASHS_D2
#define CC_N_HASHS_D2 2
#elif CC_N_HASHS_D2 == 2
#undef CC_N_HASHS_D2
#define CC_N_HASHS_D2 3
#elif CC_N_HASHS_D2 == 3
#undef CC_N_HASHS_D2
#define CC_N_HASHS_D2 4
#elif CC_N_HASHS_D2 == 4
#undef CC_N_HASHS_D2
#define CC_N_HASHS_D2 5
#elif CC_N_HASHS_D2 == 5
#undef CC_N_HASHS_D2
#define CC_N_HASHS_D2 6
#elif CC_N_HASHS_D2 == 6
#undef CC_N_HASHS_D2
#define CC_N_HASHS_D2 7
#elif CC_N_HASHS_D2 == 7
#undef CC_N_HASHS_D2
#define CC_N_HASHS_D2 0
#if CC_N_HASHS_D3 == 0
#undef CC_N_HASHS_D3
#define CC_N_HASHS_D3 1
#elif CC_N_HASHS_D3 == 1
#undef CC_N_HASHS_D3
#define CC_N_HASHS_D3 2
#elif CC_N_HASHS_D3 == 2
#undef CC_N_HASHS_D3
#define CC_N_HASHS_D3 3
#elif CC_N_HASHS_D3 == 3
#undef CC_N_HASHS_D3
#define CC_N_HASHS_D3 4
#elif CC_N_HASHS_D3 == 4
#undef CC_N_HASHS_D3
#define CC_N_HASHS_D3 5
#elif CC_N_HASHS_D3 == 5
#undef CC_N_HASHS_D3
#define CC_N_HASHS_D3 6
#elif CC_N_HASHS_D3 == 6
#undef CC_N_HASHS_D3
#define CC_N_HASHS_D3 7
#elif CC_N_HASHS_D3 == 7
#error Sorry, the number of hash functions is limited to 511.
#endif
#endif
#endif

#undef CC_HASH
#endif

#ifdef CC_LOAD

typedef CC_TYPEOF_TY( CC_1ST_ARG( CC_LOAD ) ) CC_CAT_3( cc_load_, CC_N_LOADS, _ty );

const double CC_CAT_3( cc_load_, CC_N_LOADS, _val ) = CC_OTHER_ARGS( CC_LOAD );

#if CC_N_LOADS_D1 == 0
#undef CC_N_LOADS_D1
#define CC_N_LOADS_D1 1
#elif CC_N_LOADS_D1 == 1
#undef CC_N_LOADS_D1
#define CC_N_LOADS_D1 2
#elif CC_N_LOADS_D1 == 2
#undef CC_N_LOADS_D1
#define CC_N_LOADS_D1 3
#elif CC_N_LOADS_D1 == 3
#undef CC_N_LOADS_D1
#define CC_N_LOADS_D1 4
#elif CC_N_LOADS_D1 == 4
#undef CC_N_LOADS_D1
#define CC_N_LOADS_D1 5
#elif CC_N_LOADS_D1 == 5
#undef CC_N_LOADS_D1
#define CC_N_LOADS_D1 6
#elif CC_N_LOADS_D1 == 6
#undef CC_N_LOADS_D1
#define CC_N_LOADS_D1 7
#elif CC_N_LOADS_D1 == 7
#undef CC_N_LOADS_D1
#define CC_N_LOADS_D1 0
#if CC_N_LOADS_D2 == 0
#undef CC_N_LOADS_D2
#define CC_N_LOADS_D2 1
#elif CC_N_LOADS_D2 == 1
#undef CC_N_LOADS_D2
#define CC_N_LOADS_D2 2
#elif CC_N_LOADS_D2 == 2
#undef CC_N_LOADS_D2
#define CC_N_LOADS_D2 3
#elif CC_N_LOADS_D2 == 3
#undef CC_N_LOADS_D2
#define CC_N_LOADS_D2 4
#elif CC_N_LOADS_D2 == 4
#undef CC_N_LOADS_D2
#define CC_N_LOADS_D2 5
#elif CC_N_LOADS_D2 == 5
#undef CC_N_LOADS_D2
#define CC_N_LOADS_D2 6
#elif CC_N_LOADS_D2 == 6
#undef CC_N_LOADS_D2
#define CC_N_LOADS_D2 7
#elif CC_N_LOADS_D2 == 7
#undef CC_N_LOADS_D2
#define CC_N_LOADS_D2 0
#if CC_N_LOADS_D3 == 0
#undef CC_N_LOADS_D3
#define CC_N_LOADS_D3 1
#elif CC_N_LOADS_D3 == 1
#undef CC_N_LOADS_D3
#define CC_N_LOADS_D3 2
#elif CC_N_LOADS_D3 == 2
#undef CC_N_LOADS_D3
#define CC_N_LOADS_D3 3
#elif CC_N_LOADS_D3 == 3
#undef CC_N_LOADS_D3
#define CC_N_LOADS_D3 4
#elif CC_N_LOADS_D3 == 4
#undef CC_N_LOADS_D3
#define CC_N_LOADS_D3 5
#elif CC_N_LOADS_D3 == 5
#undef CC_N_LOADS_D3
#define CC_N_LOADS_D3 6
#elif CC_N_LOADS_D3 == 6
#undef CC_N_LOADS_D3
#define CC_N_LOADS_D3 7
#elif CC_N_LOADS_D3 == 7
#error Sorry, the number of load factors is limited to 511.
#endif
#endif
#endif

#undef CC_LOAD
#endif

#endif
