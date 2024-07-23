#define NDEBUG

#include <cstdio>
#include <iostream>
#include <map>
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
    printf( "%lld\n", std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count() );

    total = 0;
    start = std::chrono::high_resolution_clock::now();
    for( int i = 0; i < key_count; ++i )
      total += *cc_key_for( &our_omap, cc_get( &our_omap, keys[ i ] ) );
    end = std::chrono::high_resolution_clock::now();
    printf( "%lld, total: %llu\n", std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count(), total );
  }

  // map.
  {
    std::map<int,int> our_map;
    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

    start = std::chrono::high_resolution_clock::now();
    for( int i = 0; i < key_count; ++i )
      our_map.insert( { keys[ i ], 0 } );
    end = std::chrono::high_resolution_clock::now();
    printf( "%lld\n", std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count() );

    total = 0;
    start = std::chrono::high_resolution_clock::now();
    for( int i = 0; i < key_count; ++i )
      total += our_map.find( keys[ i ] )->first;
    end = std::chrono::high_resolution_clock::now();
    printf( "%lld, total: %llu\n", std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count(), total );
  }

  free( keys );
  printf( "Done\n" );
}
