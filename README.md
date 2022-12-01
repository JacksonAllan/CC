# CC.H: Convenient Containers

## Introduction
`cc.h` is a usability-oriented generic container library for C. It includes vectors, doubly linked lists, unordered maps, and unordered sets.

Traditionally, C container libraries require users to define types for every container/element type combination and/or to specify the container and/or element type at every API call (whether by casting, type-specific function names, or some other mechanism). The result is verbose code.

In contrast, `cc.h` requires no type definitions and provides an API agnostic to container and element/key types. The following table compares `cc.h` to other container library paradigms:

<table>
<tr>
</tr>
<tr>
<td>

```c
// CC.H
#include "cc.h"
#include <stdio.h>

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
#include "other_container_lib_1.h"
#include <stdio.h>

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
// Typed pointer paradigm
#include "other_container_lib_2.h"
#include <stdio.h>

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
// Void pointers paradigm
#include "other_container_lib_3.h"
#include <stdio.h>

int main( void )
{
  vec our_vec;
  vec_init( &our_vec, sizeof( int ) );
  vec_push( &our_vec, &(int){ 5 } );
  printf( "%d\n", *(int *)vec_get( &our_vec, 0 ) );
  vec_cleanup( &our_vec );

  map our_map;
  map_init( &our_map, sizeof( int ), sizeof( float ) );
  map_insert( &our_map, &(int}{ 5 }, &(float}{ 0.5f } );
  printf( "%f\n", *(float *)map_get( &our_map, &(int}{ 5 } ) );
  map_cleanup( &our_map );
}





```

</td>
</tr>
<table>

`cc.h`'s other standout features include:

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

### How does does cc.h work?

`cc.h` associates type information with a container handle by declaring it as a pointer in the form of `element_type (*(*)[ container_type_id ])( key_type * )`. The pointer points to the container's metadata and contents. API macros then `typeof`, and `_Generic` tricks to infer the container, key, and element types from the pointer at compile time, selecting the relevant function and passing the type information, along with the pointer, into it. 

Destructor, compare, and hash functions are inferred via a novel technique for creating user-extendable `_Generic`-based macros.

An article detailing these and other techinques is in the works.

### How is cc.h tested?

cc.h has been tested under GCC, MingW, and Clang. `unit_tests/unit_tests.c` includes unit tests for all container types, with an emphasis on testing corner cases. `tests_against_stl/tests_against_stl.cpp` includes randomized tests that perform the same operations on equivallent cc.h and C++ STL containers and then check to ensure the same results. Both approaches use a memory-tracking and randomly failing allocator in order to detect memory leaks and test out-of-memory conditions.

### How does cc.h impact compile times?
    
...

### What is the license?

MIT.

### How will cc.h be developed?

The next major version should include `NULL`-terminated strings, ordered maps, and ordered sets.

## API Cheat Sheet
