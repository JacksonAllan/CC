#ifndef HASH /*------------------------------------------- Primary header --------------------------------------------*/

#ifndef HASH_H
#define HASH_H

#include <stddef.h>
#include <stdint.h>

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
#define hash( val ) _Generic( (val),   \
  /* Extendibility slots: */           \
  HASH_SLOTS                           \
  /* Built-ins: */                     \
  default: _Generic( (val),            \
    char:               hash_signed,   \
    unsigned char:      hash_unsigned, \
    signed char:        hash_signed,   \
    unsigned short:     hash_unsigned, \
    short:              hash_signed,   \
    unsigned int:       hash_unsigned, \
    int:                hash_signed,   \
    unsigned long:      hash_unsigned, \
    long:               hash_signed,   \
    unsigned long long: hash_unsigned, \
    long long:          hash_signed,   \
    char *:             hash_string,   \
    default:            NULL           \
  )                                    \
)( val )                               \

// Built-in functions.

// All integer types.
// We can reuse the largest type's hash function for all smaller types.
// However, explicit casting of signed types silences warnings in the case of -Wconversion.
static inline size_t hash_signed( long long val )            { return (unsigned long long)val * 2654435761ull; }
static inline size_t hash_unsigned( unsigned long long val ) { return val * 2654435761ull; }

// NULL-terminated strings: FNV-1a.
// Implementation depends on the architecture: 32 or 64 bit?

#if SIZE_MAX == 0xFFFFFFFF // 32-bit size_t.

static inline size_t hash_string( char *val )
{
    size_t hash = 0x01000193;
    while( *val )
      hash = ( (unsigned char)*val++ ^ hash ) * 0x811c9dc5;

    return hash;
}

#elif SIZE_MAX == 0xFFFFFFFFFFFFFFFF // 64-bit size_t.

static inline size_t hash_string( char *val )
{
    size_t hash = 0xcbf29ce484222325;
    while( *val )
      hash = ( (unsigned char)*val++ ^ hash ) * 0x100000001b3;

    return hash;
}

#else // Strange size_t.

static inline size_t hash_string( char *val )
{
    size_t hash = 0;
    while( *val )
        hash = hash * 131 + (unsigned char)*val++;

    return hash;
}

#endif

// Used in below section.
#define ARG_1_( _1, _2 ) _1
#define ARG_1( ... )     ARG_1_( __VA_ARGS__ )
#define ARG_2_( _1, _2 ) _2
#define ARG_2( ... )     ARG_2_( __VA_ARGS__ )

#endif

#else /*------------------------------------------ Adding new hash function ------------------------------------------*/

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

#undef HASH

#endif
