/*

Convenient Containers v1.4.3 - tests/unit_tests.c

This file tests CC containers.
It aims to cover the full API and to check corner cases, particularly transitions between placeholder containers and
non-placeholder containers.

License (MIT):

  Copyright (c) 2022-2025 Jackson L. Allan

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
  documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
  persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
  Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
  WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
  OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#define TEST_VEC
#define TEST_LIST
#define TEST_MAP
#define TEST_SET
#define TEST_OMAP
#define TEST_OSET
#define TEST_STR

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../cc.h"

// Assert macro that is not disabled by NDEBUG.
#define ALWAYS_ASSERT( xp )                                                                               \
( (xp) ? (void)0 : ( fprintf( stderr, "Assertion failed at line %d: %s\n", __LINE__, #xp ), exit( 0 ) ) ) \

// Disable this macro to turn off failing realloc.
// Doing so can help detect redundant but undesirable compiler warnings about the results of API macros going unused.
#define SIMULATE_ALLOC_FAILURES

// If realloc can fail, then we need a macro to repeat each call until it succeeds.
#ifdef SIMULATE_ALLOC_FAILURES
#define UNTIL_SUCCESS( xp ) while( !(xp) )
#else
#define UNTIL_SUCCESS( xp ) xp
#endif

// Custom realloc and free functions that track the number of outstanding allocations.
// If SIMULATE_ALLOC_FAILURES is defined above, the realloc will also sporadically fail.

size_t simulated_alloc_failures = 0;
size_t oustanding_allocs = 0;

static void *unreliable_tracking_realloc( void *ptr, size_t size )
{
#ifdef SIMULATE_ALLOC_FAILURES
  if( rand() % 5 == 0 )
  {
    ++simulated_alloc_failures;
    return NULL;
  }
#endif

  void *new_ptr = realloc( ptr, size );
  ALWAYS_ASSERT( new_ptr );

  if( !ptr )
    ++oustanding_allocs;

  return new_ptr;
}

static void tracking_free( void *ptr )
{
  if( ptr )
    --oustanding_allocs;

  free( ptr );
}

// Activate custom realloc and free functions.
#define CC_REALLOC unreliable_tracking_realloc
#define CC_FREE tracking_free

// Define a custom type that will be used to check that destructors are always called where necessary.

bool dtor_called[ 100 ];
static void check_dtors_arr( void )
{
  for( size_t i = 0; i < sizeof( dtor_called ) / sizeof( *dtor_called ); ++i )
  {
    ALWAYS_ASSERT( dtor_called[ i ] );
    dtor_called[ i ] = false;
  }
}

typedef struct { int val; } custom_ty;
#define CC_DTOR custom_ty, { dtor_called[ val.val ] = true; }
#define CC_CMPR custom_ty, { return val_1.val < val_2.val ? -1 : val_1.val > val_2.val; }
#define CC_HASH custom_ty, { return (size_t)( (unsigned long long)val.val * 2654435761ull ); }
#define CC_LOAD custom_ty, 0.7
#include "../cc.h"

// Vector tests.
#ifdef TEST_VEC

#define VEC_CHECK                                                              \
ALWAYS_ASSERT( size( &our_vec ) == sizeof( expected ) / sizeof( *expected ) ); \
for( size_t i = 0; i < size( &our_vec ); ++i )                                 \
  ALWAYS_ASSERT( *get( &our_vec, i ) == expected[ i ] )                        \

static void test_vec_reserve( void )
{
  vec( int ) our_vec;
  init( &our_vec );

  int expected [ 60 ] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59
  };

  // Reserve zero with placeholder.
  UNTIL_SUCCESS( reserve( &our_vec, 0 ) );
  ALWAYS_ASSERT( (void *)our_vec == (void *)&cc_vec_placeholder );

  // Reserve up from placeholder.
  UNTIL_SUCCESS( reserve( &our_vec, 30 ) );
  ALWAYS_ASSERT( cap( &our_vec ) >= 30 );

  // Reserve same capacity.
  size_t cap = cap( &our_vec );
  UNTIL_SUCCESS( reserve( &our_vec, 30 ) );
  ALWAYS_ASSERT( cap( &our_vec ) == cap );

  // Reserve up from non-placeholder.
  UNTIL_SUCCESS( reserve( &our_vec, 60 ) );
  ALWAYS_ASSERT( cap( &our_vec ) >= 60 );

  // Reserve lower capacity.
  cap = cap( &our_vec );
  UNTIL_SUCCESS( reserve( &our_vec, 30 ) );
  ALWAYS_ASSERT( cap( &our_vec ) == cap );

  // Test validity through use.
  UNTIL_SUCCESS( push_n( &our_vec, expected, 60 ) );

  VEC_CHECK;

  cleanup( &our_vec );
}

static void test_vec_resize( void )
{
  vec( int ) our_vec;
  init( &our_vec );

  int expected [ 30 ] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29
  };

  // Resize placeholder vec with zero.
  UNTIL_SUCCESS( resize( &our_vec, 0 ) );
  ALWAYS_ASSERT( cap( &our_vec ) == 0 );
  ALWAYS_ASSERT( size( &our_vec ) == 0 );

  // Resize up from placeholder.
  UNTIL_SUCCESS( resize( &our_vec, 30 ) );
  ALWAYS_ASSERT( cap( &our_vec ) >= 30 );
  ALWAYS_ASSERT( size( &our_vec ) == 30 );

  // Resize up from non-placeholder.
  UNTIL_SUCCESS( resize( &our_vec, 60 ) );
  ALWAYS_ASSERT( cap( &our_vec ) >= 60 );
  ALWAYS_ASSERT( size( &our_vec ) == 60 );

  // Resize down.
  UNTIL_SUCCESS( resize( &our_vec, 30 ) );
  ALWAYS_ASSERT( cap( &our_vec ) >= 60 );
  ALWAYS_ASSERT( size( &our_vec ) == 30 );

  // Test validity through use.
  for( size_t i = 0; i < 30; ++i )
    *get( &our_vec, i ) = (int)i;

  VEC_CHECK;

  cleanup( &our_vec );
}

static void test_vec_shrink( void )
{
  vec( int ) our_vec;
  init( &our_vec );

  int expected [ 30 ] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29
  };

  // Test placeholder.
  UNTIL_SUCCESS( shrink( &our_vec ) );
  ALWAYS_ASSERT( size( &our_vec ) == 0 );
  ALWAYS_ASSERT( cap( &our_vec ) == 0 );

  // Test restoration of placeholder.
  UNTIL_SUCCESS( reserve( &our_vec, 30 ) );
  UNTIL_SUCCESS( shrink( &our_vec ) );
  ALWAYS_ASSERT( size( &our_vec ) == 0 );
  ALWAYS_ASSERT( cap( &our_vec ) == 0 );
  ALWAYS_ASSERT( (void *)our_vec == (void *)&cc_vec_placeholder );

  // Test shrink same size.
  UNTIL_SUCCESS( resize( &our_vec, 30 ) );
  ALWAYS_ASSERT( size( &our_vec ) == 30 );
  ALWAYS_ASSERT( cap( &our_vec ) == 30 );
  vec( int ) same = our_vec;
  UNTIL_SUCCESS( shrink( &our_vec ) );
  ALWAYS_ASSERT( size( &our_vec ) == 30 );
  ALWAYS_ASSERT( cap( &our_vec ) == 30 );
  ALWAYS_ASSERT( our_vec == same );

  // Test shrink down.
  UNTIL_SUCCESS( reserve( &our_vec, 60 ) );
  ALWAYS_ASSERT( size( &our_vec ) == 30 );
  ALWAYS_ASSERT( cap( &our_vec ) >= 30 );
  UNTIL_SUCCESS( shrink( &our_vec ) );
  ALWAYS_ASSERT( size( &our_vec ) == 30 );
  ALWAYS_ASSERT( cap( &our_vec ) == 30 );

  // Test validity through use.
  for( size_t i = 0; i < 30; ++i )
    *get( &our_vec, i ) = (int)i;

  VEC_CHECK;

  cleanup( &our_vec ); 
}

static void test_vec_insert( void )
{
  vec( int ) our_vec;
  init( &our_vec );

  int expected [ 90 ] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89
  };

  // End.
  for( int i = 0; i < 30; ++i )
  {
    int *el;
    UNTIL_SUCCESS( el = insert( &our_vec, size( &our_vec ), 60 + i ) );

    ALWAYS_ASSERT( *el == 60 + i );
  }

  // Beginning.
  for( int i = 0; i < 30; ++i )
  {
    int *el;
    UNTIL_SUCCESS( el = insert( &our_vec, 0, 29 - i ) );
    ALWAYS_ASSERT( *el == 29 - i );
  }

  // Middle.
  for( int i = 0; i < 30; ++i )
  {
    int *el;
    UNTIL_SUCCESS( el = insert( &our_vec, 30, 59 - i ) );
    ALWAYS_ASSERT( *el == 59 - i );
  }

  VEC_CHECK;

  cleanup( &our_vec );
}

static void test_vec_insert_n( void )
{
  vec( int ) our_vec;
  init( &our_vec );

  int expected [ 30 ] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29
  };

  ALWAYS_ASSERT( !insert_n( &our_vec, size( &our_vec ), NULL, 0 ) ); // Zero-size insertion.

  int *el;
  UNTIL_SUCCESS( el = insert_n( &our_vec, size( &our_vec ), expected + 20, 10 ) ); // End.
  ALWAYS_ASSERT( *el == 20 );
  UNTIL_SUCCESS( el = insert_n( &our_vec, 0, expected, 10 ) ); // Beginning.
  ALWAYS_ASSERT( *el == 0 );
  UNTIL_SUCCESS( el = insert_n( &our_vec, 10, expected + 10, 10 ) ); // Middle.
  ALWAYS_ASSERT( *el == 10 );

  VEC_CHECK;

  cleanup( &our_vec );
}

static void test_vec_push( void )
{
  vec( int ) our_vec;
  init( &our_vec );

  int expected [ 100 ] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
    90, 91, 92, 93, 94, 95, 96, 97, 98, 99
  };

  for( int i = 0; i < 100; ++i )
  {
    int *el;
    UNTIL_SUCCESS( el = push( &our_vec, i ) );
    ALWAYS_ASSERT( ( *el == i ) );
  }

  VEC_CHECK;

  cleanup( &our_vec );
}

static void test_vec_push_n( void )
{
  vec( int ) our_vec;
  init( &our_vec );

  int expected [ 100 ] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
    90, 91, 92, 93, 94, 95, 96, 97, 98, 99
  };

  ALWAYS_ASSERT( !push_n( &our_vec, NULL, 0 ) ); // Zero-size push.

  int *el;
  UNTIL_SUCCESS( el = push_n( &our_vec, expected, 25 ) );
  ALWAYS_ASSERT( *el == expected[ 0 ] );
  UNTIL_SUCCESS( el = push_n( &our_vec, expected + 25, 25 ) );
  ALWAYS_ASSERT( *el == expected[ 25 ] );
  UNTIL_SUCCESS( el = push_n( &our_vec, expected + 50, 25 ) );
  ALWAYS_ASSERT( *el == expected[ 50 ] );
  UNTIL_SUCCESS( el = push_n( &our_vec, expected + 75, 25 ) );
  ALWAYS_ASSERT( *el == expected[ 75 ] );

  VEC_CHECK;

  cleanup( &our_vec );
}

static void test_vec_erase( void )
{
  vec( int ) our_vec;
  init( &our_vec );

  int expected [ 50 ] = {
    1, 3, 5, 7, 9,
    11, 13, 15, 17, 19,
    21, 23, 25, 27, 29,
    31, 33, 35, 37, 39,
    41, 43, 45, 47, 49,
    51, 53, 55, 57, 59,
    61, 63, 65, 67, 69,
    71, 73, 75, 77, 79,
    81, 83, 85, 87, 89,
    91, 93, 95, 97, 99
  };

  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( push( &our_vec, i ) );

  bool erase = true;
  for( size_t i = 0; i < 50; )
  {
    if( erase )
    {
      int *el = erase( &our_vec, i );
      ALWAYS_ASSERT( ( *el = *get( &our_vec, i ) ) );
    }
    else
      ++i;

    erase = !erase;
  }

  VEC_CHECK;

  cleanup( &our_vec );
}

static void test_vec_erase_n( void )
{
  vec( int ) our_vec;
  init( &our_vec );

  int expected [ 50 ] = {
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    90, 91, 92, 93, 94, 95, 96, 97, 98, 99
  };

  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( push( &our_vec, i ) );

  bool erase = true;
  for( size_t i = 0; i < 50; )
  {
    if( erase )
    {
      int *el = erase_n( &our_vec, i, 10 );
      ALWAYS_ASSERT( ( *el = *get( &our_vec, i ) ) );
    }
    else
      i += 10;

    erase = !erase;
  }

  VEC_CHECK;

  cleanup( &our_vec ); 
}

static void test_vec_clear( void )
{
  vec( int ) our_vec;
  init( &our_vec );

  int expected [ 30 ] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29
  };

  // Empty.
  clear( &our_vec );
  ALWAYS_ASSERT( size( &our_vec ) == 0 );

  // Non-empty.
  UNTIL_SUCCESS( resize( &our_vec, 30 ) );
  ALWAYS_ASSERT( size( &our_vec ) == 30 );
  clear( &our_vec );
  ALWAYS_ASSERT( size( &our_vec ) == 0 );
  ALWAYS_ASSERT( cap( &our_vec ) >= 30 );

  // Test use.
  UNTIL_SUCCESS( push_n( &our_vec, expected, 30 ) );
  VEC_CHECK;

  cleanup( &our_vec );
}

static void test_vec_cleanup( void )
{
  vec( int ) our_vec;
  init( &our_vec );

  int expected [ 30 ] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29
  };

  // Empty.
  cleanup( &our_vec );
  ALWAYS_ASSERT( (void *)our_vec == (void *)&cc_vec_placeholder );

  // Non-empty.
  UNTIL_SUCCESS( resize( &our_vec, 30 ) );
  ALWAYS_ASSERT( size( &our_vec ) == 30 );
  cleanup( &our_vec );
  ALWAYS_ASSERT( size( &our_vec ) == 0 );
  ALWAYS_ASSERT( cap( &our_vec ) == 0 );
  ALWAYS_ASSERT( (void *)our_vec == (void *)&cc_vec_placeholder );

  // Test use.
  UNTIL_SUCCESS( push_n( &our_vec, expected, 30 ) );
  VEC_CHECK;

  cleanup( &our_vec );
}

static void test_vec_iteration( void )
{
  vec( int ) our_vec;
  init( &our_vec );

  int expected [ 30 ] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29
  };

  // Empty.

  size_t n_iterations = 0;
  for( int *i = first( &our_vec ); i != end( &our_vec ); i = next( &our_vec, i ) )
    ++n_iterations;

  for_each( &our_vec, i )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 0 );

  // Non-empty.

  UNTIL_SUCCESS( push_n( &our_vec, expected, 30 ) );
  for( int *i = first( &our_vec ); i != end( &our_vec ); i = next( &our_vec, i ) )
    ++n_iterations;

  for_each( &our_vec, i )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 60 );

  // Test last and first.
  ALWAYS_ASSERT( *first( &our_vec ) == 0 );
  ALWAYS_ASSERT( *last( &our_vec ) == 29 );

  VEC_CHECK;

  cleanup( &our_vec );
}

static void test_vec_init_clone( void )
{
  vec( int ) src_vec;
  init( &src_vec );

  int expected [ 30 ] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29
  };

  // Test init_clone placeholder.
  vec( int ) empty_vec;
  UNTIL_SUCCESS( init_clone( &empty_vec, &src_vec ) );
  ALWAYS_ASSERT( (void *)empty_vec == (void *)&cc_vec_placeholder );

  // Test init_clone non-placeholder.
  vec( int ) our_vec;
  UNTIL_SUCCESS( push_n( &src_vec, expected, 30 ) );
  UNTIL_SUCCESS( init_clone( &our_vec, &src_vec ) );
  VEC_CHECK;

  cleanup( &src_vec );
  cleanup( &empty_vec );
  cleanup( &our_vec );
}

static void test_vec_dtors( void )
{
  vec( custom_ty ) our_vec;
  init( &our_vec );

  // Test erase and clear.

  UNTIL_SUCCESS( resize( &our_vec, 100 ) );
  for( size_t i = 0; i < 100; ++i )
    get( &our_vec, i )->val = (int)i;

  bool erase = true;
  for( size_t i = 0; i < 50; )
  {
    if( erase )
      erase( &our_vec, i );
    else
      i += 1;

    erase = !erase;
  }

  clear( &our_vec );
  check_dtors_arr();

  // Test cleanup.
  UNTIL_SUCCESS( resize( &our_vec, 100 ) );
  for( size_t i = 0; i < 100; ++i )
    get( &our_vec, i )->val = (int)i;

  cleanup( &our_vec );
  check_dtors_arr();
}

#endif

// List tests.
#ifdef TEST_LIST

#define LIST_CHECK                                                              \
ALWAYS_ASSERT( size( &our_list ) == sizeof( expected ) / sizeof( *expected ) ); \
do                                                                              \
{                                                                               \
  int *arr = expected;                                                          \
  for_each( &our_list, i )                                                      \
  {                                                                             \
    ALWAYS_ASSERT( *i == *arr );                                                \
    ++arr;                                                                      \
  }                                                                             \
}while( false )                                                                 \

static void test_list_insert( void )
{
  list( int ) our_list;
  init( &our_list );

  int expected [ 90 ] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89
  };

  // End.
  for( int i = 0; i < 30; ++i )
  {
    int *el;
    UNTIL_SUCCESS( el = insert( &our_list, end( &our_list ), 60 + i ) );
    ALWAYS_ASSERT( *el == 60 + i );
  }

  // Beginning.
  for( int i = 0; i < 30; ++i )
  {
    int *el;
    UNTIL_SUCCESS( el = insert( &our_list, first( &our_list ), 29 - i ) );
    ALWAYS_ASSERT( *el == 29 - i );
  }

  // Middle.
  int *mid = first( &our_list );
  for( int i = 0; i < 30; ++i )
    mid = next( &our_list, mid );

  for( int i = 0; i < 30; ++i )
  {
    int *el;
    UNTIL_SUCCESS( el = insert( &our_list, mid, 30 + i ) );
    ALWAYS_ASSERT( *el == 30 + i );
  }

  LIST_CHECK;

  cleanup( &our_list );
}

static void test_list_push( void )
{
  list( int ) our_list;
  init( &our_list );

  int expected [ 100 ] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
    90, 91, 92, 93, 94, 95, 96, 97, 98, 99
  };

  for( int i = 0; i < 100; ++i )
  {
    int *el;
    UNTIL_SUCCESS( el = push( &our_list, i ) );
    ALWAYS_ASSERT( ( *el == i ) );
  }

  LIST_CHECK;

  cleanup( &our_list );
}

static void test_list_splice( void )
{
  list( int ) our_list;
  list( int ) src_list;
  init( &our_list );
  init( &src_list );

  // Splice from one list to another.

  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( push( &src_list, i ) );

  bool splice = true;
  for( int *i = first( &src_list ); i != last( &src_list ); )
  {
    int *next = next( &src_list, i );

    if( splice )
      UNTIL_SUCCESS( splice( &our_list, end( &our_list ), &src_list, i ) );

    splice = !splice;
    i = next;
  }

  ALWAYS_ASSERT( size( &our_list ) == 50 );
  for( int *i = first( &our_list ), j = 0; i != last( &our_list ); i = next( &our_list, i ), j += 2 )
    ALWAYS_ASSERT( *i == j );

  ALWAYS_ASSERT( size( &src_list ) == 50 );
  for( int *i = first( &src_list ), j = 1; i != last( &src_list ); i = next( &src_list, i ), j += 2 )
    ALWAYS_ASSERT( *i == j );

  // Splice within the same list.

  int *i = last( &our_list );
  while( i != r_end( &our_list ) )
  {
    int *prev = prev( &our_list, i );

    UNTIL_SUCCESS( splice( &our_list, end( &our_list ), &our_list, i ) );

    i = prev;
  }

  ALWAYS_ASSERT( size( &our_list ) == 50 );
  for( int *i = first( &our_list ), j = 98; i != last( &our_list ); i = next( &our_list, i ), j -= 2 )
    ALWAYS_ASSERT( *i == j );

  cleanup( &our_list );
  cleanup( &src_list );
}

static void test_list_erase( void )
{
  list( int ) our_list;
  init( &our_list );

  int expected [ 50 ] = {
    1, 3, 5, 7, 9,
    11, 13, 15, 17, 19,
    21, 23, 25, 27, 29,
    31, 33, 35, 37, 39,
    41, 43, 45, 47, 49,
    51, 53, 55, 57, 59,
    61, 63, 65, 67, 69,
    71, 73, 75, 77, 79,
    81, 83, 85, 87, 89,
    91, 93, 95, 97, 99
  };

  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( push( &our_list, i ) );

  bool erase = true;
  for( int *i = first( &our_list ); i != end( &our_list ); )
  {
    if( erase )
      i = erase( &our_list, i );
    else
      i = next( &our_list, i );

    erase = !erase;
  }

  LIST_CHECK;

  cleanup( &our_list );
}

static void test_list_clear( void )
{
  vec( int ) our_list;
  init( &our_list );

  int expected [ 30 ] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29
  };

  // Empty.
  clear( &our_list );
  ALWAYS_ASSERT( size( &our_list ) == 0 );

  // Non-empty.
  for( int i = 0; i < 30; ++i )
    UNTIL_SUCCESS( push( &our_list, i ) );
  ALWAYS_ASSERT( size( &our_list ) == 30 );

  clear( &our_list );
  ALWAYS_ASSERT( size( &our_list ) == 0 );

  // Test use.
  for( int i = 0; i < 30; ++i )
    UNTIL_SUCCESS( push( &our_list, i ) );
  LIST_CHECK;

  cleanup( &our_list );
}

static void test_list_cleanup( void )
{
  list( int ) our_list;
  init( &our_list );

  int expected [ 30 ] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29
  };

  // Empty.
  cleanup( &our_list );
  ALWAYS_ASSERT( (void *)our_list == (void *)&cc_list_placeholder );

  // Non-empty.
  for( int i = 0; i < 30; ++i )
    UNTIL_SUCCESS( push( &our_list, i ) );
  ALWAYS_ASSERT( size( &our_list ) == 30 );
  cleanup( &our_list );
  ALWAYS_ASSERT( size( &our_list ) == 0 );
  ALWAYS_ASSERT( (void *)our_list == (void *)&cc_list_placeholder );

  // Test use.
  for( int i = 0; i < 30; ++i )
    UNTIL_SUCCESS( push( &our_list, i ) );
  LIST_CHECK;

  cleanup( &our_list );
}

// This needs to test, in particular, that r_end and end iterator-pointers are stable, especially during the transition
// from placeholder to non-placeholder.
static void test_list_iteration( void )
{
  list( int ) our_list;
  init( &our_list );

  int expected [ 30 ] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29
  };

  int *r_end = r_end( &our_list );
  int *end = end( &our_list );

  // Empty.

  // Test fist and last.
  ALWAYS_ASSERT( first( &our_list ) == end );
  ALWAYS_ASSERT( last( &our_list ) == r_end );

  // Test iteration from r_end and end.
  ALWAYS_ASSERT( next( &our_list, r_end( &our_list ) ) == first( &our_list ) );
  ALWAYS_ASSERT( prev( &our_list, end( &our_list ) ) == last( &our_list ) );

  size_t n_iterations = 0;
  for( int *i = first( &our_list ); i != end( &our_list ); i = next( &our_list, i ) )
    ++n_iterations;
  for( int *i = last( &our_list ); i != r_end( &our_list ); i = prev( &our_list, i ) )
    ++n_iterations;

  for_each( &our_list, i )
    ++n_iterations;
  for_each( &our_list, i )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 0 );

  // Non-empty.
  for( int i = 0; i < 30; ++i )
    UNTIL_SUCCESS( push( &our_list, i ) );
  
  for( int *i = first( &our_list ); i != end( &our_list ); i = next( &our_list, i ) )
    ++n_iterations;
  for( int *i = last( &our_list ); i != r_end( &our_list ); i = prev( &our_list, i ) )
    ++n_iterations;

  for_each( &our_list, i )
    ++n_iterations;
  for_each( &our_list, i )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 120 );

  // Test iterator stability.
  ALWAYS_ASSERT( r_end( &our_list ) == r_end );
  ALWAYS_ASSERT( end( &our_list ) == end );

  // Test iteration from r_end and end.
  ALWAYS_ASSERT( next( &our_list, r_end( &our_list ) ) == first( &our_list ) );
  ALWAYS_ASSERT( prev( &our_list, end( &our_list ) ) == last( &our_list ) );

  // Test fist and last.
  ALWAYS_ASSERT( *first( &our_list ) == 0 );
  ALWAYS_ASSERT( *last( &our_list ) == 29 );

  LIST_CHECK;

  cleanup( &our_list );
}

static void test_list_init_clone( void )
{
  list( int ) src_list;
  init( &src_list );

  int expected [ 10 ] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9
  };

  // Test init_clone placeholder.
  list( int ) empty_list;
  UNTIL_SUCCESS( init_clone( &empty_list, &src_list ) );
  ALWAYS_ASSERT( (void *)empty_list == (void *)&cc_list_placeholder );

  // Test init_clone non-placeholder.
  list( int ) our_list;
  for( int i = 0; i < 10; ++i )
    UNTIL_SUCCESS( push( &src_list, i ) );
  UNTIL_SUCCESS( init_clone( &our_list, &src_list ) ); // Note that because elements are allocated individually,
                                                       // this loop may never exist if realloc failure rate is set too
                                                       // high.
  LIST_CHECK;

  cleanup( &src_list );
  cleanup( &empty_list );
  cleanup( &our_list );
}

static void test_list_dtors( void )
{
  list( custom_ty ) our_list;
  init( &our_list );

  // Test erase and clear.

  for( int i = 0; i < 100; ++i )
  {
    custom_ty el = { i };
    UNTIL_SUCCESS( push( &our_list, el ) );
  }

  bool erase = true;
  for( custom_ty *i = first( &our_list ); i != end( &our_list ); )
  {
    if( erase )
      i = erase( &our_list, i );
    else
      i = next( &our_list, i );

    erase = !erase;
  }

  clear( &our_list );
  check_dtors_arr();

  // Test cleanup.

  for( int i = 0; i < 100; ++i )
  {
    custom_ty el = { i };
    UNTIL_SUCCESS( push( &our_list, el ) );
  }

  cleanup( &our_list );
  check_dtors_arr();
}

#endif

// Map tests.
#ifdef TEST_MAP

static void test_map_reserve( void )
{
  map( int, size_t ) our_map;
  init( &our_map );

  // Reserve zero with placeholder.
  UNTIL_SUCCESS( reserve( &our_map, 0 ) );
  ALWAYS_ASSERT( (void *)our_map  == (void *)&cc_map_placeholder );

  // Reserve up from placeholder.
  UNTIL_SUCCESS( reserve( &our_map, 30 ) );
  ALWAYS_ASSERT( 30 <= (double)cap( &our_map ) * CC_DEFAULT_LOAD );

  // Reserve same capacity.
  size_t cap = cap( &our_map );
  UNTIL_SUCCESS( reserve( &our_map, 30 ) );
  ALWAYS_ASSERT( cap( &our_map ) == cap );

  // Reserve up from non-placeholder.
  UNTIL_SUCCESS( reserve( &our_map, 60 ) );
  ALWAYS_ASSERT( 60 <= (double)cap( &our_map ) * CC_DEFAULT_LOAD );

  // Reserve lower capacity.
  cap = cap( &our_map );
  UNTIL_SUCCESS( reserve( &our_map, 30 ) );
  ALWAYS_ASSERT( cap( &our_map ) == cap );

  // Test validity through use.
  for( int i = 0; i < 60; ++i )
    UNTIL_SUCCESS( insert( &our_map, i, (size_t)i + 1 ) );

  // Check.
  ALWAYS_ASSERT( size( &our_map ) == 60 );
  for( int i = 0; i < 60; ++i )
    ALWAYS_ASSERT( *get( &our_map, i ) == (size_t)i + 1 );

  cleanup( &our_map );
}

static void test_map_shrink( void )
{
  map( int, size_t ) our_map;
  init( &our_map );

  // Test placeholder.
  UNTIL_SUCCESS( shrink( &our_map ) );
  ALWAYS_ASSERT( size( &our_map ) == 0 );
  ALWAYS_ASSERT( cap( &our_map ) == 0 );

  // Test restoration of placeholder.
  UNTIL_SUCCESS( reserve( &our_map, 30 ) );
  UNTIL_SUCCESS( shrink( &our_map ) );
  ALWAYS_ASSERT( size( &our_map ) == 0 );
  ALWAYS_ASSERT( cap( &our_map ) == 0 );
  ALWAYS_ASSERT( (void *)our_map == (void *)&cc_map_placeholder );

  // Test shrink same size.
  UNTIL_SUCCESS( reserve( &our_map, 30 ) );
  for( int i = 0; i < 30; ++i )
    insert( &our_map, i, (size_t)i + 1 );

  ALWAYS_ASSERT( size( &our_map ) == 30 );
  map( int, size_t ) same = our_map;
  size_t cap = cap( &our_map );
  UNTIL_SUCCESS( shrink( &our_map ) );
  ALWAYS_ASSERT( our_map == same );
  ALWAYS_ASSERT( cap( &our_map ) == cap );

  // Test shrink down.
  UNTIL_SUCCESS( reserve( &our_map, 500 ) );
  ALWAYS_ASSERT( size( &our_map ) == 30 );
  ALWAYS_ASSERT( 500 <= (double)cap( &our_map ) * CC_DEFAULT_LOAD );
  UNTIL_SUCCESS( shrink( &our_map ) );
  ALWAYS_ASSERT( size( &our_map ) == 30 );
  ALWAYS_ASSERT( cap( &our_map ) == cap );

  // Check.
  for( int i = 0; i < 30; ++i )
    ALWAYS_ASSERT( *get( &our_map, i ) == (size_t)i + 1 );

  cleanup( &our_map ); 
}

static void test_map_insert( void )
{
  map( int, size_t ) our_map;
  init( &our_map );

  // Insert new.
  for( int i = 0; i < 100; ++i )
  {
    size_t *el;
    UNTIL_SUCCESS( el = insert( &our_map, i, (size_t)i + 1 ) );
    ALWAYS_ASSERT( *el == (size_t)i + 1 );
  }

  // Insert existing.
  for( int i = 0; i < 100; ++i )
  {
    size_t *el;
    UNTIL_SUCCESS( el = insert( &our_map, i, (size_t)i + 2 ) );
    ALWAYS_ASSERT( *el == (size_t)i + 2 );
  }

  // Check.
  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( *get( &our_map, i ) == (size_t)i + 2 );

  cleanup( &our_map );
}

static void test_map_get_or_insert( void )
{
  map( int, size_t ) our_map;
  init( &our_map );

  // Test insert.
  for( int i = 0; i < 100; ++i )
  {
    size_t *el;
    UNTIL_SUCCESS( ( el = get_or_insert( &our_map, i, (size_t)i + 1 ) ) );
    ALWAYS_ASSERT( *el == (size_t)i + 1 );
  }

  ALWAYS_ASSERT( size( &our_map ) == 100 );
  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( *get( &our_map, i ) == (size_t)i + 1 );

  // Test get.
  for( int i = 0; i < 100; ++i )
  {
    size_t *el_1 = get( &our_map, i );
    size_t *el_2;
    UNTIL_SUCCESS( ( el_2 = get_or_insert( &our_map, i, (size_t)i + 1 ) ) );
    ALWAYS_ASSERT( el_2 == el_1 && *el_2 == (size_t)i + 1 );
  }

  ALWAYS_ASSERT( size( &our_map ) == 100 );

  cleanup( &our_map );
}

static void test_map_get( void )
{
  map( int, size_t ) our_map;
  init( &our_map );

  // Test empty.
  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( !get( &our_map, i ) );

  // Test get existing.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_map, i, (size_t)i + 1 ) );

  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( *get( &our_map, i ) == (size_t)i + 1 );

  // Test get non-existing.
  for( int i = 100; i < 200; ++i )
    ALWAYS_ASSERT( !get( &our_map, i ) );

  cleanup( &our_map );
}

static void test_map_erase( void )
{
  map( int, size_t ) our_map;
  init( &our_map );

  // Test erase existing.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_map, i, (size_t)i + 1 ) );

  ALWAYS_ASSERT( size( &our_map ) == 100 );

  for( int i = 0; i < 100; i += 2 )
    ALWAYS_ASSERT( erase( &our_map, i ) );

  // Test erase non-existing.
  for( int i = 0; i < 100; i += 2 )
    ALWAYS_ASSERT( !erase( &our_map, i ) );

  // Check.
  ALWAYS_ASSERT( size( &our_map ) == 50 );
  for( int i = 0; i < 100; ++i )
  {
    if( i % 2 == 0 )
      ALWAYS_ASSERT( !get( &our_map, i ) );
    else
      ALWAYS_ASSERT( *get( &our_map, i ) == (size_t)i + 1 );
  }

  cleanup( &our_map );
}

static void test_map_erase_itr( void )
{
  map( int, size_t ) our_map;
  init( &our_map );

  // In this instance, the key count and order of insert have been carefully chosen to cause skipped or repeat-visited
  // keys if erase_itr does not correctly handle the case of another key being moved to the bucket of the erased key.
  for( int i = 119; i >= 0; --i )
    UNTIL_SUCCESS( insert( &our_map, i, (size_t)i + 1 ) );

  ALWAYS_ASSERT( size( &our_map ) == 120 );

  // Test with iterator from get.
  for( int i = 0; i < 120; i += 4 )
    erase_itr( &our_map, get( &our_map, i ) );

  // Check.
  ALWAYS_ASSERT( size( &our_map ) == 90 );
  for( int i = 0; i < 120; ++i )
  {
    if( i % 4 == 0 )
      ALWAYS_ASSERT( !get( &our_map, i ) );
    else
    {
      size_t *el = get( &our_map, i );
      ALWAYS_ASSERT( el && *el == (size_t)i + 1 );
    }
  }

  // Test deletion while iterating.
  size_t *el = first( &our_map );
  size_t n_iterations = 0;
  while( el != end( &our_map ) )
  {
    ++n_iterations;

    if( *key_for( &our_map, el ) % 2 == 0 )
      el = erase_itr( &our_map, el );
    else
      el = next( &our_map, el );
  }

  ALWAYS_ASSERT( n_iterations == 90 );
  ALWAYS_ASSERT( size( &our_map ) == 60 );

  for( int i = 0; i < 120; ++i )
  {
    if( i % 2 == 0 )
      ALWAYS_ASSERT( !get( &our_map, i ) );
    else
    {
      el = get( &our_map, i );
      ALWAYS_ASSERT( el && *el == (size_t)i + 1 );
    }
  }

  cleanup( &our_map );
}

static void test_map_clear( void )
{
  map( int, size_t ) our_map;
  init( &our_map );

  // Test empty.
  clear( &our_map );
  ALWAYS_ASSERT( size( &our_map ) == 0 );

  // Test non-empty;
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_map, i, (size_t)i + 1 ) );

  clear( &our_map );
  ALWAYS_ASSERT( size( &our_map ) == 0 );
  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( !get( &our_map, i ) );

  // Test reuse.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_map, i, (size_t)i + 1 ) );

  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( *get( &our_map, i ) == (size_t)i + 1 );

  cleanup( &our_map );
}

static void test_map_cleanup( void )
{
  map( int, size_t ) our_map;
  init( &our_map );

  // Empty.
  cleanup( &our_map );
  ALWAYS_ASSERT( (void *)our_map == (void *)&cc_map_placeholder );

  // Non-empty.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_map, i, (size_t)i + 1 ) );
  ALWAYS_ASSERT( size( &our_map ) == 100 );
  cleanup( &our_map );
  ALWAYS_ASSERT( size( &our_map ) == 0 );
  ALWAYS_ASSERT( (void *)our_map == (void *)&cc_map_placeholder );

  // Test use.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_map, i, (size_t)i + 1 ) );
  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( *get( &our_map, i ) == (size_t)i + 1 );

  cleanup( &our_map );
}

static void test_map_init_clone( void )
{
  map( int, size_t ) src_map;
  init( &src_map );

  // Test init_clone placeholder.
  map( int, size_t ) empty_map;
  UNTIL_SUCCESS( init_clone( &empty_map, &src_map ) );
  ALWAYS_ASSERT( (void *)empty_map == (void *)&cc_map_placeholder );

  // Test init_clone non-placeholder.
  map( int, size_t ) our_map;
  for( int i = 0; i < 10; ++i )
    UNTIL_SUCCESS( insert( &src_map, i, (size_t)i + 1 ) );
  UNTIL_SUCCESS( init_clone( &our_map, &src_map ) );

  // Check.
  ALWAYS_ASSERT( size( &our_map ) == 10 );
  for( int i = 0; i < 10; ++i )
    ALWAYS_ASSERT( *get( &our_map, i ) == (size_t)i + 1 );

  cleanup( &src_map );
  cleanup( &empty_map );
  cleanup( &our_map );
}

static void test_map_iteration_and_get_key( void )
{
  map( int, size_t ) our_map;
  init( &our_map );

  // Empty.

  // Test first and last.
  ALWAYS_ASSERT( first( &our_map ) == end( &our_map ) );

  size_t n_iterations = 0;

  for( size_t *i = first( &our_map ); i != end( &our_map ); i = next( &our_map, i ) )
    ++n_iterations;

  for_each( &our_map, i )
    ++n_iterations;

  for_each( &our_map, k, i )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 0 );

  // Non-empty.

  for( int i = 0; i < 30; ++i )
    UNTIL_SUCCESS( insert( &our_map, i, (size_t)i + 1 ) );
  
  for( size_t *i = first( &our_map ); i != end( &our_map ); i = next( &our_map, i ) )
  {
    ALWAYS_ASSERT( (size_t)*key_for( &our_map, i ) == *i - 1 );
    ++n_iterations;
  }

  for_each( &our_map, i )
    ++n_iterations;

  for_each( &our_map, k, i )
  {
    ALWAYS_ASSERT( (size_t)*k == *i - 1 );
    ++n_iterations;
  }

  ALWAYS_ASSERT( n_iterations == 90 );

  // Iteration over empty, non-placeholder map.

  clear( &our_map );

  n_iterations = 0;

  for_each( &our_map, i )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 0 );

  cleanup( &our_map );
}

static void test_map_dtors( void )
{
  map( custom_ty, custom_ty ) our_map;
  init( &our_map );

  // Test erase and clear.

  for( int i = 0; i < 50; ++i )
  {
    custom_ty key = { i };
    custom_ty el = { i + 50 };
    UNTIL_SUCCESS( insert( &our_map, key, el ) );
  }

  for( int i = 0; i < 50; i += 2 )
  {
    custom_ty key = { i };
    erase( &our_map, key );
  }

  clear( &our_map );

  check_dtors_arr();

  // Test replace.

  for( int i = 0; i < 50; ++i )
  {
    custom_ty key = { i };
    custom_ty el = { i + 50 };
    UNTIL_SUCCESS( insert( &our_map, key, el ) );
  }
  for( int i = 0; i < 50; ++i )
  {
    custom_ty key = { i };
    custom_ty el = { i + 50 };
    UNTIL_SUCCESS( insert( &our_map, key, el ) );
  }

  check_dtors_arr();
  clear( &our_map );

  // Test cleanup.

  for( int i = 0; i < 50; ++i )
  {
    custom_ty key = { i };
    custom_ty el = { i + 50 };
    UNTIL_SUCCESS( insert( &our_map, key, el ) );
  }

  cleanup( &our_map );
  check_dtors_arr();
}

// C strings are a special case that warrant seperate testing.
static void test_map_strings( void )
{
  // Non-const strings.

  map( char *, char * ) our_map;
  init( &our_map );

  char **el;

  // String literals.
  UNTIL_SUCCESS( ( el = insert( &our_map, "This", "is" ) ) );
  ALWAYS_ASSERT( strcmp( *el, "is" ) == 0 );
  UNTIL_SUCCESS( ( el = get_or_insert( &our_map, "a", "test" ) ) );
  ALWAYS_ASSERT( strcmp( *el, "test" ) == 0 );

  // Other strings.
  char str_1[] = "of";
  char str_2[] = "maps";
  char str_3[] = "with";
  char str_4[] = "strings.";

  UNTIL_SUCCESS( ( el = insert( &our_map, str_1, str_2 ) ) );
  ALWAYS_ASSERT( strcmp( *el, str_2 ) == 0 );
  UNTIL_SUCCESS( ( el = get_or_insert( &our_map, str_3, str_4 ) ) );
  ALWAYS_ASSERT( strcmp( *el, str_4 ) == 0 );

  // Check.
  ALWAYS_ASSERT( size( &our_map ) == 4 );
  ALWAYS_ASSERT( strcmp( *get( &our_map, "This" ), "is" ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_map, "a" ), "test" ) == 0 );
  UNTIL_SUCCESS( ( el = insert( &our_map, str_1, str_2 ) ) );
  ALWAYS_ASSERT( strcmp( *el, str_2 ) == 0 );
  UNTIL_SUCCESS( ( el = insert( &our_map, str_3, str_4 ) ) );
  ALWAYS_ASSERT( strcmp( *el, str_4 ) == 0 );
  ALWAYS_ASSERT( size( &our_map ) == 4 );

  // Erase.
  erase( &our_map, "This" );
  erase( &our_map, str_1 );
  ALWAYS_ASSERT( size( &our_map ) == 2 );

  // Iteration.
  for_each( &our_map, i )
    ALWAYS_ASSERT( strcmp( *i, "test" ) == 0 || strcmp( *i, str_4 ) == 0 );

  cleanup( &our_map );

  // const strings.

  map( const char *, const char * ) our_const_map;
  init( &our_const_map );

  const char **const_el;

  // String literals.
  UNTIL_SUCCESS( ( const_el = insert( &our_const_map, "This", "is" ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, "is" ) == 0 );
  UNTIL_SUCCESS( ( const_el = get_or_insert( &our_const_map, "a", "test" ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, "test" ) == 0 );

  const char *const_str_1 = str_1;
  const char *const_str_2 = str_2;
  const char *const_str_3 = str_3;
  const char *const_str_4 = str_4;

  UNTIL_SUCCESS( ( const_el = insert( &our_const_map, const_str_1, const_str_2 ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, const_str_2 ) == 0 );
  UNTIL_SUCCESS( ( const_el = get_or_insert( &our_const_map, const_str_3, const_str_4 ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, const_str_4 ) == 0 );

  // Check.
  ALWAYS_ASSERT( size( &our_const_map ) == 4 );
  ALWAYS_ASSERT( strcmp( *get( &our_const_map, "This" ), "is" ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_const_map, "a" ), "test" ) == 0 );
  UNTIL_SUCCESS( ( const_el = insert( &our_const_map, const_str_1, const_str_2 ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, const_str_2 ) == 0 );
  UNTIL_SUCCESS( ( const_el = insert( &our_const_map, const_str_3, const_str_4 ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, const_str_4 ) == 0 );
  ALWAYS_ASSERT( size( &our_const_map ) == 4 );

  // Erase.
  erase( &our_const_map, "This" );
  erase( &our_const_map, const_str_1 );
  ALWAYS_ASSERT( size( &our_const_map ) == 2 );

  // Iteration.
  for_each( &our_const_map, i )
    ALWAYS_ASSERT( strcmp( *i, "test" ) == 0 || strcmp( *i, const_str_4 ) == 0 );

  cleanup( &our_const_map );
}

#define TEST_MAP_DEFAULT_INTEGER_TYPE( ty )        \
{                                                  \
  map( ty, int ) our_map;                          \
  init( &our_map );                                \
                                                   \
  for( int i = 0; i < 100; ++i )                   \
    UNTIL_SUCCESS( insert( &our_map, (ty)i, i ) ); \
                                                   \
  for( int i = 0; i < 100; ++i )                   \
    ALWAYS_ASSERT( *get( &our_map, (ty)i ) == i ); \
                                                   \
  cleanup( &our_map );                             \
}                                                  \

static void test_map_default_integer_types( void )
{
  TEST_MAP_DEFAULT_INTEGER_TYPE( char );
  TEST_MAP_DEFAULT_INTEGER_TYPE( unsigned char );
  TEST_MAP_DEFAULT_INTEGER_TYPE( signed char );
  TEST_MAP_DEFAULT_INTEGER_TYPE( unsigned short );
  TEST_MAP_DEFAULT_INTEGER_TYPE( short );
  TEST_MAP_DEFAULT_INTEGER_TYPE( unsigned int );
  TEST_MAP_DEFAULT_INTEGER_TYPE( int );
  TEST_MAP_DEFAULT_INTEGER_TYPE( unsigned long );
  TEST_MAP_DEFAULT_INTEGER_TYPE( long );
  TEST_MAP_DEFAULT_INTEGER_TYPE( unsigned long long );
  TEST_MAP_DEFAULT_INTEGER_TYPE( long );
  TEST_MAP_DEFAULT_INTEGER_TYPE( size_t );
}

#endif

// Set tests.
#ifdef TEST_SET

static void test_set_reserve( void )
{
  set( int ) our_set;
  init( &our_set );

  // Reserve zero with placeholder.
  UNTIL_SUCCESS( reserve( &our_set, 0 ) );
  ALWAYS_ASSERT( (void *)our_set == (void *)&cc_map_placeholder );

  // Reserve up from placeholder.
  UNTIL_SUCCESS( reserve( &our_set, 30 ) );
  ALWAYS_ASSERT( 30 <= (double)cap( &our_set ) * CC_DEFAULT_LOAD );

  // Reserve same capacity.
  size_t cap = cap( &our_set );
  UNTIL_SUCCESS( reserve( &our_set, 30 ) );
  ALWAYS_ASSERT( cap( &our_set ) == cap );

  // Reserve up from non-placeholder.
  UNTIL_SUCCESS( reserve( &our_set, 60 ) );
  ALWAYS_ASSERT( 60 <= (double)cap( &our_set ) * CC_DEFAULT_LOAD );

  // Reserve lower capacity.
  cap = cap( &our_set );
  UNTIL_SUCCESS( reserve( &our_set, 30 ) );
  ALWAYS_ASSERT( cap( &our_set ) == cap );

  // Test validity through use.
  for( int i = 0; i < 60; ++i )
    UNTIL_SUCCESS( insert( &our_set, i ) );

  // Check.
  ALWAYS_ASSERT( size( &our_set ) == 60 );
  for( int i = 0; i < 60; ++i )
    ALWAYS_ASSERT( *get( &our_set, i ) == i );

  cleanup( &our_set );
}

static void test_set_shrink( void )
{
  set( int ) our_set;
  init( &our_set );

  // Test placeholder.
  UNTIL_SUCCESS( shrink( &our_set ) );
  ALWAYS_ASSERT( size( &our_set ) == 0 );
  ALWAYS_ASSERT( cap( &our_set ) == 0 );

  // Test restoration of placeholder.
  UNTIL_SUCCESS( reserve( &our_set, 30 ) );
  UNTIL_SUCCESS( shrink( &our_set ) );
  ALWAYS_ASSERT( size( &our_set ) == 0 );
  ALWAYS_ASSERT( cap( &our_set ) == 0 );
  ALWAYS_ASSERT( (void *)our_set == (void *)&cc_map_placeholder );

  // Test shrink same size.
  UNTIL_SUCCESS( reserve( &our_set, 30 ) );
  for( int i = 0; i < 30; ++i )
    UNTIL_SUCCESS( insert( &our_set, i ) );

  ALWAYS_ASSERT( size( &our_set ) == 30 );
  set( int ) same = our_set;
  size_t cap = cap( &our_set );
  UNTIL_SUCCESS( shrink( &our_set ) );
  ALWAYS_ASSERT( our_set == same );
  ALWAYS_ASSERT( cap( &our_set ) == cap );

  // Test shrink down.
  UNTIL_SUCCESS( reserve( &our_set, 500 ) );
  ALWAYS_ASSERT( size( &our_set ) == 30 );
  ALWAYS_ASSERT( 500 <= (double)cap( &our_set ) * CC_DEFAULT_LOAD );
  UNTIL_SUCCESS( shrink( &our_set ) );

  ALWAYS_ASSERT( size( &our_set ) == 30 );
  ALWAYS_ASSERT( cap( &our_set ) == cap );

  // Check.
  for( int i = 0; i < 30; ++i )
    ALWAYS_ASSERT( *get( &our_set, i ) == i );

  cleanup( &our_set ); 
}

static void test_set_insert( void )
{
  set( int ) our_set;
  init( &our_set );

  // Insert new.
  for( int i = 0; i < 100; ++i )
  {
    int *el;
    UNTIL_SUCCESS( el = insert( &our_set, i ) );
    ALWAYS_ASSERT( ( *el == i ) );
  }

  // Insert existing.
  for( int i = 0; i < 100; ++i )
  {
    int *el;
    UNTIL_SUCCESS( el = insert( &our_set, i ) );
    ALWAYS_ASSERT( ( *el == i ) );
  }

  // Check.
  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( *get( &our_set, i ) == i );

  cleanup( &our_set );
}

static void test_set_get_or_insert( void )
{
  set( int ) our_set;
  init( &our_set );

  // Test insert.
  for( int i = 0; i < 100; ++i )
  {
    int *el;
    UNTIL_SUCCESS( ( el = get_or_insert( &our_set, i ) ) );
    ALWAYS_ASSERT( *el == i );
  }

  ALWAYS_ASSERT( size( &our_set ) == 100 );
  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( *get( &our_set, i ) == i );

  // Test get.
  for( int i = 0; i < 100; ++i )
  {
    int *el_1 = get( &our_set, i );
    int *el_2;
    UNTIL_SUCCESS( ( el_2 = get_or_insert( &our_set, i ) ) );
    ALWAYS_ASSERT( el_2 == el_1 && *el_2 == i );
  }

  ALWAYS_ASSERT( size( &our_set ) == 100 );

  cleanup( &our_set );
}

static void test_set_get( void )
{
  set( int ) our_set;
  init( &our_set );

  // Test empty.
  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( !get( &our_set, i ) );

  // Test get existing.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_set, i ) );

  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( *get( &our_set, i ) == i );

  // Test get non-existing.
  for( int i = 100; i < 200; ++i )
    ALWAYS_ASSERT( !get( &our_set, i ) );

  cleanup( &our_set );
}

static void test_set_erase( void )
{
  set( int ) our_set;
  init( &our_set );

  // Test erase existing.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_set, i ) );

  ALWAYS_ASSERT( size( &our_set ) == 100 );

  for( int i = 0; i < 100; i += 2 )
    ALWAYS_ASSERT( erase( &our_set, i ) );

  // Test erase non-existing.
  for( int i = 0; i < 100; i += 2 )
    ALWAYS_ASSERT( !erase( &our_set, i ) );

  // Check.
  ALWAYS_ASSERT( size( &our_set ) == 50 );
  for( int i = 0; i < 100; ++i )
  {
    if( i % 2 == 0 )
      ALWAYS_ASSERT( !get( &our_set, i ) );
    else
      ALWAYS_ASSERT( *get( &our_set, i ) == i );
  }

  cleanup( &our_set );
}

static void test_set_erase_itr( void )
{
  set( int ) our_set;
  init( &our_set );

  // In this instance, the key count and order of insert have been carefully chosen to cause skipped or repeat-visited
  // keys if vt_erase_itr does not correctly handle the case of another key being moved to the bucket of the erased key.
  for( int i = 119; i >= 0; --i )
    UNTIL_SUCCESS( insert( &our_set, i ) );

  ALWAYS_ASSERT( size( &our_set ) == 120 );

  // Test with iterator from get.
  for( int i = 0; i < 120; i += 4 )
    erase_itr( &our_set, get( &our_set, i ) );

  // Check.
  ALWAYS_ASSERT( size( &our_set ) == 90 );
  for( int i = 0; i < 120; ++i )
  {
    if( i % 4 == 0 )
      ALWAYS_ASSERT( !get( &our_set, i ) );
    else
    {
      int *el = get( &our_set, i );
      ALWAYS_ASSERT( el && *el == i );
    }
  }

  // Test deletion while iterating.
  int *el = first( &our_set );
  size_t n_iterations = 0;
  while( el != end( &our_set ) )
  {
    ++n_iterations;

    if( *el % 2 == 0 )
      el = erase_itr( &our_set, el );
    else
      el = next( &our_set, el );
  }

  ALWAYS_ASSERT( n_iterations == 90 );
  ALWAYS_ASSERT( size( &our_set ) == 60 );

  for( int i = 0; i < 120; ++i )
  {
    if( i % 2 == 0 )
      ALWAYS_ASSERT( !get( &our_set, i ) );
    else
    {
      el = get( &our_set, i );
      ALWAYS_ASSERT( el && *el == i );
    }
  }

  cleanup( &our_set );
}

static void test_set_clear( void )
{
  set( int ) our_set;
  init( &our_set );

  // Test empty.
  clear( &our_set );
  ALWAYS_ASSERT( size( &our_set ) == 0 );

  // Test non-empty;
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_set, i ) );

  clear( &our_set );
  ALWAYS_ASSERT( size( &our_set ) == 0 );
  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( !get( &our_set, i ) );

  // Test reuse.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_set, i ) );

  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( *get( &our_set, i ) == i );

  cleanup( &our_set );
}

static void test_set_cleanup( void )
{
  set( int ) our_set;
  init( &our_set );

  // Empty.
  cleanup( &our_set );
  ALWAYS_ASSERT( (void *)our_set == (void *)&cc_map_placeholder );

  // Non-empty.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_set, i ) );
  ALWAYS_ASSERT( size( &our_set ) == 100 );
  cleanup( &our_set );
  ALWAYS_ASSERT( size( &our_set ) == 0 );
  ALWAYS_ASSERT( (void *)our_set == (void *)&cc_map_placeholder );

  // Test use.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_set, i ) );
  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( *get( &our_set, i ) == i );

  cleanup( &our_set );
}

static void test_set_init_clone( void )
{
  set( int ) src_set;
  init( &src_set );

  // Test init_clone placeholder.
  set( int ) empty_set;
  UNTIL_SUCCESS( init_clone( &empty_set, &src_set ) );
  ALWAYS_ASSERT( (void *)empty_set == (void *)&cc_map_placeholder );

  // Test init_clone non-placeholder.
  set( int ) our_set = NULL;
  for( int i = 0; i < 10; ++i )
    UNTIL_SUCCESS( insert( &src_set, i ) );
  UNTIL_SUCCESS( init_clone( &our_set, &src_set ) );

  // Check
  ALWAYS_ASSERT( size( &our_set ) == 10 );
  for( int i = 0; i < 10; ++i )
    ALWAYS_ASSERT( *get( &our_set, i ) == i );

  cleanup( &src_set );
  cleanup( &empty_set );
  cleanup( &our_set );
}

static void test_set_iteration( void )
{
  set( int ) our_set;
  init( &our_set );

  // Empty.

  // Test first and last.
  ALWAYS_ASSERT( first( &our_set ) == end( &our_set ) );

  size_t n_iterations = 0;

  for( int *i = first( &our_set ); i != end( &our_set ); i = next( &our_set, i ) )
    ++n_iterations;

  for_each( &our_set, i )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 0 );

  // Non-empty.

  for( int i = 0; i < 30; ++i )
    UNTIL_SUCCESS( insert( &our_set, i ) );
  
  for( int *i = first( &our_set ); i != end( &our_set ); i = next( &our_set, i ) )
    ++n_iterations;

  for_each( &our_set, i )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 60 );

  // Iteration over empty, non-placeholder set.

  clear( &our_set );

  n_iterations = 0;

  for_each( &our_set, i )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 0 );

  cleanup( &our_set );
}

static void test_set_dtors( void )
{
  set( custom_ty ) our_set;
  init( &our_set );

  // Test erase and clear.

  for( int i = 0; i < 100; ++i )
  {
    custom_ty el = { i };
    UNTIL_SUCCESS( insert( &our_set, el ) );
  }

  for( int i = 0; i < 100; ++i )
  {
    custom_ty el = { i };
    erase( &our_set, el );
  }

  clear( &our_set );

  check_dtors_arr();

  // Test replace.

  for( int i = 0; i < 100; ++i )
  {
    custom_ty el = { i };
    UNTIL_SUCCESS( insert( &our_set, el ) );
  }
  for( int i = 0; i < 100; ++i )
  {
    custom_ty el = { i };
    UNTIL_SUCCESS( insert( &our_set, el ) );
  }

  check_dtors_arr();
  clear( &our_set );

  // Test cleanup.

  for( int i = 0; i < 100; ++i )
  {
    custom_ty el = { i };
    UNTIL_SUCCESS( insert( &our_set, el ) );
  }

  cleanup( &our_set );
  check_dtors_arr();
}

static void test_set_strings( void )
{
  // Non-const strings.

  set( char * ) our_set;
  init( &our_set );

  char **el;

  // String literals.
  UNTIL_SUCCESS( ( el = insert( &our_set, "This" ) ) );
  ALWAYS_ASSERT( strcmp( *el, "This" ) == 0 );
  UNTIL_SUCCESS( ( el = insert( &our_set, "is" ) ) );
  ALWAYS_ASSERT( strcmp( *el, "is" ) == 0 );
  UNTIL_SUCCESS( ( el = insert( &our_set, "a" ) ) );
  ALWAYS_ASSERT( strcmp( *el, "a" ) == 0 );
  UNTIL_SUCCESS( ( el = insert( &our_set, "test" ) ) );
  ALWAYS_ASSERT( strcmp( *el, "test" ) == 0 );

  // Other strings.
  char str_1[] = "of";
  char str_2[] = "sets";
  char str_3[] = "with";
  char str_4[] = "strings";

  UNTIL_SUCCESS( ( el = insert( &our_set, str_1 ) ) );
  ALWAYS_ASSERT( strcmp( *el, str_1 ) == 0 );
  UNTIL_SUCCESS( ( el = insert( &our_set, str_2 ) ) );
  ALWAYS_ASSERT( strcmp( *el, str_2 ) == 0 );
  UNTIL_SUCCESS( ( el = insert( &our_set, str_3 ) ) );
  ALWAYS_ASSERT( strcmp( *el, str_3 ) == 0 );
  UNTIL_SUCCESS( ( el = insert( &our_set, str_4 ) ) );
  ALWAYS_ASSERT( strcmp( *el, str_4 ) == 0 );

  // Check.
  ALWAYS_ASSERT( size( &our_set ) == 8 );
  ALWAYS_ASSERT( strcmp( *get( &our_set, "This" ), "This" ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_set, "is" ), "is" ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_set, "a" ), "a" ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_set, "test" ), "test" ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_set, "of" ), str_1 ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_set, "sets" ), str_2 ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_set, "with" ), str_3 ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_set, "strings" ), str_4 ) == 0 );

  cleanup( &our_set );

  // const strings.

  set( const char * ) our_const_set;
  init( &our_const_set );

  const char **const_el;

  // String literals.
  UNTIL_SUCCESS( ( const_el = insert( &our_const_set, "This" ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, "This" ) == 0 );
  UNTIL_SUCCESS( ( const_el = insert( &our_const_set, "is" ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, "is" ) == 0 );
  UNTIL_SUCCESS( ( const_el = insert( &our_const_set, "a" ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, "a" ) == 0 );
  UNTIL_SUCCESS( ( const_el = insert( &our_const_set, "test" ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, "test" ) == 0 );

  // Other strings.
  const char *const_str_1 = str_1;
  const char *const_str_2 = str_2;
  const char *const_str_3 = str_3;
  const char *const_str_4 = str_4;

  UNTIL_SUCCESS( ( const_el = insert( &our_const_set, const_str_1 ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, const_str_1 ) == 0 );
  UNTIL_SUCCESS( ( const_el = insert( &our_const_set, const_str_2 ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, const_str_2 ) == 0 );
  UNTIL_SUCCESS( ( const_el = insert( &our_const_set, const_str_3 ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, const_str_3 ) == 0 );
  UNTIL_SUCCESS( ( const_el = insert( &our_const_set, const_str_4 ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, const_str_4 ) == 0 );

  // Check.
  ALWAYS_ASSERT( size( &our_const_set ) == 8 );
  ALWAYS_ASSERT( strcmp( *get( &our_const_set, "This" ), "This" ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_const_set, "is" ), "is" ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_const_set, "a" ), "a" ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_const_set, "test" ), "test" ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_const_set, str_1 ), str_1 ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_const_set, str_2 ), str_2 ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_const_set, str_3 ), str_3 ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_const_set, str_4 ), str_4 ) == 0 );

  cleanup( &our_const_set );
}

#define TEST_SET_DEFAULT_INTEGER_TYPE( ty )    \
{                                              \
  set( ty ) our_set;                           \
  init( &our_set );                            \
                                               \
  for( ty i = 0; i < 100; ++i )                \
    UNTIL_SUCCESS( insert( &our_set, i ) );    \
                                               \
  for( ty i = 0; i < 100; ++i )                \
    ALWAYS_ASSERT( *get( &our_set, i ) == i ); \
                                               \
  cleanup( &our_set );                         \
}                                              \

static void test_set_default_integer_types( void )
{
  TEST_SET_DEFAULT_INTEGER_TYPE( char );
  TEST_SET_DEFAULT_INTEGER_TYPE( unsigned char );
  TEST_SET_DEFAULT_INTEGER_TYPE( signed char );
  TEST_SET_DEFAULT_INTEGER_TYPE( unsigned short );
  TEST_SET_DEFAULT_INTEGER_TYPE( short );
  TEST_SET_DEFAULT_INTEGER_TYPE( unsigned int );
  TEST_SET_DEFAULT_INTEGER_TYPE( int );
  TEST_SET_DEFAULT_INTEGER_TYPE( unsigned long );
  TEST_SET_DEFAULT_INTEGER_TYPE( long );
  TEST_SET_DEFAULT_INTEGER_TYPE( unsigned long long );
  TEST_SET_DEFAULT_INTEGER_TYPE( long );
  TEST_SET_DEFAULT_INTEGER_TYPE( size_t );
}

#endif

// Unordered map tests.
#ifdef TEST_OMAP

static void test_omap_insert( void )
{
  omap( int, size_t ) our_omap;
  init( &our_omap );

  // Sequential input.

  // Insert new.
  for( int i = 0; i < 100; ++i )
  {
    size_t *el;
    UNTIL_SUCCESS( el = insert( &our_omap, i, (size_t)i + 1 ) );
    ALWAYS_ASSERT( *el == (size_t)i + 1 );
  }

  // Insert existing.
  for( int i = 0; i < 100; ++i )
  {
    size_t *el;
    UNTIL_SUCCESS( el = insert( &our_omap, i, (size_t)i + 2 ) );
    ALWAYS_ASSERT( *el == (size_t)i + 2 );
  }

  // Check.
  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( *get( &our_omap, i ) == (size_t)i + 2 );

  clear( &our_omap );

  // Nonsequential input (alternating positive and negative integers).

  // Insert new.
  for( int i = 0; i < 100; ++i )
  {
    size_t *el;
    UNTIL_SUCCESS( el = insert( &our_omap, i * ( i % 2 ? 1 : -1 ), (size_t)i + 1 ) );
    ALWAYS_ASSERT( *el == (size_t)i + 1 );
  }

  // Insert existing.
  for( int i = 0; i < 100; ++i )
  {
    size_t *el;
    UNTIL_SUCCESS( el = insert( &our_omap, i * ( i % 2 ? 1 : -1 ), (size_t)i + 2 ) );
    ALWAYS_ASSERT( *el == (size_t)i + 2 );
  }

  // Check.
  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( *get( &our_omap, i * ( i % 2 ? 1 : -1 ) ) == (size_t)i + 2 );

  cleanup( &our_omap );
}

static void test_omap_get_or_insert( void )
{
  omap( int, size_t ) our_omap;
  init( &our_omap );

  // Test insert.
  for( int i = 0; i < 100; ++i )
  {
    size_t *el;
    UNTIL_SUCCESS( ( el = get_or_insert( &our_omap, i, (size_t)i + 1 ) ) );
    ALWAYS_ASSERT( *el == (size_t)i + 1 );
  }

  ALWAYS_ASSERT( size( &our_omap ) == 100 );
  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( *get( &our_omap, i ) == (size_t)i + 1 );

  // Test get.
  for( int i = 0; i < 100; ++i )
  {
    size_t *el_1 = get( &our_omap, i );
    size_t *el_2;
    UNTIL_SUCCESS( ( el_2 = get_or_insert( &our_omap, i, (size_t)i + 1 ) ) );
    ALWAYS_ASSERT( el_2 == el_1 && *el_2 == (size_t)i + 1 );
  }

  ALWAYS_ASSERT( size( &our_omap ) == 100 );

  cleanup( &our_omap );
}

static void test_omap_get( void )
{
  omap( int, size_t ) our_omap;
  init( &our_omap );

  // Test empty.
  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( !get( &our_omap, i ) );

  // Test get existing.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_omap, i, (size_t)i + 1 ) );

  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( *get( &our_omap, i ) == (size_t)i + 1 );

  // Test get non-existing.
  for( int i = 100; i < 200; ++i )
    ALWAYS_ASSERT( !get( &our_omap, i ) );

  cleanup( &our_omap );
}

static void test_omap_erase( void )
{
  omap( int, size_t ) our_omap;
  init( &our_omap );

  // Sequential input.

  // Test erase existing.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_omap, i, (size_t)i + 1 ) );

  ALWAYS_ASSERT( size( &our_omap ) == 100 );

  for( int i = 0; i < 100; i += 2 )
    ALWAYS_ASSERT( erase( &our_omap, i ) );

  // Test erase non-existing.
  for( int i = 0; i < 100; i += 2 )
    ALWAYS_ASSERT( !erase( &our_omap, i ) );

  // Check.
  ALWAYS_ASSERT( size( &our_omap ) == 50 );
  for( int i = 0; i < 100; ++i )
  {
    if( i % 2 == 0 )
      ALWAYS_ASSERT( !get( &our_omap, i ) );
    else
      ALWAYS_ASSERT( *get( &our_omap, i ) == (size_t)i + 1 );
  }

  clear( &our_omap );

  // Nonsequential input (alternating positive and negative integers).

  // Test erase existing.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_omap, i * ( i % 2 ? 1 : -1 ), (size_t)i + 1 ) );

  ALWAYS_ASSERT( size( &our_omap ) == 100 );

  for( int i = 0; i < 100; i += 2 )
    ALWAYS_ASSERT( erase( &our_omap, i * ( i % 2 ? 1 : -1 ) ) );

  // Test erase non-existing.
  for( int i = 0; i < 100; i += 2 )
    ALWAYS_ASSERT( !erase( &our_omap, i * ( i % 2 ? 1 : -1 ) ) );

  // Check.
  ALWAYS_ASSERT( size( &our_omap ) == 50 );
  for( int i = 0; i < 100; ++i )
  {
    if( i % 2 == 0 )
      ALWAYS_ASSERT( !get( &our_omap, i * ( i % 2 ? 1 : -1 ) ) );
    else
      ALWAYS_ASSERT( *get( &our_omap, i * ( i % 2 ? 1 : -1 ) ) == (size_t)i + 1 );
  }

  cleanup( &our_omap );
}

static void test_omap_erase_itr( void )
{
  omap( int, size_t ) our_omap;
  init( &our_omap );

  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_omap, i, (size_t)i + 1 ) );

  ALWAYS_ASSERT( size( &our_omap ) == 100 );

  // Test with iterator from get.
  for( int i = 0; i < 100; i += 4 )
    erase_itr( &our_omap, get( &our_omap, i ) );

  // Check.
  ALWAYS_ASSERT( size( &our_omap ) == 75 );
  for( int i = 0; i < 100; ++i )
  {
    if( i % 4 == 0 )
      ALWAYS_ASSERT( !get( &our_omap, i ) );
    else
    {
      size_t *el = get( &our_omap, i );
      ALWAYS_ASSERT( el && *el == (size_t)i + 1 );
    }
  }

  // Test deletion while iterating.

  size_t *el = first( &our_omap );
  size_t n_iterations = 0;
  while( el != end( &our_omap ) )
  {
    ++n_iterations;

    if( *key_for( &our_omap, el ) % 2 == 0 )
      el = erase_itr( &our_omap, el );
    else
      el = next( &our_omap, el );
  }

  ALWAYS_ASSERT( n_iterations == 75 );
  ALWAYS_ASSERT( size( &our_omap ) == 50 );

  for( int i = 0; i < 100; ++i )
  {
    if( i % 2 == 0 )
      ALWAYS_ASSERT( !get( &our_omap, i ) );
    else
    {
      el = get( &our_omap, i );
      ALWAYS_ASSERT( el && *el == (size_t)i + 1 );
    }
  }

  cleanup( &our_omap );
}

static void test_omap_clear( void )
{
  omap( int, size_t ) our_omap;
  init( &our_omap );

  // Test empty.
  clear( &our_omap );
  ALWAYS_ASSERT( size( &our_omap ) == 0 );

  // Test non-empty;
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_omap, i, (size_t)i + 1 ) );

  clear( &our_omap );
  ALWAYS_ASSERT( size( &our_omap ) == 0 );
  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( !get( &our_omap, i ) );

  // Test reuse.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_omap, i, (size_t)i + 1 ) );

  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( *get( &our_omap, i ) == (size_t)i + 1 );

  cleanup( &our_omap );
}

static void test_omap_cleanup( void )
{
  omap( int, size_t ) our_omap;
  init( &our_omap );

  // Empty.
  cleanup( &our_omap );
  ALWAYS_ASSERT( (void *)our_omap == (void *)&cc_omap_placeholder );

  // Non-empty.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_omap, i, (size_t)i + 1 ) );
  ALWAYS_ASSERT( size( &our_omap ) == 100 );
  cleanup( &our_omap );
  ALWAYS_ASSERT( size( &our_omap ) == 0 );
  ALWAYS_ASSERT( (void *)our_omap == (void *)&cc_omap_placeholder );

  // Test use.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_omap, i, (size_t)i + 1 ) );
  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( *get( &our_omap, i ) == (size_t)i + 1 );

  cleanup( &our_omap );
}

static void test_omap_init_clone( void )
{
  omap( int, size_t ) src_omap;
  init( &src_omap );

  // Test init_clone placeholder.
  omap( int, size_t ) empty_omap;
  UNTIL_SUCCESS( init_clone( &empty_omap, &src_omap ) );
  ALWAYS_ASSERT( (void *)empty_omap == (void *)&cc_omap_placeholder );

  // Test init_clone non-placeholder.
  omap( int, size_t ) our_omap;
  for( int i = 0; i < 10; ++i )
    UNTIL_SUCCESS( insert( &src_omap, i, (size_t)i + 1 ) );
  UNTIL_SUCCESS( init_clone( &our_omap, &src_omap ) );

  // Check.
  ALWAYS_ASSERT( size( &our_omap ) == 10 );
  for( int i = 0; i < 10; ++i )
    ALWAYS_ASSERT( *get( &our_omap, i ) == (size_t)i + 1 );

  cleanup( &src_omap );
  cleanup( &empty_omap );
  cleanup( &our_omap );
}

// This needs to test, in particular, that r_end and end iterator-pointers are stable, especially during the transition
// from placeholder to non-placeholder.
static void test_omap_iteration_and_get_key( void )
{
  omap( int, size_t ) our_omap;
  init( &our_omap );

  size_t *r_end = r_end( &our_omap );
  size_t *end = end( &our_omap );

  // Empty.

  // Test first and last.
  ALWAYS_ASSERT( first( &our_omap ) == end( &our_omap ) );
  ALWAYS_ASSERT( last( &our_omap ) == r_end( &our_omap ) );

  // Test iteration from r_end and end.
  ALWAYS_ASSERT( next( &our_omap, r_end( &our_omap ) ) == first( &our_omap ) );
  ALWAYS_ASSERT( prev( &our_omap, end( &our_omap ) ) == last( &our_omap ) );

  size_t n_iterations = 0;
  for( size_t *i = first( &our_omap ); i != end( &our_omap ); i = next( &our_omap, i ) )
    ++n_iterations;
  for( size_t *i = last( &our_omap ); i != r_end( &our_omap ); i = prev( &our_omap, i ) )
    ++n_iterations;
  for_each( &our_omap, i )
    ++n_iterations;
  r_for_each( &our_omap, i )
    ++n_iterations;
  for_each( &our_omap, k, i )
    ++n_iterations;
  r_for_each( &our_omap, k, i )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 0 );

  // Non-empty.

  // Insert keys in random order.

  const int keys[ 100 ] = {
    12, 10, 29, 8, 27, 9, 14, 23, 18, 19, 11, 20, 24, 1, 0, 5, 2, 3, 6, 13, 28, 25, 22, 21, 15, 4, 7, 16, 26, 17
  };

  for( int i = 0; i < 30; ++i )
    UNTIL_SUCCESS( insert( &our_omap, keys[ i ], (size_t)keys[ i ] + 1 ) );

  size_t *last_iteration = NULL;
  for( size_t *i = first( &our_omap ); i != end( &our_omap ); i = next( &our_omap, i ) )
  {
    ALWAYS_ASSERT( (size_t)*key_for( &our_omap, i ) == *i - 1 );
    ALWAYS_ASSERT( !last_iteration || *key_for( &our_omap, i ) > *key_for( &our_omap, last_iteration ) );
    ++n_iterations;
    last_iteration = i;
  }

  last_iteration = NULL;
  for( size_t *i = last( &our_omap ); i != r_end( &our_omap ); i = prev( &our_omap, i ) )
  {
    ALWAYS_ASSERT( (size_t)*key_for( &our_omap, i ) == *i - 1 );
    ALWAYS_ASSERT( !last_iteration || *key_for( &our_omap, i ) < *key_for( &our_omap, last_iteration ) );
    ++n_iterations;
    last_iteration = i;
  }

  for_each( &our_omap, i )
    ++n_iterations;
  r_for_each( &our_omap, i )
    ++n_iterations;

  for_each( &our_omap, k, i )
  {
    ALWAYS_ASSERT( (size_t)*k == *i - 1 );
    ++n_iterations;
  }
  r_for_each( &our_omap, k, i )
  {
    ALWAYS_ASSERT( (size_t)*k == *i - 1 );
    ++n_iterations;
  }

  ALWAYS_ASSERT( n_iterations == 180 );

  // Test iterator stability.
  ALWAYS_ASSERT( r_end( &our_omap ) == r_end );
  ALWAYS_ASSERT( end( &our_omap ) == end );

  // Test iteration from r_end and end.
  ALWAYS_ASSERT( next( &our_omap, r_end( &our_omap ) ) == first( &our_omap ) );
  ALWAYS_ASSERT( prev( &our_omap, end( &our_omap ) ) == last( &our_omap ) );

  // Iteration over empty, non-placeholder ordered map.

  clear( &our_omap );

  n_iterations = 0;

  for_each( &our_omap, i )
    ++n_iterations;

  r_for_each( &our_omap, i )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 0 );

  cleanup( &our_omap );
}

static void test_omap_iteration_over_range( void )
{
  omap( int, size_t ) our_omap;
  init( &our_omap );

  // Empty.

  size_t n_iterations = 0;
  for(
    size_t *i = first( &our_omap, 25 ), *range_end = first( &our_omap, 75 );
    i != range_end;
    i = next( &our_omap, i )
  )
    ++n_iterations;

  for(
    size_t *i = last( &our_omap, 74 ), *range_end = last( &our_omap, 24 );
    i != range_end;
    i = prev( &our_omap, i )
  )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 0 );

  // Non-empty.

  // Insert keys in random order.

  const int keys[ 100 ] = {
    44, 13, 39, 68, 33, 88, 87, 58, 73, 28, 95, 56, 93, 8, 50, 92, 78, 80, 97, 53,
    27, 77, 35, 38, 91, 45, 3, 37, 98, 81, 63, 65, 32, 90, 72, 5, 36, 99, 17, 6,
    16, 11, 67, 47, 48, 71, 1, 82, 69, 21, 54, 15, 61, 9, 19, 84, 60, 26, 42, 70,
    64, 18, 34, 23, 75, 52, 89, 83, 86, 10, 94, 24, 57, 59, 41, 20, 25, 12, 85, 96,
    66, 55, 7, 2, 76, 46, 14, 31, 43, 4, 22, 30, 40, 29, 0, 74, 51, 49, 62, 79
  };

  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_omap, keys[ i ], 0 ) );

  // Test ranges that do not include r_end or end.

  for(
    size_t *i = first( &our_omap, 25 ), *range_end = first( &our_omap, 75 );
    i != range_end;
    i = next( &our_omap, i )
  )
  {
    const int *key = key_for( &our_omap, i );
    ALWAYS_ASSERT( *key >= 25 && *key < 75 );
    ++n_iterations;
  }

  for(
    size_t *i = last( &our_omap, 75 ), *range_end = last( &our_omap, 25 );
    i != range_end;
    i = prev( &our_omap, i )
  )
  {
    const int *key = key_for( &our_omap, i );
    ALWAYS_ASSERT( *key > 25 && *key <= 75 );
    ++n_iterations;
  }

  ALWAYS_ASSERT( n_iterations == 100 );

  // Test ranges that overlap r_end or end.

  for(
    size_t *i = first( &our_omap, -1 ), *range_end = first( &our_omap, 50 );
    i != range_end;
    i = next( &our_omap, i )
  )
  {
    ALWAYS_ASSERT( *key_for( &our_omap, i ) < 50 );
    ++n_iterations;
  }

  for(
    size_t *i = first( &our_omap, 50 ), *range_end = first( &our_omap, 100 );
    i != range_end;
    i = next( &our_omap, i )
  )
  {
    ALWAYS_ASSERT( *key_for( &our_omap, i ) >= 50 );
    ++n_iterations;
  }

  for(
    size_t *i = last( &our_omap, 100 ), *range_end = last( &our_omap, 49 );
    i != range_end;
    i = prev( &our_omap, i )
  )
  {
    ALWAYS_ASSERT( *key_for( &our_omap, i ) >= 50 );
    ++n_iterations;
  }

  for(
    size_t *i = last( &our_omap, 49 ), *range_end = last( &our_omap, -1 );
    i != range_end;
    i = prev( &our_omap, i )
  )
  {
    ALWAYS_ASSERT( *key_for( &our_omap, i ) <= 49 );
    ++n_iterations;
  }

  for(
    size_t *i = first( &our_omap, -1 ), *range_end = first( &our_omap, 100 );
    i != range_end;
    i = next( &our_omap, i )
  )
    ++n_iterations;

  for(
    size_t *i = last( &our_omap, 100 ), *range_end = last( &our_omap, -1 );
    i != range_end;
    i = prev( &our_omap, i )
  )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 500 );

  // Test ranges with no keys.

  for(
    size_t *i = first( &our_omap, 100 ), *range_end = first( &our_omap, 200 );
    i != range_end;
    i = next( &our_omap, i )
  )
    ++n_iterations;

  for(
    size_t *i = last( &our_omap, -1 ), *range_end = last( &our_omap, -100 );
    i != range_end;
    i = prev( &our_omap, i )
  )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 500 );

  cleanup( &our_omap );
}

static void test_omap_dtors( void )
{
  omap( custom_ty, custom_ty ) our_omap;
  init( &our_omap );

  // Test erase and clear.

  for( int i = 0; i < 50; ++i )
  {
    custom_ty key = { i };
    custom_ty el = { i + 50 };
    UNTIL_SUCCESS( insert( &our_omap, key, el ) );
  }

  for( int i = 0; i < 50; i += 2 )
  {
    custom_ty key = { i };
    erase( &our_omap, key );
  }

  clear( &our_omap );

  check_dtors_arr();

  // Test replace.

  for( int i = 0; i < 50; ++i )
  {
    custom_ty key = { i };
    custom_ty el = { i + 50 };
    UNTIL_SUCCESS( insert( &our_omap, key, el ) );
  }
  for( int i = 0; i < 50; ++i )
  {
    custom_ty key = { i };
    custom_ty el = { i + 50 };
    UNTIL_SUCCESS( insert( &our_omap, key, el ) );
  }

  check_dtors_arr();
  clear( &our_omap );

  // Test cleanup.

  for( int i = 0; i < 50; ++i )
  {
    custom_ty key = { i };
    custom_ty el = { i + 50 };
    UNTIL_SUCCESS( insert( &our_omap, key, el ) );
  }

  cleanup( &our_omap );
  check_dtors_arr();
}

// Strings are a special case that warrant seperate testing.
static void test_omap_strings( void )
{
  // Non-const strings.

  omap( char *, char * ) our_omap;
  init( &our_omap );

  char **el;

  // String literals.
  UNTIL_SUCCESS( ( el = insert( &our_omap, "This", "is" ) ) );
  ALWAYS_ASSERT( strcmp( *el, "is" ) == 0 );
  UNTIL_SUCCESS( ( el = get_or_insert( &our_omap, "a", "test" ) ) );
  ALWAYS_ASSERT( strcmp( *el, "test" ) == 0 );

  // Other strings.
  char str_1[] = "of";
  char str_2[] = "omaps";
  char str_3[] = "with";
  char str_4[] = "strings.";

  UNTIL_SUCCESS( ( el = insert( &our_omap, str_1, str_2 ) ) );
  ALWAYS_ASSERT( strcmp( *el, str_2 ) == 0 );
  UNTIL_SUCCESS( ( el = get_or_insert( &our_omap, str_3, str_4 ) ) );
  ALWAYS_ASSERT( strcmp( *el, str_4 ) == 0 );

  // Check.
  ALWAYS_ASSERT( size( &our_omap ) == 4 );
  ALWAYS_ASSERT( strcmp( *get( &our_omap, "This" ), "is" ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_omap, "a" ), "test" ) == 0 );
  UNTIL_SUCCESS( ( el = insert( &our_omap, str_1, str_2 ) ) );
  ALWAYS_ASSERT( strcmp( *el, str_2 ) == 0 );
  UNTIL_SUCCESS( ( el = insert( &our_omap, str_3, str_4 ) ) );
  ALWAYS_ASSERT( strcmp( *el, str_4 ) == 0 );
  ALWAYS_ASSERT( size( &our_omap ) == 4 );

  // Erase.
  erase( &our_omap, "This" );
  erase( &our_omap, str_1 );
  ALWAYS_ASSERT( size( &our_omap ) == 2 );

  // Iteration.
  for_each( &our_omap, i )
    ALWAYS_ASSERT( strcmp( *i, "test" ) == 0 || strcmp( *i, str_4 ) == 0 );

  cleanup( &our_omap );

  // const strings.

  omap( const char *, const char * ) our_const_omap;
  init( &our_const_omap );

  const char **const_el;

  // String literals.
  UNTIL_SUCCESS( ( const_el = insert( &our_const_omap, "This", "is" ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, "is" ) == 0 );
  UNTIL_SUCCESS( ( const_el = get_or_insert( &our_const_omap, "a", "test" ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, "test" ) == 0 );

  const char *const_str_1 = str_1;
  const char *const_str_2 = str_2;
  const char *const_str_3 = str_3;
  const char *const_str_4 = str_4;

  UNTIL_SUCCESS( ( const_el = insert( &our_const_omap, const_str_1, const_str_2 ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, const_str_2 ) == 0 );
  UNTIL_SUCCESS( ( const_el = get_or_insert( &our_const_omap, const_str_3, const_str_4 ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, const_str_4 ) == 0 );

  // Check.
  ALWAYS_ASSERT( size( &our_const_omap ) == 4 );
  ALWAYS_ASSERT( strcmp( *get( &our_const_omap, "This" ), "is" ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_const_omap, "a" ), "test" ) == 0 );
  UNTIL_SUCCESS( ( const_el = insert( &our_const_omap, const_str_1, const_str_2 ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, const_str_2 ) == 0 );
  UNTIL_SUCCESS( ( const_el = insert( &our_const_omap, const_str_3, const_str_4 ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, const_str_4 ) == 0 );
  ALWAYS_ASSERT( size( &our_const_omap ) == 4 );

  // Erase.
  erase( &our_const_omap, "This" );
  erase( &our_const_omap, const_str_1 );
  ALWAYS_ASSERT( size( &our_const_omap ) == 2 );

  // Iteration.
  for_each( &our_const_omap, i )
    ALWAYS_ASSERT( strcmp( *i, "test" ) == 0 || strcmp( *i, const_str_4 ) == 0 );

  cleanup( &our_const_omap );
}

#define TEST_OMAP_DEFAULT_INTEGER_TYPE( ty )        \
{                                                   \
  omap( ty, int ) our_omap;                         \
  init( &our_omap );                                \
                                                    \
  for( int i = 0; i < 100; ++i )                    \
    UNTIL_SUCCESS( insert( &our_omap, (ty)i, i ) ); \
                                                    \
  for( int i = 0; i < 100; ++i )                    \
    ALWAYS_ASSERT( *get( &our_omap, (ty)i ) == i ); \
                                                    \
  cleanup( &our_omap );                             \
}                                                   \

static void test_omap_default_integer_types( void )
{
  TEST_OMAP_DEFAULT_INTEGER_TYPE( char );
  TEST_OMAP_DEFAULT_INTEGER_TYPE( unsigned char );
  TEST_OMAP_DEFAULT_INTEGER_TYPE( signed char );
  TEST_OMAP_DEFAULT_INTEGER_TYPE( unsigned short );
  TEST_OMAP_DEFAULT_INTEGER_TYPE( short );
  TEST_OMAP_DEFAULT_INTEGER_TYPE( unsigned int );
  TEST_OMAP_DEFAULT_INTEGER_TYPE( int );
  TEST_OMAP_DEFAULT_INTEGER_TYPE( unsigned long );
  TEST_OMAP_DEFAULT_INTEGER_TYPE( long );
  TEST_OMAP_DEFAULT_INTEGER_TYPE( unsigned long long );
  TEST_OMAP_DEFAULT_INTEGER_TYPE( long );
  TEST_OMAP_DEFAULT_INTEGER_TYPE( size_t );
}

#endif

// Unordered set tests.
#ifdef TEST_OSET

static void test_oset_insert( void )
{
  oset( int ) our_oset;
  init( &our_oset );

  // Sequential input.

  // Insert new.
  for( int i = 0; i < 100; ++i )
  {
    int *el;
    UNTIL_SUCCESS( el = insert( &our_oset, i ) );
    ALWAYS_ASSERT( *el == i );
  }

  // Insert existing.
  for( int i = 0; i < 100; ++i )
  {
    int *el;
    UNTIL_SUCCESS( el = insert( &our_oset, i ) );
    ALWAYS_ASSERT( *el == i );
  }

  // Check.
  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( *get( &our_oset, i ) == i );

  clear( &our_oset );

  // Nonsequential input (alternating positive and negative integers).

  // Insert new.
  for( int i = 0; i < 100; ++i )
  {
    int *el;
    UNTIL_SUCCESS( el = insert( &our_oset, i * ( i % 2 ? 1 : -1 ) ) );
    ALWAYS_ASSERT( *el == i * ( i % 2 ? 1 : -1 ) );
  }

  // Insert existing.
  for( int i = 0; i < 100; ++i )
  {
    int *el;
    UNTIL_SUCCESS( el = insert( &our_oset, i * ( i % 2 ? 1 : -1 ) ) );
    ALWAYS_ASSERT( *el == i * ( i % 2 ? 1 : -1 ) );
  }

  // Check.
  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( *get( &our_oset, i * ( i % 2 ? 1 : -1 ) ) == i * ( i % 2 ? 1 : -1 ) );

  cleanup( &our_oset );
}

static void test_oset_get_or_insert( void )
{
  oset( int ) our_oset;
  init( &our_oset );

  // Test insert.
  for( int i = 0; i < 100; ++i )
  {
    int *el;
    UNTIL_SUCCESS( ( el = get_or_insert( &our_oset, i ) ) );
    ALWAYS_ASSERT( *el == i );
  }

  ALWAYS_ASSERT( size( &our_oset ) == 100 );
  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( *get( &our_oset, i ) == i );

  // Test get.
  for( int i = 0; i < 100; ++i )
  {
    int *el_1 = get( &our_oset, i );
    int *el_2;
    UNTIL_SUCCESS( ( el_2 = get_or_insert( &our_oset, i ) ) );
    ALWAYS_ASSERT( el_2 == el_1 && *el_2 == i );
  }

  ALWAYS_ASSERT( size( &our_oset ) == 100 );

  cleanup( &our_oset );
}

static void test_oset_get( void )
{
  oset( int ) our_oset;
  init( &our_oset );

  // Test empty.
  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( !get( &our_oset, i ) );

  // Test get existing.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_oset, i ) );

  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( *get( &our_oset, i ) == i );

  // Test get non-existing.
  for( int i = 100; i < 200; ++i )
    ALWAYS_ASSERT( !get( &our_oset, i ) );

  cleanup( &our_oset );
}

static void test_oset_erase( void )
{
  oset( int ) our_oset;
  init( &our_oset );

  // Sequential input.

  // Test erase existing.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_oset, i ) );

  ALWAYS_ASSERT( size( &our_oset ) == 100 );

  for( int i = 0; i < 100; i += 2 )
    ALWAYS_ASSERT( erase( &our_oset, i ) );

  // Test erase non-existing.
  for( int i = 0; i < 100; i += 2 )
    ALWAYS_ASSERT( !erase( &our_oset, i ) );

  // Check.
  ALWAYS_ASSERT( size( &our_oset ) == 50 );
  for( int i = 0; i < 100; ++i )
  {
    if( i % 2 == 0 )
      ALWAYS_ASSERT( !get( &our_oset, i ) );
    else
      ALWAYS_ASSERT( *get( &our_oset, i ) == i );
  }

  clear( &our_oset );

  // Nonsequential input (alternating positive and negative integers).

  // Test erase existing.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_oset, i * ( i % 2 ? 1 : -1 ) ) );

  ALWAYS_ASSERT( size( &our_oset ) == 100 );

  for( int i = 0; i < 100; i += 2 )
    ALWAYS_ASSERT( erase( &our_oset, i * ( i % 2 ? 1 : -1 ) ) );

  // Test erase non-existing.
  for( int i = 0; i < 100; i += 2 )
    ALWAYS_ASSERT( !erase( &our_oset, i * ( i % 2 ? 1 : -1 ) ) );

  // Check.
  ALWAYS_ASSERT( size( &our_oset ) == 50 );
  for( int i = 0; i < 100; ++i )
  {
    if( i % 2 == 0 )
      ALWAYS_ASSERT( !get( &our_oset, i * ( i % 2 ? 1 : -1 ) ) );
    else
      ALWAYS_ASSERT( *get( &our_oset, i * ( i % 2 ? 1 : -1 ) ) == i * ( i % 2 ? 1 : -1 ) );
  }

  cleanup( &our_oset );
}

static void test_oset_erase_itr( void )
{
  oset( int ) our_oset;
  init( &our_oset );

  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_oset, i ) );

  ALWAYS_ASSERT( size( &our_oset ) == 100 );

  // Test with iterator from get.
  for( int i = 0; i < 100; i += 4 )
    erase_itr( &our_oset, get( &our_oset, i ) );

  // Check.
  ALWAYS_ASSERT( size( &our_oset ) == 75 );
  for( int i = 0; i < 100; ++i )
  {
    if( i % 4 == 0 )
      ALWAYS_ASSERT( !get( &our_oset, i ) );
    else
    {
      int *el = get( &our_oset, i );
      ALWAYS_ASSERT( el && *el == i );
    }
  }

  // Test deletion while iterating.

  int *el = first( &our_oset );
  size_t n_iterations = 0;
  while( el != end( &our_oset ) )
  {
    ++n_iterations;

    if( *el % 2 == 0 )
      el = erase_itr( &our_oset, el );
    else
      el = next( &our_oset, el );
  }

  ALWAYS_ASSERT( n_iterations == 75 );
  ALWAYS_ASSERT( size( &our_oset ) == 50 );

  for( int i = 0; i < 100; ++i )
  {
    if( i % 2 == 0 )
      ALWAYS_ASSERT( !get( &our_oset, i ) );
    else
    {
      el = get( &our_oset, i );
      ALWAYS_ASSERT( el && *el == i  );
    }
  }

  cleanup( &our_oset );
}

static void test_oset_clear( void )
{
  oset( int ) our_oset;
  init( &our_oset );

  // Test empty.
  clear( &our_oset );
  ALWAYS_ASSERT( size( &our_oset ) == 0 );

  // Test non-empty;
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_oset, i ) );

  clear( &our_oset );
  ALWAYS_ASSERT( size( &our_oset ) == 0 );
  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( !get( &our_oset, i ) );

  // Test reuse.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_oset, i ) );

  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( *get( &our_oset, i ) == i );

  cleanup( &our_oset );
}

static void test_oset_cleanup( void )
{
  oset( int ) our_oset;
  init( &our_oset );

  // Empty.
  cleanup( &our_oset );
  ALWAYS_ASSERT( (void *)our_oset == (void *)&cc_omap_placeholder );

  // Non-empty.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_oset, i ) );
  ALWAYS_ASSERT( size( &our_oset ) == 100 );
  cleanup( &our_oset );
  ALWAYS_ASSERT( size( &our_oset ) == 0 );
  ALWAYS_ASSERT( (void *)our_oset == (void *)&cc_omap_placeholder );

  // Test use.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_oset, i ) );
  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( *get( &our_oset, i ) == i );

  cleanup( &our_oset );
}

static void test_oset_init_clone( void )
{
  oset( int ) src_oset;
  init( &src_oset );

  // Test init_clone placeholder.
  oset( int ) empty_oset;
  UNTIL_SUCCESS( init_clone( &empty_oset, &src_oset ) );
  ALWAYS_ASSERT( (void *)empty_oset == (void *)&cc_omap_placeholder );

  // Test init_clone non-placeholder.
  oset( int ) our_oset;
  for( int i = 0; i < 10; ++i )
    UNTIL_SUCCESS( insert( &src_oset, i ) );
  UNTIL_SUCCESS( init_clone( &our_oset, &src_oset ) );

  // Check.
  ALWAYS_ASSERT( size( &our_oset ) == 10 );
  for( int i = 0; i < 10; ++i )
    ALWAYS_ASSERT( *get( &our_oset, i ) == i );

  cleanup( &src_oset );
  cleanup( &empty_oset );
  cleanup( &our_oset );
}

// This needs to test, in particular, that r_end and end iterator-pointers are stable, especially during the transition
// from placeholder to non-placeholder.
static void test_oset_iteration( void )
{
  oset( int ) our_oset;
  init( &our_oset );

  int *r_end = r_end( &our_oset );
  int *end = end( &our_oset );

  // Empty.

  // Test first and last.
  ALWAYS_ASSERT( first( &our_oset ) == end( &our_oset ) );
  ALWAYS_ASSERT( last( &our_oset ) == r_end( &our_oset ) );

  // Test iteration from r_end and end.
  ALWAYS_ASSERT( next( &our_oset, r_end( &our_oset ) ) == first( &our_oset ) );
  ALWAYS_ASSERT( prev( &our_oset, end( &our_oset ) ) == last( &our_oset ) );

  size_t n_iterations = 0;
  for( int *i = first( &our_oset ); i != end( &our_oset ); i = next( &our_oset, i ) )
    ++n_iterations;
  for( int *i = last( &our_oset ); i != r_end( &our_oset ); i = prev( &our_oset, i ) )
    ++n_iterations;
  for_each( &our_oset, i )
    ++n_iterations;
  r_for_each( &our_oset, i )
    ++n_iterations;
  for_each( &our_oset, i )
    ++n_iterations;
  r_for_each( &our_oset, i )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 0 );

  // Non-empty.

  // Insert keys in random order.

  const int keys[ 100 ] = {
    12, 10, 29, 8, 27, 9, 14, 23, 18, 19, 11, 20, 24, 1, 0, 5, 2, 3, 6, 13, 28, 25, 22, 21, 15, 4, 7, 16, 26, 17
  };

  for( int i = 0; i < 30; ++i )
    UNTIL_SUCCESS( insert( &our_oset, keys[ i ] ) );

  int *last_iteration = NULL;
  for( int *i = first( &our_oset ); i != end( &our_oset ); i = next( &our_oset, i ) )
  {
    ALWAYS_ASSERT( !last_iteration || *i > *last_iteration );
    ++n_iterations;
    last_iteration = i;
  }

  last_iteration = NULL;
  for( int *i = last( &our_oset ); i != r_end( &our_oset ); i = prev( &our_oset, i ) )
  {
    ALWAYS_ASSERT( !last_iteration || *i < *last_iteration );
    ++n_iterations;
    last_iteration = i;
  }

  for_each( &our_oset, i )
    ++n_iterations;
  r_for_each( &our_oset, i )
    ++n_iterations;

  for_each( &our_oset, i )
    ++n_iterations;
  r_for_each( &our_oset, i )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 180 );

  // Test iterator stability.
  ALWAYS_ASSERT( r_end( &our_oset ) == r_end );
  ALWAYS_ASSERT( end( &our_oset ) == end );

  // Test iteration from r_end and end.
  ALWAYS_ASSERT( next( &our_oset, r_end( &our_oset ) ) == first( &our_oset ) );
  ALWAYS_ASSERT( prev( &our_oset, end( &our_oset ) ) == last( &our_oset ) );

  // Iteration over empty, non-placeholder ordered map.

  clear( &our_oset );

  n_iterations = 0;

  for_each( &our_oset, i )
    ++n_iterations;

  r_for_each( &our_oset, i )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 0 );

  cleanup( &our_oset );
}

static void test_oset_iteration_over_range( void )
{
  oset( int ) our_oset;
  init( &our_oset );

  // Empty.

  size_t n_iterations = 0;
  for(
    int *i = first( &our_oset, 25 ), *range_end = first( &our_oset, 75 );
    i != range_end;
    i = next( &our_oset, i )
  )
    ++n_iterations;

  for(
    int *i = last( &our_oset, 74 ), *range_end = last( &our_oset, 24 );
    i != range_end;
    i = prev( &our_oset, i )
  )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 0 );

  // Non-empty.

  // Insert keys in random order.

  const int keys[ 100 ] = {
    44, 13, 39, 68, 33, 88, 87, 58, 73, 28, 95, 56, 93, 8, 50, 92, 78, 80, 97, 53,
    27, 77, 35, 38, 91, 45, 3, 37, 98, 81, 63, 65, 32, 90, 72, 5, 36, 99, 17, 6,
    16, 11, 67, 47, 48, 71, 1, 82, 69, 21, 54, 15, 61, 9, 19, 84, 60, 26, 42, 70,
    64, 18, 34, 23, 75, 52, 89, 83, 86, 10, 94, 24, 57, 59, 41, 20, 25, 12, 85, 96,
    66, 55, 7, 2, 76, 46, 14, 31, 43, 4, 22, 30, 40, 29, 0, 74, 51, 49, 62, 79
  };

  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_oset, keys[ i ] ) );

  // Test ranges that do not include r_end or end.

  for(
    int *i = first( &our_oset, 25 ), *range_end = first( &our_oset, 75 );
    i != range_end;
    i = next( &our_oset, i )
  )
  {
    ALWAYS_ASSERT( *i >= 25 && *i < 75 );
    ++n_iterations;
  }

  for(
    int *i = last( &our_oset, 75 ), *range_end = last( &our_oset, 25 );
    i != range_end;
    i = prev( &our_oset, i )
  )
  {
    ALWAYS_ASSERT( *i > 25 && *i <= 75 );
    ++n_iterations;
  }

  ALWAYS_ASSERT( n_iterations == 100 );

  // Test ranges that overlap r_end or end.

  for(
    int *i = first( &our_oset, -1 ), *range_end = first( &our_oset, 50 );
    i != range_end;
    i = next( &our_oset, i )
  )
  {
    ALWAYS_ASSERT( *i < 50 );
    ++n_iterations;
  }

  for(
    int *i = first( &our_oset, 50 ), *range_end = first( &our_oset, 100 );
    i != range_end;
    i = next( &our_oset, i )
  )
  {
    ALWAYS_ASSERT( *i >= 50 );
    ++n_iterations;
  }

  for(
    int *i = last( &our_oset, 100 ), *range_end = last( &our_oset, 49 );
    i != range_end;
    i = prev( &our_oset, i )
  )
  {
    ALWAYS_ASSERT( *i >= 50 );
    ++n_iterations;
  }

  for(
    int *i = last( &our_oset, 49 ), *range_end = last( &our_oset, -1 );
    i != range_end;
    i = prev( &our_oset, i )
  )
  {
    ALWAYS_ASSERT( *i <= 49 );
    ++n_iterations;
  }

  for(
    int *i = first( &our_oset, -1 ), *range_end = first( &our_oset, 100 );
    i != range_end;
    i = next( &our_oset, i )
  )
    ++n_iterations;

  for(
    int *i = last( &our_oset, 100 ), *range_end = last( &our_oset, -1 );
    i != range_end;
    i = prev( &our_oset, i )
  )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 500 );

  // Test ranges with no keys.

  for(
    int *i = first( &our_oset, 100 ), *range_end = first( &our_oset, 200 );
    i != range_end;
    i = next( &our_oset, i )
  )
    ++n_iterations;

  for(
    int *i = last( &our_oset, -1 ), *range_end = last( &our_oset, -100 );
    i != range_end;
    i = prev( &our_oset, i )
  )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 500 );

  cleanup( &our_oset );
}

static void test_oset_dtors( void )
{
  oset( custom_ty ) our_oset;
  init( &our_oset );

  // Test erase and clear.

  for( int i = 0; i < 100; ++i )
  {
    custom_ty el = { i };
    UNTIL_SUCCESS( insert( &our_oset, el ) );
  }

  for( int i = 0; i < 100; ++i )
  {
    custom_ty el = { i };
    erase( &our_oset, el );
  }

  clear( &our_oset );

  check_dtors_arr();

  // Test replace.

  for( int i = 0; i < 100; ++i )
  {
    custom_ty el = { i };
    UNTIL_SUCCESS( insert( &our_oset, el ) );
  }
  for( int i = 0; i < 100; ++i )
  {
    custom_ty el = { i };
    UNTIL_SUCCESS( insert( &our_oset, el ) );
  }

  check_dtors_arr();
  clear( &our_oset );

  // Test cleanup.

  for( int i = 0; i < 100; ++i )
  {
    custom_ty el = { i };
    UNTIL_SUCCESS( insert( &our_oset, el ) );
  }

  cleanup( &our_oset );
  check_dtors_arr();
}

// Strings are a special case that warrant seperate testing.
static void test_oset_strings( void )
{
  oset( char * ) our_oset;
  init( &our_oset );

  char **el;

  // String literals.
  UNTIL_SUCCESS( ( el = insert( &our_oset, "This" ) ) );
  ALWAYS_ASSERT( strcmp( *el, "This" ) == 0 );
  UNTIL_SUCCESS( ( el = insert( &our_oset, "is" ) ) );
  ALWAYS_ASSERT( strcmp( *el, "is" ) == 0 );
  UNTIL_SUCCESS( ( el = insert( &our_oset, "a" ) ) );
  ALWAYS_ASSERT( strcmp( *el, "a" ) == 0 );
  UNTIL_SUCCESS( ( el = insert( &our_oset, "test" ) ) );
  ALWAYS_ASSERT( strcmp( *el, "test" ) == 0 );

  // Other strings.
  char str_1[] = "of";
  char str_2[] = "osets";
  char str_3[] = "with";
  char str_4[] = "strings";

  UNTIL_SUCCESS( ( el = insert( &our_oset, str_1 ) ) );
  ALWAYS_ASSERT( strcmp( *el, str_1 ) == 0 );
  UNTIL_SUCCESS( ( el = insert( &our_oset, str_2 ) ) );
  ALWAYS_ASSERT( strcmp( *el, str_2 ) == 0 );
  UNTIL_SUCCESS( ( el = insert( &our_oset, str_3 ) ) );
  ALWAYS_ASSERT( strcmp( *el, str_3 ) == 0 );
  UNTIL_SUCCESS( ( el = insert( &our_oset, str_4 ) ) );
  ALWAYS_ASSERT( strcmp( *el, str_4 ) == 0 );

  // Check.
  ALWAYS_ASSERT( size( &our_oset ) == 8 );
  ALWAYS_ASSERT( strcmp( *get( &our_oset, "This" ), "This" ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_oset, "is" ), "is" ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_oset, "a" ), "a" ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_oset, "test" ), "test" ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_oset, "of" ), str_1 ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_oset, "osets" ), str_2 ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_oset, "with" ), str_3 ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_oset, "strings" ), str_4 ) == 0 );

  cleanup( &our_oset );

  // const strings.

  oset( const char * ) our_const_oset;
  init( &our_const_oset );

  const char **const_el;

  // String literals.
  UNTIL_SUCCESS( ( const_el = insert( &our_const_oset, "This" ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, "This" ) == 0 );
  UNTIL_SUCCESS( ( const_el = insert( &our_const_oset, "is" ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, "is" ) == 0 );
  UNTIL_SUCCESS( ( const_el = insert( &our_const_oset, "a" ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, "a" ) == 0 );
  UNTIL_SUCCESS( ( const_el = insert( &our_const_oset, "test" ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, "test" ) == 0 );

  // Other strings.
  const char *const_str_1 = str_1;
  const char *const_str_2 = str_2;
  const char *const_str_3 = str_3;
  const char *const_str_4 = str_4;

  UNTIL_SUCCESS( ( const_el = insert( &our_const_oset, const_str_1 ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, const_str_1 ) == 0 );
  UNTIL_SUCCESS( ( const_el = insert( &our_const_oset, const_str_2 ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, const_str_2 ) == 0 );
  UNTIL_SUCCESS( ( const_el = insert( &our_const_oset, const_str_3 ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, const_str_3 ) == 0 );
  UNTIL_SUCCESS( ( const_el = insert( &our_const_oset, const_str_4 ) ) );
  ALWAYS_ASSERT( strcmp( *const_el, const_str_4 ) == 0 );

  // Check.
  ALWAYS_ASSERT( size( &our_const_oset ) == 8 );
  ALWAYS_ASSERT( strcmp( *get( &our_const_oset, "This" ), "This" ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_const_oset, "is" ), "is" ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_const_oset, "a" ), "a" ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_const_oset, "test" ), "test" ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_const_oset, str_1 ), str_1 ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_const_oset, str_2 ), str_2 ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_const_oset, str_3 ), str_3 ) == 0 );
  ALWAYS_ASSERT( strcmp( *get( &our_const_oset, str_4 ), str_4 ) == 0 );

  cleanup( &our_const_oset );
}

#define TEST_OSET_DEFAULT_INTEGER_TYPE( ty )    \
{                                               \
  oset( ty ) our_oset;                          \
  init( &our_oset );                            \
                                                \
  for( ty i = 0; i < 100; ++i )                 \
    UNTIL_SUCCESS( insert( &our_oset, i ) );    \
                                                \
  for( ty i = 0; i < 100; ++i )                 \
    ALWAYS_ASSERT( *get( &our_oset, i ) == i ); \
                                                \
  cleanup( &our_oset );                         \
}                                               \

static void test_oset_default_integer_types( void )
{
  TEST_OSET_DEFAULT_INTEGER_TYPE( char );
  TEST_OSET_DEFAULT_INTEGER_TYPE( unsigned char );
  TEST_OSET_DEFAULT_INTEGER_TYPE( signed char );
  TEST_OSET_DEFAULT_INTEGER_TYPE( unsigned short );
  TEST_OSET_DEFAULT_INTEGER_TYPE( short );
  TEST_OSET_DEFAULT_INTEGER_TYPE( unsigned int );
  TEST_OSET_DEFAULT_INTEGER_TYPE( int );
  TEST_OSET_DEFAULT_INTEGER_TYPE( unsigned long );
  TEST_OSET_DEFAULT_INTEGER_TYPE( long );
  TEST_OSET_DEFAULT_INTEGER_TYPE( unsigned long long );
  TEST_OSET_DEFAULT_INTEGER_TYPE( long );
  TEST_OSET_DEFAULT_INTEGER_TYPE( size_t );
}

#endif

// Vector tests.
#ifdef TEST_STR

static bool compare_strings8( const void *str_1, const void *str_2 )
{
  return strcmp( (const char *)str_1, (const char *)str_2 ) == 0;
}

static bool compare_strings16( const char16_t *str_1, const char16_t *str_2 )
{
  while( true )
  {
    if( *str_1 != *str_2 )
      return false;

    if( !*str_1 )
      return true;

    ++str_1;
    ++str_2;
  }
}

static bool compare_strings32( const char32_t *str_1, const char32_t *str_2 )
{
  while( true )
  {
    if( *str_1 != *str_2 )
      return false;

    if( !*str_1 )
      return true;

    ++str_1;
    ++str_2;
  }
}

static void test_str_reserve( void )
{
  str( char ) our_str8;
  init( &our_str8 );
  str( char16_t ) our_str16;
  init( &our_str16 );
  str( char32_t ) our_str32;
  init( &our_str32 );

  // Test null termination of placeholders.
  ALWAYS_ASSERT( compare_strings8( first( &our_str8 ), "" ) );
  ALWAYS_ASSERT( compare_strings16( first( &our_str16 ), u"" ) );
  ALWAYS_ASSERT( compare_strings32( first( &our_str32 ), U"" ) );

  // Reserve zero with placeholder.

  UNTIL_SUCCESS( reserve( &our_str8, 0 ) );
  ALWAYS_ASSERT( (void *)our_str8 == (void *)&cc_str_placeholder_char8 );

  UNTIL_SUCCESS( reserve( &our_str16, 0 ) );
  ALWAYS_ASSERT( (void *)our_str16 == (void *)&cc_str_placeholder_char16 );

  UNTIL_SUCCESS( reserve( &our_str32, 0 ) );
  ALWAYS_ASSERT( (void *)our_str32 == (void *)&cc_str_placeholder_char32 );

  // Reserve up from placeholder.

  UNTIL_SUCCESS( reserve( &our_str8, 30 ) );
  ALWAYS_ASSERT( cap( &our_str8 ) >= 30 );

  UNTIL_SUCCESS( reserve( &our_str16, 30 ) );
  ALWAYS_ASSERT( cap( &our_str16 ) >= 30 );

  UNTIL_SUCCESS( reserve( &our_str32, 30 ) );
  ALWAYS_ASSERT( cap( &our_str32 ) >= 30 );

  // Reserve same capacity.

  size_t cap;

  cap = cap( &our_str8 );
  UNTIL_SUCCESS( reserve( &our_str8, 30 ) );
  ALWAYS_ASSERT( cap( &our_str8 ) == cap );

  cap = cap( &our_str16 );
  UNTIL_SUCCESS( reserve( &our_str16, 30 ) );
  ALWAYS_ASSERT( cap( &our_str16 ) == cap );

  cap = cap( &our_str32 );
  UNTIL_SUCCESS( reserve( &our_str32, 30 ) );
  ALWAYS_ASSERT( cap( &our_str32 ) == cap );

  // Reserve up from non-placeholder.

  UNTIL_SUCCESS( reserve( &our_str8, 60 ) );
  ALWAYS_ASSERT( cap( &our_str8 ) >= 60 );

  UNTIL_SUCCESS( reserve( &our_str16, 60 ) );
  ALWAYS_ASSERT( cap( &our_str16 ) >= 60 );

  UNTIL_SUCCESS( reserve( &our_str32, 60 ) );
  ALWAYS_ASSERT( cap( &our_str32 ) >= 60 );

  // Reserve lower capacity.

  cap = cap( &our_str8 );
  UNTIL_SUCCESS( reserve( &our_str8, 30 ) );
  ALWAYS_ASSERT( cap( &our_str8 ) == cap );

  cap = cap( &our_str16 );
  UNTIL_SUCCESS( reserve( &our_str16, 30 ) );
  ALWAYS_ASSERT( cap( &our_str16 ) == cap );

  cap = cap( &our_str32 );
  UNTIL_SUCCESS( reserve( &our_str32, 30 ) );
  ALWAYS_ASSERT( cap( &our_str32 ) == cap );

  // Test validity through use.

  UNTIL_SUCCESS( push_fmt( &our_str8, "Validity test." ) );
  ALWAYS_ASSERT( compare_strings8( first( &our_str8 ), "Validity test." ) );

  UNTIL_SUCCESS( push_fmt( &our_str16, u"Validity test." ) );
  ALWAYS_ASSERT( compare_strings16( first( &our_str16 ), u"Validity test." ) );

  UNTIL_SUCCESS( push_fmt( &our_str32, U"Validity test." ) );
  ALWAYS_ASSERT( compare_strings32( first( &our_str32 ), U"Validity test." ) );

  cleanup( &our_str8 );
  cleanup( &our_str16 );
  cleanup( &our_str32 );
}

static void test_str_resize( void )
{
  str( char ) our_str8;
  init( &our_str8 );
  str( char16_t ) our_str16;
  init( &our_str16 );
  str( char32_t ) our_str32;
  init( &our_str32 );

  // Resize placeholder vec with zero.

  UNTIL_SUCCESS( resize( &our_str8, 0, '-' ) );
  ALWAYS_ASSERT( cap( &our_str8 ) == 0 );
  ALWAYS_ASSERT( size( &our_str8 ) == 0 );

  UNTIL_SUCCESS( resize( &our_str16, 0, u'-' ) );
  ALWAYS_ASSERT( cap( &our_str16 ) == 0 );
  ALWAYS_ASSERT( size( &our_str16 ) == 0 );

  UNTIL_SUCCESS( resize( &our_str32, 0, U'-' ) );
  ALWAYS_ASSERT( cap( &our_str32 ) == 0 );
  ALWAYS_ASSERT( size( &our_str32 ) == 0 );

  // Resize up from placeholder.

  UNTIL_SUCCESS( resize( &our_str8, 20, '-' ) );
  ALWAYS_ASSERT( cap( &our_str8 ) >= 20 );
  ALWAYS_ASSERT( size( &our_str8 ) == 20 );
  ALWAYS_ASSERT( compare_strings8( first( &our_str8 ), "--------------------" ) );

  UNTIL_SUCCESS( resize( &our_str16, 20, u'-' ) );
  ALWAYS_ASSERT( cap( &our_str16 ) >= 20 );
  ALWAYS_ASSERT( size( &our_str16 ) == 20 );
  ALWAYS_ASSERT( compare_strings16( first( &our_str16 ), u"--------------------" ) );

  UNTIL_SUCCESS( resize( &our_str32, 20, U'-' ) );
  ALWAYS_ASSERT( cap( &our_str32 ) >= 20 );
  ALWAYS_ASSERT( size( &our_str32 ) == 20 );
  ALWAYS_ASSERT( compare_strings32( first( &our_str32 ), U"--------------------" ) );

  // Resize up from non-placeholder.

  UNTIL_SUCCESS( resize( &our_str8, 40, '*' ) );
  ALWAYS_ASSERT( cap( &our_str8 ) >= 40 );
  ALWAYS_ASSERT( size( &our_str8 ) == 40 );
  ALWAYS_ASSERT( compare_strings8( first( &our_str8 ), "--------------------********************" ) );

  UNTIL_SUCCESS( resize( &our_str16, 40, u'*' ) );
  ALWAYS_ASSERT( cap( &our_str16 ) >= 40 );
  ALWAYS_ASSERT( size( &our_str16 ) == 40 );
  ALWAYS_ASSERT( compare_strings16( first( &our_str16 ), u"--------------------********************" ) );

  UNTIL_SUCCESS( resize( &our_str32, 40, U'*' ) );
  ALWAYS_ASSERT( cap( &our_str32 ) >= 40 );
  ALWAYS_ASSERT( size( &our_str32 ) == 40 );
  ALWAYS_ASSERT( compare_strings32( first( &our_str32 ), U"--------------------********************" ) );

  // Resize down.

  UNTIL_SUCCESS( resize( &our_str8, 20, '^' ) );
  ALWAYS_ASSERT( cap( &our_str8 ) >= 40 );
  ALWAYS_ASSERT( size( &our_str8 ) == 20 );
  ALWAYS_ASSERT( compare_strings8( first( &our_str8 ), "--------------------" ) );

  UNTIL_SUCCESS( resize( &our_str16, 20, u'^' ) );
  ALWAYS_ASSERT( cap( &our_str16 ) >= 40 );
  ALWAYS_ASSERT( size( &our_str16 ) == 20 );
  ALWAYS_ASSERT( compare_strings16( first( &our_str16 ), u"--------------------" ) );

  UNTIL_SUCCESS( resize( &our_str32, 20, U'^' ) );
  ALWAYS_ASSERT( cap( &our_str32 ) >= 40 );
  ALWAYS_ASSERT( size( &our_str32 ) == 20 );
  ALWAYS_ASSERT( compare_strings32( first( &our_str32 ), U"--------------------" ) );

  // Test validity through use.

  for( size_t i = 0; i < 20; ++i )
  {
    *get( &our_str8, i ) = (char)( 'a' + i );
    *get( &our_str16, i ) = (char16_t)( 'a' + i );
    *get( &our_str32, i ) = (char32_t)( 'a' + i );
  }

  ALWAYS_ASSERT( compare_strings8( first( &our_str8 ), "abcdefghijklmnopqrst" ) );
  ALWAYS_ASSERT( compare_strings16( first( &our_str16 ), u"abcdefghijklmnopqrst" ) );
  ALWAYS_ASSERT( compare_strings32( first( &our_str32 ), U"abcdefghijklmnopqrst" ) );

  cleanup( &our_str8 );
  cleanup( &our_str16 );
  cleanup( &our_str32 );
}

static void test_str_shrink( void )
{
  str( char ) our_str8;
  init( &our_str8 );
  str( char16_t ) our_str16;
  init( &our_str16 );
  str( char32_t ) our_str32;
  init( &our_str32 );

  // Test placeholder.

  UNTIL_SUCCESS( shrink( &our_str8 ) );
  ALWAYS_ASSERT( size( &our_str8 ) == 0 );
  ALWAYS_ASSERT( cap( &our_str8 ) == 0 );

  UNTIL_SUCCESS( shrink( &our_str16 ) );
  ALWAYS_ASSERT( size( &our_str16 ) == 0 );
  ALWAYS_ASSERT( cap( &our_str16 ) == 0 );

  UNTIL_SUCCESS( shrink( &our_str32 ) );
  ALWAYS_ASSERT( size( &our_str32 ) == 0 );
  ALWAYS_ASSERT( cap( &our_str32 ) == 0 );

  // Test restoration of placeholder.

  UNTIL_SUCCESS( reserve( &our_str8, 30 ) );
  UNTIL_SUCCESS( shrink( &our_str8 ) );
  ALWAYS_ASSERT( size( &our_str8 ) == 0 );
  ALWAYS_ASSERT( cap( &our_str8 ) == 0 );
  ALWAYS_ASSERT( (void *)our_str8 == (void *)&cc_str_placeholder_char8 );

  UNTIL_SUCCESS( reserve( &our_str16, 30 ) );
  UNTIL_SUCCESS( shrink( &our_str16 ) );
  ALWAYS_ASSERT( size( &our_str16 ) == 0 );
  ALWAYS_ASSERT( cap( &our_str16 ) == 0 );
  ALWAYS_ASSERT( (void *)our_str16 == (void *)&cc_str_placeholder_char16 );

  UNTIL_SUCCESS( reserve( &our_str32, 30 ) );
  UNTIL_SUCCESS( shrink( &our_str32 ) );
  ALWAYS_ASSERT( size( &our_str32 ) == 0 );
  ALWAYS_ASSERT( cap( &our_str32 ) == 0 );
  ALWAYS_ASSERT( (void *)our_str32 == (void *)&cc_str_placeholder_char32 );

  // Test shrink same size.

  UNTIL_SUCCESS( resize( &our_str8, 30, '-' ) );
  ALWAYS_ASSERT( size( &our_str8 ) == 30 );
  ALWAYS_ASSERT( cap( &our_str8 ) == 30 );
  str( char ) same8 = our_str8;
  UNTIL_SUCCESS( shrink( &our_str8 ) );
  ALWAYS_ASSERT( size( &our_str8 ) == 30 );
  ALWAYS_ASSERT( cap( &our_str8 ) == 30 );
  ALWAYS_ASSERT( our_str8 == same8 );

  UNTIL_SUCCESS( resize( &our_str16, 30, u'-' ) );
  ALWAYS_ASSERT( size( &our_str16 ) == 30 );
  ALWAYS_ASSERT( cap( &our_str16 ) == 30 );
  str( char16_t ) same16 = our_str16;
  UNTIL_SUCCESS( shrink( &our_str16 ) );
  ALWAYS_ASSERT( size( &our_str16 ) == 30 );
  ALWAYS_ASSERT( cap( &our_str16 ) == 30 );
  ALWAYS_ASSERT( our_str16 == same16 );

  UNTIL_SUCCESS( resize( &our_str32, 30, U'-' ) );
  ALWAYS_ASSERT( size( &our_str32 ) == 30 );
  ALWAYS_ASSERT( cap( &our_str32 ) == 30 );
  str( char32_t ) same32 = our_str32;
  UNTIL_SUCCESS( shrink( &our_str32 ) );
  ALWAYS_ASSERT( size( &our_str32 ) == 30 );
  ALWAYS_ASSERT( cap( &our_str32 ) == 30 );
  ALWAYS_ASSERT( our_str32 == same32 );

  // Test shrink down.

  UNTIL_SUCCESS( reserve( &our_str8, 60 ) );
  ALWAYS_ASSERT( size( &our_str8 ) == 30 );
  ALWAYS_ASSERT( cap( &our_str8 ) >= 30 );
  UNTIL_SUCCESS( shrink( &our_str8 ) );
  ALWAYS_ASSERT( size( &our_str8 ) == 30 );
  ALWAYS_ASSERT( cap( &our_str8 ) == 30 );

  UNTIL_SUCCESS( reserve( &our_str16, 60 ) );
  ALWAYS_ASSERT( size( &our_str16 ) == 30 );
  ALWAYS_ASSERT( cap( &our_str16 ) >= 30 );
  UNTIL_SUCCESS( shrink( &our_str16 ) );
  ALWAYS_ASSERT( size( &our_str16 ) == 30 );
  ALWAYS_ASSERT( cap( &our_str16 ) == 30 );

  UNTIL_SUCCESS( reserve( &our_str32, 60 ) );
  ALWAYS_ASSERT( size( &our_str32 ) == 30 );
  ALWAYS_ASSERT( cap( &our_str32 ) >= 30 );
  UNTIL_SUCCESS( shrink( &our_str32 ) );
  ALWAYS_ASSERT( size( &our_str32 ) == 30 );
  ALWAYS_ASSERT( cap( &our_str32 ) == 30 );

  // Test validity through use.

  for( size_t i = 0; i < 30; ++i )
  {
    *get( &our_str8, i ) = (char)( 'a' + i );
    *get( &our_str16, i ) = (char16_t)( 'a' + i );
    *get( &our_str32, i ) = (char32_t)( 'a' + i );
  }

  ALWAYS_ASSERT( compare_strings8( first( &our_str8 ), "abcdefghijklmnopqrstuvwxyz{|}~" ) );
  ALWAYS_ASSERT( compare_strings16( first( &our_str16 ), u"abcdefghijklmnopqrstuvwxyz{|}~" ) );
  ALWAYS_ASSERT( compare_strings32( first( &our_str32 ), U"abcdefghijklmnopqrstuvwxyz{|}~" ) );

  cleanup( &our_str8 );
  cleanup( &our_str16 );
  cleanup( &our_str32 );
}

static void test_str_insert( void )
{
  str( char ) our_str8;
  init( &our_str8 );
  str( char16_t ) our_str16;
  init( &our_str16 );
  str( char32_t ) our_str32;
  init( &our_str32 );

  // End.
  for( size_t i = 0; i < 30; ++i )
  {
    char *el8;
    UNTIL_SUCCESS( el8 = insert( &our_str8, size( &our_str8 ), (char)( 'a' + i ) ) );
    ALWAYS_ASSERT( *el8 == (char)( 'a' + i ) );

    char16_t *el16;
    UNTIL_SUCCESS( el16 = insert( &our_str16, size( &our_str16 ), (char16_t)( 'a' + i ) ) );
    ALWAYS_ASSERT( *el16 == (char16_t)( 'a' + i ) );

    char32_t *el32;
    UNTIL_SUCCESS( el32 = insert( &our_str32, size( &our_str32 ), (char32_t)( 'a' + i ) ) );
    ALWAYS_ASSERT( *el32 == (char32_t)( 'a' + i ) );
  }

  // Beginning.
  for( size_t i = 0; i < 30; ++i )
  {
    char *el8;
    UNTIL_SUCCESS( el8 = insert( &our_str8, 0, (char)( 'a' + i ) ) );
    ALWAYS_ASSERT( *el8 == (char)( 'a' + i ) );

    char16_t *el16;
    UNTIL_SUCCESS( el16 = insert( &our_str16, 0, (char16_t)( 'a' + i ) ) );
    ALWAYS_ASSERT( *el16 == (char16_t)( 'a' + i ) );

    char32_t *el32;
    UNTIL_SUCCESS( el32 = insert( &our_str32, 0, (char32_t)( 'a' + i ) ) );
    ALWAYS_ASSERT( *el32 == (char32_t)( 'a' + i ) );
  }

  // Middle.
  for( size_t i = 0; i < 30; ++i )
  {
    char *el8;
    UNTIL_SUCCESS( el8 = insert( &our_str8, 30, (char)( 'a' + i ) ) );
    ALWAYS_ASSERT( *el8 == (char)( 'a' + i ) );

    char16_t *el16;
    UNTIL_SUCCESS( el16 = insert( &our_str16, 30, (char16_t)( 'a' + i ) ) );
    ALWAYS_ASSERT( *el16 == (char16_t)( 'a' + i ) );

    char32_t *el32;
    UNTIL_SUCCESS( el32 = insert( &our_str32, 30, (char32_t)( 'a' + i ) ) );
    ALWAYS_ASSERT( *el32 == (char32_t)( 'a' + i ) );
  }

  // Check.

  ALWAYS_ASSERT(
    compare_strings8(
      first( &our_str8 ),
      "~}|{zyxwvutsrqponmlkjihgfedcba~}|{zyxwvutsrqponmlkjihgfedcbaabcdefghijklmnopqrstuvwxyz{|}~"
    )
  );

  ALWAYS_ASSERT(
    compare_strings16(
      first( &our_str16 ),
      u"~}|{zyxwvutsrqponmlkjihgfedcba~}|{zyxwvutsrqponmlkjihgfedcbaabcdefghijklmnopqrstuvwxyz{|}~"
    )
  );

  ALWAYS_ASSERT(
    compare_strings32(
      first( &our_str32 ),
      U"~}|{zyxwvutsrqponmlkjihgfedcba~}|{zyxwvutsrqponmlkjihgfedcbaabcdefghijklmnopqrstuvwxyz{|}~"
    )
  );

  cleanup( &our_str8 );
  cleanup( &our_str16 );
  cleanup( &our_str32 );
}

static void test_str_insert_fmt( void )
{
  str( char ) our_str8;
  init( &our_str8 );
  str( char16_t ) our_str16;
  init( &our_str16 );
  str( char32_t ) our_str32;
  init( &our_str32 );

  // End.
  for( size_t i = 0; i < 30; ++i )
  {
    char el8[ 2 ] = { (char)( 'a' + i ), '\0' };
    char *el_result8;
    UNTIL_SUCCESS( el_result8 = insert_fmt( &our_str8, size( &our_str8 ), el8 ) );
    ALWAYS_ASSERT( *el_result8 == *el8 );

    char16_t el16[ 2 ] = { (char16_t)( 'a' + i ), u'\0' };
    char16_t *el_result16;
    UNTIL_SUCCESS( el_result16 = insert_fmt( &our_str16, size( &our_str16 ), el16 ) );
    ALWAYS_ASSERT( *el_result16 == *el16 );

    char32_t el32[ 2 ] = { (char32_t)( 'a' + i ), U'\0' };
    char32_t *el_result32;
    UNTIL_SUCCESS( el_result32 = insert_fmt( &our_str32, size( &our_str32 ), el32 ) );
    ALWAYS_ASSERT( *el_result32 == *el32 );
  }

  // Beginning.
  for( size_t i = 0; i < 30; ++i )
  {
    char el8[ 2 ] = { (char)( 'a' + i ), '\0' };
    char *el_result8;
    UNTIL_SUCCESS( el_result8 = insert_fmt( &our_str8, 0, el8 ) );
    ALWAYS_ASSERT( *el_result8 == *el8 );

    char16_t el16[ 2 ] = { (char16_t)( 'a' + i ), u'\0' };
    char16_t *el_result16;
    UNTIL_SUCCESS( el_result16 = insert_fmt( &our_str16, 0, el16 ) );
    ALWAYS_ASSERT( *el_result16 == *el16 );

    char32_t el32[ 2 ] = { (char32_t)( 'a' + i ), U'\0' };
    char32_t *el_result32;
    UNTIL_SUCCESS( el_result32 = insert_fmt( &our_str32, 0, el32 ) );
    ALWAYS_ASSERT( *el_result32 == *el32 );
  }

  // Middle.
  for( size_t i = 0; i < 30; ++i )
  {
    char el8[ 2 ] = { (char)( 'a' + i ), '\0' };
    char *el_result8;
    UNTIL_SUCCESS( el_result8 = insert_fmt( &our_str8, 30, el8 ) );
    ALWAYS_ASSERT( *el_result8 == *el8 );

    char16_t el16[ 2 ] = { (char16_t)( 'a' + i ), u'\0' };
    char16_t *el_result16;
    UNTIL_SUCCESS( el_result16 = insert_fmt( &our_str16, 30, el16 ) );
    ALWAYS_ASSERT( *el_result16 == *el16 );

    char32_t el32[ 2 ] = { (char32_t)( 'a' + i ), U'\0' };
    char32_t *el_result32;
    UNTIL_SUCCESS( el_result32 = insert_fmt( &our_str32, 30, el32 ) );
    ALWAYS_ASSERT( *el_result32 == *el32 );
  }

  // Integers.

  UNTIL_SUCCESS(
    insert_fmt(
      &our_str8, 30, (char)1, (unsigned char)2, (signed char)3, (unsigned short)4, (short)5, (unsigned int)6, (int)7,
      (unsigned long)8, (long)9, (unsigned long long)10, (long long)11, integer_dec( 2 ), 1, 2, 3, integer_hex( 3 ), 10,
      20, 30, integer_oct( 4 ), 10, 20, 30
    )
  );

  UNTIL_SUCCESS(
    insert_fmt(
      &our_str16, 30, (char)1, (unsigned char)2, (signed char)3, (unsigned short)4, (short)5, (unsigned int)6, (int)7,
      (unsigned long)8, (long)9, (unsigned long long)10, (long long)11, integer_dec( 2 ), 1, 2, 3, integer_hex( 3 ), 10,
      20, 30, integer_oct( 4 ), 10, 20, 30
    )
  );

  UNTIL_SUCCESS(
    insert_fmt(
      &our_str32, 30, (char)1, (unsigned char)2, (signed char)3, (unsigned short)4, (short)5, (unsigned int)6, (int)7,
      (unsigned long)8, (long)9, (unsigned long long)10, (long long)11, integer_dec( 2 ), 1, 2, 3, integer_hex( 3 ), 10,
      20, 30, integer_oct( 4 ), 10, 20, 30
    )
  );

  // Floating points.

  UNTIL_SUCCESS(
    insert_fmt(
      &our_str8, 30, 1.0f, 2.0, float_dec( 3 ), 3.0, 4.0, 5.0, float_hex( 4 ), 6.0, 7.0, float_sci( 5 ), 8.0, 9.0,
      float_shortest( 2 ), 10.0, 12345.0
    )
  );

  UNTIL_SUCCESS(
    insert_fmt(
      &our_str16, 30, 1.0f, 2.0, float_dec( 3 ), 3.0, 4.0, 5.0, float_hex( 4 ), 6.0, 7.0, float_sci( 5 ), 8.0, 9.0,
      float_shortest( 2 ), 10.0, 12345.0
    )
  );

  UNTIL_SUCCESS(
    insert_fmt(
      &our_str32, 30, 1.0f, 2.0, float_dec( 3 ), 3.0, 4.0, 5.0, float_hex( 4 ), 6.0, 7.0, float_sci( 5 ), 8.0, 9.0,
      float_shortest( 2 ), 10.0, 12345.0
    )
  );

  // C strings.

  UNTIL_SUCCESS( insert_fmt( &our_str8, 30, "Test insert C string: föóföóföó." ) );
  UNTIL_SUCCESS( insert_fmt( &our_str16, 30, u"Test insert C string: föóföóföó." ) );
  UNTIL_SUCCESS( insert_fmt( &our_str32, 30, U"Test insert C string: föóföóföó." ) );

  // CC strings.

  str( char ) our_other_str8;
  init( &our_other_str8 );
  UNTIL_SUCCESS( push_fmt( &our_other_str8, "Test insert CC string: föóföóföó." ) );

  str( char16_t ) our_other_str16;
  init( &our_other_str16 );
  UNTIL_SUCCESS( push_fmt( &our_other_str16, u"Test insert CC string: föóföóföó." ) );

  str( char32_t ) our_other_str32;
  init( &our_other_str32 );
  UNTIL_SUCCESS( push_fmt( &our_other_str32, U"Test insert CC string: föóföóföó." ) );

  UNTIL_SUCCESS( insert_fmt( &our_str8, 30, our_other_str8 ) );
  UNTIL_SUCCESS( insert_fmt( &our_str16, 30, our_other_str16 ) );
  UNTIL_SUCCESS( insert_fmt( &our_str32, 30, our_other_str32 ) );

  // Maximum number of arguments.

  UNTIL_SUCCESS(
    insert_fmt(
      &our_str8, 30, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
      28, 29, 30, 31, 32
    )
  );

  UNTIL_SUCCESS(
    insert_fmt(
      &our_str16, 30, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
      28, 29, 30, 31, 32
    )
  );

  UNTIL_SUCCESS(
    insert_fmt(
      &our_str32, 30, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
      28, 29, 30, 31, 32
    )
  );

  cleanup( &our_other_str8 );
  cleanup( &our_other_str16 );
  cleanup( &our_other_str32 );

  // Check.

  ALWAYS_ASSERT(
    compare_strings8(
      first( &our_str8 ),
      "~}|{zyxwvutsrqponmlkjihgfedcba"
      "1234567891011121314151617181920212223242526272829303132"
      "Test insert CC string: föóföóföó."
      "Test insert C string: föóföóföó."
      "1.002.003.0004.0005.0000x1.8000p+20x1.c000p+28.00000e+009.00000e+00101.2e+04"
      "123456789101101020300a01401e001200240036"
      "~}|{zyxwvutsrqponmlkjihgfedcbaabcdefghijklmnopqrstuvwxyz{|}~"
    )
  );

  ALWAYS_ASSERT(
    compare_strings16(
      first( &our_str16 ),
      u"~}|{zyxwvutsrqponmlkjihgfedcba"
      u"1234567891011121314151617181920212223242526272829303132"
      u"Test insert CC string: föóföóföó."
      u"Test insert C string: föóföóföó."
      u"1.002.003.0004.0005.0000x1.8000p+20x1.c000p+28.00000e+009.00000e+00101.2e+04"
      u"123456789101101020300a01401e001200240036"
      u"~}|{zyxwvutsrqponmlkjihgfedcbaabcdefghijklmnopqrstuvwxyz{|}~"
    )
  );

  ALWAYS_ASSERT(
    compare_strings32(
      first( &our_str32 ),
      U"~}|{zyxwvutsrqponmlkjihgfedcba"
      U"1234567891011121314151617181920212223242526272829303132"
      U"Test insert CC string: föóföóföó."
      U"Test insert C string: föóföóföó."
      U"1.002.003.0004.0005.0000x1.8000p+20x1.c000p+28.00000e+009.00000e+00101.2e+04"
      U"123456789101101020300a01401e001200240036"
      U"~}|{zyxwvutsrqponmlkjihgfedcbaabcdefghijklmnopqrstuvwxyz{|}~"
    )
  );

  cleanup( &our_str8 );
  cleanup( &our_str16 );
  cleanup( &our_str32 );
}

static void test_str_insert_n( void )
{
  str( char ) our_str8;
  init( &our_str8 );
  str( char16_t ) our_str16;
  init( &our_str16 );
  str( char32_t ) our_str32;
  init( &our_str32 );

  // Zero-size insertion.
  ALWAYS_ASSERT( !insert_n( &our_str8, size( &our_str8 ), NULL, 0 ) );
  ALWAYS_ASSERT( !insert_n( &our_str16, size( &our_str16 ), NULL, 0 ) );
  ALWAYS_ASSERT( !insert_n( &our_str32, size( &our_str32 ), NULL, 0 ) );

  // Normal insert.

  char *el8;
  UNTIL_SUCCESS( el8 = insert_n( &our_str8, size( &our_str8 ), "uvwxyz{|}~", 10 ) ); // End.
  ALWAYS_ASSERT( *el8 == 'u' );
  UNTIL_SUCCESS( el8 = insert_n( &our_str8, 0, "abcdefghij", 10 ) ); // Beginning.
  ALWAYS_ASSERT( *el8 == 'a' );
  UNTIL_SUCCESS( el8 = insert_n( &our_str8, 10, "klmnopqrst", 10 ) ); // Middle.
  ALWAYS_ASSERT( *el8 == 'k' );

  char16_t *el16;
  UNTIL_SUCCESS( el16 = insert_n( &our_str16, size( &our_str16 ), u"uvwxyz{|}~", 10 ) ); // End.
  ALWAYS_ASSERT( *el16 == u'u' );
  UNTIL_SUCCESS( el16 = insert_n( &our_str16, 0, u"abcdefghij", 10 ) ); // Beginning.
  ALWAYS_ASSERT( *el16 == u'a' );
  UNTIL_SUCCESS( el16 = insert_n( &our_str16, 10, u"klmnopqrst", 10 ) ); // Middle.
  ALWAYS_ASSERT( *el16 == u'k' );

  char32_t *el32;
  UNTIL_SUCCESS( el32 = insert_n( &our_str32, size( &our_str32 ), U"uvwxyz{|}~", 10 ) ); // End.
  ALWAYS_ASSERT( *el32 == U'u' );
  UNTIL_SUCCESS( el32 = insert_n( &our_str32, 0, U"abcdefghij", 10 ) ); // Beginning.
  ALWAYS_ASSERT( *el32 == U'a' );
  UNTIL_SUCCESS( el32 = insert_n( &our_str32, 10, U"klmnopqrst", 10 ) ); // Middle.
  ALWAYS_ASSERT( *el32 == U'k' );

  // Check.

  ALWAYS_ASSERT( compare_strings8( first( &our_str8 ), "abcdefghijklmnopqrstuvwxyz{|}~" ) );
  ALWAYS_ASSERT( compare_strings16( first( &our_str16 ), u"abcdefghijklmnopqrstuvwxyz{|}~" ) );
  ALWAYS_ASSERT( compare_strings32( first( &our_str32 ), U"abcdefghijklmnopqrstuvwxyz{|}~" ) );

  cleanup( &our_str8 );
  cleanup( &our_str16 );
  cleanup( &our_str32 );
}

static void test_str_push( void )
{
  str( char ) our_str8;
  init( &our_str8 );
  str( char16_t ) our_str16;
  init( &our_str16 );
  str( char32_t ) our_str32;
  init( &our_str32 );

  for( size_t i = 0; i < 30; ++i )
  {
    char *el8;
    UNTIL_SUCCESS( el8 = push( &our_str8, (char)( 'a' + i ) ) );
    ALWAYS_ASSERT( *el8 == (char)( 'a' + i ) );

    char16_t *el16;
    UNTIL_SUCCESS( el16 = push( &our_str16, (char16_t)( 'a' + i ) ) );
    ALWAYS_ASSERT( *el16 == (char16_t)( 'a' + i ) );

    char32_t *el32;
    UNTIL_SUCCESS( el32 = push( &our_str32, (char32_t)( 'a' + i ) ) );
    ALWAYS_ASSERT( *el32 == (char32_t)( 'a' + i ) );
  }

  ALWAYS_ASSERT( compare_strings8( first( &our_str8 ), "abcdefghijklmnopqrstuvwxyz{|}~" ) );
  ALWAYS_ASSERT( compare_strings16( first( &our_str16 ), u"abcdefghijklmnopqrstuvwxyz{|}~" ) );
  ALWAYS_ASSERT( compare_strings32( first( &our_str32 ), U"abcdefghijklmnopqrstuvwxyz{|}~" ) );

  cleanup( &our_str8 );
  cleanup( &our_str16 );
  cleanup( &our_str32 );
}

static void test_str_push_fmt( void )
{
  str( char ) our_str8;
  init( &our_str8 );
  str( char16_t ) our_str16;
  init( &our_str16 );
  str( char32_t ) our_str32;
  init( &our_str32 );

  // Push a variety of strings, integers, and floating point values.

  for( size_t i = 0; i < 30; ++i )
  {
    if( i % 2 == 0 )
    {
      char el[ 2 ] = { (char)( 'a' + i ), '\0' };
      char *el_result;
      UNTIL_SUCCESS( el_result = push_fmt( &our_str8, el ) );
      ALWAYS_ASSERT( ( *el_result == *el ) );
    }
    else if( i % 3 == 0 )
      UNTIL_SUCCESS( push_fmt( &our_str8, i ) );
    else
      UNTIL_SUCCESS( push_fmt( &our_str8, (double)i ) );
  }

  for( size_t i = 0; i < 30; ++i )
  {
    if( i % 2 == 0 )
    {
      char16_t el[ 2 ] = { (char16_t)( 'a' + i ), u'\0' };
      char16_t *el_result;
      UNTIL_SUCCESS( el_result = push_fmt( &our_str16, el ) );
      ALWAYS_ASSERT( ( *el_result == *el ) );
    }
    else if( i % 3 == 0 )
      UNTIL_SUCCESS( push_fmt( &our_str16, i ) );
    else
      UNTIL_SUCCESS( push_fmt( &our_str16, (double)i ) );
  }

  for( size_t i = 0; i < 30; ++i )
  {
    if( i % 2 == 0 )
    {
      char32_t el[ 2 ] = { (char32_t)( 'a' + i ), U'\0' };
      char32_t *el_result;
      UNTIL_SUCCESS( el_result = push_fmt( &our_str32, el ) );
      ALWAYS_ASSERT( ( *el_result == *el ) );
    }
    else if( i % 3 == 0 )
      UNTIL_SUCCESS( push_fmt( &our_str32, i ) );
    else
      UNTIL_SUCCESS( push_fmt( &our_str32, (double)i ) );
  }

  // Maximum number of arguments.

  UNTIL_SUCCESS(
    push_fmt(
      &our_str8, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
      29, 30, 31, 32
    )
  );

  UNTIL_SUCCESS(
    push_fmt(
      &our_str16, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
      29, 30, 31, 32
    )
  );

  UNTIL_SUCCESS(
    push_fmt(
      &our_str32, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
      29, 30, 31, 32
    )
  );

  // Check.

  ALWAYS_ASSERT(
    compare_strings8(
      first( &our_str8 ), 
      "a1.00c3e5.00g7.00i9k11.00m13.00o15q17.00s19.00u21w23.00y25.00{27}29.00"
      "1234567891011121314151617181920212223242526272829303132"
    )
  );

  ALWAYS_ASSERT(
    compare_strings16(
      first( &our_str16 ), 
      u"a1.00c3e5.00g7.00i9k11.00m13.00o15q17.00s19.00u21w23.00y25.00{27}29.00"
      u"1234567891011121314151617181920212223242526272829303132"
    )
  );

  ALWAYS_ASSERT(
    compare_strings32(
      first( &our_str32 ), 
      U"a1.00c3e5.00g7.00i9k11.00m13.00o15q17.00s19.00u21w23.00y25.00{27}29.00"
      U"1234567891011121314151617181920212223242526272829303132"
    )
  );

  cleanup( &our_str8 );
  cleanup( &our_str16 );
  cleanup( &our_str32 );
}

static void test_str_push_n( void )
{
  str( char ) our_str8;
  init( &our_str8 );
  str( char16_t ) our_str16;
  init( &our_str16 );
  str( char32_t ) our_str32;
  init( &our_str32 );

  // Zero-size insertion.
  ALWAYS_ASSERT( !push_n( &our_str8, NULL, 0 ) );
  ALWAYS_ASSERT( !push_n( &our_str16, NULL, 0 ) );
  ALWAYS_ASSERT( !push_n( &our_str32, NULL, 0 ) );

  // Normal insert.

  char *el8;
  UNTIL_SUCCESS( el8 = push_n( &our_str8, "abcdefghij", 10 ) );
  ALWAYS_ASSERT( *el8 == 'a' );
  UNTIL_SUCCESS( el8 = push_n( &our_str8, "klmnopqrst", 10 ) );
  ALWAYS_ASSERT( *el8 == 'k' );
  UNTIL_SUCCESS( el8 = push_n( &our_str8, "uvwxyz{|}~", 10 ) );
  ALWAYS_ASSERT( *el8 == 'u' );

  char16_t *el16;
  UNTIL_SUCCESS( el16 = push_n( &our_str16, u"abcdefghij", 10 ) );
  ALWAYS_ASSERT( *el16 == 'a' );
  UNTIL_SUCCESS( el16 = push_n( &our_str16, u"klmnopqrst", 10 ) );
  ALWAYS_ASSERT( *el16 == 'k' );
  UNTIL_SUCCESS( el16 = push_n( &our_str16, u"uvwxyz{|}~", 10 ) );
  ALWAYS_ASSERT( *el16 == 'u' );

  char32_t *el32;
  UNTIL_SUCCESS( el32 = push_n( &our_str32, U"abcdefghij", 10 ) );
  ALWAYS_ASSERT( *el32 == 'a' );
  UNTIL_SUCCESS( el32 = push_n( &our_str32, U"klmnopqrst", 10 ) );
  ALWAYS_ASSERT( *el32 == 'k' );
  UNTIL_SUCCESS( el32 = push_n( &our_str32, U"uvwxyz{|}~", 10 ) );
  ALWAYS_ASSERT( *el32 == 'u' );

  // Check.

  ALWAYS_ASSERT( compare_strings8( first( &our_str8 ), "abcdefghijklmnopqrstuvwxyz{|}~" ) );
  ALWAYS_ASSERT( compare_strings16( first( &our_str16 ), u"abcdefghijklmnopqrstuvwxyz{|}~" ) );
  ALWAYS_ASSERT( compare_strings32( first( &our_str32 ), U"abcdefghijklmnopqrstuvwxyz{|}~" ) );

  cleanup( &our_str8 );
  cleanup( &our_str16 );
  cleanup( &our_str32 );
}

static void test_str_erase( void )
{
  str( char ) our_str8;
  init( &our_str8 );
  str( char16_t ) our_str16;
  init( &our_str16 );
  str( char32_t ) our_str32;
  init( &our_str32 );

  UNTIL_SUCCESS( push_fmt( &our_str8, "abcdefghijklmnopqrstuvwxyz{|}~" ) );
  UNTIL_SUCCESS( push_fmt( &our_str16, u"abcdefghijklmnopqrstuvwxyz{|}~" ) );
  UNTIL_SUCCESS( push_fmt( &our_str32, U"abcdefghijklmnopqrstuvwxyz{|}~" ) );

  bool erase;

  erase = true;
  for( size_t i = 0; i < 15; )
  {
    if( erase )
    {
      char *el = erase( &our_str8, i );
      ALWAYS_ASSERT( ( *el = *get( &our_str8, i ) ) );
    }
    else
      ++i;

    erase = !erase;
  }

  erase = true;
  for( size_t i = 0; i < 15; )
  {
    if( erase )
    {
      char16_t *el = erase( &our_str16, i );
      ALWAYS_ASSERT( ( *el = *get( &our_str16, i ) ) );
    }
    else
      ++i;

    erase = !erase;
  }

  erase = true;
  for( size_t i = 0; i < 15; )
  {
    if( erase )
    {
      char32_t *el = erase( &our_str32, i );
      ALWAYS_ASSERT( ( *el = *get( &our_str32, i ) ) );
    }
    else
      ++i;

    erase = !erase;
  }

  ALWAYS_ASSERT( compare_strings8( first( &our_str8 ), "bdfhjlnprtvxz|~" ) );
  ALWAYS_ASSERT( compare_strings16( first( &our_str16 ), u"bdfhjlnprtvxz|~" ) );
  ALWAYS_ASSERT( compare_strings32( first( &our_str32 ), U"bdfhjlnprtvxz|~" ) );

  cleanup( &our_str8 );
  cleanup( &our_str16 );
  cleanup( &our_str32 );
}

static void test_str_erase_n( void )
{
  str( char ) our_str8;
  init( &our_str8 );
  str( char16_t ) our_str16;
  init( &our_str16 );
  str( char32_t ) our_str32;
  init( &our_str32 );

  UNTIL_SUCCESS( push_fmt( &our_str8, "abcdefghijklmnopqrstuvwxyz{|}~" ) );
  UNTIL_SUCCESS( push_fmt( &our_str16, u"abcdefghijklmnopqrstuvwxyz{|}~" ) );
  UNTIL_SUCCESS( push_fmt( &our_str32, U"abcdefghijklmnopqrstuvwxyz{|}~" ) );

  bool erase;

  erase = true;
  for( size_t i = 0; i < 15; )
  {
    if( erase )
    {
      char *el = erase_n( &our_str8, i, 5 );
      ALWAYS_ASSERT( ( *el = *get( &our_str8, i ) ) );
    }
    else
      i += 5;

    erase = !erase;
  }

  erase = true;
  for( size_t i = 0; i < 15; )
  {
    if( erase )
    {
      char16_t *el = erase_n( &our_str16, i, 5 );
      ALWAYS_ASSERT( ( *el = *get( &our_str16, i ) ) );
    }
    else
      i += 5;

    erase = !erase;
  }

  erase = true;
  for( size_t i = 0; i < 15; )
  {
    if( erase )
    {
      char32_t *el = erase_n( &our_str32, i, 5 );
      ALWAYS_ASSERT( ( *el = *get( &our_str32, i ) ) );
    }
    else
      i += 5;

    erase = !erase;
  }

  ALWAYS_ASSERT( compare_strings8( first( &our_str8 ), "fghijpqrstz{|}~" ) );
  ALWAYS_ASSERT( compare_strings16( first( &our_str16 ), u"fghijpqrstz{|}~" ) );
  ALWAYS_ASSERT( compare_strings32( first( &our_str32 ), U"fghijpqrstz{|}~" ) );

  cleanup( &our_str8 );
  cleanup( &our_str16 );
  cleanup( &our_str32 );
}

static void test_str_clear( void )
{
  str( char ) our_str8;
  init( &our_str8 );
  str( char16_t ) our_str16;
  init( &our_str16 );
  str( char32_t ) our_str32;
  init( &our_str32 );

  // Empty.

  clear( &our_str8 );
  ALWAYS_ASSERT( size( &our_str8 ) == 0 );

  clear( &our_str16 );
  ALWAYS_ASSERT( size( &our_str16 ) == 0 );

  clear( &our_str32 );
  ALWAYS_ASSERT( size( &our_str32 ) == 0 );

  // Non-empty.

  UNTIL_SUCCESS( resize( &our_str8, 30, '-' ) );
  ALWAYS_ASSERT( size( &our_str8 ) == 30 );
  clear( &our_str8 );
  ALWAYS_ASSERT( size( &our_str8 ) == 0 );
  ALWAYS_ASSERT( cap( &our_str8 ) >= 30 );
  ALWAYS_ASSERT( compare_strings8( first( &our_str8 ), "" ) );

  UNTIL_SUCCESS( resize( &our_str16, 30, u'-' ) );
  ALWAYS_ASSERT( size( &our_str16 ) == 30 );
  clear( &our_str16 );
  ALWAYS_ASSERT( size( &our_str16 ) == 0 );
  ALWAYS_ASSERT( cap( &our_str16 ) >= 30 );
  ALWAYS_ASSERT( compare_strings16( first( &our_str16 ), u"" ) );

  UNTIL_SUCCESS( resize( &our_str32, 30, U'-' ) );
  ALWAYS_ASSERT( size( &our_str32 ) == 30 );
  clear( &our_str32 );
  ALWAYS_ASSERT( size( &our_str32 ) == 0 );
  ALWAYS_ASSERT( cap( &our_str32 ) >= 30 );
  ALWAYS_ASSERT( compare_strings32( first( &our_str32 ), U"" ) );

  // Test use.

  UNTIL_SUCCESS( push_fmt( &our_str8, "abcdefghijklmnopqrstuvwxyz{|}~" ) );
  ALWAYS_ASSERT( compare_strings8( first( &our_str8 ), "abcdefghijklmnopqrstuvwxyz{|}~" ) );

  UNTIL_SUCCESS( push_fmt( &our_str16, u"abcdefghijklmnopqrstuvwxyz{|}~" ) );
  ALWAYS_ASSERT( compare_strings16( first( &our_str16 ), u"abcdefghijklmnopqrstuvwxyz{|}~" ) );

  UNTIL_SUCCESS( push_fmt( &our_str32, U"abcdefghijklmnopqrstuvwxyz{|}~" ) );
  ALWAYS_ASSERT( compare_strings32( first( &our_str32 ), U"abcdefghijklmnopqrstuvwxyz{|}~" ) );

  cleanup( &our_str8 );
  cleanup( &our_str16 );
  cleanup( &our_str32 );
}

static void test_str_cleanup( void )
{
  str( char ) our_str8;
  init( &our_str8 );
  str( char16_t ) our_str16;
  init( &our_str16 );
  str( char32_t ) our_str32;
  init( &our_str32 );

  // Empty.

  cleanup( &our_str8 );
  ALWAYS_ASSERT( (void *)our_str8 == (void *)&cc_str_placeholder_char8 );

  cleanup( &our_str16 );
  ALWAYS_ASSERT( (void *)our_str16 == (void *)&cc_str_placeholder_char16 );

  cleanup( &our_str32 );
  ALWAYS_ASSERT( (void *)our_str32 == (void *)&cc_str_placeholder_char32 );

  // Non-empty.

  UNTIL_SUCCESS( resize( &our_str8, 30, '-' ) );
  ALWAYS_ASSERT( size( &our_str8 ) == 30 );
  cleanup( &our_str8 );
  ALWAYS_ASSERT( size( &our_str8 ) == 0 );
  ALWAYS_ASSERT( cap( &our_str8 ) == 0 );
  ALWAYS_ASSERT( (void *)our_str8 == (void *)&cc_str_placeholder_char8 );

  UNTIL_SUCCESS( resize( &our_str16, 30, u'-' ) );
  ALWAYS_ASSERT( size( &our_str16 ) == 30 );
  cleanup( &our_str16 );
  ALWAYS_ASSERT( size( &our_str16 ) == 0 );
  ALWAYS_ASSERT( cap( &our_str16 ) == 0 );
  ALWAYS_ASSERT( (void *)our_str16 == (void *)&cc_str_placeholder_char16 );

  UNTIL_SUCCESS( resize( &our_str32, 30, U'-' ) );
  ALWAYS_ASSERT( size( &our_str32 ) == 30 );
  cleanup( &our_str32 );
  ALWAYS_ASSERT( size( &our_str32 ) == 0 );
  ALWAYS_ASSERT( cap( &our_str32 ) == 0 );
  ALWAYS_ASSERT( (void *)our_str32 == (void *)&cc_str_placeholder_char32 );

  // Test use.

  UNTIL_SUCCESS( push_fmt( &our_str8, "abcdefghijklmnopqrstuvwxyz{|}~" ) );
  ALWAYS_ASSERT( compare_strings8( first( &our_str8 ), "abcdefghijklmnopqrstuvwxyz{|}~" ) );

  UNTIL_SUCCESS( push_fmt( &our_str16, u"abcdefghijklmnopqrstuvwxyz{|}~" ) );
  ALWAYS_ASSERT( compare_strings16( first( &our_str16 ), u"abcdefghijklmnopqrstuvwxyz{|}~" ) );

  UNTIL_SUCCESS( push_fmt( &our_str32, U"abcdefghijklmnopqrstuvwxyz{|}~" ) );
  ALWAYS_ASSERT( compare_strings32( first( &our_str32 ), U"abcdefghijklmnopqrstuvwxyz{|}~" ) );

  cleanup( &our_str8 );
  cleanup( &our_str16 );
  cleanup( &our_str32 );
}

static void test_str_iteration( void )
{
  str( char ) our_str8;
  init( &our_str8 );
  str( char16_t ) our_str16;
  init( &our_str16 );
  str( char32_t ) our_str32;
  init( &our_str32 );

  // Empty.

  size_t n_iterations = 0;

  for( char *i = first( &our_str8 ); i != end( &our_str8 ); i = next( &our_str8, i ) )
    ++n_iterations;

  for_each( &our_str8, i )
    ++n_iterations;

  for( char16_t *i = first( &our_str16 ); i != end( &our_str16 ); i = next( &our_str16, i ) )
    ++n_iterations;

  for_each( &our_str16, i )
    ++n_iterations;

  for( char32_t *i = first( &our_str32 ); i != end( &our_str32 ); i = next( &our_str32, i ) )
    ++n_iterations;

  for_each( &our_str32, i )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 0 );

  // Non-empty.

  UNTIL_SUCCESS( push_fmt( &our_str8, "abcdefghijklmnopqrstuvwxyz{|}~" ) );

  for( char *i = first( &our_str8 ); i != end( &our_str8 ); i = next( &our_str8, i ) )
    ++n_iterations;

  for_each( &our_str8, i )
    ++n_iterations;

  UNTIL_SUCCESS( push_fmt( &our_str16, u"abcdefghijklmnopqrstuvwxyz{|}~" ) );

  for( char16_t *i = first( &our_str16 ); i != end( &our_str16 ); i = next( &our_str16, i ) )
    ++n_iterations;

  for_each( &our_str16, i )
    ++n_iterations;

  UNTIL_SUCCESS( push_fmt( &our_str32, U"abcdefghijklmnopqrstuvwxyz{|}~" ) );

  for( char32_t *i = first( &our_str32 ); i != end( &our_str32 ); i = next( &our_str32, i ) )
    ++n_iterations;

  for_each( &our_str32, i )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 180 );

  // Test last and first.

  ALWAYS_ASSERT( *first( &our_str8 ) == 'a' );
  ALWAYS_ASSERT( *last( &our_str8 ) == '~' );

  ALWAYS_ASSERT( *first( &our_str16 ) == u'a' );
  ALWAYS_ASSERT( *last( &our_str16 ) == u'~' );

  ALWAYS_ASSERT( *first( &our_str32 ) == U'a' );
  ALWAYS_ASSERT( *last( &our_str32 ) == U'~' );

  cleanup( &our_str8 );
  cleanup( &our_str16 );
  cleanup( &our_str32 );
}

static void test_str_init_clone( void )
{
  str( char ) src_str8;
  init( &src_str8 );
  str( char16_t ) src_str16;
  init( &src_str16 );
  str( char32_t ) src_str32;
  init( &src_str32 );

  // Test init_clone placeholder.

  str( char ) empty_str8;
  UNTIL_SUCCESS( init_clone( &empty_str8, &src_str8 ) );
  ALWAYS_ASSERT( (void *)empty_str8 == (void *)&cc_str_placeholder_char8 );

  str( char16_t) empty_str16;
  UNTIL_SUCCESS( init_clone( &empty_str16, &src_str16 ) );
  ALWAYS_ASSERT( (void *)empty_str16 == (void *)&cc_str_placeholder_char16 );

  str( char32_t ) empty_str32;
  UNTIL_SUCCESS( init_clone( &empty_str32, &src_str32 ) );
  ALWAYS_ASSERT( (void *)empty_str32 == (void *)&cc_str_placeholder_char32 );

  // Test init_clone non-placeholder.

  str( char ) our_str8;
  init( &our_str8 );
  UNTIL_SUCCESS( push_fmt( &src_str8, "abcdefghijklmnopqrstuvwxyz{|}~" ) );
  UNTIL_SUCCESS( init_clone( &our_str8, &src_str8 ) );
  ALWAYS_ASSERT( compare_strings8( first( &our_str8 ), "abcdefghijklmnopqrstuvwxyz{|}~" ) );

  str( char16_t ) our_str16;
  init( &our_str16 );
  UNTIL_SUCCESS( push_fmt( &src_str16, u"abcdefghijklmnopqrstuvwxyz{|}~" ) );
  UNTIL_SUCCESS( init_clone( &our_str16, &src_str16 ) );
  ALWAYS_ASSERT( compare_strings16( first( &our_str16 ), u"abcdefghijklmnopqrstuvwxyz{|}~" ) );

  str( char32_t ) our_str32;
  init( &our_str32 );
  UNTIL_SUCCESS( push_fmt( &src_str32, U"abcdefghijklmnopqrstuvwxyz{|}~" ) );
  UNTIL_SUCCESS( init_clone( &our_str32, &src_str32 ) );
  ALWAYS_ASSERT( compare_strings32( first( &our_str32 ), U"abcdefghijklmnopqrstuvwxyz{|}~" ) );

  cleanup( &src_str8 );
  cleanup( &src_str16 );
  cleanup( &src_str32 );
  cleanup( &our_str8 );
  cleanup( &our_str16 );
  cleanup( &our_str32 );
}

#define STRING_LITERAL_CHAR( literal ) literal
#define STRING_LITERAL_UNSIGNED_CHAR( literal ) (unsigned char *)literal
#define STRING_LITERAL_SIGNED_CHAR( literal ) (signed char *)literal
#define STRING_LITERAL_CHAR8( literal ) u8##literal
#define STRING_LITERAL_CHAR16( literal ) u##literal
#define STRING_LITERAL_CHAR32( literal ) U##literal

#define TEST_STR_VEC_INEROPERABILITY( ty, string_literal_macro, compare_strings_fn )     \
{                                                                                        \
  vec( str( ty ) ) our_vec;                                                              \
  init( &our_vec );                                                                      \
                                                                                         \
  str( ty ) our_str_1 = initialized( &our_str_1 );                                       \
  str( ty ) our_str_2 = initialized( &our_str_2 );                                       \
  UNTIL_SUCCESS( push_fmt( &our_str_1, string_literal_macro( "Cat" ) ) );                \
  UNTIL_SUCCESS( push_fmt( &our_str_2, string_literal_macro( "Dog" ) ) );                \
                                                                                         \
  UNTIL_SUCCESS( push( &our_vec, our_str_1 ) );                                          \
  UNTIL_SUCCESS( push( &our_vec, our_str_2 ) );                                          \
                                                                                         \
  str( ty ) *el;                                                                         \
  el = get( &our_vec, 0 );                                                               \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Cat" ) ) );     \
  el = get( &our_vec, 1 );                                                               \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Dog" ) ) );     \
                                                                                         \
  erase( &our_vec, 0 );                                                                  \
  clear( &our_vec );                                                                     \
                                                                                         \
  /* Heterogeneous push. */                                                              \
  UNTIL_SUCCESS( push( &our_vec, string_literal_macro( "Fish" ) ) );                     \
  UNTIL_SUCCESS( push( &our_vec, string_literal_macro( "Parrot" ) ) );                   \
  UNTIL_SUCCESS( push( &our_vec, string_literal_macro( "Hamster" ) ) );                  \
  UNTIL_SUCCESS( push( &our_vec, string_literal_macro( "Ferret" ) ) );                   \
                                                                                         \
  /* Heterogeneous insert. */                                                            \
  UNTIL_SUCCESS( insert( &our_vec, 0, string_literal_macro( "Spider" ) ) );              \
  UNTIL_SUCCESS( insert( &our_vec, 0, string_literal_macro( "Mouse" ) ) );               \
  UNTIL_SUCCESS( insert( &our_vec, 0, string_literal_macro( "Snake" ) ) );               \
  UNTIL_SUCCESS( insert( &our_vec, 0, string_literal_macro( "Lizard" ) ) );              \
                                                                                         \
  el = get( &our_vec, 0 );                                                               \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Lizard" ) ) );  \
  el = get( &our_vec, 1 );                                                               \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Snake" ) ) );   \
  el = get( &our_vec, 2 );                                                               \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Mouse" ) ) );   \
  el = get( &our_vec, 3 );                                                               \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Spider" ) ) );  \
  el = get( &our_vec, 4 );                                                               \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Fish" ) ) );    \
  el = get( &our_vec, 5 );                                                               \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Parrot" ) ) );  \
  el = get( &our_vec, 6 );                                                               \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Hamster" ) ) ); \
  el = get( &our_vec, 7 );                                                               \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Ferret" ) ) );  \
                                                                                         \
  cleanup( &our_vec );                                                                   \
}                                                                                        \

