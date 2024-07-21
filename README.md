<picture><img src="./header.svg" alt="CC: Convenient Containers"></picture>

Convenient Containers (**CC**) is a small, usability-oriented generic container library for C that provides **vectors**, **doubly linked lists**, **unordered maps**, **unordered sets**, **ordered maps**, and **ordered sets**.

Its features include:

* Fully generic API.
* Type safety.
* User-defined destructor, comparison, and hash functions associated with element and key types.
* No assumption of successful memory allocation.
* Single header.
* Compiles in C and C++.

It requires C23, or C11 and compiler support for `typeof`, or C++11.

It is distributed under the MIT license.

Try it online [here](https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1AB9U8lJL6yAngGVG6AMKpaAVxYMQAJgCcpBwAyeAyYAHLuAEaYxBIapAAOqAqEtgzObh7efonJNgJBIeEsUTFccZaY1qlCBEzEBOnunr4WmFZ5DDV1BAVhkdGxFrX1jZktCsM9wX3FA2UAlBaorsTI7BwApF4AzMHIblgA1BvbjhP4qAB0CCfYGxoAglu7DPuuRyeOCAQE8QogAHoAcQmAB3S7AQgIVwRVxKVayRgES5oFgAgBSTGQAGsFAIHrQDAwAY5HACWExggDkMhrrd7k9HsECIcKcEIIcAG6oPDoQ7zBkbADsACEGYdDvFiMyqBytl4AGqVEDHLxefknMWPCWcyoc5n8w7LYjGXXITXiw7BQhyrwANmNpsqGu2WsZDwlQMOD3Q%2BCMhzamDYggUhyIAYY6Eulv4xH1gitx22ABFDhpNYnPocyhmtmKvCK8BrtRKrbLDmAwPE4QhbQ6Vk7zIn5sWPaXhW7S6WvQB5Vws1BUVlBkgAT1IhzxhyYERIyMtXf2mEMrnidcdZpdna7h2ImAIKwY2YtJYlwuTgpLXoAkgx4TZ/YYA/RgyymG%2Bj8EsKpoyXY/GWSLE5U3TV1M1ObNQJFVV80LVsuzwctK2Ce91wbM0JyLAAqQ4vEww0BVPY5RQXbsAUOPsByHNgWDHCcpxnOdfzbRd6BXNdVXrE1NwFV1SIlPcD2II8uBPFjiIvR5LS9AAlfdpUwTlgmAadIwDEFkn9QNXwUZiJX/K0EyAlM0wzYzHEnPAAC9MDQ7jnV46DCPExCOSw4B9zsxt8JbLYAFZDm2JMLxMjR4J3aImCULyMObMSuzaJR%2BJgvM8DE6TyOvAhIo6VVti4PTDljYxl2QWtOI3SoJzacKJSlGVbS8fy%2BXlCcsJqxyMsOZRpUETB0BVLhs1wkbJEOMaAoCoVDmmgAOQ5ZsvNsb2ykFcueLxCoMg12toYKirwYgJhihyMxqytgIjdATuQLdn32kJVAIG7qr2ltnK7erBHLeVmtVPDDl2rcup65l%2BpVIRWEwadQ0Y3VmMtJd2Ju4GpJLL6CB%2BtUNj8xwGCCCYVXlVG21oPBjsMlkW0dMmJniwybQqhtaap9Kr3In0/RU7SkVDcMHC2kgAPAkCzKTCyczAvNUtqssOUrasFHKrYuOMFmfNljtkt7fsjWokdiHHSdUGnWd6kK1jlwYVcvJZkmd13fdD2PPiiPPJbPUyu9ogfFSnx5hMoljaHDFQAgEGiZ8gyRQW40pwGasumgjuepmTTtxz7oukyHFt8nWbAxOTMe1OVZp/PXoI0jXIrMAUJ9vOJletqOs1kiiM9ijdcHYdaMN%2BiTcY83kqR62OLL5n8/tndBOd0TXfE920eW8jsA05So502PhaBpPDopif06nzPzrAS7c7TtXj4zD7Sxr3evACoLgMusLb4Svbz402zL/V%2B7OqIolTAyUi6phLo3AglcAHuk7llHKqQ8oFRjCQEqWJlb2nLk3f%2BpEMZYyao/FqaoW5vXpl6UGfUBrDSGuqdUY0JqHCmjNBaC0PaHBWvAgQeVNrILjjtUBB0U4QLuqfc%2BkYhEn0/sXTAT0IFQNlrgxqf1WoJxIQvTu5DsqUMhmwGGptUDw1YaPG2v9r4L0tAo/6OM8YAFkmDxCJmqaeXpkyVAMHuacrI7GHFBFCeO2JMCjlhmpJWc5N680KhSDizJ6IIFCdTBskT6bWlLhghJXjoFdVvPeDeAcCC6R4cLYyoswLmUgrmAsMt35y1rvXeoXlImYXwnmYa71SJaw7mw8ilE9a9zosbPRw8OlGPHqkk0kTp5dlnsJF224zxCkkjAzphw15RRyS%2BcJBT45FNMiU8WZTdnSxMkFKpkVoqXwaXFMx7NDhyQIApJSWl1khm3ls/aUFwIS3edLAsRZb7tPEiE%2BoKj9oeRSarC5vz6YShrh1ZKFjfoEJAEoohVpiHTyXivbqvVNGDW8LhEAQVJAgEmiAO0M0QDzVmiAHwrD2FrQQc8IaEBcmhgELQUc8xY6oLKvUuxUCcHYrwcigGQNoHqOxeDQKDDmHqjJTS5esDVrvgZTsdUzKnl5NUnyfxgTOU8O5eg8FfLDg6v5UReF2NEXCraqa4FYqlkaMlV4Al41iXkvmlwPFflSULWpbSzKSr1o7G2C8wFLJd4mWTgfUZxhxkSNrqI665z0lnUkWA6RYLHQXNblUi1%2BC/IDWtYDHVxgDKH1jcajqaL7VkIlVoqGui4aYARiWYZvL4gk3MYKxRuNOj7gceqe1ShU4GniSaYdSSGCM3LcOzt1ysk%2BzWdHZ5myDTbPeaUyW0FvlwSqTXZC3s6mX2HRrKp/yHY6yor0/u/Sh7ziGWxMeXlZ32smU7aZ89ZkSX9cs9ejzl15JeWut5YssxbszBUo5stTk/xnfuHybNMW3PuUureq6jIgYORBcDO7IUlnPXfBMEbUyguffBy5X6YUkI6Xmotor6YYvFWDSh1DxrSrmj%2BuB9LOGMq5aVQ1joT3YPNd2yxBD/rVtIeRR1lDaFMKGgFRaCqllceVTx1VQGiP8KjZmhsL7U0JpzmI49%2B5hFpsOOAkzkD/7yNEwigtEm7VSaxcxiGDaop6IMcpttVnO3zIZBwRYtBOB%2BV4J4bgvBUCcAsniFYaxEG8AIJoQLiwI5MCwDECAixsSkp8JcLgXhZo%2BC4D4WadoNB2i8EKLgs19CcEkGF5LpAoscF4P8OISWOBaEWHAWASBpGVH7CQcglA6jAAUMoQwbQhCxNBOFngpBUTxDoGphgk2Qi0Bm6gObTWlt0AGMgYABW8J7foMQUIUNOC8FO9ESiW2dtdd4AN5ADxiDjau/4VQlQah%2Bg%2B/wQQIgxDsCkDIQQigVDqEe6QXQXB9CGBMGYfQeAIj/EgIsVA8QOj/Fa813UxBpRYFR1l0gxBXCCDwGwAAKqgFwRPFixdWMDoYfp1vTdm/NxLe51gLdBCCexEWgshca1Dlr2AvvICG8QQ4qgysAFo7RjWADSbMm01WOAnLgQgJBEHzES8l%2BYqXlwZcoNlglGhLizWqxoWaGguDbB8NsIUhW8LBY4A10g4WtDNY%2B%2B10gnXuuC44F4YXXuWt68ewb0gePkh2EkEAA%3D%3D).

