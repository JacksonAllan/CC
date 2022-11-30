# cc.h: Convenient Containers

## Introduction
`cc.h` is a usability-oriented generic container library for C. It includes vectors, doubly linked lists, unordered maps, and unordered sets.

Traditionally, C container libraries require users to pre-declare container types for every element type. They also require the user to specify the container and/or element type at every API call (whether via casting, passing types as macro arguments, prefixing the call with the name of the pre-declared container type, or some other mechanism). The consequence is code verbosity.

In contrast, `cc.h` requires no pre-declarations and provides a fully generic API:

<table>
<tr>
<td>

```c
#include "other_container_library.h"

DEFINE_VEC( int, double, int_vec )
DEFINE_MAP( int, double, int_double_map )

int main( void )
{
  int_vec our_vec;
  int_vec init( &our_vec );
  int_vec_push( &our_vec, 5 );
  int_vec_cleanup( &our_vec );

  int_double_map our_map;
  int_double_map_init( &our_map );                  
  int_double_map_insert( &our_map, 5, 0.5 );        
  int_double_map_cleanup( &our_map );
}
```

</td>
<td>

```c
#include "cc.h"

int main( void )
{
  vec( int ) our_vec;
  init( &our_vec );
  push( &our_vec, 5 );
  cleanup( &our_vec );

  map( int, double ) our_map;
  init( &our_map );
  insert( &our_map, 5, 0.5 );
  cleanup( &our_map );
}




```

</td>
</tr>
<table>

cc.h also provides these features:

- User-defined destructor, comparison, and hash functions associated with element and key types.
- Handles memory allocation failure.
- Single header.
- Compiles in C and C++.

It requires C23, or C11 and compiler support for `__typeof__`, or C++11.

## Installation

Download `cc.h` and place it in your project's folder or your shared header folder.

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

Destructors:

Comparsion and hash functions:

Containers of containers:

## FAQ

**How does does cc.h work?**

`cc.h` packs compile-time type information into each container handle, which is actually a pointer in the form of `element_type (*(*)[ container_type_id ])( key_type * )` pointing to the container's metadata and contents. API macros then use preprocessor, `typeof`, and `_Generic` tricks to infer the container, key, and element types from the container handle, selecting the relevant function and passing the type information, along with the aforementioned pointer, into it. 

Destructor, compare, and hash functions are inferred via a novel technique for creating user-extendable `_Generic`-based macros.

An article detailing these and other techinques is forthcoming.

**How is cc.h tested?**

cc.h has been tested under GCC, MingW, and Clang in two ways. unit_tests/unit_tests.c includes unit tests for all container types, with an emphasis on testing corner cases. tests_against_stl/tests_against_stl.cpp includes randomized tests that perform the same operations on equivallent cc.h and C++ STL containers and check to ensure they remain in sync. Both approaches use a tracking and randomly failing memory allocator in order to detect memory leaks and test out-of-memory conditions.

**What is the liscence?**

MIT.

**How will cc.h be developed?**

The next major version should include `NULL`-terminated strings, ordered maps, and ordered sets.

## API Quick Reference
