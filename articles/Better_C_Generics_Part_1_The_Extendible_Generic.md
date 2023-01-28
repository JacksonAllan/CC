# Better C Generics Part 1: The Extendible `_Generic`

**Jackson Allan** · 28 January 2023

> This article is part of a series describing the novel techniques that power the generic container library [Convenient Containers](https://github.com/JacksonAllan/CC) (**CC**).
> **CC** distinguishes itself by eliminating some of the inconveniences traditionally burdening container libraries in C.
> Specifically, it does not require the user to define container types, and it provides a generic API that is agnostic to both container type and content type yet also type-safe.
> In other words, **CC** containers should be almost as simple to use as containers in higher-level languages.

In this article, we will explore extendable `_Generic` macros.
**CC** uses such macros to deduce comparison, hash, and destructor functions based on a container's element or key type at the site of every API call.
These macros alleviate the need for containers to carry around function pointers at runtime and the subsequent [performance penalty](https://www.geeksforgeeks.org/c-qsort-vs-c-sort).
They also spare library users from having to specify these functions every time they declare containers.

## C11's `_Generic`

C11 [introduced](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1548.pdf#page=96) the `_Generic` keyword:

```c
#define print( val ) _Generic( /* Controlling expression: */ (val),   \
  /* Association list: */                                             \
  int:    print_int,                                                  \
  double: print_double,                                               \
  char *: print_string                                                \
)( val ) /* We may end the macro by calling the selected function. */ \

void print_int( int val )       { printf( "%d", val ); }
void print_double( double val ) { printf( "%f", val ); }
void print_string( char *val )  { printf( "%s", val ); }
```

The type of the (unevaluated) *controlling expression* determines which expression from the *association list* is selected and included at compile time.
In most cases, `_Generic` lives inside a macro and acts as a primitive means of function overloading.

However, several aspects of `_Generic` severely limit its usefulness.
In particular, all compatible types must be hard-coded by the programmer writing it.
Hence, a library developer cannot create a `_Generic` macro into which the library's users can easily plug their own types and functions (or so it seems).
This problem has [caused](https://stackoverflow.com/questions/9734982/c11-generic-adding-types) [frustration](https://stackoverflow.com/questions/32923945/extending-a-macro-in-c) for programmers seeking more flexibility.

## The Naive Extendible `_Generic` Macro

In fact, we can circumvent the aforementioned limitation by providing empty “slots” in our `_Generic` macro to be filled by user-supplied types and functions.
To do so, we must use preprocessor magic to include or omit another macro inside the `_Generic` macro based on whether it is defined.
Here is one possible naive implementation of an extendible `_Generic` macro that hashes a variable:

```c
/*------------------------------------------------------ hash.h ------------------------------------------------------*/

#ifndef HASH_H
#define HASH_H

#include <stddef.h>

// IF_DEF_INCL_SLOT( macro ) includes a _Generic slot only if the specified macro is defined (as type, function_name).
// It takes advantage of the fact that if the macro is defined, then the expanded text will contain a comma.
#define COMMA                     ,
#define ARG_1( _1, ... )          _1
#define ARG_2( _1, _2, ... )      _2
#define ARG_3( _1, _2, _3, ... )  _3
#define IF_DEF_INCL_SLOT( macro ) ARG_3( macro, ARG_1( macro, ): ARG_2( macro, ) COMMA, , )

// Generic hash macro.
#define hash( val ) _Generic( (val), \
  /* Extendibility slots: */         \
  IF_DEF_INCL_SLOT( HASH_1 )         \
  IF_DEF_INCL_SLOT( HASH_2 )         \
  IF_DEF_INCL_SLOT( HASH_3 )         \
  IF_DEF_INCL_SLOT( HASH_4 )         \
  IF_DEF_INCL_SLOT( HASH_5 )         \
  IF_DEF_INCL_SLOT( HASH_6 )         \
  IF_DEF_INCL_SLOT( HASH_7 )         \
  IF_DEF_INCL_SLOT( HASH_8 )         \
  /* Built-ins: */                   \
  short: hash_short,                 \
  int:   hash_int                    \
)( val )                             \

// Built-in functions.
static inline size_t hash_short( short val ) { return val * 2654435761ull; }
static inline size_t hash_int( int val )     { return val * 2654435761ull; }

#endif
```

Now the library user can add functions for new or unsupported types as follows:

```c
/*------------------------------------------------------ main.c ------------------------------------------------------*/

#include <stdio.h>
#include "hash.h"

// Add some user-supplied hash type and function pairs into empty slots.

size_t hash_long( long val ){ return val * 2654435761ull; }
#define HASH_1 long, hash_long

size_t hash_long_long( long long val ){ return val * 2654435761ull; }
#define HASH_2 long long, hash_long_long

// Test.
int main( void )
{
  printf( "%zu %zu %zu %zu\n", hash( (short)1 ), hash( (int)2 ), hash( (long)3 ), hash( (long long)4 ) );
}
```

## The Extendible `_Generic` Macro with Automated Slots

The above naive solution requires library users to manually manage numbered slots.
Hence, it is cumbersome and unscalable.
Fortunately, we can use the preprocessor to automate the process and completely hide the slotted interface from users.
To do so, we need an auxiliary header to transform a type and function pair supplied by the user from macro text into regular, persistent code that fills the first available slot.

```c
/*------------------------------------------------------ hash.h ------------------------------------------------------*/

#ifndef HASH_H
#define HASH_H

#include <stddef.h>

// IF_DEF( macro )( code ) includes the bracketed code only if the specified macro is defined (as empty).
#define COMMA()               ,
#define ARG_2_( _1, _2, ... ) _2
#define ARG_2( ... )          ARG_2_( __VA_ARGS__ )
#define INCL( ... )           __VA_ARGS__
#define OMIT( ... )
#define IF_DEF( macro )       ARG_2( COMMA macro () INCL, OMIT, )

// Generic hash macro.
#define hash( val ) _Generic( (val),        \
  /* Extendibility slots: */                \
  IF_DEF( HASH_1 )( hash_1_ty: hash_1_fn, ) \
  IF_DEF( HASH_2 )( hash_2_ty: hash_2_fn, ) \
  IF_DEF( HASH_3 )( hash_3_ty: hash_3_fn, ) \
  IF_DEF( HASH_4 )( hash_4_ty: hash_4_fn, ) \
  IF_DEF( HASH_5 )( hash_5_ty: hash_5_fn, ) \
  IF_DEF( HASH_6 )( hash_6_ty: hash_6_fn, ) \
  IF_DEF( HASH_7 )( hash_7_ty: hash_7_fn, ) \
  IF_DEF( HASH_8 )( hash_8_ty: hash_8_fn, ) \
  /* Built-ins: */                          \
  short: hash_short,                        \
  int:   hash_int                           \
)( val )                                    \

// Built-in functions.
static inline size_t hash_short( short val ) { return val * 2654435761ull; }
static inline size_t hash_int( int val )     { return val * 2654435761ull; }

// DEF_HASH_N_TY_FN( n ) defines a new hash type and function pair.
// This macro is used in add_hash.h.
// Its purpose is to convert the text the user supplied via the HASH macro (defined as type, function_name) into
// persistent, non-macro code.
// n denotes the slot that the pair will fill.

#define ARG_1_( _1, ... ) _1
#define ARG_1( ... )      ARG_1_( __VA_ARGS__ )

#define DEF_HASH_N_TY_FN( n )                           \
typedef ARG_1( HASH ) hash_##n##_ty;                    \
static inline size_t hash_##n##_fn( hash_##n##_ty val ) \
{                                                       \
  return ARG_2( HASH, )( val );                         \
}                                                       \

#endif
```

```c
/*---------------------------------------------------- add_hash.h ----------------------------------------------------*/

// Check for correct usage.
#ifndef HASH
#error  Define HASH as type, function_name before including add_hash.h.
#endif

// Insert hash type and function pair into the first available slot.
#if     !defined( HASH_1 )
#define HASH_1             // Activate slot 1.
DEF_HASH_N_TY_FN( 1 )      // Define a type and function for slot 1.
#elif   !defined( HASH_2 )
#define HASH_2
DEF_HASH_N_TY_FN( 2 )
#elif   !defined( HASH_3 )
#define HASH_3
DEF_HASH_N_TY_FN( 3 )
#elif   !defined( HASH_4 )
#define HASH_4
DEF_HASH_N_TY_FN( 4 )
#elif   !defined( HASH_5 )
#define HASH_5
DEF_HASH_N_TY_FN( 5 )
#elif   !defined( HASH_6 )
#define HASH_6
DEF_HASH_N_TY_FN( 6 )
#elif   !defined( HASH_7 )
#define HASH_7
DEF_HASH_N_TY_FN( 7 )
#elif   !defined( HASH_8 )
#define HASH_8
DEF_HASH_N_TY_FN( 8 )
#else
#error Sorry, too many hash functions!
#endif

// Undef HASH so that the user doesn't have to.
#undef HASH
```

Now our users can add hash functions without concerning themselves with slot numbers and collisions:

```c
/*------------------------------------------------------ main.c ------------------------------------------------------*/

#include <stdio.h>
#include "hash.h"

// Add some user-supplied hash type and function pairs (now there is no need to specify slots).

size_t hash_long( long val ){ return val * 2654435761ull; }
#define  HASH long, hash_long
#include "add_hash.h"

size_t hash_long_long( long long val ){ return val * 2654435761ull; }
#define   HASH long long, hash_long_long
#include "add_hash.h"

// Test.
int main( void )
{
  printf( "%zu %zu %zu %zu\n", hash( (short)1 ), hash( (int)2 ), hash( (long)3 ), hash( (long long)4 ) );
}
```

## The Counter-Based Extendible `_Generic` Macro

While the previous solution hides the slotted interface from the library user, it still requires the library programmer to include as many slots as users could conceivably need in a giant macro that the processor must parse upon every invocation.
A superior, albeit more complex, approach is to use more preprocessor magic to _generate_ the necessary slots (up to some inordinately high limit) instead of _omitting_ the empty ones.[<sup>[1]</sup>](#footnote_1)
To do this, we can create an octal counter inspired by the [Boost preprocessor counter](https://www.boost.org/doc/libs/1_66_0/boost/preprocessor/slot/detail/counter.hpp).
Here is an implementation that uses a three-digit counter to support up to 511 type and function pairs:

```c
/*------------------------------------------------------ hash.h ------------------------------------------------------*/

#ifndef HASH_H
#define HASH_H

#include <stddef.h>

// Octal counter that supports up to 511 hash type and function pairs.
#define HASH_COUNT_D1 0 // Digit 1, i.e. least significant digit.
#define HASH_COUNT_D2 0
#define HASH_COUNT_D3 0

// Standard concatenation macros.
#define CAT_2_( a, b )       a##b
#define CAT_2( a, b )        CAT_2_( a, b )
#define CAT_3_( a, b, c )    a##b##c
#define CAT_3( a, b, c )     CAT_3_( a, b, c )
#define CAT_4_( a, b, c, d ) a##b##c##d
#define CAT_4( a, b, c, d )  CAT_4_( a, b, c, d )

// Provides the current value of the counter as a three-digit octal number preceded by 0.
#define HASH_COUNT CAT_4( 0, HASH_COUNT_D3, HASH_COUNT_D2, HASH_COUNT_D1 )

// HASH_SLOTS generates _Generic slots from 0 to HASH_COUNT - 1 (inclusive) via macro pseudo-recursion.
// In the case of multiple extendible-_Generic macros, this code could be adjusted to support multiple counters (see CC
// library source code).

#define HASH_SLOT( n ) CAT_3( hash_, n, _ty ): CAT_3( hash_, n, _fn ),
#define R1_0( d3, d2 )
#define R1_1( d3, d2 ) HASH_SLOT( CAT_4( 0, d3, d2, 0 ) )
#define R1_2( d3, d2 ) HASH_SLOT( CAT_4( 0, d3, d2, 1 ) ) R1_1( d3, d2 )
#define R1_3( d3, d2 ) HASH_SLOT( CAT_4( 0, d3, d2, 2 ) ) R1_2( d3, d2 )
#define R1_4( d3, d2 ) HASH_SLOT( CAT_4( 0, d3, d2, 3 ) ) R1_3( d3, d2 )
#define R1_5( d3, d2 ) HASH_SLOT( CAT_4( 0, d3, d2, 4 ) ) R1_4( d3, d2 )
#define R1_6( d3, d2 ) HASH_SLOT( CAT_4( 0, d3, d2, 5 ) ) R1_5( d3, d2 )
#define R1_7( d3, d2 ) HASH_SLOT( CAT_4( 0, d3, d2, 6 ) ) R1_6( d3, d2 )
#define R1_8( d3, d2 ) HASH_SLOT( CAT_4( 0, d3, d2, 7 ) ) R1_7( d3, d2 )
#define R2_0( d3 )
#define R2_1( d3 )     R1_8( d3, 0 )
#define R2_2( d3 )     R1_8( d3, 1 ) R2_1( d3 )
#define R2_3( d3 )     R1_8( d3, 2 ) R2_2( d3 )
#define R2_4( d3 )     R1_8( d3, 3 ) R2_3( d3 )
#define R2_5( d3 )     R1_8( d3, 4 ) R2_4( d3 )
#define R2_6( d3 )     R1_8( d3, 5 ) R2_5( d3 )
#define R2_7( d3 )     R1_8( d3, 6 ) R2_6( d3 )
#define R2_8( d3 )     R1_8( d3, 7 ) R2_7( d3 )
#define R3_0
#define R3_1 R2_8( 0 )
#define R3_2 R2_8( 1 ) R3_1
#define R3_3 R2_8( 2 ) R3_2
#define R3_4 R2_8( 3 ) R3_3
#define R3_5 R2_8( 4 ) R3_4
#define R3_6 R2_8( 5 ) R3_5
#define R3_7 R2_8( 6 ) R3_6

#define HASH_SLOTS                                          \
CAT_2( R1_, HASH_COUNT_D1 )( HASH_COUNT_D3, HASH_COUNT_D2 ) \
CAT_2( R2_, HASH_COUNT_D2 )( HASH_COUNT_D3 )                \
CAT_2( R3_, HASH_COUNT_D3 )                                 \

// Generic hash macro.
#define hash( val ) _Generic( (val), \
  /* Extendibility slots: */         \
  HASH_SLOTS                         \
  /* Built-ins: */                   \
  short: hash_short,                 \
  int:   hash_int                    \
)( val )                             \

// Built-in functions.
static inline size_t hash_short( short val ) { return val * 2654435761ull; }
static inline size_t hash_int( int val )     { return val * 2654435761ull; }

// Used in add_hash.h.
#define ARG_1_( _1, _2 ) _1
#define ARG_1( ... )     ARG_1_( __VA_ARGS__ )
#define ARG_2_( _1, _2 ) _2
#define ARG_2( ... )     ARG_2_( __VA_ARGS__ )

#endif
```

```c
/*---------------------------------------------------- add_hash.h ----------------------------------------------------*/

// Check for correct usage.
#ifndef HASH
#error  Define HASH as type, function_name before including add_hash.h.
#endif

// Convert the user-supplied HASH macro (defined as type, function_name ) into hash_XXXX_ty and hash_XXXX_fn, where XXXX
// is the current value of the HASH_COUNT counter.

typedef ARG_1( HASH ) CAT_3( hash_, HASH_COUNT, _ty ); // typeof( ARG_1( HASH ) ) would allow exotic types such as array
                                                       // and function pointers.
                                                       // typeof will become standard in C23.

static inline size_t CAT_3( hash_, HASH_COUNT, _fn )( CAT_3( hash_, HASH_COUNT, _ty ) val )
{
  return ARG_2( HASH )( val );
}

// Increment the counter.
#if     HASH_COUNT_D1 == 0
#undef  HASH_COUNT_D1
#define HASH_COUNT_D1 1
#elif   HASH_COUNT_D1 == 1
#undef  HASH_COUNT_D1
#define HASH_COUNT_D1 2
#elif   HASH_COUNT_D1 == 2
#undef  HASH_COUNT_D1
#define HASH_COUNT_D1 3
#elif   HASH_COUNT_D1 == 3
#undef  HASH_COUNT_D1
#define HASH_COUNT_D1 4
#elif   HASH_COUNT_D1 == 4
#undef  HASH_COUNT_D1
#define HASH_COUNT_D1 5
#elif   HASH_COUNT_D1 == 5
#undef  HASH_COUNT_D1
#define HASH_COUNT_D1 6
#elif   HASH_COUNT_D1 == 6
#undef  HASH_COUNT_D1
#define HASH_COUNT_D1 7
#elif   HASH_COUNT_D1 == 7
#undef  HASH_COUNT_D1
#define HASH_COUNT_D1 0
#if     HASH_COUNT_D2 == 0
#undef  HASH_COUNT_D2
#define HASH_COUNT_D2 1
#elif   HASH_COUNT_D2 == 1
#undef  HASH_COUNT_D2
#define HASH_COUNT_D2 2
#elif   HASH_COUNT_D2 == 2
#undef  HASH_COUNT_D2
#define HASH_COUNT_D2 3
#elif   HASH_COUNT_D2 == 3
#undef  HASH_COUNT_D2
#define HASH_COUNT_D2 4
#elif   HASH_COUNT_D2 == 4
#undef  HASH_COUNT_D2
#define HASH_COUNT_D2 5
#elif   HASH_COUNT_D2 == 5
#undef  HASH_COUNT_D2
#define HASH_COUNT_D2 6
#elif   HASH_COUNT_D2 == 6
#undef  HASH_COUNT_D2
#define HASH_COUNT_D2 7
#elif   HASH_COUNT_D2 == 7
#undef  HASH_COUNT_D2
#define HASH_COUNT_D2 0
#if     HASH_COUNT_D3 == 0
#undef  HASH_COUNT_D3
#define HASH_COUNT_D3 1
#elif   HASH_COUNT_D3 == 1
#undef  HASH_COUNT_D3
#define HASH_COUNT_D3 2
#elif   HASH_COUNT_D3 == 2
#undef  HASH_COUNT_D3
#define HASH_COUNT_D3 3
#elif   HASH_COUNT_D3 == 3
#undef  HASH_COUNT_D3
#define HASH_COUNT_D3 4
#elif   HASH_COUNT_D3 == 4
#undef  HASH_COUNT_D3
#define HASH_COUNT_D3 5
#elif   HASH_COUNT_D3 == 5
#undef  HASH_COUNT_D3
#define HASH_COUNT_D3 6
#elif   HASH_COUNT_D3 == 6
#undef  HASH_COUNT_D3
#define HASH_COUNT_D3 7
#elif   HASH_COUNT_D3 == 7
#error  Are you mad? 512 hash functions are too many!
#endif
#endif
#endif

// Undef HASH so that the user doesn't have to.
#undef HASH
```

## Overwritable Defaults

`_Generic` disallows multiple expressions associated with the same type.
Hence, the above solutions prevent the library user from supplying custom functions for types with support built into the library.
This causes inconvenience (for example, users of our hash macro may prefer a faster or more uniformly distributed hash function for a particular type based on the patterns in their data).
To allow the built-in functions to be overwritten, we can place them inside a second `_Generic` expression nested in the `default` path of the primary one.
Thus, the built-in function for a given type will not be selected if a user-supplied function for that type short-circuits the macro first.

```c
#define hash( val ) _Generic( (val), \
  /* Extendibility slots: */         \
  HASH_SLOTS                         \
  /* Built-ins: */                   \
  default: _Generic( (val),          \
    short:   hash_short,             \
    int:     hash_int,               \
    default: NULL                    \
  )                                  \
)( val )                             \
```

## Eliminating the Auxiliary Header

Single-file libraries are [popular](https://github.com/nothings/single_file_libs) because they are very easy to use.
If our library is suitably small and we wish to follow the single-file paradigm, we can eliminate the need for a separate `add_XXXX.h` by incorporating it into the primary header and selecting between the primary code and macro-extending code depending on whether the user has defined the extension macro:

```c
/*------------------------------------------------------ hash.h ------------------------------------------------------*/

#ifndef HASH // The user intends to include the primary header.
#ifndef HASH_H
#define HASH_H

// Primary header code...

#endif
#else // The user intends to add a new type and function pair.

// Marco-extending and counter-incrementing code...

#undef HASH
#endif
```

Now the user need only re-include the same header when supplying a new type and function pair:

```c
#include "hash.h" // Include once at the top of the file.

size_t hash_long( long val ){ return val * 2654435761ull; }
#define  HASH long, hash_long
#include "hash.h" // Include again to add a new type and function pair.
```

## Conclusion

Thus, with a little work and creativity, we can overcome a key limitation of `_Generic` and turn it into a powerful tool for generic programming.
Moreover, the technique described above is not limited to `_Generic`—it could easily be applied to other library macros that could benefit from user extendibility.
The final code implementing all aspects of this article can be found [here](Better_C_Generics_Part_1_The_Extendible_Generic_code) and tested [here](https://godbolt.org/#z:OYLghAFBqd5QCxAFwE4FN0BoCWIIDGAtgIYDW6AgqsAM4gDkAtACIDCAspQNICiA%2BgCEAqgEkAMi34AVAJoAFXgFIAzCxboARgFdgDLAQD2RAA44ANulTiSAO2DaSwdKIAmIfp4IHttZMYB5bWQTYIAxC3RbEiJ0FHQ/LAAzSPpYdFccf1Q3EABGZMjcgpTLaNiQdAAPGJNLADpvUAZ9HFpRWwJzbVcMxgAGVtoOEhxbAGVDbVQCOIZB3FoAkyiB/XM7YFyWgEosCAyswxz3ACZCy1zz0qiYuIQSWgR6hCxmoY6unr75oZGxybTWaMPJDZarX5YDb2bYMHZ7NqccjoeSoQwAK3QBGQIP06PyWFs6AA7hFLu4ACx7cyMACs%2BlsaywhkY0gw6AA1EpTipQVgcb94VgEOgSL1UPg9mR8gA2eoUgCcAA5lQB2GUqJUqFQU04y1VQxgU/REfL9fqEpkslpYegWgWDPZwWBIIy9XiZbK4fA3cpxaq1BreJKGWzIcYEEiWfIU5Kh5DCWgoqpM/EFaGgTz8bxJyzYnCh/BRVxsQzdIiMhVYYviMboABy2iImis%2BVpWBMhloWQLtlL5crHa7PdDtaJjebrby7dzWOQvfGyBIqGQ/abg9n%2BdDi%2BXyDHDabLYl09tS5Xa4rICrfl3%2B4nR7be1oUxmc25KjGX16XJUbD8mUMF5VGUfpKHfT9um/blTgeJ4gNOU4lFApDKAAelQjkAgANysYlUCyTk7A5ao2nnewOVghAOSSbROnnUN6hQ7sAC90H4ZAOQIXx/CIfhKP4cxQ2AAShIgDlBPIiTgA5LCow5HYuVVQQOQwZBplsDkIFo7tgCJVxxKEgz7B2WTzA5AAqDleVUZSlFVFgUPfXoUiJDkOQACUocZ3KM6SpKwTjuOMPjHgQET7HC4BHJ5CDvi5BDKPgxDkNA9COUoVx9JICjQuo2itw0kNUA5bLfCsJhnLrfTkAATxWRjQNqlZnI5PxUG0bFFOU7ScF0jJfIGnBXBsxSWECvxgtqmymJwVj2Jyp5syC3jarEriJpWmqZLkhS7OU1T1O28x6iGiyrLyEa7Ic5CeUq1yPK8nz1p49iaoC/jnsmmroo/TpIM5aDEoQaDHNSjDpASZAGsoMYONIMYxKwwxTp2RylJQtyTHwsMkggDG3PixDTlpZjtHikmye5CnydJmntCUWk2FsaCsHxtzKLEiAnmOZAdjyeTWdAgmFoQTnYZ2U4BbZkXOaknYVCloWCY5zSpN8nYKUVyhhZVwhlte1GlI5WlRvk/HUZUQQ0euyg4UNBh6UtSEWQDUwg35NYnVdQx3U9Y5vQgX07hAIG4zDCMoziPJYxDMNE2TVMQHODMPC8W10DzejGQOWwSzLdcCRrOt71bdtO27LOL0ZApy5HWw70PUv08zhcz1XfPLwKTcs53FcG8nCUZzbquCRvPvi8bwenxfYEGHApJc/QJIHu8jlUPMphN637ed93vf973jlURwUhUC2kUxSsDkD5v2%2B7838zUNBsCYoXlrPO8/h3J%2Bu7OQ/9yv7P3An9OKqg/zIEykvICKgQIv1%2Bl%2BAGv5/yw2gbAlCaUAjYjkkYWiyAr7IAeBxWg2gTCdhXLQDkJCOT%2BGNnkfmlFqF1UIrnPKdFewchMKMVAtBoZOSXnWFeAC2ABGEPWaQ/AWD836GvDCLBepZA5AUDkOB6joHqOJUUfhWq9VsDgFIkYwwckyMALIvDbr8Ncv/fgwjRHiJYJLfoP8LF/0etYkRYiJEK0cSlNCGEdy52XPpIwnQSB4OiFnDkpACBoh4U4lynI2CUHEacfgYkSABU0PJYWBMSDvnfJoOJAjEnJLSRkrJ2S3LFP4Ck0pHJMmoxuioX%2BHIqkqFSSVDJAUCDlLcrknk%2BS8k8gIIU1yrTamaC6T0ypST%2BBtPGZMhpcDmlVIpO09JdSukBX0gpPpKgBn9KGYMppIyEkzIpPMziWzykrLWZ0y5RizY%2BLSqiQwWEhoJGoSKQKqAMCGNMtoTkhhl4EM5DgsMV9HglU%2BeyCq8iOKGCwWZWwk8OEYFmL0fSmgtr9DMU05xgi3G2JaWcsSFoCU2I8SwFQAUrEUrseccl7i7H80WegjCVjxjiACNIcYHJnBElQKEj5/AADiUQrA4G6bQQSyAKFJDREQDk0iaG0qZdfRRmlYrdhwgpN52Uolog4UmHohgmBoumN2BibKOQdE%2BaCx4gLl5EG0OYecdROTVDCZkTQlgmCivFfhbpBquwBQIW0TiPtQVTHMJiwirh0TcX6jQ4hpCeaRJdW6ywEbcFWAoVzTALS2DWvMDgTQgqz6tRnlG3oOxeGNOaRyrl0gxIaQUmMkW/AAq2ACq9eSIBiXiJUGJfiXae0Ly1nw%2BJHIABKeR%2BD9DEq4alRjJasvMVO2d/A8iLuXa4VdBLOXcrEis0lWzd0MukQpNdeKN1ztODurZ%2B7G1HoHfwc5Sqz2PoCiyrJm7t1GPPY8pZ%2BLN1DoA4%2BrJz7m2vvfWSpdX6OT7oUpu%2B94GV1AcnQIzd774PoYUlB49JKP1ob3QFBWV6Z1zrA7hvdGH11YbnbSB9eGD1NsI%2BI2Dn6V0BU1hR7DzHaPXuaZumUAmn2uMPdBk9xGaMMpNnxxjYm6M3oY/wVUSn8MSbYzB09JGGUyiychudom9PKeE3OpUGnWMvuk3B89AVVSGco2pjTJyZ0pIXQBszIGUn/qXVM5zlm0OXrc9OlJqH/MKWFpuoLuGf1ha3Tu7zG6UnUfI9kmLzH90JYi%2BR0LKScPpeixZ5j6WEtpeS1hlJTGvNRYJpltDvH3NvqS0Jnz/ATORYyyVtD8nms1ci/llztXuv8Fi8ugzyGUmdby/W9r42AsNdw45qbw3BtzY3W07xwHNtbua0FkLG2sNtMlgloL8W2kXSO65adbSFZnbEtlk7oW2mawe1ZX9d2Xv8BNu9prt233fYM%2B9vrAPaTfcc%2B9ybM62kyiAfRyxWnuW8oqajtH6OMdckZihKpqHN00tcXSiRLKxKqtsZ4gnn8if2KyQzItoFcdiQS5ToRTKJGrtJ4TtnVKAuY7pzjmZeO2ks8JZSormOJdo/508jCYqBWSpFpEkg0TAJuZVqZLJ/r5cEE5qZPYBNpfazXpZXgVQvWlosFkLa0rDCyv7Y/Pn2OlZQZR5LiXhu3Lrw5IIbQFhkBMDGPQCyGE3cVI90YpeJAM39q1xKnXmk9cBUd/To3bkCAPAlKH/iOk9JJ6xyn4WPU%2BpBIz/27J/Ei%2B5/z9LHP/V0/LjLxU7POiMh5/D25Sv/VuYrkb%2BzUK/BO%2BuACu31qCAea94x834vbenep8obYWv%2BlYYT774tQfw/Z/C2X6Hjti%2BZ8F4JoP3yK%2BK8L5b0P6vSs3JSRX%2BjqfVe3Ij6P2rG/Hb1%2BX7ny/oSt/lb973wbzfAmevYqcyH/cvP/NAMYYAffaWZyKPV1MAgmesYQcQcQZPc2HfTAgAlPHYRGHaLAyXQ3a1H3P3APQqfKLOWJGXdKcwMyWGdAZwYqJqBIaGNKAAdXtQ0gwDKjtXEmXGcC0WYLAAYAoQYRojYVDGomOBKloNalIFoPwSYSoN8Q8kMGJHQBwlQACmqDqElQUUjD8CgI5CBW0WL0YRWAoW7DKFmAoWJGXF0XsAoTGF4IMMdWvjYOCU0MtVsGhhvHnG6TGBLVchYjYg4nvwyDEi/3Ig1zq3Rz2hUnQDUlQA0i0jPzMKiOABMjkksj1FpApApBUFpHVDyBdXMEunsiYiXH8OUVsCCM5BCPmlP0XzEmf0MjVhiK6gSKSI0g1xyJlDyIKKKJlBKNoPKJtmtWQNQKYDwVQCIDGCFX0jaigKDzCHrAADUmA8gSBWCMJRA3Z0BYgwxQl2FegVhc4KFJCQUSoZgEACJsRpg4grJJZpCZRNZNAshVAwh4cPxl5xhRAAAtAQLgAADR/AcjUCVSqDCGhJhOhJkSeKYHeKIVmlCLrUoD8IV0CIEQaLCIgOxmADWgzwshiLRitiv20TmlxKeDBMhP6DyHNDpIVD2UtmlmJFuMsDEnMhJPJNXyolUDGk5iP2AJ2C5KjG5CtlOFslpF4EVwUksn6CqCVDoQIAVFcAIHBxZJ8WFgOmSJFmmmQgqJ8XfAzj0Q5D%2BMBP4BBLBP5MhNhLtPtLCHhNeMRIURxLRIxICNqOxJRMaLxKgMJOXGJJ2lJJrx9KpL5IhIVIIE0CSFOAVFmApCVF1AQhUGJn1LnzZMiE5O5Ln15JpMFLSL0k4gzxFNMnFPFKxxlIYTlMhPpIZIZLyE0GZLJLAnJJ1I0konTKum%2BIziTHhMXEFXsHqLDPdKqMxK9OCLDN30gPsADJAJzL2lDMpMVxtMcU1IzPZPQGzODJ5N/2pJtIYUsjyF5HimUlSMX2LOXFLLFMlPLPXOlnbL1PvMNNbLgWLD0WfjSnjiXw0hbEEmJFajnF7FxWaUoGnRFS3XaS3R7X3S3TczAogv/XqGQoCwQsgrEk8DWMoH4AQvGE8Eq1cjQpqQ5GgpItguSh2wESIrEmQvUViPSnAuqSgv4CwpwvArwv4Aw0aXfKSB7PMD7PXnviEuEvSkyiMKJAArEIoPYWEtktvkfk/IwlLFsE0I4iuLKlQCYBTV0P6n/iVxV00l/iygoWYICnEIKn4D9CyVhkMA7WBPsuBN7TsH0n4gcscoXgCjZKsE5DcutXDSuK4h%2BSiA4n%2BTcKuLJzEWzXBVQDROYJajQv/T0rbRmTAxHUZVsR7VqjNktnhOYKBTEgSs51Xgo2JGjSyloLUOIiqFtwV2YMsI6iokhWXEFW%2Bl3IIPaoxzSmctYQKg4WRmiuUI6qGolzSjyuXmJAsDMhbCMFiFaiXACVQB/JaR5FHOOM9LqIpNCNfVSv7xFzpTHVbXY1mWHV2vSrEUyq2l1R3LAnRiVkfOosEXkjwLMgthbO7OoM%2BAwEOLUq%2BTBRmNxVNOFgiuZWtMjJ%2BlohajOuZTc2BuJ0UR%2BhNOXjclhskVBrGiuzgQhqXmRq53J0kRhtxspX5gouNJLSRqhrhv5JtJJp5CxqRpRoxsw0Ryp2535j2W4rJoJgZrRqsnBsXnpsJuhuuxcRZrxv5gpARs5pxtFqJp5olsaTpultZzFoJplpBvBw5sBoptRqpohI1sxv5qVtFyFsouZuVtlrh01vJu5t1rGktoNshoZtVvNpBtVElq1ptrUBtLdoVsNu1sZoRxFpdrhu23nmyRRslltqVT5sdsFvZ2duNvZ3hqtq5rjppyjoDpUEVu1pppUzNsTpp1zsRtTrVqTqjtzuzojoTup0lnZrfKlpzp5rrvfErrTtzobTbo5Hlvro9s7qju7pbr9qruFsbv1tJt7tLvTq9r1pjuxpzuru50lntvHutr7unrttnoFsnvbvxQjo5B9p7tXu3p5oPsHtju3oXrxocR%2Bi1qNupwVijtDtpqHrTubsDu1oVkzuLrvu5wfvXuTodrnpRrfrzqDoLoViLobuAZ5orpfsnpAY7vgd5pTp/rxr/vBLGhAdbvgcvrFy7vdqPuDp537s3tQbF1wbsQVjHp5G/o/p5uoazrgaIYQd3tfo5GXpoagbYajo4cYfPuYYoc8X3oIZLuYZPoRp%2BWkPSgwA5BqimCV2GhUEdNpDyElikokIX2uM5H8FstIFsBqjADAARtzg/O4pMd4rMcyAsdfLPuxv/h7PMZQjthpAdgZCtBDlCheA9kFD2AvnFElCwGlGGPqBVGjlpE1CVGGNpCZPtmNCdgWGtH0DtG8cdH2BgEQEqCqCxGCH9jwAgH4NoHkDsAznGDH2JEZAYD5BmrMA2CziKaJHMFKbUIqYWGqciAlAIGAFUfODacsFQCCHDDKZaf0GqCxGoDoEYBGayYIEXEyAqf0FjnDEjGjGjjDgTCTHkBTEhDTChE2FTmzChFLTSAgD2EMBMEoMmdtCrVxFPDmfqZKaGZubQEwAJDwhIBMFxGpDpDcedkYFN2yeyA5EVJlCYFeL5QIG6VUfqElggDYACjZALXfDyB2H0AdDhF8dFH8ZOcCaTjyHqBlH6AVFpFpCTNVGJf6FeJlFiZ%2BYScueSbRaFBcdOBpf0ESZSfRawC8N7BAApCAA).
Future articles in this series will explore other ways that we can harness `_Generic` and overcome its limitations, as well as how we can manipulate C's type system to associate multiple types with one pointer at compile time.

<a name="footnote_1"></a><sup>1. Credit for the preprocessor counter idea is [owed](https://www.reddit.com/r/Cprog/comments/xqg5s4/comment/iqd1lz6/?utm_source=reddit&utm_medium=web2x&context=3) to Reddit user Jinren.</sup>

## [Discussion](https://github.com/JacksonAllan/CC/discussions/1)
