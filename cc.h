/*----------------------------------------- CC: CONVENIENT CONTAINERS v1.0.2 -------------------------------------------

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
    supports it). In the documentation below, these pointers are called "pointer-iterators".
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

      Ensures that the the capacity is large enough to support n elements.
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

  Map (an unordered container associating elements with keys, implemented as a Robin Hood hash table):

    map( key_ty, el_ty ) cntr

      Declares an uninitialized map named cntr.
      key_ty must be a type, or alias for a type, for which comparison and hash functions have been defined.
      This requirement is enforced internally such that neglecting it causes a compiler error.
      For types with in-built comparison and hash functions, and for details on how to declare new comparison and hash
      functions, see "Destructor, comparison, and hash functions and custom max load factors" below.

    size_t cap( map( key_ty, el_ty ) *cntr )

      Returns the current capacity, i.e. bucket count.
      Note that the number of elements a map can support without rehashing is not its capacity but its capacity
      multiplied by the max load factor associated with its key type.

    bool reserve( map( key_ty, el_ty ) *cntr, size_t n )

      Ensures that the capacity is large enough to support n elements without rehashing.
      Returns true, or false if unsuccessful due to memory allocation failure.

    bool shrink( map( key_ty, el_ty ) *cntr )

      Shrinks the capacity to best accommodate the current size.
      Returns true, or false if unsuccessful due to memory allocation failure.

    el_ty *insert( map( key_ty, el_ty ) *cntr, key_ty key, el_ty el )

      Inserts element el with the specified key.
      If an element with the same key already exists, the existing element is replaced.
      Returns a pointer-iterator to the new element, or NULL in the case of memory allocation failure.
      If adding one element would violate the map's max load factor, failure can occur even if it already contains the
      key.

    el_ty *get( map( key_ty, el_ty ) *cntr, key_ty key )

      Returns a pointer-iterator to the element with the specified key, or NULL if no such element exists.

    el_ty *get_or_insert( map( key_ty, el_ty ) *cntr, key_ty key, el_ty el )

      Inserts element el if no element with the specified key already exist.
      Returns a pointer-iterator to the new element if it was inserted, or a pointer-iterator to the existing
      element with the same key, or NULL in the case of memory allocation failure.
      If adding one element would violate the map's max load factor, failure can occur even if it already contains the
      key.
      Determine whether an element was inserted by comparing the map's size before and after the call.

    const key_ty *key_for( map( key_ty, el_ty ) *cntr, el_ty *i )

      Returns a const pointer to the key for the element pointed to by pointer-iterator i.

    bool erase( map( key_ty, el_ty ) *cntr, key_ty key )

      Erases the element with the specified key, if it exists.
      Returns true if an element was erased, or false if no such element exists.

    void erase_itr( map( key_ty, el_ty ) *cntr, el_ty *i )

      Erases the element pointed to by pointer-iterator i.

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

  Set (Robin Hood hash table for elements without a separate key):

    set( el_ty ) cntr

      Declares an uninitialized set named cntr.
      el_ty must be a type, or alias for a type, for which comparison and hash functions have been defined.
      This requirement is enforced internally such that neglecting it causes a compiler error.
      For types with in-built comparison and hash functions, and for details on how to declare new comparison and hash
      functions, see "Destructor, comparison, and hash functions and custom max load factors" below.

    size_t cap( set( el_ty ) *cntr )

      Returns the current capacity, i.e. bucket count.
      Note that the number of elements a set can support without rehashing is not its capacity but its capacity
      multiplied by the max load factor associated with its key type.

    bool reserve( set( el_ty ) *cntr, size_t n )

      Ensures that the capacity is large enough to support n elements without rehashing.
      Returns true, or false if unsuccessful due to memory allocation failure.

    bool shrink( set( el_ty ) *cntr )

      Shrinks the capacity to best accommodate the current size.
      Returns true, or false if unsuccessful due to memory allocation failure.

    el_ty *insert( set( el_ty ) *cntr, el_ty el )

      Inserts element el.
      If the element already exists, the existing element is replaced.
      Returns a pointer-iterator to the new element, or NULL in the case of memory allocation failure.
      Note that if adding one element would violate the set's max load factor, failure can occur even if it already
      contains el.

    el_ty *get( set( el_ty ) *cntr, el_ty el )

      Returns a pointer-iterator to element el, or NULL if no such element exists.

    el_ty *get_or_insert( set( el_ty ) *cntr, el_ty el )

      Inserts element el if it does not already exist.
      Returns a pointer-iterator to the new element if it was inserted, or a pointer-iterator to the existing element,
      or NULL in the case of memory allocation failure.
      If adding one element would violate the set's max load factor, failure can occur even if it already contains the
      element.
      Determine whether an element was inserted by comparing the set's size before and after the call.

    bool erase( set( el_ty ) *cntr, el_ty el )

      Erases the element el, if it exists.
      Returns true if an element was erased, or false if no such element exists.

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
      positive integer if val_1 is more than val_2.

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
      #define CC_CMPR our_type, { return ( val_1.x > val_2.x ) - ( val_1.x < val_2.x ); }
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

  04/02/2023 1.0.2: Fixed bug preventing custom hash table load factors from taking effect when CC_LOAD is defined in
                    isolation of CC_HASH, CC_CMPR, or CC_DTOR.
                    Made minor adjustment to code comments and documentation so that they are more consistent.
  27/01/2023 1.0.1: Made minor corrections to code comments.
  26/12/2022 1.0.0: Initial release.

License (MIT):

  Copyright (c) 2022-2023 Jackson L. Allan

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
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

#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>
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

// In GCC and Clang, we can generate a warning if the user passes an expression that may have side effects as the first
// argument of API macros.
// If the expression could have side effects, the compiler will not able able to resolve a comparision of it with itself
// at compile time, which we can check using __builtin_constant_p.
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

// CC_SELECT_ON_NUM_ARGS macro for overloading API macros based on number of arguments.
#define CC_CAT_( a, b ) a##b
#define CC_CAT( a, b ) CC_CAT_( a, b )
#define CC_N_ARGS_( _1, _2, _3, _4, _5, _6, n, ... ) n
#define CC_N_ARGS( ... ) CC_N_ARGS_( __VA_ARGS__, _6, _5, _4, _3, _2, _1, x )
#define CC_SELECT_ON_NUM_ARGS( func, ... ) CC_CAT( func, CC_N_ARGS( __VA_ARGS__ ) )( __VA_ARGS__ )

// Typeof for expressions and abstract declarations.
#ifdef __cplusplus
#define CC_TYPEOF_XP( xp ) std::decay<std::remove_reference<decltype( xp )>::type>::type
#define CC_TYPEOF_TY( ty ) std::decay<std::remove_reference<decltype( std::declval<ty>() )>::type>::type
#else
// TODO: Add C23 check once C23 is supported by major compilers.
#define CC_TYPEOF_XP( xp ) __typeof__( xp )
#define CC_TYPEOF_TY( ty ) __typeof__( ty )
#endif

// Macro used with CC_STATIC_ASSERT to provide type safety in cc_init_clone and cc_splice calls.
#ifdef __cplusplus
#define CC_IS_SAME_TY( a, b ) std::is_same<CC_TYPEOF_XP( a ), CC_TYPEOF_XP( b )>::value
#else
#define CC_IS_SAME_TY( a, b ) _Generic( (a), CC_TYPEOF_XP( b ): true, default: false )
#endif

// Macro used primarily for silencing unused-expression warnings for macros that return cast pointers.
// This issued seems to affect Clang in particular.
// GCC, on the other hand, seems to accept that pointer casts may be redundant.
#ifdef __cplusplus
template<typename ty_1, typename ty_2> ty_1 cc_maybe_unused( ty_2 xp ){ return (ty_1)xp; }
#define CC_CAST_MAYBE_UNUSED( ty, xp ) cc_maybe_unused<ty>( xp )
#else
#define CC_CAST_MAYBE_UNUSED( ty, xp ) ( ( ty ){ 0 } = ( (ty)( xp ) ) )
#endif

// Some functions that must return true/false must return the value in the form of a pointer.
// This is because they are paired in ternary expressions inside API macros with other functions for other containers
// that return a pointer (primarily cc_erase).
// While any suitably aligned pointer - e.g. the container handle - would do, we declare a global cc_dummy_true_ptr for
// the sake of code readability.
static max_align_t cc_dummy_true;
static void *cc_dummy_true_ptr = &cc_dummy_true;

// Container ids to identify container type at compile-time.
#define CC_VEC  1
#define CC_LIST 2
#define CC_MAP  3
#define CC_SET  4

// Produces underlying function pointer type for a given element/key type pair.
#define CC_MAKE_BASE_FNPTR_TY( el_ty, key_ty ) CC_TYPEOF_TY( CC_TYPEOF_TY( el_ty ) (*)( CC_TYPEOF_TY( key_ty )* ) )

// Produces container handle for a given element type, key type, and container id.
// In other words, this macro creates a pointer that carries all the information needed to identify and operate on a
// container at compile time: el_ty (*(*)[ cntr_id ])( key_ty * ).
// That is a pointer to an array of function pointers whose signature is el_ty ( key_ty * ).
#define CC_MAKE_CNTR_TY( el_ty, key_ty, cntr_id ) CC_TYPEOF_TY( CC_MAKE_BASE_FNPTR_TY( el_ty, key_ty )(*)[ cntr_id ] )

// Dummy type used as key type in containers that don't use keys.
// This greatly reduces the complexity of API macros.
typedef struct{ char nothing; } cc_dummy_ty;

// API macros for declaring containers.

#define cc_vec( el_ty )         CC_MAKE_CNTR_TY( el_ty, cc_dummy_ty, CC_VEC )

#define cc_list( el_ty )        CC_MAKE_CNTR_TY( el_ty, cc_dummy_ty, CC_LIST )

#define cc_map( key_ty, el_ty ) CC_MAKE_CNTR_TY(                                                           \
                                  el_ty,                                                                   \
                                  key_ty,                                                                  \
                                  /* Compiler error if key type lacks compare and hash functions. */       \
                                  CC_MAP * ( ( CC_HAS_CMPR( key_ty ) && CC_HAS_HASH( key_ty ) ) ? 1 : -1 ) \
                                )                                                                          \

#define cc_set( el_ty )         CC_MAKE_CNTR_TY(                                                                   \
                                  /* As set simply wraps map, we use el_ty as both the element and key types. */   \
                                  /* This allows minimal changes to map macros and functions to make sets work. */ \
                                  el_ty,                                                                           \
                                  el_ty,                                                                           \
                                  /* Compiler error if el type lacks compare and hash functions. */                \
                                  CC_SET * ( ( CC_HAS_CMPR( el_ty ) && CC_HAS_HASH( el_ty ) ) ? 1 : -1 )           \
                                )                                                                                  \

// Retrieves a container's id (CC_VEC, CC_LIST, etc.) from its handle.
#define CC_CNTR_ID( cntr ) ( sizeof( *cntr ) / sizeof( **cntr ) )

// Retrieves a container's element type from its handle.
#define CC_EL_TY( cntr ) CC_TYPEOF_XP( (**cntr)( NULL ) )

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
      // coupled with a compare function.
      // Returns cc_dummy_t if no compare function for type has been defined.

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
      default: (cc_dummy_ty){ 0 }                                                                 \
    )                                                                                             \
  )                                                                                               \
)                                                                                                 \

#endif

// Macros to provide easy access to a container's element and key sizes.
#define CC_EL_SIZE( cntr )  sizeof( CC_EL_TY( cntr ) )
#define CC_KEY_SIZE( cntr ) sizeof( CC_KEY_TY( cntr ) )

// Macro to round up a number to a multiple of a factor.
// This is used to determine bucket size and offsets in sets and maps.
#define CC_ROUND_UP( n, factor ) ( ( ( (n) + (factor) - 1 ) / (factor) ) * (factor) )

// If the user has defined CC_REALLOC and CC_FREE, then CC_GET_REALLOC and CC_GET_FREE are replaced with those macros.
// Otherwise, they are replaced by realloc and free from the standard library.
#define CC_ARG_2_( _1, _2, ... ) _2
#define CC_ARG_2( ... ) CC_ARG_2_( __VA_ARGS__ )
#define CC_REALLOC_COMMA ,
#define CC_GET_REALLOC CC_ARG_2( CC_CAT( CC_REALLOC, _COMMA ) realloc, CC_REALLOC, )
#define CC_FREE_COMMA ,
#define CC_GET_FREE CC_ARG_2( CC_CAT( CC_FREE, _COMMA ) free, CC_FREE, )

// Default max load factor for maps and sets.
#define CC_DEFAULT_LOAD 0.8

// Swaps a block of memory (used for Robin-Hooding in maps and sets).
static inline void cc_memswap( void *a, void *b, size_t size )
{
  for( size_t i = 0; i < size; ++i )
  {
    char temp = ( (char *)a )[ i ];
    ( (char *)a )[ i ] = ( (char *)b )[ i ];
    ( (char *)b )[ i ] = temp;
  }
}

// CC_MAKE_LVAL_COPY macro for making an addressable temporary copy of a variable or expression.
// The copy is valid until at least the end of full expression surrounding the macro call.
// In C, this is accomplished using a compound literal.
// In C++, we use rvalue reference magic.
// This is used, for example, to pass a pointer to an element (which the user may have provided as an rvalue) into an
// insert function.
#ifdef __cplusplus
template<class ty> ty& cc_unmove( ty&& var ) { return var; }
#define CC_MAKE_LVAL_COPY( ty, xp ) cc_unmove( ty( xp ) )
#else
#define CC_MAKE_LVAL_COPY( ty, xp ) *( ty[ 1 ] ){ xp }
#endif

// CC_IF_THEN_XP_ELSE_DUMMY macro for allowing an expression only if a condition is true.
// If not true, then the macro returns a dummy value of a specified type.
// This macros allows API macros to have arguments whose type depends on the container id where the different container
// types are incompatible.
// Without it, those macros could not compile.
// In other words, this macro achieves SFINAE-like functionality.
#ifdef __cplusplus

template<bool cond, class ty, class xp_ty, typename std::enable_if<cond, bool>::type = true>
xp_ty cc_if_then_xp_else_dummy( xp_ty xp ){ return xp; }

template<bool cond, class ty, class xp_ty, typename std::enable_if<!cond, bool>::type = true>
ty cc_if_then_xp_else_dummy( xp_ty xp ){ return ty(); }

#define CC_IF_THEN_XP_ELSE_DUMMY( cond, xp, ty ) ( cc_if_then_xp_else_dummy<cond, ty>( xp ) )

#else

