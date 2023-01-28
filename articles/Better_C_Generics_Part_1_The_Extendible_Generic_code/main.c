#include <stdio.h>
#include "hash.h"

// Overwrite an existing hash function.
size_t custom_hash_long_long( long long val ) { return (unsigned long long)val * 31; }
#define  HASH long long, custom_hash_long_long
#include "hash.h"

// Add a hash function for a user-defined type.
typedef struct { unsigned long long id; } custom_ty;
size_t hash_custom_ty( custom_ty val ) { return val.id * 31; }
#define  HASH custom_ty, hash_custom_ty
#include "hash.h"

// Test.
int main( void )
{
  printf(
    "%zu %zu %zu %zu %zu\n",
    hash( (short)1 ),
    hash( (int)2 ),
    hash( (long)3 ),
    hash( (long long)4 ),
    hash( (custom_ty){ 5 } )
  );
}
