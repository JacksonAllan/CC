# CC.H: Convenient Containers

## Overview
cc.h is a usability-oriented container library for C. It provides generic vectors, doubly linked lists, unordered maps, and unordered sets.

Traditionally, C container libraries require users to pre-declare container types for every element type and/or specify the container and element type at every API call (whether via casting, passing types as macro arguments, prefixing the call with the name of the pre-declared container type, or some other mechanism):

```c
#include "other_container_library.h"

DEFINE_MAP( int, double, int_double_map )

int main( void )
{
  int_double_map our_map;
  int_double_map_init( &our_map );                  
  int_double_map_insert( &our_map, 5, 0.5 );        
  int_double_map_cleanup( &our_map );
}
```

In contrast, cc.h requires no pre-declarations and provides a fully generic yet type-safe API:

```c
#include "cc.h"

int main( void )
{
  map( int, double ) our_map;
  init( &our_map );
  insert( &our_map, 5, 0.5 );
  cleanup( &our_map );
}
```
It also provides these features:

- User-defined destructor, comparison, and hash functions associated with element and key types.
- Handles memory allocation failure.
- Single header.
- Compiles in C and C++.

It requires C23, or C11 and compiler support for `__typeof__`, or C++11.

## Installation
Download cc.h and place it in your project's folder.

## Simple Usage

Vector:

List:

Map:

Set:

## Advanced Usage

Destructors:

Comparsion and hash functions:

Containers of containers:

## Advanced Usage

Destructors:

Comparsion and hash functions:

Containers of containers:

## Questions

**How does does cc.h work?**

cc.h packs compile-time type information into each container handle.

Various preprocessor, `typeof`, and `_Generic` tricks are then used to infer type information into 

...

**How is cc.h tested?**

cc.h has been tested under GCC, MingW, and Clang in two ways. unit_tests/unit_tests.c includes unit tests for all container types, with an emphasis on testing corner cases. tests_against_stl/tests_against_stl.cpp includes randomized tests that perform the same operations on equivallent cc.h and C++ STL containers and check to ensure they remain in sync. Both approaches use a tracking and randomly failing memory allocator in order to detect memory leaks and test out-of-memory conditions.

**What is the liscence?**

MIT.


## API
