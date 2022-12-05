# CC.H: Convenient Containers

**CC.H** is a usability-oriented generic container library for C that provides vectors, doubly linked lists, unordered maps, and unordered sets.

Its features include:

- Fully generic API.
- Type safety.
- User-defined destructor, comparison, and hash functions associated with element and key types.
- No assumption of successful memory allocation.
- Single header.
- Compiles in C and C++.

It requires C23, or C11 and compiler support for `typeof`, or C++11.

It is distributed under the MIT license.

## Rationale

Traditionally, C container libraries require users to define types for every container/element type combination and to specify the container type and (often) the element type (whether by casting, type-specific function names, or some other mechanism) at every API call. This causes verbosity and syntax noise.

In contrast, **CC.H** requires no type definitions and provides an API agnostic to container and element types. The result is simpler, more readable code. The following table compares **CC.H** usage to other container library paradigms:

<table>
<tr>
</tr>
<tr>
<td>

```c
// CC.H
#include <stdio.h>
#include "cc.h"

int main( void )
{
  vec( int ) our_vec;
  init( &our_vec );
  push( &our_vec, 5 );
  printf( "%d\n", *get( &our_vec, 0 ) );
  cleanup( &our_vec );

  map( int, float ) our_map;
  init( &our_map );
  insert( &our_map, 5, 0.5f );
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
// Typed pointer/hidden metadata paradigm
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
  map_insert( our_map, 5, 0.5f );
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

### Vector

A `vec` is dynamic array that stores elements in contiguous memory.

```c
#include <stdio.h>
#include "cc.h"

int main( void )
{
  vec( int ) our_vec;
  init( &our_vec );

  // Adding elements to end.
  for( int i = 0; i < 10; ++i )
    if( !push( &our_vec, i ) )
      return 1; // Out of memory.

  // Inserting an element at an index.
  for( int i = 0; i < 10; ++i )
    if( !insert( &our_vec, i * 2, i ) )
      return 1; // Out of memory.

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
  // Printed: 1 1 2 2 4 4 5 5 7 7 8 8

  // Iteration #2.
  for( int *el = first( &our_vec ); el != end( &our_vec ); el = next( &our_vec, el ) )
    printf( "%d ", *el );
  // Printed: Same as above.

  cleanup( &our_vec );
}
```

### List

A `list` is a doubly-linked list with sentinels.

```c
#include <stdio.h>
#include "cc.h"

int main( void )
{
  list( int ) our_list;
  init( &our_list );

  // Adding elements to end.
  for( int i = 0; i < 10; ++i )
    if( !push( &our_list, i ) )
      return 1; // Out of memory.

  // Inserting an element before another element.
  for( int *el = first( &our_list ); el != end( &our_list ); el = next( &our_list, el ) )
    if( !insert( &our_list, el, *el ) )
      return 1; // Out of memory.

  // Erasing elements.
  for( int *el = first( &our_list ); el != end( &our_list ); )
  {
    if( *el % 3 == 0 )
      el = erase( &our_list, el );
    else
      el = next( &our_list, el );
  }

  // Iteration #1.
  for_each( &our_list, el )
    printf( "%d ", *el );
  // Printed: 1 1 2 2 4 4 5 5 7 7 8 8

  // Iteration #2.
  for( int *el = first( &our_list ); el != end( &our_list ); el = next( &our_list, el ) )
    printf( "%d ", *el );
  // Printed: Same as above.

  cleanup( &our_list );
}
```

### Map

A `map` is an unordered container associating elements with keys, implemented as a [Robin Hood hash table](https://www.sebastiansylvan.com/post/robin-hood-hashing-should-be-your-default-hash-table-implementation).

```c
#include <stdio.h>
#include "cc.h"

int main( void )
{
  map( int, short ) our_map;
  init( &our_map );

  // Inserting elements.
  for( int i = 0; i < 10; ++i )
    if( !insert( &our_map, i, i + 1 ) )
      return 1; // Out of memory.

  // Erasing elements.
  for( int i = 0; i < 10; i += 3 )
    erase( &our_map, i );

  // Retrieving elements.
  for( int i = 0; i < 10; ++i )
  {
    short *el = get( &our_map, i );
    if( el )
      printf( "%d:%d ", i, *el );
  }
  // Printed: 1:2 2:3 4:5 5:6 7:8 8:9

  // Iteration #1 (elements only).
  for_each( &our_map, el )
    printf( "%d ", *el );
  // Printed: 2 3 5 6 8 9

  // Iteration #2 (elements and keys).
  for_each( &our_map, key, el )
    printf( "%d:%d ", *key, *el );
  // Printed: 1:2 2:3 4:5 5:6 7:8 8:9

  // Iteration #3 (elements and keys).
  for( short *el = first( &our_map ); el != end( &our_map ); el = next( &our_map, el ) )
    printf( "%d:%d ", *key_for( &our_map, el ), *el );
  // Printed: Same as above.

  cleanup( &our_map );
}
```

### Set

A `set` is a [Robin Hood hash table](https://www.sebastiansylvan.com/post/robin-hood-hashing-should-be-your-default-hash-table-implementation) for elements without a separate key.

```c
```

### Prefixed API

**CC.H** macro names like `get` and `init` may collide with names in your own code. If so, define `CC_NO_SHORT_NAMES` before including `cc.h` to expose only prefixed API.

```c
#define CC_NO_SHORT_NAMES
#include "cc.h"