#define CC_IF_THEN_XP_ELSE_DUMMY( cond, xp, dummy_type ) \
_Generic( (char (*)[ 1 + (bool)( cond ) ]){ 0 },         \
  char (*)[ 1 ]: (dummy_type){ 0 },                      \
  char (*)[ 2 ]: xp                                      \
)                                                        \

#endif

// CC_IF_THEN_PTR_TO_BOOL_ELSE_PTR macro for casting a pointer to bool only if a condition is true.
// It is necessary because some API macros (e.g. cc_erase) should return a bool to the user for certain containers and a
// pointer to an element for others.
// With this macro, all the corresponding internal functions can return pointers, and the pointer is cast to bool at the
// highest level if dictated by the container id.
#ifdef __cplusplus

template<bool cond, class ptr_ty, typename std::enable_if<cond, bool>::type = true> \
bool cc_if_then_ptr_to_bool_else_ptr( ptr_ty ptr ){ return ptr; }                   \

template<bool cond, class ptr_ty, typename std::enable_if<!cond, bool>::type = true> \
ptr_ty cc_if_then_ptr_to_bool_else_ptr( ptr_ty ptr ){ return ptr; }                  \

#define CC_IF_THEN_PTR_TO_BOOL_ELSE_PTR( cond, ptr ) \
cc_if_then_ptr_to_bool_else_ptr<cond>( ptr )         \

#else

#define CC_IF_THEN_PTR_TO_BOOL_ELSE_PTR( cond, ptr ) \
_Generic( (char (*)[ 1 + (bool)( cond ) ]){ 0 },     \
    char (*)[ 1 ]: ptr,                              \
    char (*)[ 2 ]: CC_CAST_MAYBE_UNUSED( bool, ptr ) \
)                                                    \

#endif

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

// Helper function for one-line cc_allocing_fn_result_ty.
static inline cc_allocing_fn_result_ty cc_make_allocing_fn_result( void *new_cntr, void *other_ptr )
{
  cc_allocing_fn_result_ty result;
  result.new_cntr = new_cntr;
  result.other_ptr = other_ptr;
  return result;
}

// Performs memcpy and returns ptr.
// Used in conjunction with cc_allocing_fn_result_ty (see below).
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

#define CC_VEC_INIT( cntr )                         \
(                                                   \
  cntr = (CC_TYPEOF_XP( cntr ))&cc_vec_placeholder, \
  (void)0                                           \
)                                                   \

// Provides easy access to a vector's header.
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

// Returns a pointer-iterator to the element at index i.
static inline void *cc_vec_get( void *cntr, size_t i, size_t el_size )
{
  return (char *)cntr + sizeof( cc_vec_hdr_ty ) + el_size * i;
}

#define CC_VEC_GET( cntr, i ) cc_vec_get( cntr, i, CC_EL_SIZE( cntr ) )

// Ensures that the capacity is large enough to support n elements without reallocation.
// Returns a cc_allocing_fn_result_ty containing the new handle and a pointer that evaluates to true if the operation was
// successful.
static inline cc_allocing_fn_result_ty cc_vec_reserve(
  void *cntr,
  size_t n,
  size_t el_size,
  void *(*realloc_)( void *, size_t )
)
{
  if( cc_vec_cap( cntr ) >= n )
    return cc_make_allocing_fn_result( cntr, cc_dummy_true_ptr );

  bool is_placeholder = cc_vec_is_placeholder( cntr );

  cc_vec_hdr_ty *new_cntr = (cc_vec_hdr_ty *)realloc_(
    is_placeholder ? NULL : cntr,
    sizeof( cc_vec_hdr_ty ) + el_size * n
  );

  if( !new_cntr )
    return cc_make_allocing_fn_result( cntr, NULL );

  if( is_placeholder )
    new_cntr->size = 0;

  new_cntr->cap = n;
  return cc_make_allocing_fn_result( new_cntr, cc_dummy_true_ptr );
}

#define CC_VEC_RESERVE( cntr, n )                                 \
(                                                                 \
  CC_POINT_HNDL_TO_ALLOCING_FN_RESULT(                            \
    cntr,                                                         \
    cc_vec_reserve( cntr, n, CC_EL_SIZE( cntr ), CC_GET_REALLOC ) \
  ),                                                              \
  (bool)CC_FIX_HNDL_AND_RETURN_OTHER_PTR( cntr )                  \
)                                                                 \

// Inserts elements at index i.
// Returns a cc_allocing_fn_result_ty containing the new handle and a pointer-iterator to the newly inserted elements.
// If the underlying storage needed to be expanded and an allocation failure occurred, or if n is zero, the latter
// pointer will be NULL.
static inline cc_allocing_fn_result_ty cc_vec_insert(
  void *cntr,
  size_t i,
  void *els,
  size_t n,
  size_t el_size,
  void *( *realloc_ )( void *, size_t )
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

    cc_allocing_fn_result_ty result = cc_vec_reserve( cntr, cap, el_size, realloc_ );
    if( !result.other_ptr )
      return result;

    cntr = result.new_cntr;
  }

  void *new_els = cc_vec_get( cntr, i, el_size );
  memmove( cc_vec_get( cntr, i + n, el_size ), new_els, el_size * ( cc_vec_hdr( cntr )->size - i ) );
  memcpy( new_els, els, el_size * n );
  cc_vec_hdr( cntr )->size += n;

  return cc_make_allocing_fn_result( cntr, new_els );
}

#define CC_VEC_INSERT_N( cntr, i, els, n )                               \
(                                                                        \
  CC_POINT_HNDL_TO_ALLOCING_FN_RESULT(                                   \
    cntr,                                                                \
    cc_vec_insert( cntr, i, els, n, CC_EL_SIZE( cntr ), CC_GET_REALLOC ) \
  ),                                                                     \
  CC_FIX_HNDL_AND_RETURN_OTHER_PTR( cntr )                               \
)                                                                        \

#define CC_VEC_INSERT( cntr, i, el ) CC_VEC_INSERT_N( cntr, i, &CC_MAKE_LVAL_COPY( CC_EL_TY( cntr ), el ), 1 )

#define CC_VEC_PUSH_N( cntr, els, n ) CC_VEC_INSERT_N( cntr, cc_vec_size( cntr ), els, n )

#define CC_VEC_PUSH( cntr, el ) CC_VEC_PUSH_N( cntr, &CC_MAKE_LVAL_COPY( CC_EL_TY( cntr ), el ), 1 )

// Erases n elements at index i.
// Returns a pointer-iterator to the element after the erased elements, or an end pointer-iterator if there is no
// subsequent element.
static inline void *cc_vec_erase_n( void *cntr, size_t i, size_t n, size_t el_size, void ( *dtor )( void * ) )
{
  if( n == 0 )
    return cc_vec_get( cntr, i, el_size );

  if( dtor )
    for( size_t j = 0; j < n; ++j )
      dtor( cc_vec_get( cntr, i + j, el_size )  );

  memmove(
    cc_vec_get( cntr, i, el_size ),
    cc_vec_get( cntr, i + n, el_size ),
    ( cc_vec_hdr( cntr )->size - n - i ) * el_size
  );

  cc_vec_hdr( cntr )->size -= n;
  return cc_vec_get( cntr, i, el_size );
}

#define CC_VEC_ERASE_N( cntr, i, n ) cc_vec_erase_n( cntr, i, n, CC_EL_SIZE( cntr ), CC_EL_DTOR( cntr ) )

#define CC_VEC_ERASE( cntr, i ) CC_VEC_ERASE_N( cntr, i, 1 )

// Sets the number of elements in the vector.
// If n is below the current size, then the destructor is called for all erased elements.
// In this case, the vector's capacity is not changed.
// If n is above the current size, the new elements are uninitialized.
// Returns a cc_allocing_fn_result_ty containing new container handle and a pointer that evaluates to true if the
// operation was successful and false in the case of allocation failure.
static inline cc_allocing_fn_result_ty cc_vec_resize(
  void *cntr,
  size_t n,
  size_t el_size,
  void ( *dtor )( void * ),
  void *( *realloc_ )( void *, size_t )
)
{
  // No resize necessary (also handles placeholder).
  if( n == cc_vec_size( cntr ) )
    return cc_make_allocing_fn_result( cntr, cc_dummy_true_ptr );

  // Downsizing.
  if( n < cc_vec_size( cntr ) )
  {
    cc_vec_erase_n( cntr, n, cc_vec_size( cntr ) - n, el_size, dtor );
    return cc_make_allocing_fn_result( cntr, cc_dummy_true_ptr );
  }

  // Up-sizing.
  cc_allocing_fn_result_ty result = cc_vec_reserve( cntr, n, el_size, realloc_ );
  if( !result.other_ptr )
    return result;

  cc_vec_hdr( result.new_cntr )->size = n;

  return result;
}

#define CC_VEC_RESIZE( cntr, n )                                                     \
(                                                                                    \
  CC_POINT_HNDL_TO_ALLOCING_FN_RESULT(                                               \
    cntr,                                                                            \
    cc_vec_resize( cntr, n, CC_EL_SIZE( cntr ), CC_EL_DTOR( cntr ), CC_GET_REALLOC ) \
  ),                                                                                 \
  (bool)CC_FIX_HNDL_AND_RETURN_OTHER_PTR( cntr )                                     \
)                                                                                    \

// Shrinks vector's capacity to its current size.
// Returns a cc_allocing_fn_result_ty containing the new container handle and a pointer that evaluates to true if the
// operation was successful and false in the case of allocation failure.
static inline cc_allocing_fn_result_ty cc_vec_shrink(
  void *cntr,
  size_t el_size,
  void *( *realloc_ )( void *, size_t ),
  void ( *free_ )( void * )
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
  if( !new_cntr )
    return cc_make_allocing_fn_result( cntr, NULL );

  cc_vec_hdr( new_cntr )->cap = cc_vec_size( new_cntr );
  return cc_make_allocing_fn_result( new_cntr, cc_dummy_true_ptr );
}

#define CC_VEC_SHRINK( cntr )                                              \
(                                                                          \
  CC_POINT_HNDL_TO_ALLOCING_FN_RESULT(                                     \
    cntr,                                                                  \
    cc_vec_shrink( cntr, CC_EL_SIZE( cntr ), CC_GET_REALLOC, CC_GET_FREE ) \
  ),                                                                       \
  (bool)CC_FIX_HNDL_AND_RETURN_OTHER_PTR( cntr )                           \
)                                                                          \

// Initializes a shallow copy of the source vector.
// The capacity of the new vector is the size of the source vector, not its capacity.
// Returns a the pointer to the copy, or NULL in the case of allocation failure.
// That return value is cast to bool in the corresponding macro.
static inline void *cc_vec_init_clone( void *src, size_t el_size, void *( *realloc_ )( void *, size_t ) )
{
  if( cc_vec_size( src ) == 0 )
    return (void *)&cc_vec_placeholder;

  cc_allocing_fn_result_ty result = cc_vec_resize(
    (void *)&cc_vec_placeholder,
    cc_vec_size( src ),
    el_size,
    NULL, // dtor unused.
    realloc_
  );

  if( !result.other_ptr )
    return NULL;

  memcpy( cc_vec_get( result.new_cntr, 0, el_size ), cc_vec_get( src, 0, el_size ), el_size * cc_vec_size( src ) );
  return result.new_cntr;
}

#define CC_VEC_INIT_CLONE( cntr, src )                                                        \
( cntr = (CC_TYPEOF_XP( cntr ))cc_vec_init_clone( src, CC_EL_SIZE( cntr ), CC_GET_REALLOC ) ) \

// Erases all elements, calling the destructors if necessary, without changing the vector's capacity.
void cc_vec_clear( void *cntr, size_t el_size, void (*dtor)( void * ) )
{
  cc_vec_erase_n( cntr, 0, cc_vec_size( cntr ), el_size, dtor );
}

#define CC_VEC_CLEAR( cntr ) cc_vec_clear( cntr, CC_EL_SIZE( cntr ), CC_EL_DTOR( cntr ) )

// Clears the vector and frees its memory if it is not a placeholder.
void cc_vec_cleanup( void *cntr, size_t el_size, void (*dtor)( void * ), void (*free_)( void * ) )
{
  cc_vec_clear( cntr, el_size, dtor );

  if( !cc_vec_is_placeholder( cntr ) )
    free_( cntr );
}

#define CC_VEC_CLEANUP( cntr )                                                 \
(                                                                              \
  cc_vec_cleanup( cntr, CC_EL_SIZE( cntr ), CC_EL_DTOR( cntr ), CC_GET_FREE ), \
  CC_VEC_INIT( cntr )                                                          \
)                                                                              \

static inline void *cc_vec_first( void *cntr )
{
  return (char *)cntr + sizeof( cc_vec_hdr_ty );
}

static inline void *cc_vec_last( void *cntr, size_t el_size )
{
  return cc_vec_get( cntr, cc_vec_size( cntr ) - 1, el_size );
}

#define CC_VEC_LAST( cntr ) cc_vec_last( cntr, CC_EL_SIZE( cntr ) )

static inline void *cc_vec_end( void *cntr, size_t el_size )
{
  return cc_vec_get( cntr, cc_vec_size( cntr ), el_size );
}

#define CC_VEC_END( cntr ) cc_vec_end( cntr, CC_EL_SIZE( cntr ) )

static inline void *cc_vec_next( void *i, size_t el_size )
{
  return (char *)i + el_size;
}

#define CC_VEC_NEXT( cntr, i ) cc_vec_next( i, CC_EL_SIZE( cntr ) )

/*--------------------------------------------------------------------------------------------------------------------*/
/*                                                        List                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/

// List is implemented a doubly linked list with sentinel nodes.

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
  cc_listnode_hdr_ty  r_end;
  cc_listnode_hdr_ty  end;
} cc_list_hdr_ty;

// Placeholder for list with no allocated header.
// The main purpose this serves is to provide every list with stable r_end and end iterators across translation units
// and irrespective of whether any memory has been allocated for its header.
// Every list initially (after cc_init) points to this placeholder, which differs across translation units, and is then
// associated with that placeholder until cc_cleanup is called.
// Calls to cc_r_end and cc_end on a list return pointers to the associated placeholder's r_end and end elements even
// after a header has been dynamically allocated to it.
// The way this works is that the placeholder's r_end.prev pointer points to the placeholder's r_end (i.e. a circular
// link), and ditto for end.next.
// Meanwhile, a list with an allocated header has it's r_end.prev pointer point to the placeholder's r_end, and ditto
// for it's end.next pointer.
// Therefore cc_list_hdr( cntr )->r_end.prev always produces the associated placeholder's r_end, and ditto for
// cc_list_hdr( cntr )->end.next.
// Unfortunately, this means that r_end and end must be handled as special cases during inserts, splices, and iteration.
const static cc_list_hdr_ty cc_list_placeholder = {
  0,
  {
    (cc_listnode_hdr_ty *)&cc_list_placeholder.r_end /* Circular link */,
    (cc_listnode_hdr_ty *)&cc_list_placeholder.end
  },
  {
    (cc_listnode_hdr_ty *)&cc_list_placeholder.r_end,
    (cc_listnode_hdr_ty *)&cc_list_placeholder.end /* Circular link */
  }
};