static void test_str_interoperability_vec( void )
{
  TEST_STR_VEC_INEROPERABILITY( char, STRING_LITERAL_CHAR, compare_strings8 );
  TEST_STR_VEC_INEROPERABILITY( unsigned char, STRING_LITERAL_UNSIGNED_CHAR, compare_strings8 );
  TEST_STR_VEC_INEROPERABILITY( signed char, STRING_LITERAL_SIGNED_CHAR, compare_strings8 );
#if ( defined( __cplusplus ) && __cplusplus >= 202101L ) ||        \
    ( defined( __STDC_VERSION__ ) && __STDC_VERSION__ >= 202311L )
  TEST_STR_VEC_INEROPERABILITY( char8_t, STRING_LITERAL_CHAR8, compare_strings8 );
#endif
  TEST_STR_VEC_INEROPERABILITY( char16_t, STRING_LITERAL_CHAR16, compare_strings16 );
  TEST_STR_VEC_INEROPERABILITY( char32_t, STRING_LITERAL_CHAR32, compare_strings32 );
}

#define TEST_STR_LIST_INEROPERABILITY( ty, string_literal_macro, compare_strings_fn )         \
{                                                                                             \
  list( str( ty ) ) our_list;                                                                 \
  init( &our_list );                                                                          \
                                                                                              \
  str( ty ) our_str_1 = initialized( &our_str_1 );                                            \
  str( ty ) our_str_2 = initialized( &our_str_2 );                                            \
  UNTIL_SUCCESS( push_fmt( &our_str_1, string_literal_macro( "Cat" ) ) );                     \
  UNTIL_SUCCESS( push_fmt( &our_str_2, string_literal_macro( "Dog" ) ) );                     \
                                                                                              \
  UNTIL_SUCCESS( push( &our_list, our_str_1 ) );                                              \
  UNTIL_SUCCESS( push( &our_list, our_str_2 ) );                                              \
                                                                                              \
  str( ty ) *el;                                                                              \
  el = first( &our_list );                                                                    \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Cat" ) ) );          \
  el = last( &our_list );                                                                     \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Dog" ) ) );          \
                                                                                              \
  erase( &our_list, first( &our_list ) );                                                     \
  clear( &our_list );                                                                         \
                                                                                              \
  /* Heterogeneous push. */                                                                   \
  UNTIL_SUCCESS( push( &our_list, string_literal_macro( "Fish" ) ) );                         \
  UNTIL_SUCCESS( push( &our_list, string_literal_macro( "Parrot" ) ) );                       \
  UNTIL_SUCCESS( push( &our_list, string_literal_macro( "Hamster" ) ) );                      \
  UNTIL_SUCCESS( push( &our_list, string_literal_macro( "Ferret" ) ) );                       \
                                                                                              \
  /* Heterogeneous insert. */                                                                 \
  UNTIL_SUCCESS( insert( &our_list, first( &our_list ), string_literal_macro( "Spider" ) ) ); \
  UNTIL_SUCCESS( insert( &our_list, first( &our_list ), string_literal_macro( "Mouse" ) ) );  \
  UNTIL_SUCCESS( insert( &our_list, first( &our_list ), string_literal_macro( "Snake" ) ) );  \
  UNTIL_SUCCESS( insert( &our_list, first( &our_list ), string_literal_macro( "Lizard" ) ) ); \
                                                                                              \
  el = first( &our_list );                                                                    \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Lizard" ) ) );       \
  el = next( &our_list, el );                                                                 \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Snake" ) ) );        \
  el = next( &our_list, el );                                                                 \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Mouse" ) ) );        \
  el = next( &our_list, el );                                                                 \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Spider" ) ) );       \
  el = next( &our_list, el );                                                                 \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Fish" ) ) );         \
  el = next( &our_list, el );                                                                 \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Parrot" ) ) );       \
  el = next( &our_list, el );                                                                 \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Hamster" ) ) );      \
  el = next( &our_list, el );                                                                 \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Ferret" ) ) );       \
                                                                                              \
  cleanup( &our_list );                                                                       \
}                                                                                             \

