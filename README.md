<div align="center">
<picture>
<source media="(prefers-color-scheme: dark)" srcset="images/logo_dark.svg">
<img src="images/logo_light.svg" alt="Convenient Containers">
</picture>
</div>
<br>
<div align="center">
<a href="#Overview"><picture><source media="(prefers-color-scheme: dark)" srcset="images/overview_dark.svg"><img src="images/overview_light.svg" alt="Overview"/></picture></a><!--
--><picture><source media="(prefers-color-scheme: dark)" srcset="images/separator_dark.svg" alt="·"><img src="images/separator_light.svg"/></picture><!--
--><a href="#Rationale"><picture><source media="(prefers-color-scheme: dark)" srcset="images/rationale_dark.svg"><img src="images/rationale_light.svg" alt="Rationale"/></picture></a><!--
--><picture><source media="(prefers-color-scheme: dark)" srcset="images/separator_dark.svg" alt="·"><img src="images/separator_light.svg"/></picture><!--
--><a href="#Installation"><picture><source media="(prefers-color-scheme: dark)" srcset="images/installation_dark.svg"><img src="images/installation_light.svg" alt="Installation"/></picture></a><!--
--><picture><source media="(prefers-color-scheme: dark)" srcset="images/separator_dark.svg" alt="·"><img src="images/separator_light.svg"/></picture><!--
--><a href="#Simple-Examples"><picture><source media="(prefers-color-scheme: dark)" srcset="images/simple_examples_dark.svg"><img src="images/simple_examples_light.svg" alt="Simple Examples"/></picture></a><!--
--><picture><source media="(prefers-color-scheme: dark)" srcset="images/separator_dark.svg" alt="·"><img src="images/separator_light.svg"/></picture><!--
--><a href="#Advanced-Examples"><picture><source media="(prefers-color-scheme: dark)" srcset="images/advanced_examples_dark.svg"><img src="images/advanced_examples_light.svg" alt="Advanced Examples"/></picture></a><!--
--><picture><source media="(prefers-color-scheme: dark)" srcset="images/separator_dark.svg" alt="·"><img src="images/separator_light.svg"/></picture><!--
--><a href="api_reference.md"><picture><source media="(prefers-color-scheme: dark)" srcset="images/api_reference_dark.svg"><img src="images/api_reference_light.svg" alt="API Reference"/></picture></a><!--
--><picture><source media="(prefers-color-scheme: dark)" srcset="images/separator_dark.svg" alt="·"><img src="images/separator_light.svg"/></picture><!--
--><a href="#FAQ"><picture><source media="(prefers-color-scheme: dark)" srcset="images/faq_dark.svg"><img src="images/faq_light.svg" alt="FAQ"/></picture></a><!--
--><picture><source media="(prefers-color-scheme: dark)" srcset="images/separator_dark.svg" alt="·"><img src="images/separator_light.svg"/></picture><!--
--><a href="#Support-and-Contribute"><picture><source media="(prefers-color-scheme: dark)" srcset="images/support_and_contribute_dark.svg"><img src="images/support_and_contribute_light.svg" alt="Support and Contribute"/></picture></a>
</div>

## Overview
Convenient Containers (**CC**) is an ergonomic, high-performance generic container library for C that provides **vectors**, **doubly linked lists**, **unordered maps**, **unordered sets**, **ordered maps**, **ordered sets**, and **null-terminated strings**.

Its features include:

* Fully generic API.
* Type safety without boilerplate container-type definitions.
* User-defined destructor, comparison, and hash functions associated with element and key types.
* No assumption of successful memory allocation.
* Single header.
* Compiles in C and C++.

It requires C23, or C11 and support for `typeof` (available in every major compiler), or C++11.

It is distributed under the MIT license.