#define CC_LIST_INIT( cntr )                         \
(                                                    \
  cntr = (CC_TYPEOF_XP( cntr ))&cc_list_placeholder, \
  (void)0                                            \
)                                                    \

// Provides easy access to list header.
static inline cc_list_hdr_ty *cc_list_hdr( void *cntr )
{
  return (cc_list_hdr_ty *)cntr;
}

// Provides easy access to a list node header from a pointer-iterator.
static inline cc_listnode_hdr_ty *cc_listnode_hdr( void *i )
{
  return (cc_listnode_hdr_ty *)( (char *)i - sizeof( cc_listnode_hdr_ty ) );
}

// Provides easy access to a pointer-iterator from pointer to a list node header.
static inline void *cc_list_el( void *i )
{
  return (char *)i + sizeof( cc_listnode_hdr_ty );
}

static inline bool cc_list_is_placeholder( void *cntr )
{
  return cc_list_hdr( cntr )->r_end.prev == &cc_list_hdr( cntr )->r_end;
}

// Iteration.

static inline void *cc_list_r_end( void *cntr )
{
  return cc_list_el( cc_list_hdr( cntr )->r_end.prev );
}

static inline void *cc_list_end( void *cntr )
{
  return cc_list_el( cc_list_hdr( cntr )->end.next );
}

static inline void *cc_list_prev( void *cntr, void *i )
{
  cc_listnode_hdr_ty *prev = cc_listnode_hdr( i )->prev;

  // If i is r_end, then we need to decrement the iterator once more to ensure that the returned iterator is the r_end
  // of the placeholder originally associated with the list.
  if( prev == &cc_list_hdr( cntr )->r_end )
    prev = prev->prev;

  return cc_list_el( prev );
}

static inline void *cc_list_next( void *cntr, void *i )
{
  cc_listnode_hdr_ty *next = cc_listnode_hdr( i )->next;

  // See comment in cc_list_prev above.
  if( next == &cc_list_hdr( cntr )->end )
    next = next->next;

  return cc_list_el( next );
}

static inline void *cc_list_first( void *cntr )
{
  return cc_list_next( cntr, cc_list_el( &cc_list_hdr( cntr )->r_end ) );
}

static inline void *cc_list_last( void *cntr )
{
  return cc_list_prev( cntr, cc_list_el( &cc_list_hdr( cntr )->end ) );
}

static inline size_t cc_list_size( void *cntr )
{
  return cc_list_hdr( cntr )->size;
}

// Allocates a header for a list that is currently a placeholder.
// Returns the new container handle, or NULL in the case of allocation failure.
static inline void *cc_list_alloc_hdr( void *cntr, void *( *realloc_ )( void *, size_t ) )
{
  cc_list_hdr_ty *new_cntr = (cc_list_hdr_ty *)realloc_( NULL, sizeof( cc_list_hdr_ty ) );
  if( !new_cntr )
    return NULL;

  new_cntr->r_end.next = &new_cntr->end;
  new_cntr->end.prev = &new_cntr->r_end;

  // Link the new header's r_end and end with the original placeholder's r_end and end.
  new_cntr->r_end.prev = &cc_list_hdr( cntr )->r_end;
  new_cntr->end.next = &cc_list_hdr( cntr )->end;

  new_cntr->size = 0; 
  return new_cntr;
}

// Attaches a node to the list before the node pointed to be pointer-iterator i.
static inline void cc_list_attach( void *cntr, void *i, cc_listnode_hdr_ty *node )
{
  // Handle r_end and end iterators as a special case.
  // We need to convert the iterator from the global placeholder's r_end or end to the local r_end or end.
  if( i == cc_list_r_end( cntr ) )
    i = cc_list_el( &cc_list_hdr( cntr )->r_end );
  else if( i == cc_list_end( cntr ) )
    i = cc_list_el( &cc_list_hdr( cntr )->end );

  // Link node.
  node->next = cc_listnode_hdr( i );
  node->prev = node->next->prev;
  node->next->prev = node;
  node->prev->next = node;
}

// Inserts an element into the list before before the node pointed to be pointer-iterator i.
// Returns a cc_allocing_fn_result_ty containing the new container handle and a pointer-iterator to the newly inserted
// element (or NULL in the case of allocation failure).
static inline cc_allocing_fn_result_ty cc_list_insert(
  void *cntr,
  void *i,
  void *el,
  size_t el_size,
  void *( *realloc_ )( void *, size_t )
)
{
  if( cc_list_is_placeholder( cntr ) )
  {
    void *new_cntr = cc_list_alloc_hdr( cntr, realloc_ );
    if( !new_cntr )
      return cc_make_allocing_fn_result( cntr, NULL );

    cntr = new_cntr;
  }

  cc_listnode_hdr_ty *new_node = (cc_listnode_hdr_ty *)realloc_( NULL, sizeof( cc_listnode_hdr_ty ) + el_size );
  if( !new_node )
    return cc_make_allocing_fn_result( cntr, NULL );

  memcpy( cc_list_el( new_node ), el, el_size );

  // Handle r_end and end iterators as a special case.
  // We need to convert the iterator from the associated placeholder's r_end or end to the local r_end or end.
  if( i == cc_list_r_end( cntr ) )
    i = cc_list_el( &cc_list_hdr( cntr )->r_end );
  else if( i == cc_list_end( cntr ) )
    i = cc_list_el( &cc_list_hdr( cntr )->end );

  cc_list_attach( cntr, i, new_node );

  ++cc_list_hdr( cntr )->size;

  return cc_make_allocing_fn_result( cntr, cc_list_el( new_node ) );
}

#define CC_LIST_INSERT( cntr, i, el )                                                                         \
(                                                                                                             \
  CC_POINT_HNDL_TO_ALLOCING_FN_RESULT(                                                                        \
    cntr,                                                                                                     \
    cc_list_insert( cntr, i, &CC_MAKE_LVAL_COPY( CC_EL_TY( cntr ), el ), CC_EL_SIZE( cntr ), CC_GET_REALLOC ) \
  ),                                                                                                          \
  CC_FIX_HNDL_AND_RETURN_OTHER_PTR( cntr )                                                                    \
)                                                                                                             \

#define CC_LIST_PUSH( cntr, el ) CC_LIST_INSERT( cntr, cc_list_end( cntr ), el )

// Erases the element pointed to by pointer-iterator i and returns a pointer-iterator to next element (or end if the
// element was the last element in the list.)
static inline void *cc_list_erase( void *cntr, void *i, void ( *dtor )( void * ), void ( *free_ )( void * ) )
{
  cc_listnode_hdr_ty *hdr = cc_listnode_hdr( i );
  cc_listnode_hdr_ty *next = hdr->next;
  hdr->prev->next = next;
  next->prev = hdr->prev;
  
  if( dtor )
    dtor( i );

  free_( hdr );
  --cc_list_hdr( cntr )->size;

  // If next is end, we need to make sure we're returning the associated placeholder's end.
  if( next == &cc_list_hdr( cntr )->end )
    next = next->next;

  return cc_list_el( next );
}

#define CC_LIST_ERASE( cntr, i ) cc_list_erase( cntr, i, CC_EL_DTOR( cntr ), CC_GET_FREE )

// Removes the element pointed to by pointer-iterator src_i from the source list and attaches it to the list.
// Although this function never allocates memory for the element/node itself, it must allocate the list's header if the
// list is currently a placeholder.
// Returns a cc_allocing_fn_result_ty containing the new container handle and a pointer that evaluates to true if the
// operation was successful or false in the case of allocation failure.
static inline cc_allocing_fn_result_ty cc_list_splice(
  void *cntr,
  void *i,
  void *src,
  void *src_i,
  void *( *realloc_ )( void *, size_t )
)
{
  if( cc_list_is_placeholder( cntr ) )
  {
    void *new_cntr = cc_list_alloc_hdr( cntr, realloc_ );
    if( !new_cntr )
      return cc_make_allocing_fn_result( cntr, NULL );

    cntr = new_cntr;
  }

  cc_listnode_hdr( src_i )->prev->next = cc_listnode_hdr( src_i )->next;
  cc_listnode_hdr( src_i )->next->prev = cc_listnode_hdr( src_i )->prev;
  cc_list_attach( cntr, i, cc_listnode_hdr( src_i ) );

  --cc_list_hdr( src )->size;
  ++cc_list_hdr( cntr )->size;

  return cc_make_allocing_fn_result( cntr, cc_dummy_true_ptr );
}

#define CC_LIST_SPLICE( cntr, i, src, src_i )             \
(                                                         \
  CC_POINT_HNDL_TO_ALLOCING_FN_RESULT(                    \
    cntr,                                                 \
    cc_list_splice( cntr, i, src, src_i, CC_GET_REALLOC ) \
  ),                                                      \
  CC_FIX_HNDL_AND_RETURN_OTHER_PTR( cntr )                \
)                                                         \