static void test_str_interoperability_list( void )
{
  TEST_STR_LIST_INEROPERABILITY( char, STRING_LITERAL_CHAR, compare_strings8 );
  TEST_STR_LIST_INEROPERABILITY( unsigned char, STRING_LITERAL_UNSIGNED_CHAR, compare_strings8 );
  TEST_STR_LIST_INEROPERABILITY( signed char, STRING_LITERAL_SIGNED_CHAR, compare_strings8 );
#if ( defined( __cplusplus ) && __cplusplus >= 202101L ) ||        \
    ( defined( __STDC_VERSION__ ) && __STDC_VERSION__ >= 202311L )
  TEST_STR_LIST_INEROPERABILITY( char8_t, STRING_LITERAL_CHAR8, compare_strings8 );
#endif
  TEST_STR_LIST_INEROPERABILITY( char16_t, STRING_LITERAL_CHAR16, compare_strings16 );
  TEST_STR_LIST_INEROPERABILITY( char32_t, STRING_LITERAL_CHAR32, compare_strings32 );
}

#define TEST_STR_MAP_INEROPERABILITY( ty, string_literal_macro, compare_strings_fn )                                  \
{                                                                                                                     \
  map( str( ty ), str( ty ) ) our_map;                                                                                \
  init( &our_map );                                                                                                   \
                                                                                                                      \
  /* Insert. */                                                                                                       \
                                                                                                                      \
  str( ty ) our_str_1 = initialized( &our_str_1 );                                                                    \
  str( ty ) our_str_2 = initialized( &our_str_2 );                                                                    \
  str( ty ) our_str_3 = initialized( &our_str_3 );                                                                    \
  str( ty ) our_str_4 = initialized( &our_str_4 );                                                                    \
  str( ty ) our_str_5 = initialized( &our_str_5 );                                                                    \
  str( ty ) our_str_6 = initialized( &our_str_6 );                                                                    \
  UNTIL_SUCCESS( push_fmt( &our_str_1, string_literal_macro( "Short string" ) ) );                                    \
  UNTIL_SUCCESS( push_fmt( &our_str_2, string_literal_macro( "Medium-length string" ) ) );                            \
  UNTIL_SUCCESS( push_fmt( &our_str_3, string_literal_macro( "This here is a significantly longer string" ) ) );      \
  UNTIL_SUCCESS( push_fmt( &our_str_4, string_literal_macro( "Cat" ) ) );                                             \
  UNTIL_SUCCESS( push_fmt( &our_str_5, string_literal_macro( "Dog" ) ) );                                             \
  UNTIL_SUCCESS( push_fmt( &our_str_6, string_literal_macro( "Rabbit" ) ) );                                          \
                                                                                                                      \
  UNTIL_SUCCESS( insert( &our_map, our_str_1, our_str_4 ) );                                                          \
  UNTIL_SUCCESS( insert( &our_map, our_str_2, our_str_5 ) );                                                          \
  UNTIL_SUCCESS( insert( &our_map, our_str_3, our_str_6 ) );                                                          \
                                                                                                                      \
  /* Look-up. */                                                                                                      \
                                                                                                                      \
  str( ty ) our_str_7 = initialized( &our_str_7 );                                                                    \
  str( ty ) our_str_8 = initialized( &our_str_8 );                                                                    \
  str( ty ) our_str_9 = initialized( &our_str_9 );                                                                    \
  UNTIL_SUCCESS( push_fmt( &our_str_7, string_literal_macro( "Short string" ) ) );                                    \
  UNTIL_SUCCESS( push_fmt( &our_str_8, string_literal_macro( "Medium-length string" ) ) );                            \
  UNTIL_SUCCESS( push_fmt( &our_str_9, string_literal_macro( "This here is a significantly longer string" ) ) );      \
                                                                                                                      \
  str( ty ) *el;                                                                                                      \
  el = get( &our_map, our_str_7 );                                                                                    \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Cat" ) ) );                                  \
  el = get( &our_map, our_str_8 );                                                                                    \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Dog" ) ) );                                  \
  el = get( &our_map, our_str_9 );                                                                                    \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Rabbit" ) ) );                               \
                                                                                                                      \
  cleanup( &our_str_7 );                                                                                              \
  cleanup( &our_str_8 );                                                                                              \
  cleanup( &our_str_9 );                                                                                              \
                                                                                                                      \
  /* Replace and check. */                                                                                            \
  str( ty ) our_str_10 = initialized( &our_str_10 );                                                                  \
  str( ty ) our_str_11 = initialized( &our_str_11 );                                                                  \
  UNTIL_SUCCESS( push_fmt( &our_str_10, string_literal_macro( "Short string" ) ) );                                   \
  UNTIL_SUCCESS( push_fmt( &our_str_11, string_literal_macro( "Hamster" ) ) );                                        \
  UNTIL_SUCCESS( insert( &our_map, our_str_10, our_str_11 ) );                                                        \
  UNTIL_SUCCESS( push_fmt( &our_str_7, string_literal_macro( "Short string" ) ) );                                    \
  el = get( &our_map, our_str_7 );                                                                                    \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Hamster" ) ) );                              \
  cleanup( &our_str_7 );                                                                                              \
                                                                                                                      \
  /* Heterogeneous get. */                                                                                            \
  el = get( &our_map, string_literal_macro( "Short string" ) );                                                       \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Hamster" ) ) );                              \
  el = get( &our_map, string_literal_macro( "Medium-length string" ) );                                               \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Dog" ) ) );                                  \
  el = get( &our_map, string_literal_macro( "This here is a significantly longer string" ) );                         \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Rabbit" ) ) );                               \
                                                                                                                      \
  /* Heterogeneous erase. */                                                                                          \
  ALWAYS_ASSERT( erase( &our_map, string_literal_macro( "Short string" ) ) );                                         \
  ALWAYS_ASSERT( erase( &our_map, string_literal_macro( "Medium-length string" ) ) );                                 \
  ALWAYS_ASSERT( erase( &our_map, string_literal_macro( "This here is a significantly longer string" ) ) );           \
  ALWAYS_ASSERT( !get( &our_map, string_literal_macro( "Short string" ) ) );                                          \
  ALWAYS_ASSERT( !get( &our_map, string_literal_macro( "Medium-length string" ) ) );                                  \
  ALWAYS_ASSERT( !get( &our_map, string_literal_macro( "This here is a significantly longer string" ) ) );            \
                                                                                                                      \
  /* Heterogenous insert. */                                                                                          \
  UNTIL_SUCCESS( insert( &our_map, string_literal_macro( "Apple" ), string_literal_macro( "Potato" ) ) );             \
  UNTIL_SUCCESS( insert( &our_map, string_literal_macro( "Orange" ), string_literal_macro( "Carrot" ) ) );            \
  UNTIL_SUCCESS( insert( &our_map, string_literal_macro( "Pear" ), string_literal_macro( "Onion" ) ) );               \
  el = get( &our_map, string_literal_macro( "Apple" ) );                                                              \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Potato" ) ) );                               \
  el = get( &our_map, string_literal_macro( "Orange" ) );                                                             \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Carrot" ) ) );                               \
  el = get( &our_map, string_literal_macro( "Pear" ) );                                                               \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Onion" ) ) );                                \
                                                                                                                      \
  /* Heterogenous get or insert. */                                                                                   \
  UNTIL_SUCCESS( get_or_insert( &our_map, string_literal_macro( "Grapefruit" ), string_literal_macro( "Tomato" ) ) ); \
  UNTIL_SUCCESS( get_or_insert( &our_map, string_literal_macro( "Melon" ), string_literal_macro( "Garlic" ) ) );      \
  UNTIL_SUCCESS( get_or_insert( &our_map, string_literal_macro( "Orange" ), string_literal_macro( "Capsicum" ) ) );   \
  UNTIL_SUCCESS( get_or_insert( &our_map, string_literal_macro( "Pear" ), string_literal_macro( "Spinach" ) ) );      \
  el = get( &our_map, string_literal_macro( "Grapefruit" ) );                                                         \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Tomato" ) ) );                               \
  el = get( &our_map, string_literal_macro( "Melon" ) );                                                              \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Garlic" ) ) );                               \
  el = get( &our_map, string_literal_macro( "Orange" ) );                                                             \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Carrot" ) ) );                               \
  el = get( &our_map, string_literal_macro( "Pear" ) );                                                               \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Onion" ) ) );                                \
                                                                                                                      \
  cleanup( &our_map );                                                                                                \
}                                                                                                                     \

