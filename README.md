<picture><img src="./header.svg" alt="CC: Convenient Containers"></picture>

**CC** is a small, usability-oriented generic container library for C that provides **vectors**, **doubly linked lists**, **unordered maps**, and **unordered sets**.

Its features include:

- Fully generic API.
- Type safety.
- User-defined destructor, comparison, and hash functions associated with element and key types.
- No assumption of successful memory allocation.
- Single header.
- Compiles in C and C++.

It requires C23, or C11 and compiler support for `typeof`, or C++11.

It is distributed under the MIT license.

Try it online [here](https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1AB9U8lJL6yAngGVG6AMKpaAVxYMJATlIOAMngMmABy7gBGmMR6AA6oCoS2DM5uHt6ksfE2AgFBoSwRUVwWmFZZDEIETMQEye6eXD6WmNaJFVUEOSHhkXoKldW1qQ0W/R2BXfk9XACUFqiuxMjsHACkAEwAzIHIblgA1Csbjn34qAB0CIfYKxoAgutbDDuu%2B4eOCAQE0QogAPS/xCYAHczsBCAhXGFXEpFrJGAQzmgWL8AFJMZAAawUAlutAMDF%2BjkcvxYTECv2QyAuVxu9zugQIe1JgQgewAbqg8Og9tNaSsAOwAIVpez20WIDKorPWawAas0QAc1mseYdhXdRWzmqyGTy9vNiMYtcg1SK9oFCNK1gA2A1G5qqjbqum3UX/Pa3dD4Ix7EqYNiCBR7Ii%2BhjoM5m/jEHWCc0HDYAET2GjVcbeey4KadSuFa0FeFVGtF5qlezAYGi0IQVttC3t5jj00LruLoswqktGcdgr27oA8q5GagqEz/SQAJ6kPbYvbaaGMphhEgIvlF90ASQYMJsPsMvvoAYXC4Y5rD7YjRajMcZBcOSazPdvRwzD5z63zzdbeFL5cC25rdrGlOBYAFR7GswF6ryRatu2nZcN2va/HsA5DiObAsBOF4um6yEAEqYAQEqYGygTAHshjcpETDxD6fqHgo2Gilep43vG96pk%2BjjTngABemAAXWxqIdBLYCs6X6liBwCEYJhpAY2SoAKx7Bs7F3smn6tr6gJKHJ9aQbyTpmm2tBKCZxbvu%2BeCmkWAoJquLYbgQ1FlEqGxcExexRsYmDotWSq1vJzRTiUWmiuKkpWms6xKdyMpTiBYVGRJSF7MoEqCJg6CKghCEqiqkh7EVKkqfyezlQAHHslWObhezri5gJuQ8axeSxupJbQ7HeXgxB9PpwkpfuZZgBpDiDQ6w1hRpQSqAQk0Nsl4ViplBCljKsXxcqiXJbZTnIRlDLZYqQisJgFFBouqBathZo7H5DCuNEi3dnVq1RUqMVKY4DABH0ioym9Ra0HgA2sXqdqg30%2B2ihaC2BVDYOMil73up63rkfR8JBiGDjtSQ15pomyacfG3GZqmVl5gWomSay5aVgoAXrEFxjQwQhkrW2HYIwhw39oO%2BroWOxDjnda7IZu25kRRJ7Y7GERRhdhioAQCCRPu/rwgT0YQ11PU0P1COs0jfSIWF5bjWG%2Bkcxb3Wze2Js2mbnMjU2dPFt%2BDNgH%2BkTO2zHOhbQu3dR7Fk8/BiGC2ho6YWLEsHXs2C6bLCsEIxkaE/rM0k0b4Om3WdvTd1VskxNiNF8jIlmuJFne3sBuxap6kkxo3MjeNukCZXhpB%2B7sPFiU5kwUPDsk3NAeu8HwNifyDl3GazmuYk7meVnhp%2BcgLMu1XfQz57EVrRtypbV9oez/VR1ZTlXb5eBxWP6VFUv9VtWL5LDVNUwLWbG1G9EybnnPqBdd592riXUa1t0C2wgamXOSZJ6wP3u7FakVBAnximsOK59G57WMp/a%2BLlb5nTYJdCiS5brvQeoYZ6yCUb7TNOg9a0UVg/QYAAWSYNEQGypL5pQTM0AwxAVZMm4XsIE4IIYYkwOOK6YZpwIGXFrBiXlSQvVYlOZmyimx2nUYPeG%2Bl1GzyXlLLc/s04HhxrrImT4OLZi4i%2BKmeZrIrQbr%2Bcx1QjHcOApBd8XZw6j0jnzaOyFULCzjlhNGyEU40UsdrQMNiIZ2NJg48mTi0lWRJmpQ%2BOkaI90LoadRhlGGfwIkRPAJF4mqIAcknqr5HGU2zNTD8ns66j20dUPB48kwySnnWYpjZB5w1LHtIJH0MGsOwTlM%2BCVzQX1RnZeepj0prROhmEABUQBqUkCAUqIBrQVRAG/EAXholfxXgINeewIDpyDAIWg45pi618v5bx0QD4WWYZg2ZO1un8PdEQ9ZKo1IqUOdVM5H8k6NUuSeVqNy7ly25DIuRzyN6vO3u8qcKLPmj2%2BVMuKIBfkQUbji/5iyk5AtvlwTZ4FtnFT2XsJSByjknMhThNKMLmqrweBsJJnTGRAKTPnfpRTxGQLLkmCuhTjDGMgY7eaWLUG5PxV9LaRLpm4JAii4wLEZWDOSgswegK1kkPOuQ66VCoWihoU9DR%2BrxWlJbKqza7ChCEV4SqCloolAI11LousvqDEME7DK31JjP7SwsXRKxiTam6hSQ09JTSewtNpvXH8vtPGiuML6rmuTgmsn5qmGOESMJROtWlWJtEsaxozkkhN9SybphTWmFx2SVrUT0r3XNhESkEI5e6cpxFSIxoSfW%2BNsZE3NufK2tNWl2ktjhrGIVew%2Bn6TzUMgd9MB7jJdafTVcyDbeoOMswhprcoPxKq/c5XKf48s2OvS8JAMU7zZpusZS6JksLVYev5x7jWHQvV2QqTKb2VuXtyq5rUG0roQb1Y2G7CL2ygeXG2Pbw3yonk7JDbtloquPgS7aJKAPbrSlS065qaIUJupgRONr6C0PtWA3tDCB32Q4LMWgnAlK8E8BwLQpBUCcG4tiBYSw168AIJoTjswNZMCwFECAswMQHK8GcLgaxKpeAaJVa0GhrRrH5FwSq%2BhOCSD4zJoTnBeA/A0KQaTAnOOkDgLAJA7ZmiDhIOQSgVRgAKGUIYEoQglFAn4zwUgSJoh0HvQIQLQRaAhdQGFqzUW6A9GQMATTEE0v0GIKhJLKWnO%2BFUM0W4xB/M2ZK80Co3oqv8EECIMQ7ApAyEEIoFQ6hiu6CKPiEwZh9B4DCD8SAsxUDRDKD8DgtmDRLF6AQb08XguhfC1JkRywItAkBDw7gznuMcF46QfjgnhMcGwKV5AXniB7FUHpgAtNaIqwBKQZjaiqCAjgpy4EICQNe0wpMyemHJvyinKAqe2RoM4lVjMaEqhoLgGwvAbH5FpiC%2B2LNHas6d2zIB7OOa0EDszHA1iWeK9jhzgPZhan6okEAkggA).