// Initializes a shallow copy of the source list.
// This requires allocating memory for every node, as well as for the list's header unless src is a placeholder.
// Returns a the pointer to the copy, or NULL in the case of allocation failure.
// That return value is cast to bool in the corresponding macro.
static inline void *cc_list_init_clone(
  void *src,
  size_t el_size,
  void *( *realloc_ )( void *, size_t ),
  void ( *free_ )( void * )
)
{
  cc_allocing_fn_result_ty result = { (void *)&cc_list_placeholder, cc_dummy_true_ptr };
  for( void *i = cc_list_first( src ); i != cc_list_end( src ); i = cc_list_next( src, i ) )
  {
    result = cc_list_insert( result.new_cntr, cc_list_end( result.new_cntr ), i, el_size, realloc_ );
    if( !result.other_ptr )
    {
      // Erase incomplete clone without invoking destructors.
      
      void *j = cc_list_first( result.new_cntr );
      while( j != cc_list_end( result.new_cntr ) )
      {
        void *next = cc_list_next( result.new_cntr, j );
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

#define CC_LIST_INIT_CLONE( cntr, src )                                                                     \
( cntr = (CC_TYPEOF_XP( cntr ))cc_list_init_clone( src, CC_EL_SIZE( cntr ), CC_GET_REALLOC, CC_GET_FREE ) ) \

// Erases all elements, calling their destructors if necessary.
static inline void cc_list_clear( void *cntr, void ( *dtor )( void * ), void ( *free_ )( void * ) )
{
  while( cc_list_first( cntr ) != cc_list_end( cntr ) )
    cc_list_erase( cntr, cc_list_first( cntr ), dtor, free_ );
}

#define CC_LIST_CLEAR( cntr ) cc_list_clear( cntr, CC_EL_DTOR( cntr ), CC_GET_FREE )

// Erases all elements, calling their destructors if necessary, and frees memory for the list's header if it is not
// a placeholder.
static inline void cc_list_cleanup( void *cntr, void ( *dtor )( void * ), void ( *free_ )( void * ) )
{
  cc_list_clear( cntr, dtor, free_ );
  if( !cc_list_is_placeholder( cntr ) )
    free_( cntr );
}

#define CC_LIST_CLEANUP( cntr )                             \
(                                                           \
  cc_list_cleanup( cntr, CC_EL_DTOR( cntr ), CC_GET_FREE ), \
  CC_LIST_INIT( cntr )                                      \
)                                                           \

/*--------------------------------------------------------------------------------------------------------------------*/
/*                                                        Map                                                         */
/*--------------------------------------------------------------------------------------------------------------------*/

// Map is implemented as a Robin Hood (open-addressing) hash table with a power-of-2 capacity.

// Probe length type.
// An unsigned char would probably be fine, but we use unsigned int just in case.
// A probe length of 0 denotes an empty bucket, whereas a probe length of 1 denotes an element in its home bucket.
// This optimization allows us to eliminate separate checks for empty buckets.
typedef unsigned int cc_probelen_ty;

// Macros for calculating the position of element and probe length inside a bucket, as well as bucket size.
// The element is at the beginning of bucket.

#define CC_MAP_KEY_OFFSET( cntr ) CC_ROUND_UP( CC_EL_SIZE( cntr ), alignof( CC_KEY_TY( cntr ) ) )

#define CC_MAP_PROBELEN_OFFSET( cntr )                                                    \
CC_ROUND_UP( CC_MAP_KEY_OFFSET( cntr ) + CC_KEY_SIZE( cntr ), alignof( cc_probelen_ty ) ) \

#define CC_MAP_BUCKET_SIZE( cntr )                             \
CC_ROUND_UP(                                                   \
  CC_MAP_PROBELEN_OFFSET( cntr ) + sizeof( cc_probelen_ty ),   \
  alignof( CC_KEY_TY( cntr ) ) > alignof( CC_EL_TY( cntr ) ) ? \
    alignof( CC_KEY_TY( cntr ) )                               \
  :                                                            \
    alignof( CC_EL_TY( cntr ) )                                \
)                                                              \

// Map header.
typedef struct
{
  alignas( max_align_t )
  size_t size;
  size_t cap;
} cc_map_hdr_ty;

// Placeholder for map with no allocated memory.
// In the case of maps, this placeholder allows us to avoid checking for a NULL handle inside functions.
static const cc_map_hdr_ty cc_map_placeholder = { 0, 0 };

#define CC_MAP_INIT( cntr )                         \
(                                                   \
  cntr = (CC_TYPEOF_XP( cntr ))&cc_map_placeholder, \
  (void)0                                           \
)                                                   \

// Provides easy access to map header.
static inline cc_map_hdr_ty *cc_map_hdr( void *cntr )
{
  return (cc_map_hdr_ty *)cntr;
}

// Functions for easily accessing element, key, and probe length for the bucket at index i.
// The element pointer also denotes the beginning of the bucket.

static inline void *cc_map_el( void *cntr, size_t i, size_t bucket_size )
{
  return (char *)cntr + sizeof( cc_map_hdr_ty ) + bucket_size * i;
}

static inline void *cc_map_key( void *cntr, size_t i, size_t bucket_size, size_t key_offset )
{
  return (char *)cc_map_el( cntr, i, bucket_size ) + key_offset;
}

static inline cc_probelen_ty *cc_map_probelen( void *cntr, size_t i, size_t bucket_size, size_t probelen_offset )
{
  return (cc_probelen_ty *)( (char *)cc_map_el( cntr, i, bucket_size ) + probelen_offset );
}

static inline size_t cc_map_size( void *cntr )
{
  return cc_map_hdr( cntr )->size;
}

static inline size_t cc_map_cap( void *cntr )
{
  return cc_map_hdr( cntr )->cap;
}

static inline bool cc_map_is_placeholder( void *cntr )
{
  return cc_map_cap( cntr ) == 0;
}

// Inserts an element into the map.
// Assumes that the map has empty slots and therefore that failure cannot occur (hence the "raw" label).
// If replace is true, then el will replace any existing element with the same key.
// Returns a pointer-iterator to the newly inserted element, or to the existing element with the same key if replace is
// false.
// For the exact mechanics of Robin-Hood hashing, see Sebastian Sylvan's helpful article:
// www.sebastiansylvan.com/post/robin-hood-hashing-should-be-your-default-hash-table-implementation
static inline void *cc_map_insert_raw(
  void *cntr,
  void *el,
  void *key,
  bool replace,
  size_t bucket_size,
  size_t el_size,
  size_t key_offset,
  size_t key_size,
  size_t probelen_offset,
  int ( *cmpr )( void *, void *),
  size_t ( *hash )( void *),
  void ( *key_dtor )( void * ),
  void ( *el_dtor )( void * )
)
{
  size_t i = hash( key ) & ( cc_map_hdr( cntr )->cap - 1 );
  cc_probelen_ty probelen = 1;
  void *placed_original_el = NULL;

  while( true )
  {
    // Empty bucket.
    if( !*cc_map_probelen( cntr, i, bucket_size, probelen_offset ) )
    {
      memcpy( cc_map_key( cntr, i, bucket_size, key_offset ), key, key_size );
      memcpy( cc_map_el( cntr, i, bucket_size ), el, el_size );

      *cc_map_probelen( cntr, i, bucket_size, probelen_offset ) = probelen;

      ++cc_map_hdr( cntr )->size;

      return placed_original_el ? placed_original_el : cc_map_el( cntr, i, bucket_size ); 
    }

    // Existing element with same key.
    // This case can only occur before any steal occurs.
    if(
      probelen == *cc_map_probelen( cntr, i, bucket_size, probelen_offset ) &&
      cmpr( cc_map_key( cntr, i, bucket_size, key_offset ), key ) == 0
    )
    {
      if( replace )
      {
        if( key_dtor )
          key_dtor( cc_map_key( cntr, i, bucket_size, key_offset ) );

        if( el_dtor )
          el_dtor( cc_map_el( cntr, i, bucket_size ) );

        memcpy( cc_map_key( cntr, i, bucket_size, key_offset ), key, key_size );
        memcpy( cc_map_el( cntr, i, bucket_size ), el, el_size );
      }

      return cc_map_el( cntr, i, bucket_size );
    }

    // Stealing bucket.
    if( probelen > *cc_map_probelen( cntr, i, bucket_size, probelen_offset ) )
    {
      cc_memswap( key, cc_map_key( cntr, i, bucket_size, key_offset ), key_size );
      cc_memswap( el, cc_map_el( cntr, i, bucket_size ), el_size );

      cc_probelen_ty temp_probelen = *cc_map_probelen( cntr, i, bucket_size, probelen_offset );
      *cc_map_probelen( cntr, i, bucket_size, probelen_offset ) = probelen;
      probelen = temp_probelen;

      if( !placed_original_el )
        placed_original_el = cc_map_el( cntr, i, bucket_size );
    }

    i = ( i + 1 ) & ( cc_map_hdr( cntr )->cap - 1 );
    ++probelen;
  }
}

// Returns the minimum capacity required to accommodate n elements, which is governed by the max load factor associated
// with the map's key type.
static inline size_t cc_map_min_cap_for_n_els( size_t n, double max_load )
{
  if( n == 0 )
    return 0;

  // Round up to power of 2.
  size_t cap = 8;
  while( n > cap * max_load )
    cap *= 2;

  return cap;
}

// Creates a rehashed duplicate of cntr with capacity cap.
// Assumes that cap is large enough to accommodate all elements in cntr without violating the max load factor.
// Returns pointer to the duplicate, or NULL in the case of allocation failure.
static inline void *cc_map_make_rehash(
  void *cntr,
  size_t cap,
  size_t bucket_size,
  size_t el_size,
  size_t key_offset,
  size_t key_size,
  size_t probelen_offset,
  int ( *cmpr )( void *, void * ),
  size_t ( *hash )( void * ),
  void *( *realloc_ )( void *, size_t )
)
{
  cc_map_hdr_ty *new_cntr = (cc_map_hdr_ty *)realloc_( NULL, sizeof( cc_map_hdr_ty ) + bucket_size * cap );
  if( !new_cntr )
    return NULL;

  new_cntr->size = 0;
  new_cntr->cap = cap;
  for( size_t i = 0; i < cap; ++i )
    *cc_map_probelen( new_cntr, i, bucket_size, probelen_offset ) = 0;

  for( size_t i = 0; i < cc_map_hdr( cntr )->cap; ++i )
    if( *cc_map_probelen( cntr, i, bucket_size, probelen_offset ) )
      cc_map_insert_raw(
        new_cntr,
        cc_map_el( cntr, i, bucket_size ),
        cc_map_key( cntr, i, bucket_size, key_offset ),
        false, // No replacements can occur anyway as all keys already in the map are unique.
        bucket_size,
        el_size,
        key_offset,
        key_size,
        probelen_offset,
        cmpr,
        hash,
        // No need to pass destructors as no elements will be erased.
        NULL, 
        NULL
      );

  return new_cntr;
}

// Reserves capacity such that the map can accommodate n elements without reallocation (i.e. without violating the
// max load factor).
// Returns a cc_allocing_fn_result_ty containing new container handle and a pointer that evaluates to true if the
// operation successful or false in the case of allocation failure.
static inline cc_allocing_fn_result_ty cc_map_reserve(
  void *cntr,
  size_t n,
  size_t bucket_size,
  size_t el_size,
  size_t key_offset,
  size_t key_size,
  size_t probelen_offset,
  int ( *cmpr )( void *, void * ),
  size_t ( *hash )( void * ),
  double max_load,
  void *( *realloc_ )( void *, size_t ),
  void ( *free_ )( void * )
)
{
  size_t cap = cc_map_min_cap_for_n_els( n, max_load );

  if( cc_map_cap( cntr ) >= cap )
    return cc_make_allocing_fn_result( cntr, cc_dummy_true_ptr );

  void *new_cntr = cc_map_make_rehash(
    cntr,
    cap,
    bucket_size,
    el_size,
    key_offset,
    key_size,
    probelen_offset,
    cmpr,
    hash,
    realloc_
  );
  if( !new_cntr )
    return cc_make_allocing_fn_result( cntr, NULL );

  if( !cc_map_is_placeholder( cntr ) )
    free_( cntr );

  return cc_make_allocing_fn_result( new_cntr, cc_dummy_true_ptr );
}

#define CC_MAP_RESERVE( cntr, n )                \
(                                                \
  CC_POINT_HNDL_TO_ALLOCING_FN_RESULT(           \
    cntr,                                        \
    cc_map_reserve(                              \
      cntr,                                      \
      n,                                         \
      CC_MAP_BUCKET_SIZE( cntr ),                \
      CC_EL_SIZE( cntr ),                        \
      CC_MAP_KEY_OFFSET( cntr ),                 \
      CC_KEY_SIZE( cntr ),                       \
      CC_MAP_PROBELEN_OFFSET( cntr ),            \
      CC_KEY_CMPR( cntr ),                       \
      CC_KEY_HASH( cntr ),                       \
      CC_KEY_LOAD( cntr ),                       \
      CC_GET_REALLOC,                            \
      CC_GET_FREE                                \
    )                                            \
  ),                                             \
  (bool)CC_FIX_HNDL_AND_RETURN_OTHER_PTR( cntr ) \
)                                                \

// Inserts an element.
// If replace is true, then el replaces any existing element with the same key.
// If the map exceeds its load factor, the underlying storage is expanded and a complete rehash occurs.
// Returns a cc_allocing_fn_result_ty containing the new container handle and a pointer to the newly inserted element, or
// to the existing element with the same key if replace is false.
// If the underlying storage needed to be expanded and an allocation failure occurred, the latter pointer will be NULL.
// This function checks to ensure that the map could accommodate an insertion before searching for the existing element.
// Therefore, failure can occur even if an element with the same key already exists and no reallocation was actually
// necessary.
// This was a design choice in favor of code simplicity and readability over ideal behavior in a corner case.
static inline cc_allocing_fn_result_ty cc_map_insert(
  void *cntr,
  void *el,
  void *key,
  bool replace,
  size_t bucket_size,
  size_t el_size,
  size_t key_offset,
  size_t key_size,
  size_t probelen_offset,
  int (*cmpr)( void *, void * ),
  size_t (*hash)( void * ),
  double max_load,
  void ( *key_dtor )( void * ),
  void ( *el_dtor )( void * ),
  void *( *realloc_ )( void *, size_t ),
  void ( *free_ )( void * )
)
{
  if( cc_map_size( cntr ) + 1 > cc_map_cap( cntr ) * max_load )
  {
    cc_allocing_fn_result_ty result = cc_map_reserve(
      cntr,
      cc_map_size( cntr ) + 1,
      bucket_size,
      el_size,
      key_offset,
      key_size,
      probelen_offset,
      cmpr,
      hash,
      max_load,
      realloc_,
      free_
    );

    if( !result.other_ptr )
      return result;

    cntr = result.new_cntr;
  }

  void *new_el = cc_map_insert_raw(
    cntr,
    el,
    key,
    replace,
    bucket_size,
    el_size,
    key_offset,
    key_size,
    probelen_offset,
    cmpr,
    hash,
    key_dtor,
    el_dtor
  );

  return cc_make_allocing_fn_result( cntr, new_el );
}

#define CC_MAP_INSERT( cntr, key, el, replace )     \
(                                                   \
  CC_POINT_HNDL_TO_ALLOCING_FN_RESULT(              \
    cntr,                                           \
    cc_map_insert(                                  \
      cntr,                                         \
      &CC_MAKE_LVAL_COPY( CC_EL_TY( cntr ), el ),   \
      &CC_MAKE_LVAL_COPY( CC_KEY_TY( cntr ), key ), \
      replace,                                      \
      CC_MAP_BUCKET_SIZE( cntr ),                   \
      CC_EL_SIZE( cntr ),                           \
      CC_MAP_KEY_OFFSET( cntr ),                    \
      CC_KEY_SIZE( cntr ),                          \
      CC_MAP_PROBELEN_OFFSET( cntr ),               \
      CC_KEY_CMPR( cntr ),                          \
      CC_KEY_HASH( cntr ),                          \
      CC_KEY_LOAD( cntr ),                          \
      CC_KEY_DTOR( cntr ),                          \
      CC_EL_DTOR( cntr ),                           \
      CC_GET_REALLOC,                               \
      CC_GET_FREE                                   \
    )                                               \
  ),                                                \
  CC_FIX_HNDL_AND_RETURN_OTHER_PTR( cntr )          \
)                                                   \

// Returns a pointer-iterator to the element with the specified key, or NULL if no such element exists.
static inline void *cc_map_get(
  void *cntr,
  void *key,
  size_t bucket_size,
  size_t key_offset,
  size_t probelen_offset,
  int (*cmpr)( void *, void *),
  size_t (*hash)( void *) )
{
  if( cc_map_size( cntr ) == 0 )
    return NULL;

  size_t i = hash( key ) & ( cc_map_hdr( cntr )->cap - 1 );
  cc_probelen_ty probelen = 1;

  while( probelen <= *cc_map_probelen( cntr, i, bucket_size, probelen_offset ) )
  {
    if(
      probelen == *cc_map_probelen( cntr, i, bucket_size, probelen_offset ) &&
      cmpr( cc_map_key( cntr, i, bucket_size, key_offset ), key ) == 0
    )
      return cc_map_el( cntr, i, bucket_size );

    i = ( i + 1 ) & ( cc_map_hdr( cntr )->cap - 1 );
    ++probelen;
  }

  return NULL;
}

#define CC_MAP_GET( cntr, key )                 \
cc_map_get(                                     \
  cntr,                                         \
  &CC_MAKE_LVAL_COPY( CC_KEY_TY( cntr ), key ), \
  CC_MAP_BUCKET_SIZE( cntr ),                   \
  CC_MAP_KEY_OFFSET( cntr ),                    \
  CC_MAP_PROBELEN_OFFSET( cntr ),               \
  CC_KEY_CMPR( cntr ),                          \
  CC_KEY_HASH( cntr )                           \
)                                               \

// Returns a pointer to the key for the element pointed to by pointer-iterator i.
static inline void *cc_map_key_for( void *i, size_t key_offset )
{
  return (char *)i + key_offset;
}

#define CC_MAP_KEY_FOR( cntr, i ) cc_map_key_for( i, CC_MAP_KEY_OFFSET( cntr ) )

// Erases the element pointer to by pointer-iterator itr.
// For the exact mechanics of erasing elements in a Robin-Hood hash table, see Sebastian Sylvan's:
// www.sebastiansylvan.com/post/more-on-robin-hood-hashing-2/
static inline void cc_map_erase_itr(
  void *cntr,
  void *itr,
  size_t bucket_size,
  size_t el_size,
  size_t key_offset,
  size_t key_size,
  size_t probelen_offset,
  void ( *key_dtor )( void * ),
  void ( *el_dtor )( void * )
)
{
  size_t i = ( (char *)itr - (char *)cc_map_el( cntr, 0, bucket_size ) ) / bucket_size;
  *cc_map_probelen( cntr, i, bucket_size, probelen_offset ) = 0;
  --cc_map_hdr( cntr )->size;

  if( key_dtor )
    key_dtor( cc_map_key( cntr, i, bucket_size, key_offset ) );

  if( el_dtor )
    el_dtor( cc_map_el( cntr, i, bucket_size ) );

  while( true )
  {
    size_t next = ( i + 1 ) & ( cc_map_hdr( cntr )->cap - 1 );
    if( *cc_map_probelen( cntr, next, bucket_size, probelen_offset ) <= 1 )
      break; // Empty slot or key already in its home bucket, so all done.
    
    //Bump backwards.

    memcpy(
      cc_map_key( cntr, i, bucket_size, key_offset ),
      cc_map_key( cntr, next, bucket_size, key_offset ),
      key_size
    );
    memcpy( cc_map_el( cntr, i, bucket_size ), cc_map_el( cntr, next, bucket_size ), el_size );

    *cc_map_probelen( cntr, i, bucket_size, probelen_offset ) =
      *cc_map_probelen( cntr, next, bucket_size, probelen_offset ) - 1;
    *cc_map_probelen( cntr, next, bucket_size, probelen_offset ) = 0;

    i = next;
  }
}

#define CC_MAP_ERASE_ITR( cntr, i ) \
cc_map_erase_itr(                   \
  cntr,                             \
  i,                                \
  CC_MAP_BUCKET_SIZE( cntr ),       \
  CC_EL_SIZE( cntr ),               \
  CC_MAP_KEY_OFFSET( cntr ),        \
  CC_KEY_SIZE( cntr ),              \
  CC_MAP_PROBELEN_OFFSET( cntr ),   \
  CC_KEY_DTOR( cntr ),              \
  CC_EL_DTOR( cntr )                \
)                                   \


// Erases the element with the specified key, if it exists.
// Returns a pointer that evaluates to true if an element was erased, or else is NULL.
// This pointer is eventually cast to bool by the cc_erase API macro.
static inline void *cc_map_erase(
  void *cntr,
  void *key,
  size_t bucket_size,
  size_t el_size,
  size_t key_offset,
  size_t key_size,
  size_t probelen_offset,
  int ( *cmpr )( void *, void *),
  size_t ( *hash )( void * ),
  void ( *key_dtor )( void * ),
  void ( *el_dtor )( void * )
)
{
  if( cc_map_size( cntr ) == 0 )
    return NULL;

  size_t i = hash( key ) & ( cc_map_hdr( cntr )->cap - 1 );
  cc_probelen_ty probelen = 1;

  while( probelen <= *cc_map_probelen( cntr, i, bucket_size, probelen_offset ) )
  {
    if(
      probelen == *cc_map_probelen( cntr, i, bucket_size, probelen_offset ) &&
      cmpr( cc_map_key( cntr, i, bucket_size, key_offset ), key ) == 0
    )
    {
      cc_map_erase_itr(
        cntr,
        cc_map_el( cntr, i, bucket_size ),
        bucket_size,
        el_size,
        key_offset,
        key_size,
        probelen_offset,
        key_dtor,
        el_dtor
      );

      return cc_dummy_true_ptr;
    }

    i = ( i + 1 ) & ( cc_map_hdr( cntr )->cap - 1 );
    ++probelen;
  }

  return NULL;
}

#define CC_MAP_ERASE( cntr, key )               \
cc_map_erase(                                   \
  cntr,                                         \
  &CC_MAKE_LVAL_COPY( CC_KEY_TY( cntr ), key ), \
  CC_MAP_BUCKET_SIZE( cntr ),                   \
  CC_EL_SIZE( cntr ),                           \
  CC_MAP_KEY_OFFSET( cntr ),                    \
  CC_KEY_SIZE( cntr ),                          \
  CC_MAP_PROBELEN_OFFSET( cntr ),               \
  CC_KEY_CMPR( cntr ),                          \
  CC_KEY_HASH( cntr ),                          \
  CC_KEY_DTOR( cntr ),                          \
  CC_EL_DTOR( cntr )                            \
)                                               \

// Initializes a shallow copy of the source map.
// The capacity of the copy is the same as the capacity of the source map, unless the source map is empty, in which case
// the copy is a placeholder.
// Hence, this function does no rehashing.
// Returns a the pointer to the copy, or NULL in the case of allocation failure.
// That return value is cast to bool in the corresponding macro.
static inline void *cc_map_init_clone(
  void *src,
  size_t bucket_size,
  void *( *realloc_ )( void *, size_t )
)
{
  if( cc_map_size( src ) == 0 ) // Also handles placeholder.
    return (void *)&cc_map_placeholder;

  cc_map_hdr_ty *new_cntr = (cc_map_hdr_ty*)realloc_( NULL, sizeof( cc_map_hdr_ty ) + bucket_size * cc_map_cap( src ) );
  if( !new_cntr )
    return NULL;

  memcpy( new_cntr, src, sizeof( cc_map_hdr_ty ) + bucket_size * cc_map_cap( src ) );
  return new_cntr;
}

#define CC_MAP_INIT_CLONE( cntr, src )                                                                \
( cntr = (CC_TYPEOF_XP( cntr ))cc_map_init_clone( src, CC_MAP_BUCKET_SIZE( cntr ), CC_GET_REALLOC ) ) \

// Shrinks map's capacity to the minimum possible without violating the max load factor associated with the key type.
// If shrinking is necessary, then a complete rehash occurs.
// Returns a cc_allocing_fn_result_ty containing the new container handle and a pointer that evaluates to true if the
// operation was successful and false in the case of allocation failure.
static inline cc_allocing_fn_result_ty cc_map_shrink(
  void *cntr,
  size_t bucket_size,
  size_t el_size,
  size_t key_offset,
  size_t key_size,
  size_t probelen_offset,
  int ( *cmpr )( void *, void * ),
  size_t ( *hash )( void * ),
  double max_load,
  void *( *realloc_ )( void *, size_t ),
  void ( *free_ )( void * )
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
    bucket_size,
    el_size,
    key_offset,
    key_size,
    probelen_offset,
    cmpr,
    hash,
    realloc_
  );
  if( !new_cntr )
    return cc_make_allocing_fn_result( cntr, NULL );

  if( !cc_map_is_placeholder( cntr ) )
    free_( cntr );

  return cc_make_allocing_fn_result( new_cntr, cc_dummy_true_ptr );
}

#define CC_MAP_SHRINK( cntr )                    \
(                                                \
  CC_POINT_HNDL_TO_ALLOCING_FN_RESULT(           \
    cntr,                                        \
    cc_map_shrink(                               \
      cntr,                                      \
      CC_MAP_BUCKET_SIZE( cntr ),                \
      CC_EL_SIZE( cntr ),                        \
      CC_MAP_KEY_OFFSET( cntr ),                 \
      CC_KEY_SIZE( cntr ),                       \
      CC_MAP_PROBELEN_OFFSET( cntr ),            \
      CC_KEY_CMPR( cntr ),                       \
      CC_KEY_HASH( cntr ),                       \
      CC_KEY_LOAD( cntr ),                       \
      CC_GET_REALLOC,                            \
      CC_GET_FREE                                \
    )                                            \
  ),                                             \
  (bool)CC_FIX_HNDL_AND_RETURN_OTHER_PTR( cntr ) \
)                                                \

// Erases all elements, calling the destructors for the key and element types if necessary, without changing the map's
// capacity.
static inline void cc_map_clear(
  void *cntr,
  size_t bucket_size,
  size_t key_offset,
  size_t probelen_offset,
  void ( *key_dtor )( void * ),
  void ( *el_dtor )( void * )
)
{
  if( cc_map_size( cntr ) == 0 ) // Also handles placeholder map.
    return;

  for( size_t i = 0; i < cc_map_hdr( cntr )->cap; ++i )
    if( *cc_map_probelen( cntr, i, bucket_size, probelen_offset ) )
    {
      if( key_dtor )
        key_dtor( cc_map_key( cntr, i, bucket_size, key_offset ) );

      if( el_dtor )
        el_dtor( cc_map_el( cntr, i, bucket_size ) );

      *cc_map_probelen( cntr, i, bucket_size, probelen_offset ) = 0;
    }

  cc_map_hdr( cntr )->size = 0;
}

#define CC_MAP_CLEAR( cntr )      \
cc_map_clear(                     \
  cntr,                           \
  CC_MAP_BUCKET_SIZE( cntr ),     \
  CC_MAP_KEY_OFFSET( cntr ),      \
  CC_MAP_PROBELEN_OFFSET( cntr ), \
  CC_KEY_DTOR( cntr ),            \
  CC_EL_DTOR( cntr )              \
)                                 \

// Clears the map and frees its memory if is not placeholder.
static inline void cc_map_cleanup(
  void *cntr,
  size_t bucket_size,
  size_t key_offset,
  size_t probelen_offset,
  void (*key_dtor)( void * ),
  void (*el_dtor)( void * ),
  void (*free_)( void * )
)
{
  cc_map_clear( cntr, bucket_size, key_offset, probelen_offset, key_dtor, el_dtor );

  if( !cc_map_is_placeholder( cntr ) )
    free_( cntr );
}

#define CC_MAP_CLEANUP( cntr )      \
(                                   \
  cc_map_cleanup(                   \
    cntr,                           \
    CC_MAP_BUCKET_SIZE( cntr ),     \
    CC_MAP_KEY_OFFSET( cntr ),      \
    CC_MAP_PROBELEN_OFFSET( cntr ), \
    CC_KEY_DTOR( cntr ),            \
    CC_EL_DTOR( cntr ),             \
    CC_GET_FREE                     \
  ),                                \
  CC_MAP_INIT( cntr )               \
)                                   \

// For maps, the container handle doubles up as r_end.
static inline void *cc_map_r_end( void *cntr )
{
  return cntr;
}

// Returns a pointer-iterator to the end of the bucket array.
static inline void *cc_map_end( void *cntr, size_t bucket_size )
{
  return cc_map_el( cntr, cc_map_hdr( cntr )->cap, bucket_size );
}

#define CC_MAP_END( cntr ) cc_map_end( cntr, CC_MAP_BUCKET_SIZE( cntr ) )

// Returns a pointer-iterator to the first element, or end if the map is empty.
static inline void *cc_map_first( void *cntr, size_t bucket_size, size_t probelen_offset )
{
  for( size_t i = 0; i < cc_map_hdr( cntr )->cap; ++i )
    if( *cc_map_probelen( cntr, i, bucket_size, probelen_offset ) )
      return cc_map_el( cntr, i, bucket_size );

  return cc_map_end( cntr, bucket_size );
}

#define CC_MAP_FIRST( cntr ) cc_map_first( cntr, CC_MAP_BUCKET_SIZE( cntr ), CC_MAP_PROBELEN_OFFSET( cntr ) )

// Returns a pointer-iterator to the last element, or r_end if the map is empty.
static inline void *cc_map_last( void *cntr, size_t bucket_size, size_t probelen_offset )
{
  for( size_t i = cc_map_hdr( cntr )->cap; i-- > 0; )
    if( *cc_map_probelen( cntr, i, bucket_size, probelen_offset ) )
      return cc_map_el( cntr, i, bucket_size );

  return cc_map_r_end( cntr );
}

#define CC_MAP_LAST( cntr ) cc_map_last( cntr, CC_MAP_BUCKET_SIZE( cntr ), CC_MAP_PROBELEN_OFFSET( cntr ) )

static inline void *cc_map_next( void *cntr, void *i, size_t bucket_size, size_t probelen_offset )
{
  size_t j = ( (char *)i - (char *)cc_map_el( cntr, 0, bucket_size ) ) / bucket_size + 1;

  while( j < cc_map_hdr( cntr )->cap && !*cc_map_probelen( cntr, j, bucket_size, probelen_offset ) )
    ++j;

  return cc_map_el( cntr, j, bucket_size );
}

#define CC_MAP_NEXT( cntr, i ) cc_map_next( cntr, i, CC_MAP_BUCKET_SIZE( cntr ), CC_MAP_PROBELEN_OFFSET( cntr ) )

static inline void *cc_map_prev( void *cntr, void *i, size_t bucket_size, size_t probelen_offset )
{
  size_t j = ( (char *)i - (char *)cc_map_el( cntr, 0, bucket_size ) ) / bucket_size;

  while( true )
  {
    if( j == 0 )
      return cc_map_r_end( cntr );

    --j;
    if( *cc_map_probelen( cntr, j, bucket_size, probelen_offset ) )
      return cc_map_el( cntr, j, bucket_size );
  }
}

#define CC_MAP_PREV( cntr, i ) cc_map_prev( cntr, i, CC_MAP_BUCKET_SIZE( cntr ), CC_MAP_PROBELEN_OFFSET( cntr ) )

/*--------------------------------------------------------------------------------------------------------------------*/
/*                                                        Set                                                         */
/*--------------------------------------------------------------------------------------------------------------------*/

// Set is implemented as a map where the key and element are merged into one space in memory.
// Hence, it reuses the functions for map, except that the key offset passed in is zero and the element size passed in
// is also zero to avoid double-memcpying.
// For simplicity's sake, the idea here is to deviate as little from the code for map as possible.
// Note that for set, CC_EL_TYPE and CC_KEY_TYPE are synonymous (as are CC_KEY_SIZE and CC_EL_SIZE).

#define CC_SET_PROBELEN_OFFSET( cntr ) CC_ROUND_UP( CC_EL_SIZE( cntr ), alignof( cc_probelen_ty ) )

#define CC_SET_BUCKET_SIZE( cntr )                                                                    \
CC_ROUND_UP( CC_SET_PROBELEN_OFFSET( cntr ) + sizeof( cc_probelen_ty ), alignof( CC_EL_TY( cntr ) ) ) \

#define CC_SET_INIT CC_MAP_INIT

#define CC_SET_SIZE cc_map_size

#define CC_SET_CAP cc_map_cap

#define CC_SET_RESERVE( cntr, n )                \
(                                                \
  CC_POINT_HNDL_TO_ALLOCING_FN_RESULT(           \
    cntr,                                        \
    cc_map_reserve(                              \
      cntr,                                      \
      n,                                         \
      CC_SET_BUCKET_SIZE( cntr ),                \
      0, /* zero el size */                      \
      0, /* zero key offset */                   \
      CC_KEY_SIZE( cntr ),                       \
      CC_SET_PROBELEN_OFFSET( cntr ),            \
      CC_KEY_CMPR( cntr ),                       \
      CC_KEY_HASH( cntr ),                       \
      CC_KEY_LOAD( cntr ),                       \
      CC_GET_REALLOC,                            \
      CC_GET_FREE                                \
    )                                            \
  ),                                             \
  (bool)CC_FIX_HNDL_AND_RETURN_OTHER_PTR( cntr ) \
)                                                \

#define CC_SET_INSERT( cntr, el, replace )         \
(                                                  \
  CC_POINT_HNDL_TO_ALLOCING_FN_RESULT(             \
    cntr,                                          \
    cc_map_insert(                                 \
      cntr,                                        \
      /* Copying el is a non-op, but we still      \
      need to pass a valid pointer for memcpy. */  \
      cntr,                                        \
      &CC_MAKE_LVAL_COPY( CC_KEY_TY( cntr ), el ), \
      replace,                                     \
      CC_SET_BUCKET_SIZE( cntr ),                  \
      0, /* Zero el size. */                       \
      0, /* Zero key offset. */                    \
      CC_KEY_SIZE( cntr ),                         \
      CC_SET_PROBELEN_OFFSET( cntr ),              \
      CC_KEY_CMPR( cntr ),                         \
      CC_KEY_HASH( cntr ),                         \
      CC_KEY_LOAD( cntr ),                         \
      CC_KEY_DTOR( cntr ),                         \
      NULL, /* Only need one dtor. */              \
      CC_GET_REALLOC,                              \
      CC_GET_FREE                                  \
    )                                              \
  ),                                               \
  CC_FIX_HNDL_AND_RETURN_OTHER_PTR( cntr )         \
)                                                  \

#define CC_SET_GET( cntr, el )                 \
cc_map_get(                                    \
  cntr,                                        \
  &CC_MAKE_LVAL_COPY( CC_KEY_TY( cntr ), el ), \
  CC_SET_BUCKET_SIZE( cntr ),                  \
  0, /* Zero key offset. */                    \
  CC_SET_PROBELEN_OFFSET( cntr ),              \
  CC_KEY_CMPR( cntr ),                         \
  CC_KEY_HASH( cntr )                          \
)                                              \

#define CC_SET_ERASE_ITR( cntr, i ) \
cc_map_erase_itr(                   \
  cntr,                             \
  i,                                \
  CC_SET_BUCKET_SIZE( cntr ),       \
  0, /* Zero el size. */            \
  0, /* Zero key offset. */         \
  CC_KEY_SIZE( cntr ),              \
  CC_SET_PROBELEN_OFFSET( cntr ),   \
  CC_KEY_DTOR( cntr ),              \
  NULL /* Only need one dtor */     \
)                                   \

#define CC_SET_ERASE( cntr, key )               \
cc_map_erase(                                   \
  cntr,                                         \
  &CC_MAKE_LVAL_COPY( CC_KEY_TY( cntr ), key ), \
  CC_SET_BUCKET_SIZE( cntr ),                   \
  0, /* Zero el size. */                        \
  0, /* Zero key offset. */                     \
  CC_KEY_SIZE( cntr ),                          \
  CC_SET_PROBELEN_OFFSET( cntr ),               \
  CC_KEY_CMPR( cntr ),                          \
  CC_KEY_HASH( cntr ),                          \
  CC_KEY_DTOR( cntr ),                          \
  NULL /* Only need one dtor. */                \
)                                               \

#define CC_SET_INIT_CLONE( cntr, src )                                                                \
( cntr = (CC_TYPEOF_XP( cntr ))cc_map_init_clone( src, CC_SET_BUCKET_SIZE( cntr ), CC_GET_REALLOC ) ) \

#define CC_SET_SHRINK( cntr )                    \
(                                                \
  CC_POINT_HNDL_TO_ALLOCING_FN_RESULT(           \
    cntr,                                        \
    cc_map_shrink(                               \
      cntr,                                      \
      CC_SET_BUCKET_SIZE( cntr ),                \
      0, /* Zero el size. */                     \
      0, /* Zero key offset. */                  \
      CC_KEY_SIZE( cntr ),                       \
      CC_SET_PROBELEN_OFFSET( cntr ),            \
      CC_KEY_CMPR( cntr ),                       \
      CC_KEY_HASH( cntr ),                       \
      CC_KEY_LOAD( cntr ),                       \
      CC_GET_REALLOC,                            \
      CC_GET_FREE                                \
    )                                            \
  ),                                             \
  (bool)CC_FIX_HNDL_AND_RETURN_OTHER_PTR( cntr ) \
)                                                \

#define CC_SET_CLEAR( cntr )      \
cc_map_clear(                     \
  cntr,                           \
  CC_SET_BUCKET_SIZE( cntr ),     \
  0,  /* Zero key offset. */      \
  CC_SET_PROBELEN_OFFSET( cntr ), \
  CC_KEY_DTOR( cntr ),            \
  NULL /* Only need one dtor. */  \
)                                 \

#define CC_SET_CLEANUP( cntr )      \
(                                   \
  cc_map_cleanup(                   \
    cntr,                           \
    CC_SET_BUCKET_SIZE( cntr ),     \
    0,  /* Zero key offset. */      \
    CC_SET_PROBELEN_OFFSET( cntr ), \
    CC_KEY_DTOR( cntr ),            \
    NULL, /* Only need one dtor. */ \
    CC_GET_FREE                     \
  ),                                \
  CC_SET_INIT( cntr )               \
)                                   \

#define CC_SET_R_END cc_map_r_end

#define CC_SET_END( cntr ) cc_map_end( cntr, CC_SET_BUCKET_SIZE( cntr ) )

#define CC_SET_FIRST( cntr ) cc_map_first( cntr, CC_SET_BUCKET_SIZE( cntr ), CC_SET_PROBELEN_OFFSET( cntr ) )

#define CC_SET_LAST( cntr ) cc_map_last( cntr, CC_SET_BUCKET_SIZE( cntr ), CC_SET_PROBELEN_OFFSET( cntr ) )

#define CC_SET_NEXT( cntr, i ) cc_map_next( cntr, i, CC_SET_BUCKET_SIZE( cntr ), CC_SET_PROBELEN_OFFSET( cntr ) )

#define CC_SET_PREV( cntr, i ) cc_map_prev( cntr, i, CC_SET_BUCKET_SIZE( cntr ), CC_SET_PROBELEN_OFFSET( cntr ) )

/*--------------------------------------------------------------------------------------------------------------------*/
/*                                                        API                                                         */
/*--------------------------------------------------------------------------------------------------------------------*/

// Generally, API macros need to do several things:
// - For GCC and Clang, check the first argument - the pointer to the container handle - for potential duplicate side
//   effects and generate a compile-time warning if they exist.
//   Although this is only strictly necessary for macros that may cause reallocation operating on containers that store
//   their headers in the same block of memory as their elements, we extend this check to all API macros for the sake of
//   consistency.
// - Check that the aforementioned pointer points to a container handle compatible with the macro.
// - Convert the aforementioned pointer to the actual container handle (i.e. derference it).
//   While the internal macros take the container handle directly, API macros take a pointer to the handle so that it's
//   clear to the user that they may modify it.
// - Call the correct container-specific function or macro.
// - Provide dummy values as arguments to the non-called container-specific functions or macros to avoid compiler
//   errors.
// - Cast the result of the container-specific function or macro, if it is a void pointer, to the correct pointer type
//   (usually the container's element type).
// - For container-specific functions or macros that return a void pointer that actually denotes whether the operation
//   was successful, convert it to bool in the case of those containers.
// Although these macros are long and ramose, the compiler should optimize away all the checks and irrelevant paths.

#define cc_init( cntr )                                        \
(                                                              \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                      \
  CC_STATIC_ASSERT(                                            \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ||                        \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ||                        \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ||                        \
    CC_CNTR_ID( *(cntr) ) == CC_SET                            \
  ),                                                           \
  CC_CNTR_ID( *(cntr) ) == CC_VEC  ? CC_VEC_INIT( *(cntr) )  : \
  CC_CNTR_ID( *(cntr) ) == CC_LIST ? CC_LIST_INIT( *(cntr) ) : \
  CC_CNTR_ID( *(cntr) ) == CC_MAP  ? CC_MAP_INIT( *(cntr) )  : \
  CC_CNTR_ID( *(cntr) ) == CC_SET  ? CC_SET_INIT( *(cntr) )  : \
  (void)0 /* Unreachable. */                                   \
)                                                              \

#define cc_init_clone( cntr, src )                                           \
CC_CAST_MAYBE_UNUSED( bool, (                                                \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                    \
  CC_STATIC_ASSERT( CC_IS_SAME_TY( *(cntr), *(src) ) ),                      \
  CC_STATIC_ASSERT(                                                          \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ||                                      \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ||                                      \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ||                                      \
    CC_CNTR_ID( *(cntr) ) == CC_SET                                          \
  ),                                                                         \
  CC_CNTR_ID( *(cntr) ) == CC_VEC  ? CC_VEC_INIT_CLONE( *(cntr), *(src) )  : \
  CC_CNTR_ID( *(cntr) ) == CC_LIST ? CC_LIST_INIT_CLONE( *(cntr), *(src) ) : \
  CC_CNTR_ID( *(cntr) ) == CC_MAP  ? CC_MAP_INIT_CLONE( *(cntr), *(src) )  : \
  CC_CNTR_ID( *(cntr) ) == CC_SET  ? CC_SET_INIT_CLONE( *(cntr), *(src) )  : \
  NULL /* Unreachable. */                                                    \
) )                                                                          \


#define cc_size( cntr )                                        \
(                                                              \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                      \
  CC_STATIC_ASSERT(                                            \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ||                        \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ||                        \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ||                        \
    CC_CNTR_ID( *(cntr) ) == CC_SET                            \
  ),                                                           \
  CC_CNTR_ID( *(cntr) ) == CC_VEC  ? cc_vec_size( *(cntr) )  : \
  CC_CNTR_ID( *(cntr) ) == CC_LIST ? cc_list_size( *(cntr) ) : \
  CC_CNTR_ID( *(cntr) ) == CC_MAP  ? cc_map_size( *(cntr) )  : \
  CC_CNTR_ID( *(cntr) ) == CC_SET  ? CC_SET_SIZE( *(cntr) )  : \
  0 /* Unreachable. */                                         \
)                                                              \

#define cc_cap( cntr )                                       \
(                                                            \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                    \
  CC_STATIC_ASSERT(                                          \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ||                      \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ||                      \
    CC_CNTR_ID( *(cntr) ) == CC_SET                          \
  ),                                                         \
  CC_CNTR_ID( *(cntr) ) == CC_VEC  ? cc_vec_cap( *(cntr) ) : \
  CC_CNTR_ID( *(cntr) ) == CC_MAP  ? cc_map_cap( *(cntr) ) : \
  CC_CNTR_ID( *(cntr) ) == CC_SET  ? CC_SET_CAP( *(cntr) ) : \
  0 /* Unreachable. */                                       \
)                                                            \

#define cc_reserve( cntr, n )                                        \
CC_CAST_MAYBE_UNUSED( bool, (                                        \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                            \
  CC_STATIC_ASSERT(                                                  \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ||                              \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ||                              \
    CC_CNTR_ID( *(cntr) ) == CC_SET                                  \
  ),                                                                 \
  CC_CNTR_ID( *(cntr) ) == CC_VEC ? CC_VEC_RESERVE( *(cntr), (n) ) : \
  CC_CNTR_ID( *(cntr) ) == CC_MAP ? CC_MAP_RESERVE( *(cntr), (n) ) : \
  CC_CNTR_ID( *(cntr) ) == CC_SET ? CC_SET_RESERVE( *(cntr), (n) ) : \
  false /* Unreachable. */                                           \
) )                                                                  \

#define cc_resize( cntr, n )                           \
CC_CAST_MAYBE_UNUSED( bool, (                          \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),              \
  CC_STATIC_ASSERT( CC_CNTR_ID( *(cntr) ) == CC_VEC ), \
  CC_VEC_RESIZE( *(cntr), (n) )                        \
) )                                                    \

#define cc_shrink( cntr )                                      \
CC_CAST_MAYBE_UNUSED( bool, (                                  \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                      \
  CC_STATIC_ASSERT(                                            \
    CC_CNTR_ID( *(cntr) ) == CC_VEC ||                         \
    CC_CNTR_ID( *(cntr) ) == CC_MAP ||                         \
    CC_CNTR_ID( *(cntr) ) == CC_SET                            \
  ),                                                           \
  CC_CNTR_ID( *(cntr) ) == CC_VEC ? CC_VEC_SHRINK( *(cntr) ) : \
  CC_CNTR_ID( *(cntr) ) == CC_MAP ? CC_MAP_SHRINK( *(cntr) ) : \
  CC_CNTR_ID( *(cntr) ) == CC_SET ? CC_SET_SHRINK( *(cntr) ) : \
  false /* Unreachable. */                                     \
) )                                                            \

#define cc_insert( ... ) CC_SELECT_ON_NUM_ARGS( cc_insert, __VA_ARGS__ )

#define cc_insert_2( cntr, el )                        \
CC_CAST_MAYBE_UNUSED( CC_EL_TY( *(cntr) ) *, (         \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),              \
  CC_STATIC_ASSERT( CC_CNTR_ID( *(cntr) ) == CC_SET ), \
  CC_SET_INSERT( *(cntr), (el), true )                 \
) )                                                    \

#define cc_insert_3( cntr, i, el )                                                                             \
CC_CAST_MAYBE_UNUSED( CC_EL_TY( *(cntr) ) *, (                                                                 \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                                                      \
  CC_STATIC_ASSERT(                                                                                            \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ||                                                                        \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ||                                                                        \
    CC_CNTR_ID( *(cntr) ) == CC_MAP                                                                            \
  ),                                                                                                           \
  CC_CNTR_ID( *(cntr) ) == CC_VEC ?                                                                            \
    CC_VEC_INSERT( *(cntr), CC_IF_THEN_XP_ELSE_DUMMY( CC_CNTR_ID( *(cntr) ) == CC_VEC, (i), size_t ), (el) ) : \
  CC_CNTR_ID( *(cntr) ) == CC_LIST ?                                                                           \
    CC_LIST_INSERT(                                                                                            \
      *(cntr),                                                                                                 \
      CC_IF_THEN_XP_ELSE_DUMMY( CC_CNTR_ID( *(cntr) ) == CC_LIST, (i), CC_EL_TY( *(cntr) )* ),                 \
      (el)                                                                                                     \
    ) :                                                                                                        \
  CC_CNTR_ID( *(cntr) ) == CC_MAP ?                                                                            \
    CC_MAP_INSERT(                                                                                             \
      *(cntr),                                                                                                 \
      CC_IF_THEN_XP_ELSE_DUMMY( CC_CNTR_ID( *(cntr) ) == CC_MAP, (i), CC_KEY_TY( *(cntr) ) ),                  \
      (el),                                                                                                    \
      true                                                                                                     \
    ) :                                                                                                        \
  NULL /* Unreachable. */                                                                                      \
) )                                                                                                            \

#define cc_insert_n( cntr, i, els, n )                 \
CC_CAST_MAYBE_UNUSED( CC_EL_TY( *(cntr) ) *, (         \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),              \
  CC_STATIC_ASSERT( CC_CNTR_ID( *(cntr) ) == CC_VEC ), \
  CC_VEC_INSERT_N( *(cntr), (i), (els), (n) )          \
) )                                                    \