Try it online [here](https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1AB9U8lJL6yAngGVG6AMKpaAVxYMJATlIOAMngMmABy7gBGmMR6AA6oCoS2DM5uHt6ksfE2AgFBoSwRUVwWmFZZDEIETMQEye6eXD6WmNaJFVUEOSHhkXoKldW1qQ0W/R2BXfk9XACUFqiuxMjsHACkAEwAzIHIblgA1Csbjn34qAB0CIfYKxoAgutbDDuu%2B4eOCAQE0QogAPS/xCYAHczsBCAhXGFXEpFrJGAQzmgWL8AFJMZAAawUAlutAMDF%2BjkcAMwVAUvwQmCY6HJADcuGdJGcNMZosRIiUqUpfshkBcrjd7ndAgQ9iwmIEIHtaag8Og9tNBSsAOwAIUFez2bJFVCl6zWADVmkQogc1msFYd1XdNbTmlKRQq9vNiMY7cgrRq9oFCHq1gA2F1u5qWjbWoW3TX/Pa3dD4Ix7TlsQQKPZERMMdBnL38YgOwTeg4bAAiew0VsLbz2XHLYbN6rWqrwlptmu9ur2YDA0WhCD9gYWwfMhemLcjbZV4bbbejAHlXKLUFQxZgWCQAJ6kPbYvZMMIkBFe6c7KkMVzRftB92hqfTvbsggLBjVz2tzUq4tK1vRgCSDBhNgJoYib0MmopMOBz6BFgqjZq2ub5qKzaHKWtaqpWRzVmh9brE2Y7TngHZdoEAGXoO7pbs2ABUexrJRTqKm%2BBxqkeM6/Hs86LsubBrsQm7bqgu77tUcHjse9CGOeZGuteiphqxmoPk%2BL7yUxH5fuO0YAEqYAQxB4JgtKBMAu6ZomgLxAmSbwgoomagh3oFshJZlhWzmONueAAF6YNJQ43vhbaEVKVHALpfkUSOZoAKx7BsRafi5GiBdOkRMEoEXNPRcm3pqJRKApOGNngr4RlG7E/gQaVlGaGwMjmJDGFSyB9maA4yVlIEpVq%2BmCB2%2BrrNF8r6luVElDeXrRsovVVegIDVgtFoWpIewrbFsXKnsm0ABx7NtGnlXslXVYktVrHZewOY6Y20All14MQfSZcgAXjV2KEZugz2vbdH1BKoBDPVu42joxYk9TqfprINw3mqNIOlYd00ipgc17EIrCYLuqZ7qgdqiV6J6SRebVXiGOUHRDfVQys0WOAwAR9PN%2BoTa2tB4E9jmiqOQbs30iOOb6pODnz3OlZN7GxvGJnWSmaaCQ4F1XU5d3Ye5WEVrhuHNmDBFEd2vZ%2BaL2XdZOhVzguzrcauG5bjuuMiYVRNniT6ztcYous%2BDim6cpXAC%2B%2ByqfncEtHf%2BkSASZwGy6KES5ljhioAQlLECBq7wkrJCIXsN13TQj2A8LrqezlXXvS5DhGxzYt1uNf2YADVd9MDt2g6xwWdmAJER03BAt/DremyxTGHZxVsrrx/H28Jh4j3sztSUXHvV17d73r7xDPv7qng%2BpIffux2AWcZadgbZDV5lzOd1y5%2Bec27vMr6Xb1gB9ldLyXFa60FHa54NcUJQ%2Bslb%2BqVfoVwsr5D%2B1cW6rzbPlTAhUb6ln%2BoXB%2BItoFdQpvvTSFUqqAhqg8eq8FGrNVamg4uGCEZMW1NTM00M1hDToQPWBU0Zqo3mlwRatFVo8PWltfhu19rYMOsdfBp0HjnQvtnP%2Bt8Hr3wDI/PoP1O5v0zL3ZR9dG5QObpg7qNCCD9XNDDJh19W4C1YSjNGGM2DYyEnjTABNWwL1dgo9BSisFlSpgYmmdMGAAFkmDRGZuaFh7FizNAMOyXcYpAl7CBOCK%2BGJMDrhxmZBQCADynxshdcUJMRR2wydUJ0QZckCx9Kg1xrpcle1Dn%2BACJ8Y7n2IZfR0zlUJuSLB5GsmtGza26h3Yi4dqh%2BVyZReiuEFptzUsPb2ewLZcQnrbASdjHZz2cSM2JHi15KU3ipXKzFg6eOjEfdKDTQLZKkVfNprk6zq26bcnCLl4ogPMulSB5DjCjJHOLA%2BewdJ6QMkZKy5yUyZxaSrIBHSqz3PQlrYqgUzZMXSZkmRpYwoVPdl8nWAd2xSiobM/Rhj6FDRAMYka3pmFbIOaHZGgh2HVhAEtEA8VJAgHWiAf0W0QCCJAF4Smv48EQXEZsThEBGnOgYLQdc0wwVNXRGQypnzAkwJeYSmmDDYZ0VMaEvYtLZrzXirFXaFpOV8uEXM3BJ0BBnT2GKkFBBUnyiSSkmVF85UtQ2dELczqVWsTVXQmGpKNUmKoj67VVKLF0rRmsZlq02Vct2lwRlexoocr2ry/llqxHWoeBsMFUpkVFNRfdAunrlHl1LO/D51Tn5gOQQ3DFJTlW6NVTNIlgayVwxzs64wDlq3NpBpS8x7E9X0usQnHG%2B58aU3WUvGtPzxz%2BoGr4oQulgkWipUoQujoeaDi3WUhgQsPlbpqb8upEcznp1BZc1pqsoWYRhUVPCLyBldyGY2vdukTYvMRbM%2BZ48eJLOngeC64lTyL2PbpWB04dlbxxXvI5h9j7AqvQ6/NVy70POhWrR5pZnmsTShlJeW7soLsOv8/ShlL1n3Q7eyFWGH3YThc%2Br0v6CIFmLeivyJHvk7z1nisxc8l1GODeS3OVKENIzYWjThy0U0CMzUdQVBCRWytIdxr9mC/VtvVYwsTCM%2BMWt1dJ%2BacnNqcKNYp0RQqc2bEkc06RSCS3yPdie2tKiK5qOI1B9zmiP2uh4yDPROmA2ia7eJ4dxnLHzXHbY3G07zWzsgzXcMXphPQ18bOYgWB2TylyeunV4SdhVATs%2BEgOXUYxOiHEhJjpnWOu3IU0UjSLqoFyYhApmTd2uja4Eg9R7FW9eqx42p77qMXIcxh%2Bj6E7lMd6fCl9%2Btu7DKXkNsZlZGyTKHvs0elslyLL4nbQSDtZ6zKS4N%2BdhmYMbzg1d6l5rjnIZlvapp45lZIUwzNzpGssO9Ked1Qj7yLvNuxTvUOFHAXjevZNujSV71dLmw2FjrY2NtkLaKTj4VVtYug7irTQmQsDQ1UGvTXa8BDsM5Jozo6ZPJpjSy%2BNqbOXKm5ems1iGlNWufIQ214qBBStdc091Cr3Zrfx%2BDdLnatURcM5G/V3DDV7E5btdntTlPCo2BaO1qGGv1cF29kh8q/Ji7Dfi6ckvidS9GmGmXt45f0qTUyhn7Lmes%2B2hmh7WabPc82Hmy56PtUfTvv54wQ3y2v0819bHmyKxOZQcbr5QXW2Q1CySq33bkm9qztHr1mCKd25HSZ9GmM4tTocVZ9X1r7GpyYHsZ195DBhS3MwNgUrLrEDa9wiA2w3DxDtKOdMm0xWqGeH3zA%2Bv7JZ1YgH4tweE/No3aNQERhMBNTMkHuRIexebSpXlW6FaG8r7X%2BgHFceG3z9z2b4LKeidp7C9LntfbgeX9bvnmlRf6dxpdzOiSLsL%2BnsXUJyMUy2yw5HlC3QKypTiCxx3WdEHGgIIH6y3zczIyM3PWqChzQxvQhThwYwRx6SR2xSYlfWW2QM0x1m23NnYjHn20A0O2WRO1AzbHO1FxQLux9kfF2W3n2QQ1DhOUsmex11oxwPaTwJ%2By%2By1n%2BxeUB2Nx41B1S1%2BQhyoxQxo2wI%2B2mwwnwLrGYyIPHFR01Guicy41WzkNxw7iv3NyAOJU1TfxRyDnf2iy4TWgU09052zR9zqjUyNxMM0wsK8XbXv1sJwSiyjQ4W4WcJ2gry5zOmEIxycznx8JSzLgj0rS8w%2BQQI0Rcnj0SJVWv1oVvxsPDUixpxixL3SjsQSw52sxqmr1MiyQLGXybz2BbxKHXHb07y1x72hDwH73ln4WH1Hx6PH3zXbg43iM31kM00XxzkaNXwcDzgmJyP4V32SI%2BlmOP1PzrWaPPyWKv2T3yJE1J2lwMwLxCPlzk2ih/3AxcVYJ81QMlxXQBSMGZl8RZk3T0ilBaiqGKT3T0iQO4z0gAMOluGiGiAcBPlzHFE%2BEq3QAgiYAujQH/FFC%2BNTjXCwFoFpnVGilLA%2Bn1AAAkSAghRRHBVQvANANAAAxAaQzREpRFEmJM8KgdETgyITE2mHElyfEgQWE6k28Vwf8PAYAIIeUR0dcKkV0EUDvdAVwJYYaFyNYck6GQzfk%2BIIUyrUU8U4wfABQREwCVwSrD6BUjQZUAWdAeYMIegPYQpJQWIIESIO6GsPNNCL0M0yES0k0AAR31Lum2iZD5UM2ClYi7B7HSV7RYEBkKmPT0i3H1AABVKQTE0SSgYyQkOZok1wTRkB1wTw0wEAII4lyjxRGTmSFhKswg2jyUizXAmTrBSyyBIyQkqAO8WATExSqhjBJSMAZTUYUyLR0xyU2zXRtTdTAh9T0BezsxfFKoYkklUxyVrTMBbTIhSAGy1gEBqtDBCjPT9TezggWz9ttzy9XijEmJv5v59CjMaDrZJ4jsVlTswNiYATU4VjYM9lWN7DXD0DI5LoSAoTZo9hYTKhdxIJHIYIESBA6S8ya9aA8FmAbA7RPkMAShjAWi2TsS7p9QvAGheSvQSD30wyQ8%2BgyBqxlReytwxAdwMQGBUAgRnxyjk4sZyUxBYKhUEKkzaAULMYyK3iEUZlpx/1aCbZ6DgNVlwYWCgwiLcdXzuD3zDk0sQsEioznydUSjQ5NR4ysYCTN4oMSSyTKTyLaBKLqLaLbEGK9gsKfBvQcYxQk4SAsyczk58ygRCzJIayWT5Ryy1K9gCTMxa8myOjFS%2BijTlQLpoxNRpzxRZysInSNB1zTJ5RfSzgyS9zx5DzHFgj%2BDMDXs8oIEUKnytwaxtotxJBOERtWx/VFLFVJK25ZdC9osvKNLvLCSdLSTySKSDKjKaK6LUwzKLLKJrKMy7Lsz3S8zRRnLUwqy3LSyPL1wvKfLYT2iWyjS%2BzBJgr0qRFK8PCiEDdXR1NiNozxdLCb9DiPRwsTiHDQji8bFyj4ty9PzNqYjLl6TZ9FilLw9VEo83rfMsidilLcj9jvFU9TrjizFaqzix0yjJ17F1r55f8IMqrASRsPzbgOBZhaBOBopeBPAOAtBSBUBOAPJsQFglhaoeBSACBNBUbZhKRqQegIBZgMQ2UQrtoNgvBlQ1guAGFJANhopoppB0aOBJAsbKa8bOBeAfgNBybKbZg4BYAkAG5mgFwSByBKAqhgAFBlBDASghAMlaLOAyakRog6BvdNaghaAdauqRbDa6AehkB1bmBvgMk%2B5rb6BiBghMYxbSAXbIhOILa9acbeAFbkBbhiB1bPag6Kh4xPb%2BBBARAxB2ApAZBBBFAVB1AA7SBdAih8QTAzB9A8AwgfhIBZhUBogygfgOBeBq99IsBC76bSBiB%2BSbA2BYzUAXBa7ZgibFgE6Rh4xTbtbdbsaya9JMBlgyagRAQgluBUb9AMbhb078aOBsBVBFaTQ9hVBtp/QABaf0FaYAXkW1PSfkjEUcCARwLcXAQgEgUm6YXgCmgO6YamqkHLSgBmkASQc6Lgf0DQNYZUbaZUSQaKHeyQf0GewWue3Ghe8WkASWu%2BrQB%2B0BtYcByuz22Bqm0gO0R6RIN%2BoAA%3D).

