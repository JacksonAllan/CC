#define NDEBUG

#include <cstdio>
#include <iostream>
#include <map>
#include <set>
#include <thread>

#define CC_NO_SHORT_NAMES
#include "../cc.h"

int main( void )
{
  const int key_count = 10000000;

  int *keys = (int *)malloc( sizeof( int ) * key_count );
  if( !keys )
    return 1;

  for( int i = 0; i < key_count; ++i )
  {
    unsigned long long val = i;
    val ^= val >> 23;
    val *= 0x2127599BF4325C37ULL;
    val ^= val >> 47;
    keys[ i ] = val;
  }

  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::chrono::time_point<std::chrono::high_resolution_clock> end;
  unsigned long long total;

  // omap.
  {
    cc_omap( int, int ) our_omap;
    cc_init( &our_omap );
    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

    start = std::chrono::high_resolution_clock::now();
    for( int i = 0; i < key_count; ++i )
      cc_insert( &our_omap, keys[ i ], 0 );
    end = std::chrono::high_resolution_clock::now();
    printf( "omap insert %lld\n", std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count() );

    total = 0;
    start = std::chrono::high_resolution_clock::now();
    for( int i = 0; i < key_count; ++i )
      total += *cc_key_for( &our_omap, cc_get( &our_omap, keys[ i ] ) );
    end = std::chrono::high_resolution_clock::now();
    printf( "omap lookup %lld, total: %llu\n", std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count(), total );
  }

  // std::map.
  {
    std::map<int,int> our_map;
    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

    start = std::chrono::high_resolution_clock::now();
    for( int i = 0; i < key_count; ++i )
      our_map.insert( { keys[ i ], 0 } );
    end = std::chrono::high_resolution_clock::now();
    printf( "std::map insert %lld\n", std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count() );

    total = 0;
    start = std::chrono::high_resolution_clock::now();
    for( int i = 0; i < key_count; ++i )
      total += our_map.find( keys[ i ] )->first;
    end = std::chrono::high_resolution_clock::now();
    printf( "std::map lookup %lld, total: %llu\n", std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count(), total );
  }

  // oset.
  {
    cc_oset( int ) our_oset;
    cc_init( &our_oset );
    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

    start = std::chrono::high_resolution_clock::now();
    for( int i = 0; i < key_count; ++i )
      cc_insert( &our_oset, keys[ i ] );
    end = std::chrono::high_resolution_clock::now();
    printf( "oset insert %lld\n", std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count() );

    total = 0;
    start = std::chrono::high_resolution_clock::now();
    for( int i = 0; i < key_count; ++i )
      total += *cc_get( &our_oset, keys[ i ] );
    end = std::chrono::high_resolution_clock::now();
    printf( "oset lookup %lld, total: %llu\n", std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count(), total );
  }

  // std::set.
  {
    std::set<int> our_set;
    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

    start = std::chrono::high_resolution_clock::now();
    for( int i = 0; i < key_count; ++i )
      our_set.insert( keys[ i ] );
    end = std::chrono::high_resolution_clock::now();
    printf( "std::set insert %lld\n", std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count() );

    total = 0;
    start = std::chrono::high_resolution_clock::now();
    for( int i = 0; i < key_count; ++i )
      total += *our_set.find( keys[ i ] );
    end = std::chrono::high_resolution_clock::now();
    printf( "std::set lookup %lld, total: %llu\n", std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count(), total );
  }

  free( keys );
  printf( "Done\n" );
}