#define cc_push( cntr, el )                                          \
CC_CAST_MAYBE_UNUSED( CC_EL_TY( *(cntr) ) *, (                       \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                            \
  CC_STATIC_ASSERT(                                                  \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ||                              \
    CC_CNTR_ID( *(cntr) ) == CC_LIST                                 \
  ),                                                                 \
  CC_CNTR_ID( *(cntr) ) == CC_VEC  ? CC_VEC_PUSH( *(cntr), (el) )  : \
  CC_CNTR_ID( *(cntr) ) == CC_LIST ? CC_LIST_PUSH( *(cntr), (el) ) : \
  NULL /* Unreachable. */                                            \
) )                                                                  \

#define cc_push_n( cntr, els, n )                      \
CC_CAST_MAYBE_UNUSED( CC_EL_TY( *(cntr) ) *, (         \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),              \
  CC_STATIC_ASSERT( CC_CNTR_ID( *(cntr) ) == CC_VEC ), \
  CC_VEC_PUSH_N( *(cntr), (els), (n) )                 \
) )                                                    \

#define cc_splice( cntr, i, src, src_i )                \
CC_CAST_MAYBE_UNUSED( bool, (                           \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),               \
  CC_STATIC_ASSERT( CC_IS_SAME_TY( (cntr), (src) ) ),   \
  CC_STATIC_ASSERT( CC_CNTR_ID( *(cntr) ) == CC_LIST ), \
  CC_LIST_SPLICE( *(cntr), (i), *(src), (src_i) )       \
) )                                                     \

