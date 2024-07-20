#include <assert.h>
#include <stdio.h>

#include "../cc.h"

void check_node( void *cntr, cc_omapnode_hdr_ty *node, int branch_black_depth, int *overall_black_depth )
{
  if( node == cc_omap_hdr( cntr )->sentinel )
  {
    if( *overall_black_depth == -1 )
      *overall_black_depth = branch_black_depth;
    else
      assert( branch_black_depth == *overall_black_depth );
    
    return;
  }

  assert( !node->is_red || !node->parent->is_red );
  
  if( !node->is_red )
    ++branch_black_depth;
  
  check_node( cntr, node->children[ 0 ], branch_black_depth, overall_black_depth );
  check_node( cntr, node->children[ 1 ], branch_black_depth, overall_black_depth  );
}

void check( void *cntr )
{
  if( cc_omap_hdr( cntr )->root == cc_omap_hdr( cntr )->sentinel )
    return;

  assert( !cc_omap_hdr( cntr )->root->is_red );
  
  int overall_black_depth = -1;
  check_node( cntr, cc_omap_hdr( cntr )->root, 0, &overall_black_depth );
}

int main( void )
{
  const int run_count = 50000;
  const int key_count = 5000;

  int *keys = (int *)malloc( sizeof( int ) * key_count );
  if( !keys )
    return 1;

  unsigned long long seed = 1;

  omap( int, int ) tree;
  init( &tree );

  for( int run = 0; run < run_count; ++run )
  {
    unsigned long long total = 0;
    for( int i = 0; i < key_count; ++i )
    {
      unsigned long long val = seed++;
      val ^= val >> 23;
      val *= 0x2127599BF4325C37ULL;
      val ^= val >> 47;
      keys[ i ] = val;
      // keys[ i ] = i;
      total += keys[ i ];
    }

    for( int i = 0; i < key_count; ++i )
      for( int j = i + 1; j < key_count; ++j )
        assert( keys[ i ] != keys[ j ] );

    printf( "Run %d\n", run );

    for( int i = 0; i < key_count; ++i )
      insert( &tree, keys[ i ], keys[ i ] / 2 );

    assert( size( &tree ) == key_count );

    for( int i = 0; i < key_count; ++i )
    {
      int *el = get_or_insert( &tree, keys[ i ], 0 );
      assert( *el == keys[ i ] / 2 );
    }

    for( int i = 0; i < key_count; ++i )
    {
      int *el = get( &tree, keys[ i ] );
      assert( *el == keys[ i ] / 2 );
      assert( *key_for( &tree, el ) == keys[ i ] );
    }

    check( tree );

////
    omap( int, int ) clone;
    assert( init_clone( &clone, &tree ) );
    cleanup( &tree );
    tree = clone;

    for( int i = 0; i < key_count; ++i )
    {
      int *el = get( &tree, keys[ i ] );
      assert( *el == keys[ i ] / 2 );
      assert( *key_for( &tree, el ) == keys[ i ] );
    }
////

    unsigned long long iterations = 0;
    unsigned long long total_ = 0;
    int last = 0;
    // for( int *itr = first( &tree ); itr != end( &tree ); itr = next( &tree, itr ) )
    for_each( &tree, itr )
    {
      assert( last == 0 || *key_for( &tree, itr ) > last );
      last = *key_for( &tree, itr );
      total_ += *key_for( &tree, itr );
      ++iterations;
    }
    printf( "iterations: %llu\n", iterations );
    assert( total_ == total );

    iterations = 0;
    total_ = 0;
    last = INT_MAX;
    for( int *itr = last( &tree ); itr != r_end( &tree ); itr = prev( &tree, itr ) )
    {
      assert( last == INT_MAX || *key_for( &tree, itr ) < last );
      last = *key_for( &tree, itr );
      total_ += *key_for( &tree, itr );
      ++iterations;
    }
    assert( total_ == total );

    iterations = 0;
    for(
      int *itr = first( &tree, 1000000 ), *range_end = first( &tree, 2000000 );
      itr != range_end;
      itr = next( &tree, itr )
    )
      ++iterations;
    printf( "Range: %zu\n", iterations );

    iterations = 0;
    for(
      int *itr = last( &tree, 1999999 ), *range_end = last( &tree, 999999 );
      itr != range_end;
      itr = prev( &tree, itr )
    )
      ++iterations;
    printf( "Range: %zu\n", iterations );

/*
    int_rbt_node *start = int_rbt_first_above_equal( &tree, 100 );
    int_rbt_node *end = int_rbt_first_above( &tree, 200 );
    for( int_rbt_node *node = start; node != end; node = int_rbt_next( &tree, node ) )
      printf( "... %d\n", node->key );
*/

    for( int *itr = first( &tree ); itr != end( &tree ); )
    {
      if( *key_for( &tree, itr ) % 2 )
        itr = erase_itr( &tree, itr );
      else
        itr = next( &tree, itr );
    }

    for( int i = 0; i < key_count; ++i )
    {
      int *el = get( &tree, keys[ i ] );
      if( el )
        assert( *key_for( &tree, el ) % 2 == 0 );
    }

/*
    for( int i = 0; i < key_count; i += 2 )
    {
      assert( erase( &tree, keys[ i ] ) );
      check( tree );
    }

    assert( size( &tree ) == key_count / 2 );

    for( int i = 1; i < key_count; i += 2 )
    {
      erase( &tree, keys[ i ] );
      check( tree );
    }

    assert( size( &tree ) == 0 );
*/

/*
    for( int i = 0; i < key_count; ++i )
    {
      int_rbt_insert( &tree, keys[ i ] );

      if( i && i % 100 == 0 )
        for( int j = 0; j < 100; j += 2 )
        {
          int_rbt_node *node = int_rbt_find( &tree, keys[ i - 100 + j ] );
          assert( node );
          int_rbt_erase( &tree, node );
          int_rbt_check( &tree );
        }
    }*/

    clear( &tree );
  }

  cleanup( &tree );

  free( keys );
  printf( "Done\n" );
}