## Rationale

Traditionally, C container libraries require users to define types for every container/element type combination and to specify the container type and (often) the element type (whether by casting, type-specific function names, or some other mechanism) at every API call. This causes verbosity and syntax noise.

In contrast, **CC** requires no type definitions and provides an API agnostic to container and element types. The result is simpler, more readable code. The following table compares **CC** usage to other container library paradigms:

<table>
<tr>
</tr>
<tr>
<td>

```c
// CC
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
// Template-instantiation paradigm
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
// Typed-pointer/hidden-metadata paradigm
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
// void-pointers paradigm
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

Just [download](cc.h?raw=1) `cc.h` and place it in your project's directory or your shared header directory.

## Examples

### Vector

A `vec` is a dynamic array that stores elements in contiguous memory.

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
      exit( 1 ); // Out of memory, so just abort.

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
  // Printed: 1 1 2 2 4 4 5 5 7 7 8 8

  // Iteration #2.
  for( int *el = first( &our_vec ); el != end( &our_vec ); el = next( &our_vec, el ) )
    printf( "%d ", *el );
  // Printed: Same as above.

  cleanup( &our_vec );
}

```

### List

A `list` is a doubly linked list with sentinels.

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
      exit( 1 ); // Out of memory.

  // Inserting an element before another element.
  for( int *el = first( &our_list ); el != end( &our_list ); el = next( &our_list, el ) )
    if( !insert( &our_list, el, *el ) )
      exit( 1 ); // Out of memory.

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