#define cc_get( cntr, i )                                                                                           \
CC_CAST_MAYBE_UNUSED( CC_EL_TY( *(cntr) ) *, (                                                                      \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                                                           \
  CC_STATIC_ASSERT(                                                                                                 \
    CC_CNTR_ID( *(cntr) ) == CC_VEC ||                                                                              \
    CC_CNTR_ID( *(cntr) ) == CC_MAP ||                                                                              \
    CC_CNTR_ID( *(cntr) ) == CC_SET                                                                                 \
  ),                                                                                                                \
  CC_CNTR_ID( *(cntr) ) == CC_VEC ?                                                                                 \
    CC_VEC_GET( *(cntr), CC_IF_THEN_XP_ELSE_DUMMY( CC_CNTR_ID( *(cntr) ) == CC_VEC, (i), size_t ) )               : \
  CC_CNTR_ID( *(cntr) ) == CC_MAP ?                                                                                 \
    CC_MAP_GET( *(cntr), CC_IF_THEN_XP_ELSE_DUMMY( CC_CNTR_ID( *(cntr) ) == CC_MAP, (i), CC_KEY_TY( *(cntr) ) ) ) : \
  CC_CNTR_ID( *(cntr) ) == CC_SET ?                                                                                 \
    CC_SET_GET( *(cntr), CC_IF_THEN_XP_ELSE_DUMMY( CC_CNTR_ID( *(cntr) ) == CC_SET, (i), CC_KEY_TY( *(cntr) ) ) ) : \
  NULL /* Unreachable. */                                                                                           \
) )                                                                                                                 \

