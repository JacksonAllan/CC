/*

Convenient Containers v1.2.0 - tests/tests_against_stl.cpp

This file tests CC containers against equivalent C++ STL containers.
Primarily, it checks that a CC container and its equivalent STL container end up in the same state after a random
series of the same operations are performed on both.
It also checks that results of API calls that return iterators are as expected.
Note that the list tests will be considerably slower than the tests for the other containers because lists realloc
for every new node.

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

#include <ctime>
#include <iostream>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define CC_NO_SHORT_NAMES
#include "../cc.h"

// Assert macro that is not disabled by NDEBUG.
#define ALWAYS_ASSERT( xp )                                                                                     \
( (xp) ? (void)0 : ( std::cerr << "Assertion failed at line " << __LINE__ << ": " << #xp << '\n', exit( 0 ) ) ) \

// Macros to control the number of random operations to perform in each container test and the number of tests to
// perform on each container.
#define N_OPS 50000
#define N_TESTS 5

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
// If SIMULATE_ALLOC_FAILURES is defined above, unreliable_tracking_realloc will also sporadically fail.

bool failing_alloc_on = true; // Even when SIMULATE_ALLOC_FAILURES is enabled, we need to be able to toggle this
                              // behavior because in some instances (e.g. cloning lists), the operation would never
                              // succeed with a fixed realloc failure rate.

size_t simulated_alloc_failures;
std::unordered_set<void *> oustanding_allocs;

void *unreliable_tracking_realloc( void *ptr, size_t size )
{
#ifdef SIMULATE_ALLOC_FAILURES
  if( failing_alloc_on && rand() % 5 == 0 )
  {
    ++simulated_alloc_failures;
    return NULL;
  }
#endif

  if( ptr )
    oustanding_allocs.erase( ptr );

  void *new_ptr = realloc( ptr, size );
  ALWAYS_ASSERT( new_ptr );

  oustanding_allocs.insert( new_ptr );

  return new_ptr;
}

void tracking_free( void *ptr )
{
  if( ptr )
    oustanding_allocs.erase( ptr );

  free( ptr );
}

// Activate the custom realloc and free functions.
#define CC_REALLOC unreliable_tracking_realloc
#define CC_FREE tracking_free

int main()
{
  srand( std::time( nullptr ) );

  // Vector.
  for( int test = 0; test < N_TESTS; ++test )
  {
    std::cout << "Vec test " << test << "... ";
    std::vector<int> stl_vec;
    cc_vec( int ) our_vec;
    cc_init( &our_vec );

    for( int op = 0; op < N_OPS; ++op )
    {
      switch( rand() % 10 )
      {
        case 0: // cc_push.
        {
          int *el;
          int el_val = rand();
          UNTIL_SUCCESS( ( el = cc_push( &our_vec, el_val ) ) );

          ALWAYS_ASSERT( *el == el_val );

          stl_vec.push_back( el_val );
        }
        break;
        case 1: // cc_push_n.
        {
          int *el;
          int els[ 5 ] = { rand(), rand(), rand(), rand(), rand() };
          UNTIL_SUCCESS( ( el = cc_push_n( &our_vec, els, 5 ) ) );

          ALWAYS_ASSERT( *el == els[ 0 ] );

          stl_vec.insert( stl_vec.end(), std::begin( els ), std::end( els ) );
        }
        break;
        case 2: // cc_insert.
        {
          size_t i = 0;
          if( cc_size( &our_vec ) )
            i = rand() % ( cc_size( &our_vec ) + 1 );

          int *el;
          int el_val = rand();
          UNTIL_SUCCESS( ( el = cc_insert( &our_vec, i, el_val ) ) );

          ALWAYS_ASSERT( *el == el_val );

          stl_vec.insert( stl_vec.begin() + i, el_val );
        }
        break;
        case 3: // cc_insert_n.
        {
          size_t i = 0;
          if( cc_size( &our_vec ) )
            i = rand() % ( cc_size( &our_vec ) + 1 );

          int *el;
          int els[ 5 ] = { rand(), rand(), rand(), rand(), rand() };
          UNTIL_SUCCESS( ( el = cc_insert_n( &our_vec, i, els, 5 ) ) );

          ALWAYS_ASSERT( *el == els[ 0 ] );

          stl_vec.insert( stl_vec.begin() + i, std::begin( els ), std::end( els ) );
        }
        break;
        case 4: // cc_erase.
        {
          if( !cc_size( &our_vec ) )
            break;

          size_t i = rand() % cc_size( &our_vec );
          int *el = cc_erase( &our_vec, i );

          if( i == cc_size( &our_vec ) )
            ALWAYS_ASSERT( el == cc_end( &our_vec ) );
          else
            ALWAYS_ASSERT( *el == *cc_get( &our_vec, i ) );

          stl_vec.erase( stl_vec.begin() + i );
        }
        break;
        case 5: // cc_erase_n.
        {
          if( !cc_size( &our_vec ) )
            break;

          size_t i = rand() % cc_size( &our_vec );
          size_t n = rand() % 5;
          if( i + n > cc_size( &our_vec ) )
            break;

          int *el = cc_erase_n( &our_vec, i, n );

          if( i == cc_size( &our_vec ) )
            ALWAYS_ASSERT( el == cc_end( &our_vec ) );
          else
            ALWAYS_ASSERT( *el == *cc_get( &our_vec, i ) );         

          stl_vec.erase( stl_vec.begin() + i, stl_vec.begin() + i + n );
        }
        break;
        case 6: // cc_reserve.
        {
          if( rand() % 2 )
            UNTIL_SUCCESS( cc_reserve( &our_vec, cc_cap( &our_vec ) + 5 ) ); // Reserve above current capacity.
          else if( cc_cap( &our_vec ) >= 5 )
            UNTIL_SUCCESS( cc_reserve( &our_vec, cc_cap( &our_vec ) - 5 ) ); // Reserve below current capacity.
        }
        break;
        case 7: // cc_resize.
        {
          if( rand() % 2 )
          {
            int els[ 5 ] = { rand(), rand(), rand(), rand(), rand() };
            UNTIL_SUCCESS( cc_resize( &our_vec, cc_size( &our_vec ) + 5 ) );
            stl_vec.resize( stl_vec.size() + 5 );
            memcpy( cc_get( &our_vec, cc_size( &our_vec ) - 5 ), els, sizeof( els ) );
            memcpy( &stl_vec[ stl_vec.size() - 5 ], els, sizeof( els ) );
          }
          else if( cc_size( &our_vec ) >= 5 )
          {
            UNTIL_SUCCESS( cc_resize( &our_vec, cc_size( &our_vec ) - 5 ) );
            stl_vec.resize( stl_vec.size() - 5 );
          }
        }
        break;
        case 8: // cc_shrink.
        {
          UNTIL_SUCCESS( cc_shrink( &our_vec ) );
        }
        break;
        case 9: // cc_init_clone.
        {
          cc_vec( int ) clone;
          UNTIL_SUCCESS( cc_init_clone( &clone, &our_vec ) );
          cc_cleanup( &our_vec );
          our_vec = clone;
        }
        break;
      }
    }

    // Check.
    ALWAYS_ASSERT( cc_size( &our_vec ) == stl_vec.size() );
    for( size_t i = 0; i < cc_size( &our_vec ); ++i )
      ALWAYS_ASSERT( *cc_get( &our_vec, i ) == stl_vec[ i ] );

    std::cout << "Done. Final size: " << cc_size( &our_vec ) << "\n";
    cc_cleanup( &our_vec );
  }

  // List.
  for( int test = 0; test < N_TESTS; ++test )
  {
    std::cout << "List test " << test << "... ";

    // Because of splicing, we need to test using two cc_lists and two STL lists.
    std::list<int> stl_list[ 2 ];

    cc_list( int ) our_list[ 2 ];
    cc_init( &our_list[ 0 ] );
    cc_init( &our_list[ 1 ] );

    for( int op = 0; op < N_OPS; ++op )
    {
      bool list = rand() % 2;

      switch( rand() % 6 )
      {
        case 0: // cc_push.
        {
          int *el;
          int el_val = rand();
          UNTIL_SUCCESS( ( el = cc_push( &our_list[ list ], el_val ) ) );

          ALWAYS_ASSERT( *el == el_val );

          stl_list[ list ].push_back( el_val );
        }
        break;
        case 1: // cc_insert.
        {
          size_t i = 0;
          if( cc_size( &our_list[ list ] ) != 0 )
            i = rand() % cc_size( &our_list[ list ] );
          
          int *cc_itr = cc_first( &our_list[ list ] );
          for( size_t j = 0; j < i; ++j )
            cc_itr = cc_next( &our_list[ list ], cc_itr );

          auto stl_itr = stl_list[ list ].begin();
          std::advance( stl_itr, i );

          int *el;
          int el_val = rand();
          UNTIL_SUCCESS( ( el = cc_insert( &our_list[ list ], cc_itr, el_val ) ) );

          ALWAYS_ASSERT( *el == el_val );

          stl_list[ list ].insert( stl_itr, el_val );
        }
        break;
        case 2: // cc_erase.
        {
          if( cc_size( &our_list[ list ] ) == 0 )
            break;

          size_t i = rand() % cc_size( &our_list[ list ] );

          int *cc_itr = cc_first( &our_list[ list ] );
          for( size_t j = 0; j < i; ++j )
            cc_itr = cc_next( &our_list[ list ], cc_itr );

          auto stl_itr = stl_list[ list ].begin();
          std::advance( stl_itr, i );

          UNTIL_SUCCESS( cc_erase( &our_list[ list ], cc_itr ) );
          stl_list[ list ].erase( stl_itr );
        }
        break;
        case 3: // cc_splice between lists.
        {
          if( cc_size( &our_list[ list ] ) == 0 )
            break;

          size_t i_src = rand() % cc_size( &our_list[ list ] );

          int *cc_itr_src = cc_first( &our_list[ list ] );
          for( size_t j = 0; j < i_src; ++j )
            cc_itr_src = cc_next( &our_list[ list ], cc_itr_src );
          
          auto stl_itr_src = stl_list[ list ].begin();
          std::advance( stl_itr_src, i_src );

          size_t i_dest = 0;
          if( cc_size( &our_list[ !list ] ) != 0 )
            i_dest = rand() % ( cc_size( &our_list[ !list ] ) + 1 );

          int *cc_itr_dest = cc_first( &our_list[ !list ] );
          for( size_t j = 0; j < i_dest; ++j )
            cc_itr_dest = cc_next( &our_list[ !list ], cc_itr_dest );
          
          auto stl_itr_dest = stl_list[ !list ].begin();
          std::advance( stl_itr_dest, i_dest );

          UNTIL_SUCCESS( cc_splice( &our_list[ !list ], cc_itr_dest, &our_list[ list ], cc_itr_src ) );
          stl_list[ !list ].splice( stl_itr_dest, stl_list[ list ], stl_itr_src );
        }
        break;
        case 4: // cc_splice within a list.
        {
          if( cc_size( &our_list[ list ] ) == 0 )
            break;

          size_t i_src = rand() % cc_size( &our_list[ list ] );
          size_t i_dest = rand() % ( cc_size( &our_list[ list ] ) + 1 );
          if( i_src == i_dest )
            break;

          int *cc_itr_src = cc_first( &our_list[ list ] );
          for( size_t j = 0; j < i_src; ++j )
            cc_itr_src = cc_next( &our_list[ list ], cc_itr_src );
          
          auto stl_itr_src = stl_list[ list ].begin();
          std::advance( stl_itr_src, i_src );

          int *cc_itr_dest = cc_first( &our_list[ list ] );
          for( size_t j = 0; j < i_dest; ++j )
            cc_itr_dest = cc_next( &our_list[ list ], cc_itr_dest );
          
          auto stl_itr_dest = stl_list[ list ].begin();
          std::advance( stl_itr_dest, i_dest );

          UNTIL_SUCCESS( cc_splice( &our_list[ list ], cc_itr_dest, &our_list[ list ], cc_itr_src ) );
          stl_list[ list ].splice( stl_itr_dest, stl_list[ list ], stl_itr_src );
        }
        break;
        case 5: // cc_init_clone.
        {
          cc_list( int ) clone;

          if( rand() % 2 ) // Probable failure due to failing realloc.
          {
            if( cc_init_clone( &clone, &our_list[ list ] ) )
            {
              cc_cleanup( &our_list[ list ] );
              our_list[ list ] = clone;
            }
          }
          else // Non-failing.
          {
            failing_alloc_on = false;
            UNTIL_SUCCESS( cc_init_clone( &clone, &our_list[ list ] ) );

            cc_cleanup( &our_list[ list ] );
            our_list[ list ] = clone;
            failing_alloc_on = true;
          }
        }
        break;
      }
    }

    ALWAYS_ASSERT( cc_size( &our_list[ 0 ] ) == stl_list[ 0 ].size() );
    ALWAYS_ASSERT( cc_size( &our_list[ 1 ] ) == stl_list[ 1 ].size() );

    // Forward iteration.

    auto stl_itr = stl_list[ 0 ].begin();
    cc_for_each( &our_list[ 0 ], cc_itr )
    {
      ALWAYS_ASSERT( *cc_itr == *stl_itr );
      ++stl_itr;
    }
    ALWAYS_ASSERT( stl_itr == stl_list[ 0 ].end() );

    stl_itr = stl_list[ 1 ].begin();
    cc_for_each( &our_list[ 1 ], cc_itr )
    {
      ALWAYS_ASSERT( *cc_itr == *stl_itr );
      ++stl_itr;
    }
    ALWAYS_ASSERT( stl_itr == stl_list[ 1 ].end() );

    // Reverse iteration.

    auto stl_r_itr = stl_list[ 0 ].rbegin();
    cc_r_for_each( &our_list[ 0 ], cc_itr )
    {
      ALWAYS_ASSERT( *cc_itr == *stl_r_itr );
      ++stl_r_itr;
    }
    ALWAYS_ASSERT( stl_r_itr == stl_list[ 0 ].rend() );

    stl_r_itr = stl_list[ 1 ].rbegin();
    cc_r_for_each( &our_list[ 1 ], cc_itr )
    {
      ALWAYS_ASSERT( *cc_itr == *stl_r_itr );
      ++stl_r_itr;
    }
    ALWAYS_ASSERT( stl_r_itr == stl_list[ 1 ].rend() );

    std::cout << "Done. Final sizes: " << cc_size( &our_list[ 0 ] ) << ", " << cc_size( &our_list[ 1 ] ) << "\n";
    cc_cleanup( &our_list[ 0 ] );
    cc_cleanup( &our_list[ 1 ] );
  }

  // Map.
  for( int test = 0; test < N_TESTS; ++test )
  {
    std::cout << "Map test " << test << "... ";
    std::unordered_map<int, int> stl_map;
    cc_map( int, int ) our_map;
    cc_init( &our_map );

    for( int op = 0; op < N_OPS; ++op )
    {
      switch( rand() % 7 )
      {
        case 0: // cc_insert.
        {
          int *el;
          int key = rand() % ( N_OPS / 10 );
          int el_val = rand();
          UNTIL_SUCCESS( ( el = cc_insert( &our_map, key, el_val ) ) );

          ALWAYS_ASSERT( *el == el_val );
          ALWAYS_ASSERT( *cc_key_for( &our_map, el ) == key );

          stl_map[ key ] = el_val;
        }
        break;
        case 1: // cc_get_or_insert.
        {
          int *el;
          int key = rand() % ( N_OPS / 10 );
          int el_val = rand();
          size_t original_size = cc_size( &our_map );
          UNTIL_SUCCESS( ( el = cc_get_or_insert( &our_map, key, el_val ) ) );

          ALWAYS_ASSERT( *cc_key_for( &our_map, el ) == key );

          if( cc_size( &our_map ) > original_size )
          {
            ALWAYS_ASSERT( *el == el_val );

            stl_map[ key ] = el_val;
          }
          else
            ALWAYS_ASSERT( *el == stl_map.find( key )->second );
        }
        break;
        case 2: // cc_get.
        {
          int key = rand() % ( N_OPS / 10 );
          int *el = cc_get( &our_map, key );
          if( el )
            ALWAYS_ASSERT( *el == stl_map.find( key )->second );
          else
            ALWAYS_ASSERT( stl_map.find( key ) == stl_map.end() );
        }
        break;
        case 3: // cc_erase and cc_erase_itr.
        {
          if( rand() % 2 )
          {
            int key = rand() % ( N_OPS / 10 );
            ALWAYS_ASSERT( cc_erase( &our_map, key ) == (bool)stl_map.erase( key ) );
          }
          else
          {
            int key = rand() % ( N_OPS / 10 );
            int *el = cc_get( &our_map, key );
            if( el )
              cc_erase_itr( &our_map, el );
            
            stl_map.erase( key );
          }
        }
        break;
        case 4: // cc_reserve.
        {
          if( rand() % 2 )
            UNTIL_SUCCESS( cc_reserve( &our_map, cc_cap( &our_map ) ) ); // Reserve above the current capacity.
          else if( cc_cap( &our_map ) * CC_DEFAULT_LOAD  >= 5 ) // Reserve below the current capacity.
            UNTIL_SUCCESS( cc_reserve( &our_map, cc_cap( &our_map ) * CC_DEFAULT_LOAD - 5 ) );
        }
        break;
        case 5: // cc_shrink.
        {
          UNTIL_SUCCESS( cc_shrink( &our_map ) );
        }
        break;
        case 6: // cc_init_clone.
        {
          cc_map( int, int ) clone;
          UNTIL_SUCCESS( cc_init_clone( &clone, &our_map ) );
          cc_cleanup( &our_map );
          our_map = clone;
        }
        break;
      }
    }

    // Check our_map against unordered_map.
    ALWAYS_ASSERT( cc_size( &our_map ) == stl_map.size() );
    cc_for_each( &our_map, k, i )
      ALWAYS_ASSERT( *i == stl_map.find( *k )->second );

    // Check unordered_map against our_map.
    for( auto i = stl_map.begin(); i != stl_map.end(); ++i )
      ALWAYS_ASSERT( *cc_get( &our_map, i->first ) == i->second );

    std::cout << "Done. Final size: " << cc_size( &our_map ) << "\n";
    cc_cleanup( &our_map );
  }

  // Set.
  for( int test = 0; test < N_TESTS; ++test )
  {
    std::cout << "Set test " << test << "... ";
    std::unordered_set<int> stl_set;
    cc_set( int ) our_set;
    cc_init( &our_set );

    for( int op = 0; op < N_OPS; ++op )
    {
      switch( rand() % 7 )
      {
        case 0: // cc_insert.
        {
          int el_val = rand() % ( N_OPS / 10 );
          int *el;
          UNTIL_SUCCESS( ( el = cc_insert( &our_set, el_val ) ) );

          ALWAYS_ASSERT( *el == el_val );

          stl_set.insert( el_val );
        }
        break;
        case 1: // cc_get_or_insert.
        {
          int *el;
          int el_val = rand();
          size_t original_size = cc_size( &our_set );
          UNTIL_SUCCESS( ( el = cc_get_or_insert( &our_set, el_val ) ) );

          if( cc_size( &our_set ) > original_size )
          {
            ALWAYS_ASSERT( *el == el_val );

            stl_set.insert( el_val );
          }
          else
            ALWAYS_ASSERT( *el == *stl_set.find( el_val ) );
        }
        break;
        case 2: // cc_get.
        {
          int el_val = rand() % ( N_OPS / 10 );
          int *el = cc_get( &our_set, el_val );
          if( el )
            ALWAYS_ASSERT( *el == *stl_set.find( el_val ) );
          else
            ALWAYS_ASSERT( stl_set.find( el_val ) == stl_set.end() );
        }
        break;
        case 3: // cc_erase and cc_erase_itr.
        {
          if( rand() % 2 )
          {
            int el_val = rand() % ( N_OPS / 10 );
            ALWAYS_ASSERT( cc_erase( &our_set, el_val ) == (bool)stl_set.erase( el_val ) );
          }
          else
          {
            int el_val = rand() % ( N_OPS / 10 );
            int *el = cc_get( &our_set, el_val );
            if( el )
              cc_erase_itr( &our_set, el );
            
            stl_set.erase( el_val );
          }
        }
        break;
        case 4: // cc_reserve.
        {
          if( rand() % 2 )
            UNTIL_SUCCESS( cc_reserve( &our_set, cc_cap( &our_set ) ) );
          else if( cc_cap( &our_set ) * CC_DEFAULT_LOAD  >= 5 )
            UNTIL_SUCCESS( cc_reserve( &our_set, cc_cap( &our_set ) * CC_DEFAULT_LOAD - 5 ) );
        }
        break;
        case 5: // cc_shrink.
        {
          UNTIL_SUCCESS( cc_shrink( &our_set ) );
        }
        break;
        case 6: // cc_init_clone.
        {
          cc_set( int ) clone;
          UNTIL_SUCCESS( cc_init_clone( &clone, &our_set ) );
          cc_cleanup( &our_set );
          our_set = clone;
        }
        break;
      }
    }

    ALWAYS_ASSERT( cc_size( &our_set ) == stl_set.size() );

    // Check cc_set against unordered_set.
    cc_for_each( &our_set, i )
      ALWAYS_ASSERT( *i == *stl_set.find( *i ) );

    // Check unordered_set against cc_set.
    for( auto i = stl_set.begin(); i != stl_set.end(); ++i )
      ALWAYS_ASSERT( *cc_get( &our_set, *i ) == *i );

    std::cout << "Done. Final size: " << cc_size( &our_set ) << "\n";
    cc_cleanup( &our_set );
  }

  ALWAYS_ASSERT( oustanding_allocs.empty() );
  std::cout << "All done.\nSimulated allocation failures: " << simulated_alloc_failures << "\n";
}
