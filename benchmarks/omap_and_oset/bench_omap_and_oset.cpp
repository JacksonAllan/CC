/*

Convenient Containers v1.3.1 - benchmarks/omap_and_oset/bench_omap_and_oset.cpp

This file benchmarks CC's ordered map and ordered set against the equivalent C++ STL containers.

License (MIT):

  Copyright (c) 2024 Jackson L. Allan

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

#define NDEBUG

#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <random>
#include <set>
#include <thread>
#include <vector>

#define CC_NO_SHORT_NAMES
#include "../../cc.h"

int main()
{
  constexpr int key_count = 10000000;
  constexpr int run_count = 10;

  std::vector<int> keys( key_count );
  std::iota( keys.begin(), keys.end(), 1 );
  std::shuffle(
    keys.begin(),
    keys.end(),
    std::default_random_engine( std::chrono::system_clock::now().time_since_epoch().count() )
  );

  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::chrono::time_point<std::chrono::high_resolution_clock> end;
  unsigned long long optimization_preventer = 0;

  double total_omap_insert_time = 0.0;
  double total_omap_lookup_time = 0.0;
  double total_omap_erase_time = 0.0;
  double total_map_insert_time = 0.0;
  double total_map_lookup_time = 0.0;
  double total_map_erase_time = 0.0;
  double total_oset_insert_time = 0.0;
  double total_oset_lookup_time = 0.0;
  double total_oset_erase_time = 0.0;
  double total_set_insert_time = 0.0;
  double total_set_lookup_time = 0.0;
  double total_set_erase_time = 0.0;

  for( int run = 0; run < run_count; ++run )
  {
    std::cout << "Run " << run << '\n';

    // omap.
    {
      cc_omap( int, int ) our_omap;
      cc_init( &our_omap );
      std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

      start = std::chrono::high_resolution_clock::now();
      for( size_t i = 0; i < key_count; ++i )
        cc_insert( &our_omap, keys[ i ], 0 );
      end = std::chrono::high_resolution_clock::now();
      total_omap_insert_time += std::chrono::duration_cast<std::chrono::duration<double>>( end - start ).count();

      start = std::chrono::high_resolution_clock::now();
      for( size_t i = 0; i < key_count; ++i )
        optimization_preventer += *cc_key_for( &our_omap, cc_get( &our_omap, keys[ i ] ) );
      end = std::chrono::high_resolution_clock::now();
      total_omap_lookup_time += std::chrono::duration_cast<std::chrono::duration<double>>( end - start ).count();

      start = std::chrono::high_resolution_clock::now();
      for( size_t i = 0; i < key_count; ++i )
        cc_erase( &our_omap, keys[ i ] );
      end = std::chrono::high_resolution_clock::now();
      total_omap_erase_time += std::chrono::duration_cast<std::chrono::duration<double>>( end - start ).count();

      cc_cleanup( &our_omap );
    }

    // std::map.
    {
      std::map<int,int> our_map;
      std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

      start = std::chrono::high_resolution_clock::now();
      for( size_t i = 0; i < key_count; ++i )
        our_map.insert( { keys[ i ], 0 } );
      end = std::chrono::high_resolution_clock::now();
      total_map_insert_time += std::chrono::duration_cast<std::chrono::duration<double>>( end - start ).count();

      start = std::chrono::high_resolution_clock::now();
      for( size_t i = 0; i < key_count; ++i )
        optimization_preventer += our_map.find( keys[ i ] )->first;
      end = std::chrono::high_resolution_clock::now();
      total_map_lookup_time += std::chrono::duration_cast<std::chrono::duration<double>>( end - start ).count();

      start = std::chrono::high_resolution_clock::now();
      for( size_t i = 0; i < key_count; ++i )
        our_map.erase( keys[ i ] );
      end = std::chrono::high_resolution_clock::now();
      total_map_erase_time += std::chrono::duration_cast<std::chrono::duration<double>>( end - start ).count();
    }

    // oset.
    {
      cc_oset( int ) our_oset;
      cc_init( &our_oset );
      std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

      start = std::chrono::high_resolution_clock::now();
      for( size_t i = 0; i < key_count; ++i )
        cc_insert( &our_oset, keys[ i ] );
      end = std::chrono::high_resolution_clock::now();
      total_oset_insert_time += std::chrono::duration_cast<std::chrono::duration<double>>( end - start ).count();

      start = std::chrono::high_resolution_clock::now();
      for( size_t i = 0; i < key_count; ++i )
        optimization_preventer += *cc_get( &our_oset, keys[ i ] );
      end = std::chrono::high_resolution_clock::now();
      total_oset_lookup_time += std::chrono::duration_cast<std::chrono::duration<double>>( end - start ).count();

      start = std::chrono::high_resolution_clock::now();
      for( size_t i = 0; i < key_count; ++i )
        cc_erase( &our_oset, keys[ i ] );
      end = std::chrono::high_resolution_clock::now();
      total_oset_erase_time += std::chrono::duration_cast<std::chrono::duration<double>>( end - start ).count();

      cc_cleanup( &our_oset );
    }

    // std::set.
    {
      std::set<int> our_set;
      std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

      start = std::chrono::high_resolution_clock::now();
      for( size_t i = 0; i < key_count; ++i )
        our_set.insert( keys[ i ] );
      end = std::chrono::high_resolution_clock::now();
      total_set_insert_time += std::chrono::duration_cast<std::chrono::duration<double>>( end - start ).count();

      start = std::chrono::high_resolution_clock::now();
      for( size_t i = 0; i < key_count; ++i )
        optimization_preventer += *our_set.find( keys[ i ] );
      end = std::chrono::high_resolution_clock::now();
      total_set_lookup_time += std::chrono::duration_cast<std::chrono::duration<double>>( end - start ).count();

      start = std::chrono::high_resolution_clock::now();
      for( size_t i = 0; i < key_count; ++i )
        our_set.erase( keys[ i ] );
      end = std::chrono::high_resolution_clock::now();
      total_set_erase_time += std::chrono::duration_cast<std::chrono::duration<double>>( end - start ).count();
    }
  }

  std::cout << std::setprecision( 2 ) << std::fixed;

  std::cout << "---Insert results---\n";
  std::cout << "omap: " << total_omap_insert_time / run_count << "s\n";
  std::cout << "map:  " << total_map_insert_time / run_count << "s\n";
  std::cout << "oset: " << total_oset_insert_time / run_count << "s\n";
  std::cout << "set:  " << total_set_insert_time / run_count << "s\n";

  std::cout << "---Lookup results---\n";
  std::cout << "omap: " << total_omap_lookup_time / run_count << "s\n";
  std::cout << "map:  " << total_map_lookup_time / run_count << "s\n";
  std::cout << "oset: " << total_oset_lookup_time / run_count << "s\n";
  std::cout << "set:  " << total_set_lookup_time / run_count << "s\n";

  std::cout << "---Erase results---\n";
  std::cout << "omap: " << total_omap_erase_time / run_count << "s\n";
  std::cout << "map:  " << total_map_erase_time / run_count << "s\n";
  std::cout << "oset: " << total_oset_erase_time / run_count << "s\n";
  std::cout << "set:  " << total_set_erase_time / run_count << "s\n";

  std::cout << "Done " << optimization_preventer << '\n';
}