#define cc_get_or_insert( ... ) CC_SELECT_ON_NUM_ARGS( cc_get_or_insert, __VA_ARGS__ )

#define cc_get_or_insert_2( cntr, el )                 \
CC_CAST_MAYBE_UNUSED( CC_EL_TY( *(cntr) ) *, (         \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),              \
  CC_STATIC_ASSERT( CC_CNTR_ID( *(cntr) ) == CC_SET ), \
  CC_SET_INSERT( *(cntr), (el), false )                \
) )                                                    \

#define cc_get_or_insert_3( cntr, key, el )            \
CC_CAST_MAYBE_UNUSED( CC_EL_TY( *(cntr) ) *, (         \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),              \
  CC_STATIC_ASSERT( CC_CNTR_ID( *(cntr) ) == CC_MAP ), \
  CC_MAP_INSERT( *(cntr), (key), (el), false )         \
) )                                                    \

#define cc_key_for( cntr, i )                          \
CC_CAST_MAYBE_UNUSED( const CC_KEY_TY( *(cntr) ) *, (  \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),              \
  CC_STATIC_ASSERT( CC_CNTR_ID( *(cntr) ) == CC_MAP ), \
  CC_MAP_KEY_FOR( *(cntr), (i) )                       \
) )                                                    \

#define cc_erase( cntr, i )                                                                                           \
CC_IF_THEN_PTR_TO_BOOL_ELSE_PTR( CC_CNTR_ID( *(cntr) ) == CC_MAP || CC_CNTR_ID( *(cntr) ) == CC_SET,                  \
CC_CAST_MAYBE_UNUSED( CC_EL_TY( *(cntr) ) *, (                                                                        \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                                                             \
  CC_STATIC_ASSERT(                                                                                                   \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ||                                                                               \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ||                                                                               \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ||                                                                               \
    CC_CNTR_ID( *(cntr) ) == CC_SET                                                                                   \
  ),                                                                                                                  \
  CC_CNTR_ID( *(cntr) ) == CC_VEC ?                                                                                   \
    CC_VEC_ERASE( *(cntr), CC_IF_THEN_XP_ELSE_DUMMY( CC_CNTR_ID( *(cntr) ) == CC_VEC, (i), size_t ) )               : \
  CC_CNTR_ID( *(cntr) ) == CC_LIST ?                                                                                  \
    CC_LIST_ERASE(                                                                                                    \
      *(cntr),                                                                                                        \
      CC_IF_THEN_XP_ELSE_DUMMY( CC_CNTR_ID( *(cntr) ) == CC_LIST, (i), CC_EL_TY( *(cntr) ) * )                        \
    )                                                                                                               : \
  CC_CNTR_ID( *(cntr) ) == CC_MAP ?                                                                                   \
    CC_MAP_ERASE( *(cntr), CC_IF_THEN_XP_ELSE_DUMMY( CC_CNTR_ID( *(cntr) ) == CC_MAP, (i), CC_KEY_TY( *(cntr) ) ) ) : \
  CC_CNTR_ID( *(cntr) ) == CC_SET ?                                                                                   \
    CC_SET_ERASE( *(cntr), CC_IF_THEN_XP_ELSE_DUMMY( CC_CNTR_ID( *(cntr) ) == CC_SET, (i), CC_KEY_TY( *(cntr) ) ) ) : \
  NULL /* Unreachable. */                                                                                             \
) ) )                                                                                                                 \

#define cc_erase_n( cntr, i, n )                       \
CC_CAST_MAYBE_UNUSED( CC_EL_TY( *(cntr) ) *, (         \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),              \
  CC_STATIC_ASSERT( CC_CNTR_ID( *(cntr) ) == CC_VEC ), \
  CC_VEC_ERASE_N( *(cntr), (i), (n) )                  \
) )                                                    \

#define cc_erase_itr( cntr, i )                                        \
(                                                                      \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                              \
  CC_STATIC_ASSERT(                                                    \
    CC_CNTR_ID( *(cntr) ) == CC_MAP ||                                 \
    CC_CNTR_ID( *(cntr) ) == CC_SET                                    \
  ),                                                                   \
  CC_CNTR_ID( *(cntr) ) == CC_MAP ? CC_MAP_ERASE_ITR( *(cntr), (i) ) : \
  CC_CNTR_ID( *(cntr) ) == CC_SET ? CC_SET_ERASE_ITR( *(cntr), (i) ) : \
  (void)0 /* Unreachable. */                                           \
)                                                                      \

#define cc_clear( cntr )                                        \
(                                                               \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                       \
  CC_CNTR_ID( *(cntr) ) == CC_VEC  ? CC_VEC_CLEAR( *(cntr) )  : \
  CC_CNTR_ID( *(cntr) ) == CC_LIST ? CC_LIST_CLEAR( *(cntr) ) : \
  CC_CNTR_ID( *(cntr) ) == CC_MAP  ? CC_MAP_CLEAR( *(cntr) )  : \
  CC_CNTR_ID( *(cntr) ) == CC_SET  ? CC_SET_CLEAR( *(cntr) )  : \
  (void)0 /* Unreachable. */                                    \
)                                                               \

#define cc_cleanup( cntr )                                        \
(                                                                 \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                         \
  CC_CNTR_ID( *(cntr) ) == CC_VEC  ? CC_VEC_CLEANUP( *(cntr) )  : \
  CC_CNTR_ID( *(cntr) ) == CC_LIST ? CC_LIST_CLEANUP( *(cntr) ) : \
  CC_CNTR_ID( *(cntr) ) == CC_MAP  ? CC_MAP_CLEANUP( *(cntr) )  : \
  CC_CNTR_ID( *(cntr) ) == CC_SET  ? CC_SET_CLEANUP( *(cntr) )  : \
  (void)0 /* Unreachable. */                                      \
)                                                                 \

#define cc_first( cntr )                                        \
CC_CAST_MAYBE_UNUSED( CC_EL_TY( *(cntr) ) *, (                  \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                       \
  CC_STATIC_ASSERT(                                             \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ||                         \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ||                         \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ||                         \
    CC_CNTR_ID( *(cntr) ) == CC_SET                             \
  ),                                                            \
  CC_CNTR_ID( *(cntr) ) == CC_VEC  ? cc_vec_first( *(cntr) )  : \
  CC_CNTR_ID( *(cntr) ) == CC_LIST ? cc_list_first( *(cntr) ) : \
  CC_CNTR_ID( *(cntr) ) == CC_MAP  ? CC_MAP_FIRST( *(cntr) )  : \
  CC_CNTR_ID( *(cntr) ) == CC_SET  ? CC_SET_FIRST( *(cntr) )  : \
  NULL  /* Unreachable. */                                      \
) )                                                             \

#define cc_last( cntr )                                        \
CC_CAST_MAYBE_UNUSED( CC_EL_TY( *(cntr) ) *, (                 \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                      \
  CC_STATIC_ASSERT(                                            \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ||                        \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ||                        \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ||                        \
    CC_CNTR_ID( *(cntr) ) == CC_SET                            \
  ),                                                           \
  CC_CNTR_ID( *(cntr) ) == CC_VEC  ? CC_VEC_LAST( *(cntr) )  : \
  CC_CNTR_ID( *(cntr) ) == CC_LIST ? cc_list_last( *(cntr) ) : \
  CC_CNTR_ID( *(cntr) ) == CC_MAP  ? CC_MAP_LAST( *(cntr) )  : \
  CC_CNTR_ID( *(cntr) ) == CC_SET  ? CC_SET_LAST( *(cntr) )  : \
  NULL  /* Unreachable. */                                     \
) )                                                            \

#define cc_r_end( cntr )                                        \
CC_CAST_MAYBE_UNUSED( CC_EL_TY( *(cntr) ) *, (                  \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                       \
  CC_STATIC_ASSERT(                                             \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ||                         \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ||                         \
    CC_CNTR_ID( *(cntr) ) == CC_SET                             \
  ),                                                            \
  CC_CNTR_ID( *(cntr) ) == CC_LIST ? cc_list_r_end( *(cntr) ) : \
  CC_CNTR_ID( *(cntr) ) == CC_MAP  ? cc_map_r_end( *(cntr) )  : \
  CC_CNTR_ID( *(cntr) ) == CC_SET  ? CC_SET_R_END( *(cntr) )  : \
  NULL  /* Unreachable. */                                      \
) )                                                             \

#define cc_end( cntr )                                        \
CC_CAST_MAYBE_UNUSED( CC_EL_TY( *(cntr) ) *, (                \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                     \
  CC_STATIC_ASSERT(                                           \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ||                       \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ||                       \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ||                       \
    CC_CNTR_ID( *(cntr) ) == CC_SET                           \
  ),                                                          \
  CC_CNTR_ID( *(cntr) ) == CC_VEC  ? CC_VEC_END( *(cntr) )  : \
  CC_CNTR_ID( *(cntr) ) == CC_LIST ? cc_list_end( *(cntr) ) : \
  CC_CNTR_ID( *(cntr) ) == CC_MAP  ? CC_MAP_END( *(cntr) )  : \
  CC_CNTR_ID( *(cntr) ) == CC_SET  ? CC_SET_END( *(cntr) )  : \
  NULL  /* Unreachable. */                                    \
) )                                                           \

#define cc_next( cntr, i )                                          \
CC_CAST_MAYBE_UNUSED( CC_EL_TY( *(cntr) ) *, (                      \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                           \
  CC_STATIC_ASSERT(                                                 \
    CC_CNTR_ID( *(cntr) ) == CC_VEC  ||                             \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ||                             \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ||                             \
    CC_CNTR_ID( *(cntr) ) == CC_SET                                 \
  ),                                                                \
  CC_CNTR_ID( *(cntr) ) == CC_VEC  ? CC_VEC_NEXT( *(cntr), (i) )  : \
  CC_CNTR_ID( *(cntr) ) == CC_LIST ? cc_list_next( *(cntr), (i) ) : \
  CC_CNTR_ID( *(cntr) ) == CC_MAP  ? CC_MAP_NEXT( *(cntr), (i) )  : \
  CC_CNTR_ID( *(cntr) ) == CC_SET  ? CC_SET_NEXT( *(cntr), (i) )  : \
  NULL  /* Unreachable. */                                          \
) )                                                                 \

#define cc_prev( cntr, i )                                          \
CC_CAST_MAYBE_UNUSED( CC_EL_TY( *(cntr) ) *, (                      \
  CC_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                           \
  CC_STATIC_ASSERT(                                                 \
    CC_CNTR_ID( *(cntr) ) == CC_LIST ||                             \
    CC_CNTR_ID( *(cntr) ) == CC_MAP  ||                             \
    CC_CNTR_ID( *(cntr) ) == CC_SET                                 \
  ),                                                                \
  CC_CNTR_ID( *(cntr) ) == CC_LIST ? cc_list_prev( *(cntr), (i) ) : \
  CC_CNTR_ID( *(cntr) ) == CC_MAP  ? CC_MAP_PREV( *(cntr), (i) )  : \
  CC_CNTR_ID( *(cntr) ) == CC_SET  ? CC_SET_PREV( *(cntr), (i) )  : \
  NULL  /* Unreachable. */                                          \
) )                                                                 \

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
#define CC_N_DTORS_D1 0 // D1 = digit 1, i.e. least significant digit.
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
CC_CAT( CC_R1_, d1 )( m, arg, d3, d2 )         \
CC_CAT( CC_R2_, d2 )( m, arg, d3 )             \
CC_CAT( CC_R3_, d3 )( m, arg )                 \

#define CC_FOR_EACH_DTOR( m, arg ) CC_FOR_OCT_COUNT( m, arg, CC_N_DTORS_D3, CC_N_DTORS_D2, CC_N_DTORS_D1 )
#define CC_FOR_EACH_CMPR( m, arg ) CC_FOR_OCT_COUNT( m, arg, CC_N_CMPRS_D3, CC_N_CMPRS_D2, CC_N_CMPRS_D1 )
#define CC_FOR_EACH_HASH( m, arg ) CC_FOR_OCT_COUNT( m, arg, CC_N_HASHS_D3, CC_N_HASHS_D2, CC_N_HASHS_D1 )
#define CC_FOR_EACH_LOAD( m, arg ) CC_FOR_OCT_COUNT( m, arg, CC_N_LOADS_D3, CC_N_LOADS_D2, CC_N_LOADS_D1 )