static void test_str_interoperability_map( void )
{
  TEST_STR_MAP_INEROPERABILITY( char, STRING_LITERAL_CHAR, compare_strings8 );
  TEST_STR_MAP_INEROPERABILITY( unsigned char, STRING_LITERAL_UNSIGNED_CHAR, compare_strings8 );
  TEST_STR_MAP_INEROPERABILITY( signed char, STRING_LITERAL_SIGNED_CHAR, compare_strings8 );
#if ( defined( __cplusplus ) && __cplusplus >= 202101L ) ||        \
    ( defined( __STDC_VERSION__ ) && __STDC_VERSION__ >= 202311L )
  TEST_STR_MAP_INEROPERABILITY( char8_t, STRING_LITERAL_CHAR8, compare_strings8 );
#endif
  TEST_STR_MAP_INEROPERABILITY( char16_t, STRING_LITERAL_CHAR16, compare_strings16 );
  TEST_STR_MAP_INEROPERABILITY( char32_t, STRING_LITERAL_CHAR32, compare_strings32 );
}

#define TEST_STR_SET_INEROPERABILITY( ty, string_literal_macro, compare_strings_fn )                             \
{                                                                                                                \
  set( str( ty ) ) our_set;                                                                                      \
  init( &our_set );                                                                                              \
                                                                                                                 \
  /* Insert. */                                                                                                  \
                                                                                                                 \
  str( ty ) our_str_1 = initialized( &our_str_1 );                                                               \
  str( ty ) our_str_2 = initialized( &our_str_2 );                                                               \
  str( ty ) our_str_3 = initialized( &our_str_3 );                                                               \
  UNTIL_SUCCESS( push_fmt( &our_str_1, string_literal_macro( "Short string" ) ) );                               \
  UNTIL_SUCCESS( push_fmt( &our_str_2, string_literal_macro( "Medium-length string" ) ) );                       \
  UNTIL_SUCCESS( push_fmt( &our_str_3, string_literal_macro( "This here is a significantly longer string" ) ) ); \
                                                                                                                 \
  UNTIL_SUCCESS( insert( &our_set, our_str_1 ) );                                                                \
  UNTIL_SUCCESS( insert( &our_set, our_str_2 ) );                                                                \
  UNTIL_SUCCESS( insert( &our_set, our_str_3 ) );                                                                \
                                                                                                                 \
  /* Look-up. */                                                                                                 \
                                                                                                                 \
  str( ty ) our_str_4 = initialized( &our_str_4 );                                                               \
  str( ty ) our_str_5 = initialized( &our_str_5 );                                                               \
  str( ty ) our_str_6 = initialized( &our_str_6 );                                                               \
  UNTIL_SUCCESS( push_fmt( &our_str_4, string_literal_macro( "Short string" ) ) );                               \
  UNTIL_SUCCESS( push_fmt( &our_str_5, string_literal_macro( "Medium-length string" ) ) );                       \
  UNTIL_SUCCESS( push_fmt( &our_str_6, string_literal_macro( "This here is a significantly longer string" ) ) ); \
                                                                                                                 \
  str( ty ) *el;                                                                                                 \
  el = get( &our_set, our_str_4 );                                                                               \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Short string" ) ) );                    \
  el = get( &our_set, our_str_5 );                                                                               \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Medium-length string" ) ) );            \
  el = get( &our_set, our_str_6 );                                                                               \
  ALWAYS_ASSERT(                                                                                                 \
    compare_strings_fn( first( el ), string_literal_macro( "This here is a significantly longer string" ) )      \
  );                                                                                                             \
                                                                                                                 \
  cleanup( &our_str_4 );                                                                                         \
  cleanup( &our_str_5 );                                                                                         \
  cleanup( &our_str_6 );                                                                                         \
                                                                                                                 \
  /* Replace. */                                                                                                 \
  str( ty ) our_str_7 = initialized( &our_str_7 );                                                               \
  UNTIL_SUCCESS( push_fmt( &our_str_7, string_literal_macro( "Short string" ) ) );                               \
  UNTIL_SUCCESS( el = insert( &our_set, our_str_7 ) );                                                           \
  ALWAYS_ASSERT(                                                                                                 \
    *el == our_str_7 && compare_strings_fn( first( el ), string_literal_macro( "Short string" ) )                \
  );                                                                                                             \
                                                                                                                 \
  /* Heterogeneous get. */                                                                                       \
  el = get( &our_set, string_literal_macro( "Short string" ) );                                                  \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Short string" ) ) );                    \
  el = get( &our_set, string_literal_macro( "Medium-length string" ) );                                          \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Medium-length string" ) ) );            \
  el = get( &our_set, string_literal_macro( "This here is a significantly longer string" ) );                    \
  ALWAYS_ASSERT(                                                                                                 \
    compare_strings_fn( first( el ), string_literal_macro( "This here is a significantly longer string" ) )      \
  );                                                                                                             \
                                                                                                                 \
  /* Heterogeneous erase. */                                                                                     \
  ALWAYS_ASSERT( erase( &our_set, string_literal_macro( "Short string" ) ) );                                    \
  ALWAYS_ASSERT( erase( &our_set, string_literal_macro( "Medium-length string" ) ) );                            \
  ALWAYS_ASSERT( erase( &our_set, string_literal_macro( "This here is a significantly longer string" ) ) );      \
  ALWAYS_ASSERT( !get( &our_set, string_literal_macro( "Short string" ) ) );                                     \
  ALWAYS_ASSERT( !get( &our_set, string_literal_macro( "Medium-length string" ) ) );                             \
  ALWAYS_ASSERT( !get( &our_set, string_literal_macro( "This here is a significantly longer string" ) ) );       \
                                                                                                                 \
  /* Heterogenous insert. */                                                                                     \
  UNTIL_SUCCESS( insert( &our_set, string_literal_macro( "Apple" ) ) );                                          \
  UNTIL_SUCCESS( insert( &our_set, string_literal_macro( "Orange" ) ) );                                         \
  UNTIL_SUCCESS( insert( &our_set, string_literal_macro( "Pear" ) ) );                                           \
  el = get( &our_set, string_literal_macro( "Apple" ) );                                                         \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Apple" ) ) );                           \
  el = get( &our_set, string_literal_macro( "Orange" ) );                                                        \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Orange" ) ) );                          \
  el = get( &our_set, string_literal_macro( "Pear" ) );                                                          \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Pear" ) ) );                            \
                                                                                                                 \
  /* Heterogenous get or insert. */                                                                              \
  UNTIL_SUCCESS( get_or_insert( &our_set, string_literal_macro( "Grapefruit" ) ) );                              \
  UNTIL_SUCCESS( get_or_insert( &our_set, string_literal_macro( "Melon" ) ) );                                   \
  str( ty ) *orange = get( &our_set, string_literal_macro( "Orange" ) );                                         \
  str( ty ) *pear = get( &our_set, string_literal_macro( "Pear" ) );                                             \
  UNTIL_SUCCESS( get_or_insert( &our_set, string_literal_macro( "Orange" ) ) );                                  \
  UNTIL_SUCCESS( get_or_insert( &our_set, string_literal_macro( "Pear" ) ) );                                    \
  el = get( &our_set, string_literal_macro( "Grapefruit" ) );                                                    \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Grapefruit" ) ) );                      \
  el = get( &our_set, string_literal_macro( "Melon" ) );                                                         \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Melon" ) ) );                           \
  el = get( &our_set, string_literal_macro( "Orange" ) );                                                        \
  ALWAYS_ASSERT( el == orange && compare_strings_fn( first( el ), string_literal_macro( "Orange" ) ) );          \
  el = get( &our_set, string_literal_macro( "Pear" ) );                                                          \
  ALWAYS_ASSERT( el == pear && compare_strings_fn( first( el ), string_literal_macro( "Pear" ) ) );              \
                                                                                                                 \
  cleanup( &our_set );                                                                                           \
}                                                                                                                \