A `map` is an unordered container associating elements with keys, implemented as a [Robin Hood](https://www.sebastiansylvan.com/post/robin-hood-hashing-should-be-your-default-hash-table-implementation) hash table.

```c
#include <stdio.h>
#include "cc.h"

int main( void )
{
  // Declare a map with int keys and short elements.
  map( int, short ) our_map;
  init( &our_map );

  // Inserting elements.
  for( int i = 0; i < 10; ++i )
    if( !insert( &our_map, i, i + 1 ) )
      exit( 1 ); // Out of memory.

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

  // Iteration #3.
  for( short *el = first( &our_map ); el != end( &our_map ); el = next( &our_map, el ) )
    printf( "%d:%d ", *key_for( &our_map, el ), *el );
  // Printed: Same as above.

  cleanup( &our_map );
}

```

### Set

A `set` is a [Robin Hood](https://www.sebastiansylvan.com/post/robin-hood-hashing-should-be-your-default-hash-table-implementation) hash table for elements without a separate key.

```c
#include <stdio.h>
#include "cc.h"

int main( void )
{
  set( int ) our_set;
  init( &our_set );

  // Inserting elements.
  for( int i = 0; i < 10; ++i )
    if( !insert( &our_set, i ) )
      exit( 1 ); // Out of memory.

  // Erasing elements.
  for( int i = 0; i < 10; i += 3 )
    erase( &our_set, i );

  // Retrieving elements.
  for( int i = 0; i < 10; ++i )
  {
    int *el = get( &our_set, i );
    if( el )
      printf( "%d ", *el );
  }
  // Printed: 1 2 4 5 7 8

  // Iteration #1.
  for_each( &our_set, el )
    printf( "%d ", *el );
  // Printed: 1 2 4 5 7 8

  // Iteration #2.
  for( int *el = first( &our_set ); el != end( &our_set ); el = next( &our_set, el ) )
    printf( "%d ", *el );
  // Printed: Same as above.

  cleanup( &our_set );
}

```

### Prefixed API

**CC** macro names may collide with names in your own code. If so, define `CC_NO_SHORT_NAMES` before including `cc.h` to expose only the prefixed API.

```c
#define CC_NO_SHORT_NAMES
#include "cc.h"

int main( void )
{
  // All API macros, including container declarations, are now prefixed with "cc_".
  cc_vec( int ) our_vec;
  cc_init( &our_vec );
  
  if( !cc_push( &our_vec, 5 ) )
    exit( 1 ); // Out of memory.

  cc_cleanup( &our_vec );
}

```

### Destructors

**CC** supports per-type destructors with the signature `void ( type val )`. A destructor is automatically called whenever an element (or key) of the associated type is removed from a container.

```c
#include <stdio.h>
#include "cc.h"

#define CC_DTOR int, { printf( "%d ", val ); } // First #define CC_DTOR as <type>, { <function body> }.
#include "cc.h"                                // Then re-include cc.h.

int main( void )
{
  vec( int ) our_vec;
  init( &our_vec );
  
  if( !push( &our_vec, 5 ) )
    exit( 1 ); // Out of memory.
  
  cleanup( &our_vec );
  // Printed: 5
}

```

Destructors are especially useful when creating containers of containers.

```c
#include "cc.h"

#define CC_DTOR vec( int ), { cleanup( &val ); }
#include "cc.h"

int main( void )
{
  list( vec( int ) ) our_list;
  init( &our_list );
  
  vec( int ) our_vec;
  init( &our_vec );
  
  if( !push( &our_list, our_vec ) )
    exit( 1 ); // Out of memory.
  
  cleanup( &our_list ); // our_vec is cleaned-up automatically.
}

```

### Custom comparison and hash functions

**CC** includes default comparison and hash functions for fundamental integer types and `NULL`-terminated strings (`char *`). Hence, these types can be used as `map` keys and `set` elements straight away.

To use other types or overwrite the default functions for the aforementioned types, define custom comparison and hash functions with the signatures `int ( type val_1, type val_2 )` and `size_t ( type val )`, respectively.

```c
#include "cc.h"

typedef struct
{
  unsigned int id;
} our_type;

// First #define CC_CMPR and CC_HASH as comparision and hash functions.
// The comparison function should return 0 in the case of val_1 == val_2,
// < 0 in the case of val_1 < val_2, and > 0 in the case of val_1 > val_2.
#define CC_CMPR our_type, { return ( val_1.id > val_2.id ) - ( val_1.id < val_2.id ); }
#define CC_HASH our_type, { return val.id * 2654435761ull; }
#include "cc.h" // Then re-include cc.h.

int main( void )
{
  // Now we can use our own type as map keys and set elements.
  map( our_type, int ) our_map;
  set( our_type ) our_set;
}

```

### Custom allocation and free functions

By default, **CC** uses `realloc` and `free` to manage memory. Overwrite these functions by defining `CC_REALLOC` and `CC_FREE`.

```c
#include <stdio.h>
#include "cc.h"

void *our_realloc( void *ptr, size_t size )
{
  printf( "our_realloc called\n" );

  void *new_ptr = realloc( ptr, size );
  if( !new_ptr )
    exit( 1 ); // Out of memory, so just abort.

  return new_ptr;
}

void our_free( void *ptr )
{
  printf( "our_free called\n" );
  free( ptr );
}

#define CC_REALLOC our_realloc
#define CC_FREE our_free
// Now our_realloc and our_free will be used wherever the definitions are visible.

int main( void )
{
  vec( int ) our_vec;
  init( &our_vec );
  push( &our_vec, 10 ); // Printed: our_realloc called
  cleanup( &our_vec );  // Printed: our_free called
}

```

## FAQ

### How does it work?

**CC** associates type information with a container by declaring it as a pointer in the form of `element_type (*(*)[ container_type_id ])( key_type * )`. The pointer points to the container's metadata and contents. API macros use `sizeof`, `typeof`, and `_Generic` tricks to deduce the container, element, and key types from this pointer at compile time, selecting the relevant function and passing the type information, along with the pointer, into it.

Destructor, comparison, and hash functions are also deduced via a novel technique for user-extendable `_Generic` macros.

An article detailing these and other techniques  is in the works.

### How is it tested?

**CC** has been tested under GCC, MinGW, and Clang. `tests/unit_tests.c` includes unit tests for all container types, with an emphasis on corner cases. `tests/tests_against_stl.cpp` includes randomized tests that perform the same operations on equivalent **CC** and C++ STL containers and then check that they remain in sync. Both test suites use a tracking and randomly failing memory allocator in order to detect memory leaks and test out-of-memory conditions.

### Does it impact build times?
    
Yes. The compiler must do extra work deducing types, processing macros, and inlining functions at compile time. `unit_tests.c`, as an example of a program that uses **CC** extensively, compiles in 4.5 seconds in MinGW with `O3` in my development environment, whereas a roughly equivalent program using C++'s STL containers compiles in 3.1 seconds. So consider this point if your project is large and build speed is important.  

### What's next?

Future versions should include `NULL`-terminated dynamic strings, ordered maps, and ordered sets, as well as performance benchmarks.

## [API Reference](api_reference.md)
