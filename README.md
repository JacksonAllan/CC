# CC.H: Convenient Containers

**cc.h** is a usability-oriented generic container library for C that provides vectors, doubly linked lists, unordered maps, and unordered sets.

Its features include:

- Fully generic API.
- Type safety.
- User-defined destructor, comparison, and hash functions associated with element and key types.
- No assumption of successful memory allocation.
- Single header.
- Compiles in C and C++.
- Safe API macros.

It requires C23, or C11 and compiler support for `typeof`, or C++11.

It is distributed under BSD-2 simplified license.

## Why?

Traditionally, C container libraries require users to define types for every container/element type combination and/or specify the container and/or element type at every API call (whether by casting, type-specific function names, or some other mechanism). The result is verbose code.

In contrast, CC.H requires no type definitions and provides an API agnostic to container and element types. The following table compares **cc.h** usage to other container library paradigms:

<table>
<tr>
</tr>
<tr>
<td>

```c
// cc.h
#include <stdio.h>
#include "CC.H"

int main( void )
{
  vec( int ) our_vec;
  init( &our_vec );
  push( &our_vec, 5 );
  printf( "%d\n", *get( &our_vec, 0 ) );
  cleanup( &our_vec );

  map( int, float ) our_map;
  init( &our_map );
  insert( &our_map, 5, 0.5 );
  printf( "%f\n", *get( &our_map, 5 ) );
  cleanup( &our_map );
}




```

</td>
<td>

```c
// Template instantiation paradigm
#include <stdio.h>
#include "other_container_lib.h"

DEFINE_VEC( int, int_vec )
DEFINE_MAP( int, float, int_float_map )

int main( void )
{
  int_vec our_vec;
  int_vec_init( &our_vec );
  int_vec_push( &our_vec, 5 );
  printf( "%d\n", *int_vec_get( &our_vec, 0 ) );
  int_vec_cleanup( &our_vec );

  int_float_map our_map;
  int_float_map_init( &our_map );
  int_float_map_insert( &our_map, 5, 0.5f );
  printf( "%f\n", *int_float_map_get( &our_map, 5 ) );
  int_float_map_cleanup( &our_map );
}
```

</td>
<tr>
</tr>
<tr>
<td>

```c
// Typed pointer with hidden metadata paradigm
#include <stdio.h>
#include "other_container_lib.h"

typedef struct
{
  int key;
  float el;
} int_float_pair;

int main( void )
{
  int *our_vec = NULL;
  vec_push( our_vec, 5 );
  printf( "%d\n", our_vec[ 0 ] );
  free( our_vec );

  int_float_pair *our_map = NULL;
  map_insert( our_map, 5, 0.5 );
  printf( "%f\n", *map_get( our_map, 5 ) );
  free( our_map );
}
```

</td>
<td>

```c
// void pointers paradigm
#include <stdio.h>
#include "other_container_lib.h"

int main( void )
{
  vec our_vec;
  vec_init( &our_vec, sizeof( int ) );
  vec_push( &our_vec, &(int){ 5 } );
  printf( "%d\n", *(int *)vec_get( &our_vec, 0 ) );
  vec_cleanup( &our_vec );

  map our_map;
  map_init( &our_map, sizeof( int ), sizeof( float ) );
  map_insert( &our_map, &(int){ 5 }, &(float){ 0.5f } );
  printf( "%f\n", *(float *)map_get( &our_map, &(int){ 5 } ) );
  map_cleanup( &our_map );
}





```

</td>
</tr>
<table>

## Installation

Just download `cc.h` and place it in your project's directory or your shared header directory.

## Examples

**Vector** (a dynamic array):

```c
#include <stdio.h>

#define CC_SHORT_NAMES // Exposes API without the cc_ prefix
#include "cc.h"

int main( void )
{
  vec( int ) our_vec;
  init( &our_vec );

  // Adding elements to end.
  for( int i = 0; i < 10; ++i )
    if( !push( &our_vec, i ) )
      exit( 1 ); // Out of memory.

  // Inserting an element at an index.
  for( int i = 0; i < 10; ++i )
    if( !insert( &our_vec, i * 2, i ) )
      exit( 1 ); // Out of memory.

  // Retrieving and erasing elements.
  for( int i = 0; i < size( &our_vec ); )
  {
    if( *get( &our_vec, i ) % 3 == 0 )
      erase( &our_vec, i );
    else
      ++i;
  }

  // Iteration #1.
  for_each( &our_vec, el )
    printf( "%d ", *el );
  printf( "\n" );
  // Printed: 1 1 2 2 4 4 5 5 7 7 8 8

  // Iteration #2.
  for( int *el = first( &our_vec ); el != end( &our_vec ); el = next( &our_vec, el ) )
    printf( "%d ", *el );
  printf( "\n" );
  // Printed: Same as above.

  cleanup( &our_vec );
}
```

List:

Map:

Set:

Prefixed API:

Destructors:

Comparsion and hash functions:

**Containers of containers:**

Containers of containers work exactly as expected. However, you probably want to define a destructor so that the inner container is automatically cleaned-up upon clean-up of the outer container.

## FAQ

### How does does it work?

`cc.h` associates type information with a container handle by declaring it as a pointer in the form of `element_type (*(*)[ container_type_id ])( key_type * )`. The pointer points to the container's metadata and contents. API macros then use `sizeof`, `typeof`, and `_Generic` tricks to infer the container, element, and key types from the pointer at compile time, selecting the relevant function and passing the type information, along with the pointer, into it. 

Destructor, comparison, and hash functions are, where needed, infered via a novel technique for user-extendable `_Generic` macros.

An article detailing these and other techinques is in the works.

### How is it tested?

cc.h has been tested under GCC, MingW, and Clang. `unit_tests/unit_tests.c` includes unit tests for all container types, with an emphasis on corner cases. `tests_against_stl/tests_against_stl.cpp` includes randomized tests that perform the same operations on equivallent `cc.h` and C++ STL containers and then check that they remain in-sync. Both test suites use a tracking and randomly failing memory allocator in order to detect memory leaks and test out-of-memory conditions.

### How does it affect compile times?
    
...

### What's next?

The next major version should include `NULL`-terminated strings, ordered maps, and ordered sets.

<details><summary><h2>API</h2></summary>
<p>
  
> **Warning**
> API macros may evaluate their first argument - the pointer to the container - multiple times, so never use expressions with side effects (e.g. `&our_containers[ ++i ]` ) for that argument. In GCC and Clang, attempting to do so will cause a compiler warning. All other arguments are only evaluated once.

> **Note**
> If `CC_NO_SHORT_NAMES` was declared, all API macros are prefixed with `cc_`.

> **Note**
> Duplicating a container handle via assignment and then operating on the duplicate will invalidate the original. Hence, only create a duplicate via assignment (including through function parameters and return values) if you have finished with the original.

> **Note**
> An iterator is a pointer to an element in the container or to the associated `end` (or `r_end`, if the container supports it). In the documentation below, these pointers are called "pointer-iterators".

> **Note**
> In the documentation below, `el_ty` is the container's element type and `key_ty` is the container's key type (where applicable).

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
Returns true, or false if unsuccessful due to memory allocation failure.

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

</p>