## Rationale

Traditionally, C container libraries require users to define types for every container/content type combination and to specify the container type and (often) the content types (whether by casting, type-specific function names, or some other mechanism) at every API call. This causes verbosity and syntax noise.

In contrast, **CC** requires no type definitions and provides an API agnostic to container and content types. The result is simpler, more readable code. The following table compares **CC** usage to other container library paradigms.

<table>
<tr>
</tr>
<tr>
<td valign="top">
<b>CC</b>:

```c
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
<td valign="top">
Template-instantiation paradigm:

```c
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
<td valign="top">
Typed-pointer/hidden-metadata paradigm:

```c
#include <stdio.h>
#include "other_container_lib.h"

typedef struct
{
  int key;
  float element;
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
<td valign="top">
<code>void</code>-pointers paradigm:

```c
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
    {
      // Out of memory, so abort.
      cleanup( &our_vec );
      return 1;
    }

  // Inserting an element at an index.
  for( int i = 0; i < 10; ++i )
    if( !insert( &our_vec, i * 2, i ) )
    {
      // Out of memory, so abort.
      cleanup( &our_vec );
      return 1;
    }

  // Retrieving and erasing elements.
  for( int i = 0; i < size( &our_vec ); )
    if( *get( &our_vec, i ) % 3 == 0 )
      erase( &our_vec, i );
    else
      ++i;

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
    {
      // Out of memory, so abort.
      cleanup( &our_list );
      return 1;
    }

  // Inserting an element before another element.
  for( int *el = first( &our_list ); el != end( &our_list ); el = next( &our_list, el ) )
    if( !insert( &our_list, el, *el ) )
    {
      // Out of memory, so abort.
      cleanup( &our_list );
      return 1;
    }

  // Erasing elements.
  for( int *el = first( &our_list ); el != end( &our_list ); )
    if( *el % 3 == 0 )
      el = erase( &our_list, el );
    else
      el = next( &our_list, el );

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