static void test_str_interoperability_set( void )
{
  TEST_STR_SET_INEROPERABILITY( char, STRING_LITERAL_CHAR, compare_strings8 );
  TEST_STR_SET_INEROPERABILITY( unsigned char, STRING_LITERAL_UNSIGNED_CHAR, compare_strings8 );
  TEST_STR_SET_INEROPERABILITY( signed char, STRING_LITERAL_SIGNED_CHAR, compare_strings8 );
#if ( defined( __cplusplus ) && __cplusplus >= 202101L ) ||        \
    ( defined( __STDC_VERSION__ ) && __STDC_VERSION__ >= 202311L )
  TEST_STR_SET_INEROPERABILITY( char8_t, STRING_LITERAL_CHAR8, compare_strings8 );
#endif
  TEST_STR_SET_INEROPERABILITY( char16_t, STRING_LITERAL_CHAR16, compare_strings16 );
  TEST_STR_SET_INEROPERABILITY( char32_t, STRING_LITERAL_CHAR32, compare_strings32 );
}

#define TEST_STR_OMAP_INEROPERABILITY( ty, string_literal_macro, compare_strings_fn )                                  \
{                                                                                                                      \
  map( str( ty ), str( ty ) ) our_omap;                                                                                \
  init( &our_omap );                                                                                                   \
                                                                                                                       \
  /* Insert. */                                                                                                        \
                                                                                                                       \
  str( ty ) our_str_1 = initialized( &our_str_1 );                                                                     \
  str( ty ) our_str_2 = initialized( &our_str_2 );                                                                     \
  str( ty ) our_str_3 = initialized( &our_str_3 );                                                                     \
  str( ty ) our_str_4 = initialized( &our_str_4 );                                                                     \
  str( ty ) our_str_5 = initialized( &our_str_5 );                                                                     \
  str( ty ) our_str_6 = initialized( &our_str_6 );                                                                     \
  UNTIL_SUCCESS( push_fmt( &our_str_1, string_literal_macro( "Short string" ) ) );                                     \
  UNTIL_SUCCESS( push_fmt( &our_str_2, string_literal_macro( "Medium-length string" ) ) );                             \
  UNTIL_SUCCESS( push_fmt( &our_str_3, string_literal_macro( "This here is a significantly longer string" ) ) );       \
  UNTIL_SUCCESS( push_fmt( &our_str_4, string_literal_macro( "Cat" ) ) );                                              \
  UNTIL_SUCCESS( push_fmt( &our_str_5, string_literal_macro( "Dog" ) ) );                                              \
  UNTIL_SUCCESS( push_fmt( &our_str_6, string_literal_macro( "Rabbit" ) ) );                                           \
                                                                                                                       \
  UNTIL_SUCCESS( insert( &our_omap, our_str_1, our_str_4 ) );                                                          \
  UNTIL_SUCCESS( insert( &our_omap, our_str_2, our_str_5 ) );                                                          \
  UNTIL_SUCCESS( insert( &our_omap, our_str_3, our_str_6 ) );                                                          \
                                                                                                                       \
  /* Look-up. */                                                                                                       \
                                                                                                                       \
  str( ty ) our_str_7 = initialized( &our_str_7 );                                                                     \
  str( ty ) our_str_8 = initialized( &our_str_8 );                                                                     \
  str( ty ) our_str_9 = initialized( &our_str_9 );                                                                     \
  UNTIL_SUCCESS( push_fmt( &our_str_7, string_literal_macro( "Short string" ) ) );                                     \
  UNTIL_SUCCESS( push_fmt( &our_str_8, string_literal_macro( "Medium-length string" ) ) );                             \
  UNTIL_SUCCESS( push_fmt( &our_str_9, string_literal_macro( "This here is a significantly longer string" ) ) );       \
                                                                                                                       \
  str( ty ) *el;                                                                                                       \
  el = get( &our_omap, our_str_7 );                                                                                    \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Cat" ) ) );                                   \
  el = get( &our_omap, our_str_8 );                                                                                    \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Dog" ) ) );                                   \
  el = get( &our_omap, our_str_9 );                                                                                    \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Rabbit" ) ) );                                \
                                                                                                                       \
  cleanup( &our_str_7 );                                                                                               \
  cleanup( &our_str_8 );                                                                                               \
  cleanup( &our_str_9 );                                                                                               \
                                                                                                                       \
  /* Replace and check. */                                                                                             \
  str( ty ) our_str_10 = initialized( &our_str_10 );                                                                   \
  str( ty ) our_str_11 = initialized( &our_str_11 );                                                                   \
  UNTIL_SUCCESS( push_fmt( &our_str_10, string_literal_macro( "Short string" ) ) );                                    \
  UNTIL_SUCCESS( push_fmt( &our_str_11, string_literal_macro( "Hamster" ) ) );                                         \
  UNTIL_SUCCESS( insert( &our_omap, our_str_10, our_str_11 ) );                                                        \
  UNTIL_SUCCESS( push_fmt( &our_str_7, string_literal_macro( "Short string" ) ) );                                     \
  el = get( &our_omap, our_str_7 );                                                                                    \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Hamster" ) ) );                               \
  cleanup( &our_str_7 );                                                                                               \
                                                                                                                       \
  /* Heterogeneous get. */                                                                                             \
  el = get( &our_omap, string_literal_macro( "Short string" ) );                                                       \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Hamster" ) ) );                               \
  el = get( &our_omap, string_literal_macro( "Medium-length string" ) );                                               \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Dog" ) ) );                                   \
  el = get( &our_omap, string_literal_macro( "This here is a significantly longer string" ) );                         \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Rabbit" ) ) );                                \
                                                                                                                       \
  /* Heterogeneous erase. */                                                                                           \
  ALWAYS_ASSERT( erase( &our_omap, string_literal_macro( "Short string" ) ) );                                         \
  ALWAYS_ASSERT( erase( &our_omap, string_literal_macro( "Medium-length string" ) ) );                                 \
  ALWAYS_ASSERT( erase( &our_omap, string_literal_macro( "This here is a significantly longer string" ) ) );           \
  ALWAYS_ASSERT( !get( &our_omap, string_literal_macro( "Short string" ) ) );                                          \
  ALWAYS_ASSERT( !get( &our_omap, string_literal_macro( "Medium-length string" ) ) );                                  \
  ALWAYS_ASSERT( !get( &our_omap, string_literal_macro( "This here is a significantly longer string" ) ) );            \
                                                                                                                       \
  /* Heterogenous insert. */                                                                                           \
  UNTIL_SUCCESS( insert( &our_omap, string_literal_macro( "Apple" ), string_literal_macro( "Potato" ) ) );             \
  UNTIL_SUCCESS( insert( &our_omap, string_literal_macro( "Orange" ), string_literal_macro( "Carrot" ) ) );            \
  UNTIL_SUCCESS( insert( &our_omap, string_literal_macro( "Pear" ), string_literal_macro( "Onion" ) ) );               \
  el = get( &our_omap, string_literal_macro( "Apple" ) );                                                              \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Potato" ) ) );                                \
  el = get( &our_omap, string_literal_macro( "Orange" ) );                                                             \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Carrot" ) ) );                                \
  el = get( &our_omap, string_literal_macro( "Pear" ) );                                                               \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Onion" ) ) );                                 \
                                                                                                                       \
  /* Heterogenous get or insert. */                                                                                    \
  UNTIL_SUCCESS( get_or_insert( &our_omap, string_literal_macro( "Grapefruit" ), string_literal_macro( "Tomato" ) ) ); \
  UNTIL_SUCCESS( get_or_insert( &our_omap, string_literal_macro( "Melon" ), string_literal_macro( "Garlic" ) ) );      \
  UNTIL_SUCCESS( get_or_insert( &our_omap, string_literal_macro( "Orange" ), string_literal_macro( "Capsicum" ) ) );   \
  UNTIL_SUCCESS( get_or_insert( &our_omap, string_literal_macro( "Pear" ), string_literal_macro( "Spinach" ) ) );      \
  el = get( &our_omap, string_literal_macro( "Grapefruit" ) );                                                         \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Tomato" ) ) );                                \
  el = get( &our_omap, string_literal_macro( "Melon" ) );                                                              \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Garlic" ) ) );                                \
  el = get( &our_omap, string_literal_macro( "Orange" ) );                                                             \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Carrot" ) ) );                                \
  el = get( &our_omap, string_literal_macro( "Pear" ) );                                                               \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Onion" ) ) );                                 \
                                                                                                                       \
  cleanup( &our_omap );                                                                                                \
}                                                                                                                      \