## Rationale

Traditionally, C container libraries require users to define types for every container/content type combination and to specify the container type and (often) the content types (whether by casting, type-specific function names, or some other mechanism) at every API call. This causes verbosity and syntax noise.

In contrast, **CC** requires no boilerplate type definitions and provides an API agnostic to container and content types. The result is simpler, more readable code. The following table compares **CC** usage to other container library patterns.

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
Template-instantiation pattern:

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
Typed-pointer/hidden-metadata pattern:

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
<code>void</code>-pointers pattern:

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

## Simple Examples

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

A `map` is an unordered associative container mapping elements to keys, implemented as a hybrid open-addressing, chained hash table that is also available as a [standalone library](https://github.com/JacksonAllan/Verstable).

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

A `set` is an unordered associative container for elements without a separate key, implemented as a hybrid open-addressing, chained hash table also available as a [standalone library](https://github.com/JacksonAllan/Verstable).

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

### String

An `str` is a dynamic, null-terminated array representing a sequence of characters.

```c
#include <stdio.h>
#include "cc.h"

int main( void )
{
  str( char ) our_str;
  init( &our_str );

  // Appending formatted data.
  const char model[] = "Hornet CB900F";
  const char manufacturer[] = "Honda";
  unsigned int year_introduced = 2002;
  unsigned int year_discontinued = 2007;
  double horsepower = 103.0;
  double torque = 84.9;
  if(
    !push_fmt(
      &our_str, "The ", model, " is a motorcycle that was manufactured by ", manufacturer,
      " from ", year_introduced, " to ", year_discontinued, ".\nIt makes ", horsepower,
      "hp and ", torque, "Nm of torque.\n"
    )
  )
  {
    // Out of memory, so abort.
    cleanup( &our_str );
    return 1;
  }

  // Inserting formatted data at an index.
  const char alternative_model_name[] = "919";
  if( !insert_fmt( &our_str, 17, ", also known as the ", alternative_model_name, "," ) )
  {
    // Out of memory, so abort.
    cleanup( &our_str );
    return 1;
  }

  printf( first( &our_str ) );
  // Printed:
  //   The Hornet CB900F, also known as the 919, is a motorcycle that was manufactured by
  //   Honda from 2002 to 2007.
  //   It makes 103.00hp and 84.90Nm of torque.

  // Erasing elements.
  erase_n( &our_str, 108, 41 );

  printf( first( &our_str ) );
  // Printed:
  //   The Hornet CB900F, also known as the 919, is a motorcycle that was manufactured by
  //   Honda from 2002 to 2007.

  // Iteration #1.
  for_each( &our_str, el )
    printf( "%c", *el );
  // Printed: Same as above.

  // Iteration #2.
  for( char *el = first( &our_str ); el != end( &our_str ); el = next( &our_str, el ) )
    printf( "%c", *el );
  // Printed: Same as above.

  cleanup( &our_str );
}

```

## Advanced Examples

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

**CC** supports per-type destructors with the signature `void ( type val )`. A destructor is automatically called whenever an element or key of the associated type is removed from a container (except a **CC** string). Typically, a destructor frees the dynamic memory owned by the element or key.

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
    // This requires cleaning up our_vec, as well as our_list, because it was not inserted and
    // the list therefore did not take ownership of it.
    cleanup( &our_vec );
    cleanup( &our_list );
    return 1;
  }
  
  cleanup( &our_list ); // our_vec is in our_list, so it is cleaned-up automatically.
}