A `map` is an unordered associative container mapping elements to keys, implemented as a hybird open-addressing, chained hash table that is also available as a [standalone library](https://github.com/JacksonAllan/Verstable).

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
    {
      // Out of memory, so abort.
      cleanup( &our_map );
      return 1;
    }

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
  // Printed: 3 5 8 2 6 9

  // Iteration #2 (elements and keys).
  for_each( &our_map, key, el )
    printf( "%d:%d ", *key, *el );
  // Printed: 2:3 4:5 7:8 1:2 5:6 8:9

  // Iteration #3.
  for( short *el = first( &our_map ); el != end( &our_map ); el = next( &our_map, el ) )
    printf( "%d:%d ", *key_for( &our_map, el ), *el );
  // Printed: Same as above.

  cleanup( &our_map );
}

```

### Set

A `set` is an unordered associative container for elements without a separate key, implemented as a hybird open-addressing, chained hash table also available as a [standalone library](https://github.com/JacksonAllan/Verstable).

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
    {
      // Out of memory, so abort.
      cleanup( &our_set );
      return 1;
    }

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
  // Printed: 2 4 7 1 5 8

  // Iteration #2.
  for( int *el = first( &our_set ); el != end( &our_set ); el = next( &our_set, el ) )
    printf( "%d ", *el );
  // Printed: Same as above.

  cleanup( &our_set );
}

```

### Ordered map

An `omap` is an ordered associative container mapping elements to keys, implemented as a red-black tree.

```c
#include <stdio.h>
#include "cc.h"

int main( void )
{
  // Declare an ordered map with int keys and short elements.
  omap( int, short ) our_omap;
  init( &our_omap );

  // Inserting elements.
  for( int i = 0; i < 10; ++i )
    if( !insert( &our_omap, i, i + 1 ) )
    {
      // Out of memory, so abort.
      cleanup( &our_omap );
      return 1;
    }

  // Erasing elements.
  for( int i = 0; i < 10; i += 3 )
    erase( &our_omap, i );

  // Retrieving elements.
  for( int i = 0; i < 10; ++i )
  {
    short *el = get( &our_omap, i );
    if( el )
      printf( "%d:%d ", i, *el );
  }
  // Printed: 1:2 2:3 4:5 5:6 7:8 8:9

  // Iteration #1 (elements only).
  for_each( &our_omap, el )
    printf( "%d ", *el );
  // Printed: 2 3 5 6 8 9

  // Iteration #2 (elements and keys).
  for_each( &our_omap, key, el )
    printf( "%d:%d ", *key, *el );
  // Printed: 1:2 2:3 4:5 5:6 7:8 8:9

  // Iteration #3.
  for( short *el = first( &our_omap ); el != end( &our_omap ); el = next( &our_omap, el ) )
    printf( "%d:%d ", *key_for( &our_omap, el ), *el );
  // Printed: Same as above.

  // Iteration over a key range, namely from 2 (inclusive) to 7 (exclusive).
  for(
    short *el = first( &our_omap, 2 ), *range_end = first( &our_omap, 7 );
    el != range_end;
    el = next( &our_omap, el )
  )
    printf( "%d:%d ", *key_for( &our_omap, el ), *el );
  // Printed: 2:3 4:5 5:6

  cleanup( &our_omap );
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
  {
    // Out of memory, so abort.
    cc_cleanup( &our_vec );
    return 1;
  }

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
  {
    // Out of memory, so abort.
    cleanup( &our_vec );
    return 1;
  }
  
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
  {
    // Out of memory, so abort.
    cleanup( &our_list );
    return 1;
  }
  
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
#define CC_CMPR our_type, { return val_1.id < val_2.id ? -1 : val_1.id > val_2.id; }
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
#include <stdlib.h>
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

## API Reference

Full API documentation is available [here](api_reference.md).

## FAQ

### How does it work?

**CC** associates type information with a container by declaring it as a pointer in the form of `element_type (*(*)[ container_type_id ])( key_type * )`. The pointer points to the container's metadata and contents. API macros use `sizeof`, `typeof`, and `_Generic` tricks to deduce the container, element, and key types from this pointer at compile time, selecting the relevant function and passing the type information, along with the pointer, into it.

Destructor, comparison, and hash functions are also deduced via a novel technique for user-extendible `_Generic` macros.

[Articles](articles) detailing these and other techniques are in the works.

### How is it tested?

**CC** has been tested under GCC, Clang, MinGW, and MSVC. `tests/unit_tests.c` includes unit tests for all container types, with an emphasis on corner cases. `tests/tests_against_stl.cpp` includes randomized tests that perform the same operations on equivalent **CC** and C++ STL containers and then check that they remain in sync. Both test suites use a tracking and randomly failing memory allocator in order to detect memory leaks and test out-of-memory conditions.

### What's next?

Future versions should include `NULL`-terminated dynamic strings and more performance benchmarks.