static void test_str_interoperability_omap( void )
{
  TEST_STR_OMAP_INEROPERABILITY( char, STRING_LITERAL_CHAR, compare_strings8 );
  TEST_STR_OMAP_INEROPERABILITY( unsigned char, STRING_LITERAL_UNSIGNED_CHAR, compare_strings8 );
  TEST_STR_OMAP_INEROPERABILITY( signed char, STRING_LITERAL_SIGNED_CHAR, compare_strings8 );
#if ( defined( __cplusplus ) && __cplusplus >= 202101L ) ||        \
    ( defined( __STDC_VERSION__ ) && __STDC_VERSION__ >= 202311L )
  TEST_STR_OMAP_INEROPERABILITY( char8_t, STRING_LITERAL_CHAR8, compare_strings8 );
#endif
  TEST_STR_OMAP_INEROPERABILITY( char16_t, STRING_LITERAL_CHAR16, compare_strings16 );
  TEST_STR_OMAP_INEROPERABILITY( char32_t, STRING_LITERAL_CHAR32, compare_strings32 );
}

#define TEST_STR_OSET_INEROPERABILITY( ty, string_literal_macro, compare_strings_fn )                            \
{                                                                                                                \
  set( str( ty ) ) our_oset;                                                                                     \
  init( &our_oset );                                                                                             \
                                                                                                                 \
  /* Insert. */                                                                                                  \
                                                                                                                 \
  str( ty ) our_str_1 = initialized( &our_str_1 );                                                               \
  str( ty ) our_str_2 = initialized( &our_str_2 );                                                               \
  str( ty ) our_str_3 = initialized( &our_str_3 );                                                               \
  UNTIL_SUCCESS( push_fmt( &our_str_1, string_literal_macro( "Short string" ) ) );                               \
  UNTIL_SUCCESS( push_fmt( &our_str_2, string_literal_macro( "Medium-length string" ) ) );                       \
  UNTIL_SUCCESS( push_fmt( &our_str_3, string_literal_macro( "This here is a significantly longer string" ) ) ); \
                                                                                                                 \
  UNTIL_SUCCESS( insert( &our_oset, our_str_1 ) );                                                               \
  UNTIL_SUCCESS( insert( &our_oset, our_str_2 ) );                                                               \
  UNTIL_SUCCESS( insert( &our_oset, our_str_3 ) );                                                               \
                                                                                                                 \
  /* Look-up. */                                                                                                 \
                                                                                                                 \
  str( ty ) our_str_4 = initialized( &our_str_4 );                                                               \
  str( ty ) our_str_5 = initialized( &our_str_5 );                                                               \
  str( ty ) our_str_6 = initialized( &our_str_6 );                                                               \
  UNTIL_SUCCESS( push_fmt( &our_str_4, string_literal_macro( "Short string" ) ) );                               \
  UNTIL_SUCCESS( push_fmt( &our_str_5, string_literal_macro( "Medium-length string" ) ) );                       \
  UNTIL_SUCCESS( push_fmt( &our_str_6, string_literal_macro( "This here is a significantly longer string" ) ) ); \
                                                                                                                 \
  str( ty ) *el;                                                                                                 \
  el = get( &our_oset, our_str_4 );                                                                              \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Short string" ) ) );                    \
  el = get( &our_oset, our_str_5 );                                                                              \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Medium-length string" ) ) );            \
  el = get( &our_oset, our_str_6 );                                                                              \
  ALWAYS_ASSERT(                                                                                                 \
    compare_strings_fn( first( el ), string_literal_macro( "This here is a significantly longer string" ) )      \
  );                                                                                                             \
                                                                                                                 \
  cleanup( &our_str_4 );                                                                                         \
  cleanup( &our_str_5 );                                                                                         \
  cleanup( &our_str_6 );                                                                                         \
                                                                                                                 \
  /* Replace. */                                                                                                 \
  str( ty ) our_str_7 = initialized( &our_str_7 );                                                               \
  UNTIL_SUCCESS( push_fmt( &our_str_7, string_literal_macro( "Short string" ) ) );                               \
  UNTIL_SUCCESS( el = insert( &our_oset, our_str_7 ) );                                                          \
  ALWAYS_ASSERT(                                                                                                 \
    *el == our_str_7 && compare_strings_fn( first( el ), string_literal_macro( "Short string" ) )                \
  );                                                                                                             \
                                                                                                                 \
  /* Heterogeneous get. */                                                                                       \
  el = get( &our_oset, string_literal_macro( "Short string" ) );                                                 \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Short string" ) ) );                    \
  el = get( &our_oset, string_literal_macro( "Medium-length string" ) );                                         \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Medium-length string" ) ) );            \
  el = get( &our_oset, string_literal_macro( "This here is a significantly longer string" ) );                   \
  ALWAYS_ASSERT(                                                                                                 \
    compare_strings_fn( first( el ), string_literal_macro( "This here is a significantly longer string" ) )      \
  );                                                                                                             \
                                                                                                                 \
  /* Heterogeneous erase. */                                                                                     \
  ALWAYS_ASSERT( erase( &our_oset, string_literal_macro( "Short string" ) ) );                                   \
  ALWAYS_ASSERT( erase( &our_oset, string_literal_macro( "Medium-length string" ) ) );                           \
  ALWAYS_ASSERT( erase( &our_oset, string_literal_macro( "This here is a significantly longer string" ) ) );     \
  ALWAYS_ASSERT( !get( &our_oset, string_literal_macro( "Short string" ) ) );                                    \
  ALWAYS_ASSERT( !get( &our_oset, string_literal_macro( "Medium-length string" ) ) );                            \
  ALWAYS_ASSERT( !get( &our_oset, string_literal_macro( "This here is a significantly longer string" ) ) );      \
                                                                                                                 \
  /* Heterogenous insert. */                                                                                     \
  UNTIL_SUCCESS( insert( &our_oset, string_literal_macro( "Apple" ) ) );                                         \
  UNTIL_SUCCESS( insert( &our_oset, string_literal_macro( "Orange" ) ) );                                        \
  UNTIL_SUCCESS( insert( &our_oset, string_literal_macro( "Pear" ) ) );                                          \
  el = get( &our_oset, string_literal_macro( "Apple" ) );                                                        \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Apple" ) ) );                           \
  el = get( &our_oset, string_literal_macro( "Orange" ) );                                                       \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Orange" ) ) );                          \
  el = get( &our_oset, string_literal_macro( "Pear" ) );                                                         \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Pear" ) ) );                            \
                                                                                                                 \
  /* Heterogenous get or insert. */                                                                              \
  UNTIL_SUCCESS( get_or_insert( &our_oset, string_literal_macro( "Grapefruit" ) ) );                             \
  UNTIL_SUCCESS( get_or_insert( &our_oset, string_literal_macro( "Melon" ) ) );                                  \
  str( ty ) *orange = get( &our_oset, string_literal_macro( "Orange" ) );                                        \
  str( ty ) *pear = get( &our_oset, string_literal_macro( "Pear" ) );                                            \
  UNTIL_SUCCESS( get_or_insert( &our_oset, string_literal_macro( "Orange" ) ) );                                 \
  UNTIL_SUCCESS( get_or_insert( &our_oset, string_literal_macro( "Pear" ) ) );                                   \
  el = get( &our_oset, string_literal_macro( "Grapefruit" ) );                                                   \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Grapefruit" ) ) );                      \
  el = get( &our_oset, string_literal_macro( "Melon" ) );                                                        \
  ALWAYS_ASSERT( compare_strings_fn( first( el ), string_literal_macro( "Melon" ) ) );                           \
  el = get( &our_oset, string_literal_macro( "Orange" ) );                                                       \
  ALWAYS_ASSERT( el == orange && compare_strings_fn( first( el ), string_literal_macro( "Orange" ) ) );          \
  el = get( &our_oset, string_literal_macro( "Pear" ) );                                                         \
  ALWAYS_ASSERT( el == pear && compare_strings_fn( first( el ), string_literal_macro( "Pear" ) ) );              \
                                                                                                                 \
  cleanup( &our_oset );                                                                                          \
}                                                                                                                \