```

### Custom hash and comparison functions

**CC** includes default hash and comparison functions for fundamental integer types, null-terminated C strings (`char *` and `const char *`), and **CC** strings. Hence, these types can be used as map and ordered map keys, and set and ordered set elements, straight away.

To use other types or overwrite the default functions for the aforementioned types, define custom hash and/or comparison functions with the signatures `size_t ( type val )` and `int ( type val_1, type val_2 )`, respectively.

Maps and sets require both a hash and a comparison function, whereas ordered maps and ordered sets require only a comparison function.

```c
#include "cc.h"

typedef struct
{
  unsigned int id;
} our_type;

// First #define CC_CMPR and CC_HASH as comparison and hash functions.
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

### String interoperability

**CC** strings are designed for easy interoperability with other **CC** containers.

To this end, **CC** defines default hash, comparison, and memory-freeing destructor functions for all **CC** string types.

Additionally, when **CC** strings are used as the key and/or element type of another container, many API macros that operate on the container may alternatively take, as their key and/or element argument, a regular C string of the corresponding character type. In this case, **CC** automatically converts the C string into a **CC** string. This functionality is called "heterogeneous" insertion and lookup. The API macros that support heterogeneous insertion are `push`, `insert`, and `get_or_insert`, while those that support heterogeneous lookup are `get` and `erase`. In the lookup case, **CC** performs no memory allocations.

