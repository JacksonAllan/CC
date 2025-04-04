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
--><a href="#API-Reference"><picture><source media="(prefers-color-scheme: dark)" srcset="images/api_reference_dark.svg"><img src="images/api_reference_light.svg" alt="API Reference"/></picture></a><!--
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

It requires C23, or C11 and compiler support for `typeof`, or C++11.

It is distributed under the MIT license.

Try it online [here](https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1AB9U8lJL6yAngGVG6AMKpaAVxYM9DgDJ4GmADl3ACNMYhAAJgB2AGZSAAdUBUJbBmc3Dz1E5JsBX38gllDw6LjLTGtUoQImYgJ0908uC0wrXIZq2oJ8wJCwyNiLGrqGzOaFYe6/XqL%2B0oBKC1RXYmR2DgBSCJi/ZDcsAGoNmMcJ/FQAOgRj7A2NAEEtnYY910PjxwQCAniFEAB6f7EJgAdwuwEICFcwVcSlWskYBAuaBY/wAUkxkABrBQCe60AwMf6ORxAzBUBT/BCYJjoSlYABu/2QyCuNzujwefgIBxYTD8EAODNQeHQB3mHI2UQAQhyDgd4sRuVRBVsIgA1CpEcJHCIRcXHWUPeUMiqC7nig7LYjGU3IQ1yg5%2BQiqiIANmttoqBpiRs593lgIO93Q%2BCMB1amDYggUByIEYY6Aujv4xHNgidRxiABEDhpDZmPgcuPnfbrZRFpXgDcb5U6VQcwGB4rCEK6PSsveZM/MawG61K/XW60GAPKuHmoKi8qMkACepAOuIOTGCJCRjuHexpDFc8XbnrtPqHw4OxEwBBWDGLDtr8ql2cltaDAEkGHCbOHDBH6NGeUx/2vPwsFUZNa1TdMeWrY5c1LaVCxOYs4PLLYqz7Yc8AbJs/A/A9OztRdqwAKgOCJCMtCU7yOGVNxHf4DnHSdpzYFh50XZdV3XMD%2By3ehDD3PCbSPCVfVo%2BVz0vYhry4W8eOox8HkdIMACULyVTAGT8YAV0TCNgWScNIz/BRuPlCCnQzaCczzAsrMcJc8AAL0wQSu2PdC60wwUiOAC9XIIntdQAVgOGIs0fayNA84cwiYJR/IqciRJPeVWiUMSUMrPBZKU%2BiXwIWL2l1GIuFMg5U2MGlkDbXUOyExKf2ihUlUEBs1S2IKxTVRciNaY9coOZQWoK9AQGLcb9X1SQDmmkKQqiA4FoADgOJan37V8CuBIqngiMrzItXraHC8q8GICYEuQdy%2BqbGCE3QS7ruOu7/FUAhLsXPre0ouTFWVV0Ig6rq9R6r7ZMDeihu5TBRoOIRWEwFdY0401uMdbd%2BP3WrD29ZL1vlP7WoBjYgscBhfAmMa1X62taDwC6LJ5XtPTpiZwYsl1sc7VmmZy596JDMNtKMxFY3jBx9pISCENg2ys3sksC1Q1Dqx%2BjCsObVtXJ5pKmsHDKxwnK1mNnYgFyXVAVzXOoyt4ncBK5m0eZpuTxIvK8b1EqiH3xg5X3fMJP2078RYzUJU0RwxUAIaliB/KNEUltNGYOI6Tpoc73sd4xneSxrbushxtfp3myz6l7MDe4uJk%2B47vtorzGzAHDA%2Brgha9Buu9ZoqiIYYo2pxnVizfYy3OJtjKMd3LGtjqnOS5d08z3dqTPZS%2BTfaDbB9K0%2BPjKT6W07ujOGdnlmF7zm6wDuovs9zgs1c8hsj4iEKwpgu6osfmLnsL/SXLviXWui86xpUwBlcu1lXpZzPtzIBjU8aKX5gcfKhVUjFVKimEglVMQ1VgU7eBYMqKEwIG1PUQNdRkVTmDL2G1IbDRhmNLgE1SIzTYXNRanCVprSQXQlBW0ALoN2gfFOR9rInxge6c%2BEwnpNxvomNusiK5V0ATXBBTUSFkMBq/YGVC06IL4VDQQjC4YIyRlbVAqNfZTwdvg%2BeMiDGOk0cTUmDAACyTB4hUz1CAoM2YKgGHPCuXkniDggkhCnLEmA5zI10goBA6496izKnyLG3J2IJLqJaT0qT2bOkkXPVJLsBpvg/LvUOBATJYOThaKyssyx2SQkrSsKsmqN2wgHOorlUmEXIqhca9dvY91dn7eijFjZDzYhbCxE9e4HBsTPKRnYikGKXhJD2MlaHDh9rwvu284rlN/Mk6p0s6k2QafLJpFzlbWTCt/PScUAF2J6T2PmfDVIEHUppQyRyYwiNqSdZCjTFZlmVllDy%2BsqLxMSWI3MvkCk5M8Uldm8pG5EJGc4yhQMQAUO6k6TuICdmGIYbDLgkRSIgDCpIEAc0QBukWiAbhIAACcm88oCJ2tsZhEAKmxgELQOc8wk44Oqt0pFCDaKYvajoyhBLVmjMGiSsaYUQorX1PS1luyFWoO2kI7Y%2BoeW/MqTpMUUSYlCuqSKvBSybQvLNcA%2B5UryE6JxTKvFRF7XULruzIMRiRpjQiJSmaNKGUrTJfqIKdLVosrZfwtBAhioxBEdCrJsLTqZzFfEWRBdcy32eaEy%2Bv9czQMzQ6jRw0tHYtxSDVOZrjDmXzfEB1cqfX0OhrDeGbBzEo0wGjWsCzM3FNrE6wGrihAXm8fqeVSgs4WmZp2GdeSGCczsTOodfDSmB0OQnP5JyU5nKBZckF8EwVoXue05unSEULovLre5kKRmGyYpMke0zx4bjmQO7Oa75XDnWavTZ68iUDX2QZYWRqqngSlvuwFcsizHsLC025TVYrxW/be15tCBofK%2Bdu/ee6AWfzg4hBDp7VaOgfRhDMab4WuRncirZT9BToqXiO6teiaEniJX3P1JjmFTQOPNVasadWCITU8TBUGbRVWtXPejEriEVuJm6mt%2BjW2KvbQGthC1mGqpExyvVMQ9oEeo5A3MEi6MXmzdfQuCj0Ol3gmZg4Jb7NlsdUprFKmOPesY76pVpiu1xQsVYrVX7V1WcccOjz7VXGjmIFgc8YpUmTt8fRfxexaiR2vCQBLMMQlZvCTHSJ0TYlihTTyXlZVUCpMghkxJ86bTVc8UuldNrjBNazZFjdV68PHKk6c2D1ySPITI20jWLcunZw670xD8FmGDLkpR08T6JksSmRxa2H6Rlhbax1kBf6V7SRRRvLVW8d4/J3ZU/5llBvwWBYem5uY7m0VQ083bLzyNYeQThvAGleu7v6zBojQ2FYjZaeCx%2BS35Tla9SdWjU2Pv7frMx71cy2MuvYzNtTjHuMKt46S8lgaqUhsjfSqIjLo2av9H3UTnKSoHENZdvlDABUWqk1a1y02FO/Wi86zqsrYfyr85p1hKqDj0pWlTkpBnxP6oZ7yk1BwzUKDZ/2CqMnOd2uiQ6yVvPtGdVdfz91nrscnmF8Ygnk0g3UtpWTinS0Y2nfZfG68Twk17ph2mizCOC0FivvIh6PvOt%2B6Lc5yu17GsvK%2BuW/6nmDeY9rdE%2Bt0G7Fc6%2Bi23zbaLdjU7ZHZGa4QvU%2B1TL7Lpo45MCV9Es8hhfKLmYGwAV5ViDVdYRAXYbhkiml7PGBaPLVCvE75gVXZkpa0U90573qeXlTp6sCIwmBKq6WPmdU%2B73xULV/fnGzuY5%2B%2BUX%2BgY7TmXNT/FSxmPRM4%2BjQTx6pPDa19NoQRns3Wf/UUuJ7b6xfFp6a9919/sbHYt4swg8sZ0Ut5Ukg/IU4Gt2tF1GN8lOcf03kacesLt8NAdCNIpiNQdmkKwz0G5xsr0ECMNVZu514%2B5xlB41tX0NsuJJ4v9bFdtEDGMDtJIjtmCTsi8zsDlUC%2Bs1doMMD6k7sj0HskMnsUN/4iD25MM/RsM1JftvlwMmdrsoJbsEJsDQVwc8DawodREnN4dU95NPt140VUcMU9dr9OMKMogFJiURd%2BN2EuF9MXcMFhUNcpt5Mz8CZzCvMn8Bp8cmFWFZpHCnc41dVZcjNlDYdl8M13CIsQ85FbNA8DC4iy5Q9j9GCMNo93NY9pUjdVNLDkF/CAs89gte0nCwjS8wgdIkkMxd9MA68EZG8qBm8WBW929YQ8Au84xLZe9K4B9Oih8D4G5TNQ9J8MipCZ9U46j9904V8I8YCMMN9D9joc1q95999liTp0i54ICpCz9siL9cjdFfDCj/MBMgpP97ZFkdimCZCoscjyEx1PktIqZXFqZp1PlBRqpahskF1PkWt5iJg44us%2B57h4h4gHBd5Uw%2BRvg8t0AAImAyo0B3weRvi45WIsBaASZZQgpcw7o1QAAJEgfwHkRwaUZlDQDQAAMXakY2RJkTRJCV3CoExEkjCGxJJjxOskJIEHhNpJPFcHfDwGAH8DFAtDnBpBtG5Gb3QFcDWC6msgiEpMBkY0FOSBFLy3FMlOMHwAUGRM/FcDyzuiVI0CiHZnQGWGCHoAOEySUESBBCqLuhLCTTgkdAtOhGtO1AAEdDSTolpJALhWU4CVRaImwWx4l60WB3oMpV1PlFw1QAAVakAXDE1oeMnxemYJVibUZAOcbcOMBAACMJILPkZk1klYPLYIOcFM/iFk6wCssgGMnxZolvPFCU2oYwaUjAOUmGdM/UeMNs7U3U/UvwQ09APs5MVxfKEJKJWMPFW0zAe0sIUgJsiIBALNQwY47o4gH0%2BoyhAIVoweb0w0ycsmdqKiR%2BSHYZYcFbSg02c2Gg2ZOSHbOTT5JHf9NgrjawkTFA7SaEgCEaA4eEmoFcQCCyECJEgQBkws8vWgLaZgGwU0YwVM2gYwevcBIKHErkvEvUZlLgVlchYMwUDpD8SMwEuM4sKIPsxcMQZcLEBgVAEEa8ILGORGPFMQeCwRJClCtChGai94iFa8uifuZ9Kgh8seTbW2eZeg64z0IE98w7NeKwmwpxDzMY184E1LDTbPAaeUJMxGIkqSKzMkik6kmi2gOihipi8xVig4PC5lQiZGXkaOEgXM/MmOIskEEs2s8sxLA4Ks3Sg4IkxMCvFs1ok0/sy2E0qIMqIMeUacvkWcpCF0jQdcxXf0wMjQA8iZY8sokI0Df7K7R0V7NCujCiksJaRcSQebJA5qWPdSuSt8%2BuTPbS/1QK/SoK4k4y8kykqk8yyyxi5isWZM%2ByxyrMly1YPMz0wsnkLy2MUs1wOstksUAK5BeUYK%2BEpvFvCK7o0iSkmK8osTV3LlVw3BMqsgbnYcdHIKe0fInzZ/VqkxXPbtAvPKzg53Co4qYzQHRkr3OY866zAPAGwtLY8Pc6tzXXB4/XW67zLSoo56oLHtPtfsF8xq4EnKb8h4DgRYWgTgIKXgTwDgLQUgVATgeyXEFYNYDBXgAgTQbGxYakWkfoCARYLEGlGKpaGIZlKICILgV%2BSQGIIKIKaQXGjgSQAmumkmzgXgP4DQUgWmom7G0gOAWAJASuCoCcEgcgSgWoYABQZQQwVoIQBJJizgHgUgFEeIOgI6g2/wWgY2wayWy2ugfoZAPW5gX4BJduZ2%2BgYgAIBGaWi26rK232xiB202xW0gdW5Ae4YgPWwO6O6oMMQO/gQQEQMQdgKQGQQQRQFQdQSO3QZoQkEwMwfQPAYIP4SARYVAeIdoP4DgXgSxMIJULASulm0gYgQUmwNgBM1AFwNuxYCm1YTOoYMMW2o2k2wm82z5TAdYc2kEYELxbgJW0W/G0gQm4m0mjgbAVQDW7UA4VQJaN0AAWjdGmmABZAZ0%2BUFKxF7AgEcEXFwEIBIAwXmBprpvmAZppAS0oFZpAEkD2i4DdA0GiCWiiEkCCjPskDdH0E4HFvXslq3plpADloVq0E/tgY4AiAlsjqQflo/sWDL2SDsEkCAA%3D%3D).