static void test_str_interoperability_oset( void )
{
  TEST_STR_OSET_INEROPERABILITY( char, STRING_LITERAL_CHAR, compare_strings8 );
  TEST_STR_OSET_INEROPERABILITY( unsigned char, STRING_LITERAL_UNSIGNED_CHAR, compare_strings8 );
  TEST_STR_OSET_INEROPERABILITY( signed char, STRING_LITERAL_SIGNED_CHAR, compare_strings8 );
#if ( defined( __cplusplus ) && __cplusplus >= 202101L ) ||        \
    ( defined( __STDC_VERSION__ ) && __STDC_VERSION__ >= 202311L )
  TEST_STR_OSET_INEROPERABILITY( char8_t, STRING_LITERAL_CHAR8, compare_strings8 );
#endif
  TEST_STR_OSET_INEROPERABILITY( char16_t, STRING_LITERAL_CHAR16, compare_strings16 );
  TEST_STR_OSET_INEROPERABILITY( char32_t, STRING_LITERAL_CHAR32, compare_strings32 );
}

#endif

// Test in-situ initialization of global containers.
// Specifically, we need ascertain that cc_initialized is, in fact, a constant expression.
// There is no need for cleanup in this specific case because initialization allocates no memory.
vec( int ) global_initialization_test = initialized( &global_initialization_test );