The following example demonstrates how **CC** strings can be used with a map:

```c
#include <stdio.h>
#include "cc.h"

// No need to define a hash, comparison, or destructor function for CC strings here as these
// functions are defined by default.

int main( void )
{
  map( str( char ), str( char ) ) our_map;
  init( &our_map );

  // Regular insertion of CC strings.
  str( char ) our_str_key;
  str( char ) our_str_el;
  init( &our_str_key );
  init( &our_str_el );
  if(
    !push_fmt( &our_str_key, "France" ) ||
    !push_fmt( &our_str_el, "Paris" ) ||
    !insert( &our_map, our_str_key, our_str_el )
  )
  {
    // Out of memory, so abort.
    // This requires cleaning up the strings, too, since they were not inserted and the map therefore
    // did not take ownership of them.
    cleanup( &our_str_key );
    cleanup( &our_str_el );
    cleanup( &our_map );
    return 1;
  }

  // Heterogeneous insertion of C strings.
  // CC automatically creates CC strings (and cleans them up if the operation fails).
  if( !insert( &our_map, "Japan", "Tokyo" ) )
  {
    // Out of memory, so abort.
    cleanup( &our_map );
    return 1;
  }

  // Regular lookup using a CC string.
  str( char ) our_str_lookup_key;
  init( &our_str_lookup_key );
  if( !push_fmt( &our_str_lookup_key, "Japan" ) )
  {
    // Out of memory, so abort.
    cleanup( &our_str_lookup_key );
    cleanup( &our_map );
    return 1;
  }
  str( char ) *el = get( &our_map, our_str_lookup_key );
  cleanup( &our_str_lookup_key );
  printf( first( el ) );
  // Printed: Tokyo

  // Heterogeneous lookup using a C string.
  // This requires no dynamic memory allocations.
  el = get( &our_map, "France" );
  printf( first( el ) );
  // Printed: Paris

  cleanup( &our_map );
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

### What compiler warning options does it support?

When used correctly, **CC** should not generate any compiler warnings under the following settings:

<table>
<tr>
<th>Compiler</th>
<th>Warning options</th>
</tr>
<tr>
<td>
GCC
</td>
<td>

`-Wall` `-Wpedantic` `-Wextra` `-Wconversion`
</td>
</tr>
<tr>
<td>
Clang
</td>
<td>

`-Wall` `-Wpedantic` `-Wextra` `-Wconversion`
</td>
</tr>
<tr>
<td>
MSVC
</td>
<td>

`/W3`
</td>
</tr>
</table>

### What's next?

For a discussion of **CC**'s future direction, see [here](TODO: add link).

## Support and Contribute

There are several ways that you can support **CC**:

* Star this repository ⭐.
* [Report](https://github.com/JacksonAllan/CC/issues) any issues or bugs you find when using the library.
* [Sponsor or donate](https://github.com/sponsors/JacksonAllan) to the library's continued development 🩷.