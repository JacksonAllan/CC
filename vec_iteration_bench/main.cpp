#define NDEBUG

#include <cstdio>
#include <chrono>
#include <thread>

#define CC_NO_SHORT_NAMES
#include "../cc.h"

int main( void )
{
  const int key_count = 10000000;

  unsigned int *keys = (unsigned int *)malloc( sizeof( unsigned int ) * key_count );
  if( !keys )
    return 1;

  for( unsigned int i = 0; i < key_count; ++i )
  {
    unsigned long long val = i;
    val ^= val >> 23;
    val *= 0x2127599BF4325C37ULL;
    val ^= val >> 47;
    keys[ i ] = val;
  }

  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::chrono::time_point<std::chrono::high_resolution_clock> end;

  //
  {
    cc_vec( unsigned int ) our_vec;
    cc_init( &our_vec );
    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

    cc_push_n( &our_vec, keys, key_count );

    int total = 0;
    start = std::chrono::high_resolution_clock::now();
    cc_for_each( &our_vec, itr )
      total += *itr;
    end = std::chrono::high_resolution_clock::now();
    printf( "%lld, %u\n", std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count(), total );

    cc_cleanup( &our_vec );
  }

  //
  {
    cc_vec( unsigned int ) our_vec;
    cc_init( &our_vec );
    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

    cc_push_n( &our_vec, keys, key_count );

    int total = 0;
    start = std::chrono::high_resolution_clock::now();
    for( unsigned int *itr = cc_first( &our_vec ); itr != cc_end( &our_vec ); itr = (unsigned int *)cc_vec_next_original( our_vec, itr, CC_EL_SIZE( our_vec ), 0 ) )
      total += *itr;
    end = std::chrono::high_resolution_clock::now();
    printf( "%lld, %u\n", std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count(), total );

    cc_cleanup( &our_vec );
  }

  free( keys );
  printf( "Done\n" );
}