int main( void )
{
  srand( (unsigned int)time( NULL ) );

  // Repeat the tests 1000 times since realloc failures are random.
  printf( "Running unit tests...\n" );
  for( int i = 0; i < 1000; ++i )
  {
    #ifdef TEST_VEC
    // vec, init, size, cap, and get are tested implicitly.
    test_vec_reserve();
    test_vec_resize();
    test_vec_shrink();
    test_vec_insert();
    test_vec_insert_n();
    test_vec_push();
    test_vec_push_n();
    test_vec_erase();
    test_vec_erase_n();
    test_vec_clear();
    test_vec_cleanup();
    test_vec_iteration();
    test_vec_init_clone();
    test_vec_dtors();
    #endif

    #ifdef TEST_LIST
    // list, init, and size are tested implicitly.
    test_list_insert();
    test_list_push();
    test_list_splice();
    test_list_erase();
    test_list_clear();
    test_list_cleanup();
    test_list_iteration();
    test_list_init_clone();
    test_list_dtors();
    #endif

    #ifdef TEST_MAP
    // map, init, cap, and size are tested implicitly.
    test_map_reserve();
    test_map_shrink();
    test_map_insert();
    test_map_get_or_insert();
    test_map_get();
    test_map_erase();
    test_map_erase_itr();
    test_map_clear();
    test_map_cleanup();
    test_map_init_clone();
    test_map_iteration_and_get_key();
    test_map_dtors();
    test_map_strings();
    test_map_default_integer_types();
    #endif

    #ifdef TEST_SET
    // set, init, cap, and size are tested implicitly.
    test_set_reserve();
    test_set_shrink();
    test_set_insert();
    test_set_get_or_insert();
    test_set_get();
    test_set_erase();
    test_set_erase_itr();
    test_set_clear();
    test_set_cleanup();
    test_set_init_clone();
    test_set_iteration();
    test_set_dtors();
    test_set_strings();
    test_set_default_integer_types();
    #endif

    #ifdef TEST_OMAP
    // omap, init, and size are tested implicitly.
    test_omap_insert();
    test_omap_get_or_insert();
    test_omap_get();
    test_omap_erase();
    test_omap_erase_itr();
    test_omap_clear();
    test_omap_cleanup();
    test_omap_init_clone();
    test_omap_iteration_and_get_key();
    test_omap_iteration_over_range();
    test_omap_dtors();
    test_omap_strings();
    test_omap_default_integer_types();
    #endif

    #ifdef TEST_OSET
    // oset, init, and size are tested implicitly.
    test_oset_insert();
    test_oset_get_or_insert();
    test_oset_get();
    test_oset_erase();
    test_oset_erase_itr();
    test_oset_clear();
    test_oset_cleanup();
    test_oset_init_clone();
    test_oset_iteration();
    test_oset_iteration_over_range();
    test_oset_dtors();
    test_oset_strings();
    test_oset_default_integer_types();
    #endif

    #ifdef TEST_STR
    // str, init, and size are tested implicitly.
    test_str_reserve();
    test_str_resize();
    test_str_shrink();
    test_str_insert();
    test_str_insert_fmt();
    test_str_insert_n();
    test_str_push();
    test_str_push_fmt();
    test_str_push_n();
    test_str_erase();
    test_str_erase_n();
    test_str_clear();
    test_str_cleanup();
    test_str_iteration();
    test_str_init_clone();
    test_str_interoperability_vec();
    test_str_interoperability_list();
    test_str_interoperability_map();
    test_str_interoperability_set();
    test_str_interoperability_omap();
    test_str_interoperability_oset();
    #endif
  }

  ALWAYS_ASSERT( oustanding_allocs == 0 );
  printf( "All done.\n" );
  printf( "Simulated realloc failures: %zu\n", simulated_alloc_failures );
}
