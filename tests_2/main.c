#include <assert.h>
#include <stdio.h>
#include "../cc.h"



/* --- PRINTF_BYTE_TO_BINARY macro's --- */
#define PRINTF_BINARY_SEPARATOR
#define PRINTF_BINARY_PATTERN_INT8 "%c%c%c%c%c%c%c%c"
#define PRINTF_BYTE_TO_BINARY_INT8(i)    \
    (((i) & 0x80ll) ? '1' : '0'), \
    (((i) & 0x40ll) ? '1' : '0'), \
    (((i) & 0x20ll) ? '1' : '0'), \
    (((i) & 0x10ll) ? '1' : '0'), \
    (((i) & 0x08ll) ? '1' : '0'), \
    (((i) & 0x04ll) ? '1' : '0'), \
    (((i) & 0x02ll) ? '1' : '0'), \
    (((i) & 0x01ll) ? '1' : '0')

#define PRINTF_BINARY_PATTERN_INT16 \
    PRINTF_BINARY_PATTERN_INT8               PRINTF_BINARY_SEPARATOR              PRINTF_BINARY_PATTERN_INT8
#define PRINTF_BYTE_TO_BINARY_INT16(i) \
    PRINTF_BYTE_TO_BINARY_INT8((i) >> 8),   PRINTF_BYTE_TO_BINARY_INT8(i)
#define PRINTF_BINARY_PATTERN_INT32 \
    PRINTF_BINARY_PATTERN_INT16              PRINTF_BINARY_SEPARATOR              PRINTF_BINARY_PATTERN_INT16
#define PRINTF_BYTE_TO_BINARY_INT32(i) \
    PRINTF_BYTE_TO_BINARY_INT16((i) >> 16), PRINTF_BYTE_TO_BINARY_INT16(i)
#define PRINTF_BINARY_PATTERN_INT64    \
    PRINTF_BINARY_PATTERN_INT32              PRINTF_BINARY_SEPARATOR              PRINTF_BINARY_PATTERN_INT32
#define PRINTF_BYTE_TO_BINARY_INT64(i) \
    PRINTF_BYTE_TO_BINARY_INT32((i) >> 32), PRINTF_BYTE_TO_BINARY_INT32(i)
/* --- end macros --- */



#define NAME int_int_map
#define KEY_TY int
#define VAL_TY int
#define MAX_LOAD 0.95
#include "verstable.h"




#define CC_LOAD int, 0.95
#define CC_HASH int, { return vt_hash_integer( val ); }
#include "../cc.h"



typedef struct { uint64_t dummy[ 7 ]; } large;



int main( void )
{
	map( uint64_t, large ) foo;
	printf( "%zu\n", CC_BUCKET_SIZE( CC_EL_SIZE( foo ), CC_LAYOUT( foo ) ) );


	map( int, int ) our_map;
	init( &our_map );

	for( int *itr = first( &our_map ); itr != end( &our_map ); itr = next( &our_map, itr ) )
		assert( 0 );


	for( int *itr = last( &our_map ); itr != r_end( &our_map ); itr = prev( &our_map, itr ) )
		assert( 0 );


	printf( "--------------\n" );
	for( int i = 0; i < 10; ++i )
	{
		printf( "%zu %zu\n", CC_KEY_HASH( our_map )( &i ), vt_hash_integer( i ) );
	}

	printf( "--------------\n" );
	for( int i = 0; i < 10; ++i )
	{
		printf( "%u %u\n", cc_hash_frag( CC_KEY_HASH( our_map )( &i ) ), vt_hashfrag( vt_hash_integer( i ) ) );
	}

	#define N 30000000

	int *itr = insert( &our_map, 1, 5 );
	assert( itr );

	for( int i = 0; i < N; ++i )
	{
		int *itr = insert( &our_map, i, i + 1 );
		assert( itr );
	}

	int_int_map our_map_2;
	vt_init( &our_map_2 );

	vt_insert( &our_map_2, 1, 2 );
	for( int i = 0; i < N; ++i )
		vt_insert( &our_map_2, i, i + 1 );

	for( size_t i = 0; i < cap( &our_map ); ++i )
		if( our_map_2.metadata[ i ] != cc_map_hdr( our_map )->metadata[ i ] )
		{
			printf( "%zu\n", i );

			printf( "-----------------\n" );
			for( size_t j = 0; j < cap( &our_map ); ++j )
				if( cc_map_hdr( our_map )->metadata[ j ] )
					printf(
						"[%zu]: " PRINTF_BINARY_PATTERN_INT16 " %d %d\n",
						j,
						PRINTF_BYTE_TO_BINARY_INT16( cc_map_hdr( our_map )->metadata[ j ] ),
						*(int *)cc_map_key( our_map, j, CC_EL_SIZE( our_map ), CC_LAYOUT( our_map ) ),
						*(int *)cc_map_el( our_map, j, CC_EL_SIZE( our_map ), CC_LAYOUT( our_map ) )
					);
				else
					printf( "[%zu]: 0000000000000000\n", j );


			printf( "-----------------\n" );
			for( size_t j = 0; j < vt_bucket_count( &our_map_2 ); ++j )
				if( our_map_2.metadata[ j ] )
					printf(
						"[%zu]: " PRINTF_BINARY_PATTERN_INT16 " %d %d\n",
						j,
						PRINTF_BYTE_TO_BINARY_INT16( our_map_2.metadata[ j ] ),
						our_map_2.buckets[ j ].key,
						our_map_2.buckets[ j ].val
					);
				else
					printf( "[%zu]: 0000000000000000\n", j );

			assert( 0 );
		}



	for( int i = 0; i < N; ++i )
	{
		int *itr = get( &our_map, i );
		assert( itr );
		assert( *itr == i + 1 );
	}

	for( int i = 0; i < N; ++i )
	{
		int *itr = insert( &our_map, i, i + 2 );
		assert( itr );
	}

	for( int i = 0; i < N; ++i )
	{
		int *itr = get( &our_map, i );
		assert( itr );
		assert( *itr == i + 2 );
	}

	for( int i = N; i < N + N; ++i )
	{
		int *itr = get( &our_map, i );
		assert( !itr );
	}

	size_t total = 0;
	for( int *itr = first( &our_map ); itr != end( &our_map ); itr = next( &our_map, itr ) )
	{
		assert( *key_for( &our_map, itr ) == *itr - 2 );
		++total;
	}

	assert( total == N );

	total = 0;
	for( int *itr = last( &our_map ); itr != r_end( &our_map ); itr = prev( &our_map, itr ) )
	{
		assert( *key_for( &our_map, itr ) == *itr - 2 );
		++total;
	}

	assert( total == N );

	/*for( int i = 0; i < N; i += 2 )
		assert( erase( &our_map, i ) );*/
	for( int i = 0; i < N; i += 2 )
	{
		int *itr = get( &our_map, i );
		erase_itr( &our_map, itr );
	}

	total = 0;
	for( int *itr = first( &our_map ); itr != end( &our_map ); itr = next( &our_map, itr ) )
	{
		assert( *key_for( &our_map, itr ) == *itr - 2 && *itr % 2 == 1 );
		++total;
	}

	assert( total == N / 2 );

	clear( &our_map );

	for( int *itr = last( &our_map ); itr != r_end( &our_map ); itr = prev( &our_map, itr ) )
		assert( 0 );

	cleanup( &our_map );

	printf( "Done\n" );
}