// Macros for inferring the destructor, comparison, or hash function or load factor associated with a container's
// key or element type, as well as for determining whether a comparison or hash function exists for a type.
// In C, we use the CC_FOR_EACH_XXXX macros above to create _Generic expressions that select the correct user-defined
// function or load factor for the container's key or element types.
// For comparison and hash functions, the list of user-defined functions is followed by a nested _Generic statement
// containing the default functions for integer types and NULL-terminated strings, which allows the user to overwrite
// the default functions.
// In C++, we use the same macros combined with std::is_same and ternary expressions to emulate _Generic statements.
// Unfortunately, the behavior is not identical because the C++ versions won't generate compile errors if the user
// defines multiple functions (e.g. multiple destructors) for the same type.
// Hence, it is up to the user to make sure they are not doing that if they are compiling for C++.

#ifdef __cplusplus

#define CC_EL_DTOR_SLOT( n, arg ) std::is_same<arg, cc_dtor_##n##_ty>::value ? cc_dtor_##n##_fn :
#define CC_EL_DTOR( cntr )                              \
(                                                       \
  CC_FOR_EACH_DTOR( CC_EL_DTOR_SLOT, CC_EL_TY( cntr ) ) \
  (void (*)( void * ))NULL                              \
)                                                       \

#define CC_KEY_DTOR_SLOT( n, arg )                           \
std::is_same<                                                \
  CC_TYPEOF_XP(**arg),                                       \
  CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( arg ), cc_dtor_##n##_ty ) \
>::value ? cc_dtor_##n##_fn :                                \

#define CC_KEY_DTOR( cntr )                  \
(                                            \
  CC_FOR_EACH_DTOR( CC_KEY_DTOR_SLOT, cntr ) \
  (void (*)( void * ))NULL                   \
)                                            \

#define CC_KEY_CMPR_SLOT( n, arg )                                                                      \
std::is_same<CC_TYPEOF_XP(**arg), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( arg ), cc_cmpr_##n##_ty ) >::value ? \
  cc_cmpr_##n##_fn                                                                                    : \

#define CC_KEY_CMPR( cntr )                                                                                  \
(                                                                                                            \
  CC_FOR_EACH_CMPR( CC_KEY_CMPR_SLOT, cntr )                                                                 \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), char )>::value               ? \
    cc_cmpr_char                                                                                           : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned char )>::value      ? \
    cc_cmpr_unsigned_char                                                                                  : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), signed char )>::value        ? \
    cc_cmpr_signed_char                                                                                    : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned short )>::value     ? \
    cc_cmpr_unsigned_short                                                                                 : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), short )>::value              ? \
    cc_cmpr_short                                                                                          : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned int )>::value       ? \
    cc_cmpr_unsigned_int                                                                                   : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), int )>::value                ? \
    cc_cmpr_int                                                                                            : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned long )>::value      ? \
    cc_cmpr_unsigned_long                                                                                  : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), long )>::value               ? \
    cc_cmpr_long                                                                                           : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned long long )>::value ? \
    cc_cmpr_unsigned_long_long                                                                             : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), long long )>::value          ? \
    cc_cmpr_long_long                                                                                      : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), size_t )>::value             ? \
    cc_cmpr_size_t                                                                                         : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), char * )>::value             ? \
    cc_cmpr_c_string                                                                                       : \
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), const char * )>::value       ? \
    cc_cmpr_c_string                                                                                       : \
  (int (*)( void *, void *))NULL                                                                             \
)                                                                                                            \

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
  std::is_same<CC_TYPEOF_XP(**cntr), CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), const char * )>::value       ? \
    cc_hash_c_string                                                                                       : \
  (size_t (*)( void *))NULL                                                                                  \
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
  std::is_same<ty, const char *>::value       ? true : \
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
  std::is_same<ty, const char *>::value       ? true : \
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

#else

#define CC_EL_DTOR_SLOT( n, arg ) cc_dtor_##n##_ty: cc_dtor_##n##_fn,
#define CC_EL_DTOR( cntr )             \
_Generic( (CC_EL_TY( cntr )){ 0 },     \
  CC_FOR_EACH_DTOR( CC_EL_DTOR_SLOT, ) \
  default: (void (*)( void * ))NULL    \
)                                      \

#define CC_KEY_DTOR_SLOT( n, arg ) CC_MAKE_BASE_FNPTR_TY( arg, cc_dtor_##n##_ty ): cc_dtor_##n##_fn,
#define CC_KEY_DTOR( cntr )                              \
_Generic( (**cntr),                                      \
  CC_FOR_EACH_DTOR( CC_KEY_DTOR_SLOT, CC_EL_TY( cntr ) ) \
  default: (void (*)( void * ))NULL                      \
)                                                        \

#define CC_KEY_CMPR_SLOT( n, arg ) CC_MAKE_BASE_FNPTR_TY( arg, cc_cmpr_##n##_ty ): cc_cmpr_##n##_fn,
#define CC_KEY_CMPR( cntr )                                                                    \
_Generic( (**cntr),                                                                            \
  CC_FOR_EACH_CMPR( CC_KEY_CMPR_SLOT, CC_EL_TY( cntr ) )                                       \
  default: _Generic( (**cntr),                                                                 \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), char ):               cc_cmpr_char,               \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned char ):      cc_cmpr_unsigned_char,      \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), signed char ):        cc_cmpr_signed_char,        \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned short ):     cc_cmpr_unsigned_short,     \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), short ):              cc_cmpr_short,              \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned int ):       cc_cmpr_unsigned_int,       \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), int ):                cc_cmpr_int,                \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned long ):      cc_cmpr_unsigned_long,      \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), long ):               cc_cmpr_long,               \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), unsigned long long ): cc_cmpr_unsigned_long_long, \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), long long ):          cc_cmpr_long_long,          \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), cc_maybe_size_t ):    cc_cmpr_size_t,             \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), char * ):             cc_cmpr_c_string,           \
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), const char * ):       cc_cmpr_c_string,           \
    default: (int (*)( void *, void *))NULL                                                    \
  )                                                                                            \
)                                                                                              \

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
    CC_MAKE_BASE_FNPTR_TY( CC_EL_TY( cntr ), const char * ):       cc_hash_c_string,           \
    default: (size_t (*)( void *))NULL                                                         \
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
    const char *:       true,           \
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
    const char *:       true,           \
    default:            false           \
  )                                     \
)                                       \

#define CC_KEY_LOAD_SLOT( n, arg ) CC_MAKE_BASE_FNPTR_TY( arg, cc_load_##n##_ty ): cc_load_##n##_val,
#define CC_KEY_LOAD( cntr )                              \
_Generic( (**cntr),                                      \
  CC_FOR_EACH_LOAD( CC_KEY_LOAD_SLOT, CC_EL_TY( cntr ) ) \
  default: CC_DEFAULT_LOAD                               \
)                                                        \

#endif

// Macros for extracting the type and function body or load factor from user-defined DTOR, CMPR, HASH, and LOAD macros. 
#define CC_1ST_ARG_( _1, ... )    _1
#define CC_1ST_ARG( ... )         CC_1ST_ARG_( __VA_ARGS__ )
#define CC_OTHER_ARGS_( _1, ... ) __VA_ARGS__
#define CC_OTHER_ARGS( ... )      CC_OTHER_ARGS_( __VA_ARGS__ )

// Default hash and comparison functions for fundamental types.

// Integer types.

static inline int cc_cmpr_char( void *void_val_1, void *void_val_2 )
{
  return ( *(char *)void_val_1 > *(char *)void_val_2 ) - ( *(char *)void_val_1 < *(char *)void_val_2 );
}

static inline size_t cc_hash_char( void *void_val )
{
  return *(char *)void_val;
}

static inline int cc_cmpr_unsigned_char( void *void_val_1, void *void_val_2 )
{
  return ( *(unsigned char *)void_val_1 > *(unsigned char *)void_val_2 ) -
         ( *(unsigned char *)void_val_1 < *(unsigned char *)void_val_2 );
}

static inline size_t cc_hash_unsigned_char( void *void_val )
{
  return *(unsigned char *)void_val;
}

static inline int cc_cmpr_signed_char( void *void_val_1, void *void_val_2 )
{
  return ( *(signed char *)void_val_1 > *(signed char *)void_val_2 ) -
         ( *(signed char *)void_val_1 < *(signed char *)void_val_2 );
}

static inline size_t cc_hash_signed_char( void *void_val )
{
  return *(signed char *)void_val;
}

static inline int cc_cmpr_unsigned_short( void *void_val_1, void *void_val_2 )
{
  return ( *(unsigned short *)void_val_1 > *(unsigned short *)void_val_2 ) -
         ( *(unsigned short *)void_val_1 < *(unsigned short *)void_val_2 );
}

static inline size_t cc_hash_unsigned_short( void *void_val )
{
  return *(unsigned short *)void_val * 2654435761ull;
}

static inline int cc_cmpr_short( void *void_val_1, void *void_val_2 )
{
  return ( *(short *)void_val_1 > *(short *)void_val_2 ) - ( *(short *)void_val_1 < *(short *)void_val_2 );
}

static inline size_t cc_hash_short( void *void_val )
{
  return *(short *)void_val * 2654435761ull;
}

static inline int cc_cmpr_unsigned_int( void *void_val_1, void *void_val_2 )
{
  return ( *(unsigned int *)void_val_1 > *(unsigned int *)void_val_2 ) -
         ( *(unsigned int *)void_val_1 < *(unsigned int *)void_val_2 );
}

static inline size_t cc_hash_unsigned_int( void *void_val )
{
  return *(unsigned int *)void_val * 2654435761ull;
}

static inline int cc_cmpr_int( void *void_val_1, void *void_val_2 )
{
  return ( *(int *)void_val_1 > *(int *)void_val_2 ) - ( *(int *)void_val_1 < *(int *)void_val_2 );
}

static inline size_t cc_hash_int( void *void_val )
{
  return *(int *)void_val * 2654435761ull;
}

static inline int cc_cmpr_unsigned_long( void *void_val_1, void *void_val_2 )
{
  return ( *(unsigned long *)void_val_1 > *(unsigned long *)void_val_2 ) -
         ( *(unsigned long *)void_val_1 < *(unsigned long *)void_val_2 );
}

static inline size_t cc_hash_unsigned_long( void *void_val )
{
  return *(unsigned long *)void_val * 2654435761ull;
}

static inline int cc_cmpr_long( void *void_val_1, void *void_val_2 )
{
  return ( *(long *)void_val_1 > *(long *)void_val_2 ) - ( *(long *)void_val_1 < *(long *)void_val_2 );
}

static inline size_t cc_hash_long( void *void_val )
{
  return *(long *)void_val * 2654435761ull;
}

static inline int cc_cmpr_unsigned_long_long( void *void_val_1, void *void_val_2 )
{
  return ( *(unsigned long long *)void_val_1 > *(unsigned long long *)void_val_2 ) -
         ( *(unsigned long long *)void_val_1 < *(unsigned long long *)void_val_2 );
}

static inline size_t cc_hash_unsigned_long_long( void *void_val )
{
  return *(unsigned long long *)void_val * 2654435761ull;
}

static inline int cc_cmpr_long_long( void *void_val_1, void *void_val_2 )
{
  return ( *(long long *)void_val_1 > *(long long *)void_val_2 ) - 
         ( *(long long *)void_val_1 < *(long long *)void_val_2 );
}

static inline size_t cc_hash_long_long( void *void_val )
{
  return *(long long *)void_val * 2654435761ull;
}

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

static inline int cc_cmpr_size_t( void *void_val_1, void *void_val_2 )
{
  return ( *(size_t *)void_val_1 > *(size_t *)void_val_2 ) - ( *(size_t *)void_val_1 < *(size_t *)void_val_2 );
}

static inline size_t cc_hash_size_t( void *void_val )
{
  return *(size_t *)void_val * 2654435761ull;
}

// Null-terminated C strings.
// We use FNV-1a because newer, faster alternatives that process word-sized chunks require prior knowledge of the
// string's length.

static inline int cc_cmpr_c_string( void *void_val_1, void *void_val_2 )
{
  return strcmp( *(const char **)void_val_1, *(const char **)void_val_2 );
}

#if SIZE_MAX == 0xFFFFFFFF // 32-bit size_t.

static inline size_t cc_hash_c_string( void *void_val )
{
    const char *val = *(const char **)void_val;
    size_t hash = 0x01000193;
    while( *val )
      hash = ( (unsigned char)*val++ ^ hash ) * 0x811c9dc5;

    return hash;
}

#elif SIZE_MAX == 0xFFFFFFFFFFFFFFFF // 64-bit size_t.

static inline size_t cc_hash_c_string( void *void_val )
{
    const char *val = *(const char **)void_val;
    size_t hash = 0xcbf29ce484222325;
    while( *val )
      hash = ( (unsigned char)*val++ ^ hash ) * 0x100000001b3;

    return hash;
}

#else // Strange size_t.

static inline size_t cc_hash_c_string( void *void_val )
{
    const char *val = *(const char **)void_val;
    size_t hash = 0;
    while( *val )
        hash = hash * 131 + (unsigned char)*val++;

    return hash;
}

#endif

#endif
#else/*---------------------------------------------------------------------------------------------------------------*/
/*                                                                                                                    */
/*                       DEFINING DESTRUCTOR, COMPARISON, OR HASH FUNCTION OR LOAD FACTOR MODE                        */
/*                                                                                                                    */
/*--------------------------------------------------------------------------------------------------------------------*/

#ifdef CC_DTOR

// Convert the user-defined CC_DTOR macro into a cc_dtor_XXXX_ty and cc_dtor_XXXX_fn pair that can be pluged into the
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
#error Sorry, number of destructor functions is limited to 511.
#endif
#endif
#endif

#undef CC_DTOR
#endif

#ifdef CC_CMPR

typedef CC_TYPEOF_TY( CC_1ST_ARG( CC_CMPR ) ) CC_CAT_3( cc_cmpr_, CC_N_CMPRS, _ty );

static inline int CC_CAT_3( cc_cmpr_, CC_N_CMPRS, _fn )( void *void_val_1, void *void_val_2 )
{
  CC_CAT_3( cc_cmpr_, CC_N_CMPRS, _ty ) val_1 = *(CC_CAT_3( cc_cmpr_, CC_N_CMPRS, _ty ) *)void_val_1;
  CC_CAT_3( cc_cmpr_, CC_N_CMPRS, _ty ) val_2 = *(CC_CAT_3( cc_cmpr_, CC_N_CMPRS, _ty ) *)void_val_2;
  CC_OTHER_ARGS( CC_CMPR )
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
#error Sorry, number of comparison functions is limited to 511.
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
#error Sorry, number of hash functions is limited to 511.
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
#error Sorry, number of load factors is limited to 511.
#endif
#endif
#endif

#undef CC_LOAD
#endif

#endif
