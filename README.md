<div align="center">
<picture>
<source media="(prefers-color-scheme: dark)" srcset="logo_dark.svg">
<img src="logo_light.svg" alt="Convenient Containers">
</picture>
</div>

## Overview
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

Try it online [here](https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1AB9U8lJL6yAngGVG6AMKpaAVxYMQAJmkOAMngMmABy7gBGmMQgAGykAA6oCoS2DM5uHt7SCUk2AgFBoSwRUbGWmNYpQgRMxARp7p4%2BFphWuQxVNQT5IeGRMRbVtfUZTQqDXYE9RX3RAJQWqK7EyOwcAKReAMyByG5YANRrm45j%2BKgAdAhH2GsaAIIb2wy7rgdHjggEBHEKIAD0f2ITAA7udgIQEK4wq4lMtZIwCOc0Cw/gApJjIADWCgEd1oBgYf0cjj%2BLCYgT%2ByGQl2utwe90CBH2ZMCEH2ADdUHh0PtZnS1gB2ABCdP2%2BzixEZVDZGy8ADVykQoocvF5eUcRfcxezymzGbz9otiMYdcgNaL9oFCDKvNEjSbyurNpr6XcxQD9nd0PgjPsWpg2IIFPsiH6GOhzhb%2BMQ9YJLYdNgARfYaDXx977Lip50qkVeIV4dVasWW6X7MBgOIwhA2u1LB3meOzItukuCl0lksegDyriZqCozIDJAAnqR9jj9kwwiRERbO7tMIZXHFa/bTU6O539sRMAQlgxM%2Bbi2LBYn%2BcWPQBJBiwmy%2Bwx%2B%2BiBplMV%2BHwJYVSR4vR2NMwsjmTbMhXTY5MxA3MNgLFtOzwMsK0CO813rU1x0LAAqfYvHQg0%2BRPQ5hXnLs/n2Xt%2B0HNgWFHcdJ2nWcf1bBd6GXVcVTrY0Nz5Z1iLFXd92IQ8uGPJjCPPe4LQ9AAlPdJUwdlAmAKdwz9IEkl9f0XwURixT/S040ApMUzTQzHAnPAAC9MBQzjHW40D8NE%2BC2Qw4A9xshtcObDYAFZ9k2BNzyMjRYO3SImCUDy0KbETOxaJReKg/M8BEyTSKvAhwraFVNi4HT9mjYwl2QGt2PXcpxxaUKxQlKUbS8XyeVlccMKq%2By0v2ZRJUETB0BATMBrVNVJH2Ea/L8gV9kmgAOfZpovVtr0yoFsseLx8r0/VWtoQKCrwYgxiiuy0yqisgLDdAjuQTcn12oJVAIK7Kp25tHM7WrBDLWVGpVHD9m2zcOq6xlev6oRWEwKdg3onVGItRdWKuwGJOLD6CC%2B1U1h8xwGACMZ%2BtlZHW1oPBDv0plm3tEmxli/TrTK%2BtqYp1LL1Ir0fSUzSEWDUMHA2kh/zA4CTITMyszTaDoMLN6S2c8tK2rDyma86rCK3Tsez7Q1KOHYgxwnVApxnWp8uYpcGBXJXSeZniCL4vcDyPW3RLPBb3XS29InvJTHy5uMImjSHDFQAgEEiJ8AwRfmY3J/6qvOmgDsehnjSZm7TrAc6HCtsZ052877uTjYOOMZXbte4i5cQz3ahzghnpatrVfbRLNYoodqL12jDfok3EoRi22OLqnraJ7cdwdwSnfVsS3f2D1sDUxSI606PBYBhP9rJ4fGdH%2BzbrOozs5T0u97TGWxTljevD8gKgPOkKL5LeOj7U6yT7LtrabFeLMESl/kyFzrs9ImHUMpZRSDlPKUYSBFQxKVHeqdrYgKfmjDGDUb5NVVI3F6tMPTAx6n1AaXBsKkJGmNfYE0ppzTmnPJaECBA5XWjAmOW0AF7STnXPO8ss7hi4fvdhQCP7IPLqrNB9UfrNTjrg527tOrdUykQ8GbAoZG1QLDOeA9LbCNzu1FGrZxG/SxjjAAskwOIBNVRj3nqRRM5QDC7inMycx%2BxgQQljpiTAI5oYqQUAgWcK9ub5TJGxRktF/G1ANPaEJtMrRF1tNElxejXRyJvHeZefsCDaRYYLQywscymQghLfMUtVZVzAEhL2HkQnoVwtBAaFcCItztjYsiWsBwdxogbNRfcWlaKHgk%2BsITrGdn4o7YSsi2wCnEik1pi8IoZOfEEnJsc8nGQKaLIpGzJZGQCk/cKkUT41Jis7DqMkCByQUhpJZQY16rN2pBQp4scyS2SqFZpok/EBI3kZNy8SS7HOlt/UsbIv4tMMd9TBIBJHYMtDg6xrtWbyJBkQrg3hsIgACpIEA40YhTRALNaaIAACcdD0rLTfJAx4JCICZODAIWgI5ZjRzgSVap5iUHEQhZjTBv14XJLkQQxR/UAp%2BVmmqaI%2BxSX6NSRS1aWw1S0puVk5SPJPHeOZSw1lCDBnGmOeqzlBFuUYJ8n1GFf0MIGukdY/BCjQYYqxTi/Fs00Vqh8niol0rZn0JWlSrYmw7lfMiT85Midt66uMMMgRO1D7JmPogyNSSTr5yMkIhNxym6oIUegxq0LeVSMtV44wel00cvLvyvBpEhX2uUUHaGM4NEyv2P09lcRQGo2zRI7G7Q9yWLVAKic7lY6U3rEoAgsSGD0wTWO9ti0PbpOuZHW5Kz9RrMeZs55oFXkwSfuUyptcT5jpVk/D54827a06V3bpvc5x9JYoPDyM6B2jMnkJYFiLvWkXmepTmyrsm/gFvch%2BIsMybvTCU3ZqsDnv2nXuLyLM537HOZcxZS6sl3NXQ8kD4EwPbqBcWU9ss4whv2H8x9cGTkzzlmC0SNVO1GPzbCgGA6P2CrtaishlDqHzSbT6yljDqUsuKjqkuR7y5cvo5C01fLrUDttSi/qw1qEkLFWS/Y4DfUCYVRh4j7Cw3/PtE%2B5NPCj58MPXubhBdMAPXI/XURWa6oMekwWr%2Bkz5OELBhDVRMNMBw2LC28zNsXQWmNcYhg3ZiBYF3DyEJfabW2PsTUIOh4SBRd6s4ttbjQ4eK8T4nkQamR0vyqgEJ/5wkBJHcaEr5iJ1TojdVttySwE129oE5dAHWEGSw9snDkE8NlIQhUlrHkGu1PA6BEhjSXZERaeejpVEul0WNre2jzb73aITQ1kZJYxlTwmTPD9HVv2odXiurrwGeti3XTs5MeziLQZG4C2dcjkN4Hkid5ZHXcnddAk8vrJS3ky0I2KAr1rzpkZPqNyjlcyw0fHqFqF5qxvMdpqx1p1bUXoq8Ji0aTr3WSoFASuaJK1Maf44eal%2BwlVofpQwRlmqOvase2WuHJYEfOaY65rc7nhWkNFfsSVs0vVgLlX6zYiq6Wqv2OqhQDPWyFWE8zuI44rWs7o45qTZrGMWqtSjtzVb2P9VdQ63HuKCdE89aT0XWnNgBpWaDkj%2BmlfcNjRdZ30a7rWYM/WKHmaJMa55aavNHOddFpLfVjNL0K36%2BRR5/YtbvMNt81bhhKWdTECceqnchg3LjmYGwRlBViAldIRAHYbgkg6mbKGSatLVAvEr5gOXukBbEQd3pre3uqvHP7S1IERhMBFRUpvThkPjmTWfQfTORl%2B9uSH%2BgYFgivdK8Na2Bzn0JGI%2B1y1dVxbAObcj7yaP3ODcKZN9is3mj1sDJLltprHaA8NW7RFtL%2BXe2/WsYkId%2BpKvGC/%2BOyZOJEbIzU5JFNJL2D7dreXQDTDC7X7Ddf7PMHdGHNkauZCSHMTaWZuGbVbObHWTufWJbBifua/YAizSfe2ASN9SZU8aZNTY7RdU7L7IDYKbDK7EDCDW7KDN%2BMguzIFUAxDV7d7Rgz7aAzrACH7MCdgl5AHZAgjHAuCXTFNZMCHTbTA7bajXBcFSTQPLBC1LnC0NHHne1EhJTKhHjT9dTa3CnLYaBRnRXDAijNXcUHQk1PQ4/IGQ3QaUaLjGaFPTTGw8XHTJkR3TvXgl3afONMzNQ8g4zKzGzRwuzP3I1VwpHWTStWPXnBPCKNRRtSwsnbKdRcOX2ZVbPAfPPCGQvKgYvFgUvcvGEPAKvEMQ2WvazBvRopvNeSuJQ3aJ3RI8cXvf6WfQfBwXosI/o6hSfDOc6YY%2BfRfZQ/YNNerMTVndfdGTfEPDwpFDHRTHwnyK/c2DbZY2I05Og%2B4DgeYWgTgHyXgTwbgXgVATgMyHEJYFYKBXgAgTQC4%2BYMOJgKLSgeYTEGIaac4HyAUKQLwaaMEiEqEgUfQTgSQW4r40gR4jgXgX4DQUgT4jgLQeYOAWAJAazcoPsEgcgSgGoYABQZQQwFoIQfxYEO4ngUgZEOIOgcnakoIWgOk1ABk5ElkugPoZAYALgbBfk%2BgYgYICGTgXgMUyIcibk3knE3gIk5AO4YgSk6U0gFUqoH0TU/gQQEQMQdgKQGQQQRQFQdQJU0gXQLgfQHPEAUwYwcwEmMIX4SAeYVAOINoX4NElE9PSULAN0iAeYYgVwQQPANgAAFVQBcCDPmBeOWGNIGB9A5NpPpMZI%2BN3FWCZOBCBAsXuMuOuKRKtNROwFUGJKVH2FUGmmiAAFpogRpgAqRMx1pFVHBxxcBCASAoFZgPivjZgfilx/jgzSAgTcpzhJBIToTJzYSIS4SriOBETSA7itAUTNSMSsT%2BzCyOAvBizVzUS%2BylSBzSB08kg7BJAgA%3D%3D).

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
  vec_cleanup( our_vec );

  int_float_pair *our_map = NULL;
  map_insert( our_map, 5, 0.5f );
  printf( "%f\n", *map_get( our_map, 5 ) );
  map_cleanup( our_map );
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