int main( void )
{
  // All API macros, including container declarations, are now prefixed with "cc_".
  cc_vec( int ) our_vec;
  cc_init( &our_vec );
  
  if( !cc_push( &our_vec, 5 ) )
    return 1; // Out of memory.

  cc_cleanup( &our_vec );
}
```

### Destructors

**CC.H** supports per-type destructors with the signature `void ( type val )`. Once defined, the destructor is automatically called whenever an element (or key) of the specified type is removed from a container.

```c
#include <stdio.h>
#include "cc.h"

#define CC_DTOR int, { printf( "%\d ", val ); } // First #define CC_DTOR as <type>, { <function body> }.
#include "cc.h"                                 // Then re-include cc.h

int main( void )
{
  vec( int ) our_vec;
  init( &our_vec );
  
  if( !push( &our_vec, 5 ) )
    return 1; // Out of memory.
  
  cleanup( &our_vec );
  // Printed: 5
}
```

Destructors are especially useful when creating containers of containers.

```c
#define CC_DTOR vec( int ), { cleanup( &val ); }
#include "cc.h"

int main( void )
{
  list( vec( int ) ) our_list;
  init( &our_list );
  
  vec( int ) our_vec;
  init( &our_vec );
  
  if( !push( &our_list, our_vec ) )
    return 1; // Out of memory.
  
  cleanup( &our_list ); // cleanup( &our_vec ) called automatically.
}
```

### Custom comparsion and hash functions

**CC.H** includes default comparison and hash functions for fundamental integer types and `NULL`-terminated strings (`char *`). Hence, these types can be used as `map` keys and `set` elements "out of the box".

To use other types or overwrite the default functions for the aforementioned types, define custom comparison and hash functions with the signatures `int ( type val_1, type val_2 )` and `size_t ( type val )`, respectively.

```c
#include <stdio.h>
#include "cc.h"

typedef struct
{
  unsigned int id;
} our_type;

// First define comparision and hash functions.
#define CC_CMPR our_type, { return ( val_1.x > val_2.x ) - ( val_1.x < val_2.x ); } // 0 in the case of val_1 == val_2
                                                                                    // < 0 in the case of val_1 < val_2
                                                                                    // > 0 in the case of val_1 > val_2
#define CC_HASH our_type, { return val.id * 2654435761ull; }
#include "cc.h" // Then re-include cc.h

int main( void )
{
  // Now we can use our own type as map keys and set elements.
  map( our_type, int ) our_map;
  set( our_type ) our_set;
}

```

### Custom allocation and free functions

...

```c
```

## FAQ

### How does does it work?

**CC.H** associates type information with a container by declaring it as a pointer in the form of `element_type (*(*)[ container_type_id ])( key_type * )`. The pointer points to the container's metadata and contents. API macros use `sizeof`, `typeof`, and `_Generic` tricks to deduce the container, element, and key types from this pointer at compile time, selecting the relevant function and passing the type information, along with the pointer, into it.

Destructor, comparison, and hash functions are also deduced via a novel technique for user-extendable `_Generic` macros.

An article detailing these and other techinques is in the works.

### How is it tested?

**CC.H** has been tested under GCC, MingW, and Clang. `unit_tests/unit_tests.c` includes unit tests for all container types, with an emphasis on corner cases. `tests_against_stl/tests_against_stl.cpp` includes randomized tests that perform the same operations on equivallent **CC.H** and C++ STL containers and then check that they remain in-sync. Both test suites use a tracking and randomly failing memory allocator in order to detect memory leaks and test out-of-memory conditions.

### How does it affect compile times?
    
...

### What's next?

The next major version should include `NULL`-terminated strings, ordered maps, and ordered sets.

## [API reference](api_reference.md)
