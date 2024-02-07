/*
CC/tests/unit_tests.c - v1.0.5

This file tests CC containers.
It aims to cover the full API and to check corner cases, particularly transitions between placeholder containers and
non-placeholder containers.

License (MIT):

  Copyright (c) 2022-2024 Jackson L. Allan

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

void *unreliable_tracking_realloc( void *ptr, size_t size )
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

void tracking_free( void *ptr )
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
void check_dtors_arr( void )
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
#define CC_HASH custom_ty, { return val.val * 2654435761ull; }
#define CC_LOAD custom_ty, 0.7
#include "../cc.h"

// Vector tests.
#ifdef TEST_VEC

#define VEC_CHECK                                                              \
ALWAYS_ASSERT( size( &our_vec ) == sizeof( expected ) / sizeof( *expected ) ); \
for( size_t i = 0; i < size( &our_vec ); ++i )                                 \
  ALWAYS_ASSERT( *get( &our_vec, i ) == expected[ i ] )                        \

void test_vec_reserve( void )
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

void test_vec_resize( void )
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
  ALWAYS_ASSERT( cap( &our_vec ) >= 0 );
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
  for( int i = 0; i < 30; ++i )
    *get( &our_vec, i ) = i;

  VEC_CHECK;

  cleanup( &our_vec );
}

void test_vec_shrink( void )
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
  for( int i = 0; i < 30; ++i )
    *get( &our_vec, i ) = i;

  VEC_CHECK;

  cleanup( &our_vec ); 
}

void test_vec_insert( void )
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

void test_vec_insert_n( void )
{
  vec( int ) our_vec;
  init( &our_vec );

  int expected [ 30 ] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29
  };

  ALWAYS_ASSERT( !insert_n( &our_vec, size( &our_vec ), NULL, 0 ) ); // Zero-size insert.

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

void test_vec_push( void )
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

void test_vec_push_n( void )
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

void test_vec_erase( void )
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
  for( int i = 0; i < 50; )
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

void test_vec_erase_n( void )
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
  for( int i = 0; i < 50; )
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

void test_vec_clear( void )
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

void test_vec_cleanup( void )
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

void test_vec_iteration( void )
{
  vec( int ) our_vec;
  init( &our_vec );

  int expected [ 30 ] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29
  };

  // Empty.
  int n_iterations = 0;
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

void test_vec_init_clone( void )
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

void test_vec_dtors( void )
{
  vec( custom_ty ) our_vec;
  init( &our_vec );

  // Test erase and clear.

  UNTIL_SUCCESS( resize( &our_vec, 100 ) );
  for( int i = 0; i < 100; ++i )
    get( &our_vec, i )->val = i;

  bool erase = true;
  for( int i = 0; i < 50; )
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
  for( int i = 0; i < 100; ++i )
    get( &our_vec, i )->val = i;

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

void test_list_insert( void )
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

void test_list_push( void )
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

void test_list_splice( void )
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

void test_list_erase( void )
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

void test_list_clear( void )
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

void test_list_cleanup( void )
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

// This needs to test, in particular, that r_end and end iterator-pointers are stable, especially between transition
// from placeholder to non-placeholder.
void test_list_iteration( void )
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

  int n_iterations = 0;
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

  // Test fist and last.
  ALWAYS_ASSERT( *first( &our_list ) == 0 );
  ALWAYS_ASSERT( *last( &our_list ) == 29 );

  LIST_CHECK;

  cleanup( &our_list );
}

void test_list_init_clone( void )
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

void test_list_dtors( void )
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

void test_map_reserve( void )
{
  map( int, size_t ) our_map;
  init( &our_map );

  // Reserve zero with placeholder.
  UNTIL_SUCCESS( reserve( &our_map, 0 ) );
  ALWAYS_ASSERT( (void *)our_map  == (void *)&cc_map_placeholder );

  // Reserve up from placeholder.
  UNTIL_SUCCESS( reserve( &our_map, 30 ) );
  ALWAYS_ASSERT( 30 <= cap( &our_map ) * CC_DEFAULT_LOAD );

  // Reserve same capacity.
  size_t cap = cap( &our_map );
  UNTIL_SUCCESS( reserve( &our_map, 30 ) );
  ALWAYS_ASSERT( cap( &our_map ) == cap );

  // Reserve up from non-placeholder.
  UNTIL_SUCCESS( reserve( &our_map, 60 ) );
  ALWAYS_ASSERT( 60 <= cap( &our_map ) * CC_DEFAULT_LOAD );

  // Reserve lower capacity.
  cap = cap( &our_map );
  UNTIL_SUCCESS( reserve( &our_map, 30 ) );
  ALWAYS_ASSERT( cap( &our_map ) == cap );

  // Test validity through use.
  for( int i = 0; i < 60; ++i )
    UNTIL_SUCCESS( insert( &our_map, i, i + 1 ) );

  // Check.
  ALWAYS_ASSERT( size( &our_map ) == 60 );
  for( int i = 0; i < 60; ++i )
    ALWAYS_ASSERT( *get( &our_map, i ) == (size_t)i + 1 );

  cleanup( &our_map );
}

void test_map_shrink( void )
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
    insert( &our_map, i, i + 1 );

  ALWAYS_ASSERT( size( &our_map ) == 30 );
  map( int, size_t ) same = our_map;
  size_t cap = cap( &our_map );
  UNTIL_SUCCESS( shrink( &our_map ) );
  ALWAYS_ASSERT( our_map == same );
  ALWAYS_ASSERT( cap( &our_map ) == cap );

  // Test shrink down.
  UNTIL_SUCCESS( reserve( &our_map, 500 ) );
  ALWAYS_ASSERT( size( &our_map ) == 30 );
  ALWAYS_ASSERT( 500 <= cap( &our_map ) * CC_DEFAULT_LOAD );
  UNTIL_SUCCESS( shrink( &our_map ) );
  ALWAYS_ASSERT( size( &our_map ) == 30 );
  ALWAYS_ASSERT( cap( &our_map ) == cap );

  // Check.
  for( int i = 0; i < 30; ++i )
    ALWAYS_ASSERT( *get( &our_map, i ) == (size_t)i + 1 );

  cleanup( &our_map ); 
}

void test_map_insert( void )
{
  map( int, size_t ) our_map;
  init( &our_map );

  // Insert new.
  for( int i = 0; i < 100; ++i )
  {
    size_t *el;
    UNTIL_SUCCESS( el = insert( &our_map, i, i + 1 ) );
    ALWAYS_ASSERT( *el == (size_t)i + 1 );
  }

  // Insert existing.
  for( int i = 0; i < 100; ++i )
  {
    size_t *el;
    UNTIL_SUCCESS( el = insert( &our_map, i, i + 2 ) );
    ALWAYS_ASSERT( *el == (size_t)i + 2 );
  }

  // Check.
  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( *get( &our_map, i ) == (size_t)i + 2 );

  cleanup( &our_map );
}

void test_map_get_or_insert( void )
{
  map( int, size_t ) our_map;
  init( &our_map );

  // Test insert.
  for( int i = 0; i < 100; ++i )
  {
    size_t *el;
    UNTIL_SUCCESS( ( el = get_or_insert( &our_map, i, i + 1 ) ) );
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
    UNTIL_SUCCESS( ( el_2 = get_or_insert( &our_map, i, i + 1 ) ) );
    ALWAYS_ASSERT( el_2 == el_1 && *el_2 == (size_t)i + 1 );
  }

  ALWAYS_ASSERT( size( &our_map ) == 100 );

  cleanup( &our_map );
}

void test_map_get( void )
{
  map( int, size_t ) our_map;
  init( &our_map );

  // Test empty.
  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( !get( &our_map, i ) );

  // Test get existing.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_map, i, i + 1 ) );

  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( *get( &our_map, i ) == (size_t)i + 1 );

  // Test get non-existing.
  for( int i = 100; i < 200; ++i )
    ALWAYS_ASSERT( !get( &our_map, i ) );

  cleanup( &our_map );
}

void test_map_erase( void )
{
  map( int, size_t ) our_map;
  init( &our_map );

  // Test erase existing.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_map, i, i + 1 ) );

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

void test_map_erase_itr( void )
{
  map( int, size_t ) our_map;
  init( &our_map );

  // Test erase existing.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_map, i, i + 1 ) );

  ALWAYS_ASSERT( size( &our_map ) == 100 );

  for( int i = 0; i < 100; i += 2 )
    erase_itr( &our_map, get( &our_map, i ) );

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


void test_map_clear( void )
{
  map( int, size_t ) our_map;
  init( &our_map );

  // Test empty.
  clear( &our_map );
  ALWAYS_ASSERT( size( &our_map ) == 0 );

  // Test non-empty;
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_map, i, i + 1 ) );

  clear( &our_map );
  ALWAYS_ASSERT( size( &our_map ) == 0 );
  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( !get( &our_map, i ) );

  // Test reuse.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_map, i, i + 1 ) );

  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( *get( &our_map, i ) == (size_t)i + 1 );

  cleanup( &our_map );
}

void test_map_cleanup( void )
{
  map( int, size_t ) our_map;
  init( &our_map );

  // Empty.
  cleanup( &our_map );
  ALWAYS_ASSERT( (void *)our_map == (void *)&cc_map_placeholder );

  // Non-empty.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_map, i, i + 1 ) );
  ALWAYS_ASSERT( size( &our_map ) == 100 );
  cleanup( &our_map );
  ALWAYS_ASSERT( size( &our_map ) == 0 );
  ALWAYS_ASSERT( (void *)our_map == (void *)&cc_map_placeholder );

  // Test use.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_map, i, i + 1 ) );
  for( int i = 0; i < 100; ++i )
    ALWAYS_ASSERT( *get( &our_map, i ) == (size_t)i + 1 );

  cleanup( &our_map );
}

void test_map_init_clone( void )
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
    UNTIL_SUCCESS( insert( &src_map, i, i + 1 ) );
  UNTIL_SUCCESS( init_clone( &our_map, &src_map ) );

  // Check.
  ALWAYS_ASSERT( size( &our_map ) == 10 );
  for( int i = 0; i < 10; ++i )
    ALWAYS_ASSERT( *get( &our_map, i ) == (size_t)i + 1 );

  cleanup( &src_map );
  cleanup( &empty_map );
  cleanup( &our_map );
}

void test_map_iteration_and_get_key( void )
{
  map( int, size_t ) our_map;
  init( &our_map );

  // Empty.

  // Test first and last.
  ALWAYS_ASSERT( first( &our_map ) == end( &our_map ) );
  ALWAYS_ASSERT( last( &our_map ) == r_end( &our_map ) );

  int n_iterations = 0;
  for( size_t *i = first( &our_map ); i != end( &our_map ); i = next( &our_map, i ) )
    ++n_iterations;
  for( size_t *i = last( &our_map ); i != r_end( &our_map ); i = prev( &our_map, i ) )
    ++n_iterations;
  for_each( &our_map, i )
    ++n_iterations;
  r_for_each( &our_map, i )
    ++n_iterations;
  for_each( &our_map, k, i )
    ++n_iterations;
  r_for_each( &our_map, k, i )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 0 );

  // Non-empty.
  for( int i = 0; i < 30; ++i )
    UNTIL_SUCCESS( insert( &our_map, i, i + 1 ) );
  
  for( size_t *i = first( &our_map ); i != end( &our_map ); i = next( &our_map, i ) )
  {
    ALWAYS_ASSERT( (size_t)*key_for( &our_map, i ) == *i - 1 );
    ++n_iterations;
  }
  for( size_t *i = last( &our_map ); i != r_end( &our_map ); i = prev( &our_map, i ) )
  {
    ALWAYS_ASSERT( (size_t)*key_for( &our_map, i ) == *i - 1 );
    ++n_iterations;
  }

  for_each( &our_map, i )
    ++n_iterations;
  r_for_each( &our_map, i )
    ++n_iterations;

  for_each( &our_map, k, i )
  {
    ALWAYS_ASSERT( (size_t)*k == *i - 1 );
    ++n_iterations;
  }
  r_for_each( &our_map, k, i )
  {
    ALWAYS_ASSERT( (size_t)*k == *i - 1 );
    ++n_iterations;
  }

  ALWAYS_ASSERT( n_iterations == 180 );

  // Iteration over empty, non-placeholder map.

  clear( &our_map );

  n_iterations = 0;

  for_each( &our_map, i )
    ++n_iterations;

  r_for_each( &our_map, i )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 0 );

  cleanup( &our_map );
}

void test_map_dtors( void )
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

// Strings are a special case that warrant seperate testing.
void test_map_strings( void )
{
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
}

#define TEST_MAP_DEFAULT_INTEGER_TYPE( ty )    \
{                                              \
  map( ty, int ) our_map;                      \
  init( &our_map );                            \
                                               \
  for( int i = 0; i < 100; ++i )               \
    UNTIL_SUCCESS( insert( &our_map, i, i ) ); \
                                               \
  for( int i = 0; i < 100; ++i )               \
    ALWAYS_ASSERT( *get( &our_map, i ) == i ); \
                                               \
  cleanup( &our_map );                         \
}                                              \

void test_map_default_integer_types( void )
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

void test_set_reserve( void )
{
  set( int ) our_set;
  init( &our_set );

  // Reserve zero with placeholder.
  UNTIL_SUCCESS( reserve( &our_set, 0 ) );
  ALWAYS_ASSERT( (void *)our_set == (void *)&cc_map_placeholder );

  // Reserve up from placeholder.
  UNTIL_SUCCESS( reserve( &our_set, 30 ) );
  ALWAYS_ASSERT( 30 <= cap( &our_set ) * CC_DEFAULT_LOAD );

  // Reserve same capacity.
  size_t cap = cap( &our_set );
  UNTIL_SUCCESS( reserve( &our_set, 30 ) );
  ALWAYS_ASSERT( cap( &our_set ) == cap );

  // Reserve up from non-placeholder.
  UNTIL_SUCCESS( reserve( &our_set, 60 ) );
  ALWAYS_ASSERT( 60 <= cap( &our_set ) * CC_DEFAULT_LOAD );

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

void test_set_shrink( void )
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
  ALWAYS_ASSERT( 500 <= cap( &our_set ) * CC_DEFAULT_LOAD );
  UNTIL_SUCCESS( shrink( &our_set ) );

  ALWAYS_ASSERT( size( &our_set ) == 30 );
  ALWAYS_ASSERT( cap( &our_set ) == cap );

  // Check.
  for( int i = 0; i < 30; ++i )
    ALWAYS_ASSERT( *get( &our_set, i ) == i );

  cleanup( &our_set ); 
}

void test_set_insert( void )
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

void test_set_get_or_insert( void )
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

void test_set_get( void )
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

void test_set_erase( void )
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

void test_set_erase_itr( void )
{
  set( int ) our_set;
  init( &our_set );

  // Test erase existing.
  for( int i = 0; i < 100; ++i )
    UNTIL_SUCCESS( insert( &our_set, i ) );

  ALWAYS_ASSERT( size( &our_set ) == 100 );

  for( int i = 0; i < 100; i += 2 )
    erase_itr( &our_set, get( &our_set, i ) );

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

void test_set_clear( void )
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

void test_set_cleanup( void )
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

void test_set_init_clone( void )
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

void test_set_iteration( void )
{
  set( int ) our_set;
  init( &our_set );

  // Empty.

  // Test first and last.
  ALWAYS_ASSERT( first( &our_set ) == end( &our_set ) );
  ALWAYS_ASSERT( last( &our_set ) == r_end( &our_set ) );

  int n_iterations = 0;
  for( int *i = first( &our_set ); i != end( &our_set ); i = next( &our_set, i ) )
    ++n_iterations;
  for( int *i = last( &our_set ); i != r_end( &our_set ); i = prev( &our_set, i ) )
    ++n_iterations;
  for_each( &our_set, i )
    ++n_iterations;
  r_for_each( &our_set, i )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 0 );

  // Non-empty.
  for( int i = 0; i < 30; ++i )
    UNTIL_SUCCESS( insert( &our_set, i ) );
  
  for( int *i = first( &our_set ); i != end( &our_set ); i = next( &our_set, i ) )
    ++n_iterations;
  for( int *i = last( &our_set ); i != r_end( &our_set ); i = prev( &our_set, i ) )
    ++n_iterations;

  for_each( &our_set, i )
    ++n_iterations;

  r_for_each( &our_set, i )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 120 );

  // Iteration over empty, non-placeholder set.

  clear( &our_set );

  n_iterations = 0;

  for_each( &our_set, i )
    ++n_iterations;

  r_for_each( &our_set, i )
    ++n_iterations;

  ALWAYS_ASSERT( n_iterations == 0 );

  cleanup( &our_set );
}

void test_set_dtors( void )
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

void test_set_strings( void )
{
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

void test_set_default_integer_types( void )
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

int main( void )
{
  srand( time( NULL ) );

  // Repeat 1000 times since realloc failures are random.
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
  }

  ALWAYS_ASSERT( oustanding_allocs == 0 );
  printf( "All done.\n" );
  printf( "Simulated realloc failures: %zu\n", simulated_alloc_failures );
}