A `list` is a doubly linked list.

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

### Ordered set

A `oset` is an ordered associative container for elements without a separate key, implemented as a red-black tree.

```c
#include <stdio.h>
#include "cc.h"

int main( void )
{
  oset( int ) our_oset;
  init( &our_oset );

  // Inserting elements.
  for( int i = 0; i < 10; ++i )
    if( !insert( &our_oset, i ) )
    {
      // Out of memory, so abort.
      cleanup( &our_oset );
      return 1;
    }

  // Erasing elements.
  for( int i = 0; i < 10; i += 3 )
    erase( &our_oset, i );

  // Retrieving elements.
  for( int i = 0; i < 10; ++i )
  {
    int *el = get( &our_oset, i );
    if( el )
      printf( "%d ", *el );
  }
  // Printed: 1 2 4 5 7 8

  // Iteration #1.
  for_each( &our_oset, el )
    printf( "%d ", *el );
  // Printed: 1 2 4 5 7 8

  // Iteration #2.
  for( int *el = first( &our_oset ); el != end( &our_oset ); el = next( &our_oset, el ) )
    printf( "%d ", *el );
  // Printed: Same as above.

  // Iteration over an element range, namely from 2 (inclusive) to 7 (exclusive).
  for(
    int *el = first( &our_oset, 2 ), *range_end = first( &our_oset, 7 );
    el != range_end;
    el = next( &our_oset, el )
  )
    printf( "%d ", *el );
  // Printed: 2 4 5

  cleanup( &our_oset );
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

### Custom hash and comparison functions

**CC** includes default hash and comparison functions for fundamental integer types and `NULL`-terminated strings (`char *`). Hence, these types can be used as map and ordered map keys, and set and ordered set elements, straight away.

To use other types or overwrite the default functions for the aforementioned types, define custom hash and/or comparison functions with the signatures `int ( type val_1, type val_2 )` and `size_t ( type val )`, respectively.

Maps and sets require both a hash and comparison function, whereas ordered maps and ordered sets require only a comparison function.

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
  // Now we can use our own type as map and ordered map keys and set and ordered set elements.
  map( our_type, int ) our_map;
  omap( our_type, int ) our_omap;
  set( our_type ) our_set;
  oset( our_type ) our_oset;
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