## Rationale

Traditionally, C container libraries require users to define types for every container/content type combination and to specify the container type and (often) the content types (whether by casting, type-specific function names, or some other mechanism) at every API call. This causes verbosity and syntax noise.

In contrast, **CC** requires no boilerplate type definitions and provides an type-safe API agnostic to container and content types. The result is simpler, more readable code. The following table compares **CC** usage to other container library patterns.

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

**CC** supports per-type destructors with the signature `void ( type val )`. A destructor is automatically called whenever an element (or key) of the associated type is removed from a container (except a **CC** string). Typically, a destructor frees the dynamic memory owned by the key or element.

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
    cleanup( &our_vec );
    cleanup( &our_list );
    return 1;
  }
  
  cleanup( &our_list ); // our_vec is cleaned-up automatically.
}

```

### Custom hash and comparison functions

**CC** includes default hash and comparison functions for fundamental integer types, null-terminated C strings (`char *` and `const char *`), and **CC** strings. Hence, these types can be used as map and ordered map keys, and set and ordered set elements, straight away.

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

### String interoperability

**CC** strings are designed for easy interoperability with other **CC** containers.

To this end, **CC** defines default hash, comparison, and memory-freeing destructor functions for all **CC** string types.

Additionally, when **CC** strings are used as the key and/or element type of another container, many API macros that operate on the container may alternatively take, as their key and/or element argument, a regular C string of the corresponding character type. In this case, **CC** automatically handles the conversion of the C string into a **CC** string. This functionality is called "heterogeneous" insertion and look-up. The API macros that support heterogeneous insertion are `push`, `insert`, and `get_or_insert`, while those that support heterogeneous look-up are `get` and `erase`.

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
    // This requires cleaning up the keys, too, since they were not inserted and the map therefore
    // did not take ownership of them.
    cleanup( &our_str_key );
    cleanup( &our_str_el );
    cleanup( &our_map );
    return 1;
  }

  // Heterogeneous insertion of C strings.
  // CC automatically handles the creation of the strings (and their cleanup, if the operation
  // fails).
  if( !insert( &our_map, "Japan", "Tokyo" ) )
  {
    // Out of memory, so abort.
    cleanup( &our_map );
    return 1;
  }

  // Regular look-up using a CC string.
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

  // Heterogeneous look-up using a C string.
  // Unlike regular look-up, heterogeneous look-up requires no dynamic memory allocation.
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

CC should not generate any compiler warnings under the following settings:

<table>
<tr>
<th>Compiler</th>
<th>Warnings flags</th>
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

TODO: Create and link to an issue discussing CC's future direction.

## Support and Contribute

There are several ways that you can support **CC**:

* Star this repository ⭐.
* [Report](https://github.com/JacksonAllan/CC/issues) any issues or bugs you find when using the library.
* [Sponsor or donate](https://github.com/sponsors/JacksonAllan) to the library's continued development.