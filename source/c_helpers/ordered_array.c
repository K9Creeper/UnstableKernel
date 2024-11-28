#include "ordered_array.h"

// kheap.hpp
extern uint32_t kmalloc_(uint32_t size, uint8_t align, uint32_t* physAddress);
extern void kfree_(void*);

char standard_lessthan_predicate ( type_t a, type_t b )
{
	return ( a < b ) ? 1 : 0;
}

ordered_array_t orderedArray_create ( uint32_t max_size, lessthan_predicate_t less_than )
{
	ordered_array_t a;
	uint32_t address;

	address = kmalloc_( max_size * sizeof( type_t ), 0, 0 );
	a.array = ( void * ) address;
	memset( ( uint8_t * ) address, 0, max_size * sizeof( type_t ) );  // fill with zeros

	a.size = 0;
	a.max_size = max_size;
	a.less_than = less_than;

	return a;
}

ordered_array_t orderedArray_place ( void *address, uint32_t max_size, lessthan_predicate_t less_than )
{
	ordered_array_t a;

	a.array = ( type_t * ) address;
	memset( ( uint8_t * ) address, 0, max_size * sizeof( type_t ) );  // fill with zeros

	a.size = 0;
	a.max_size = max_size;
	a.less_than = less_than;

	return a;
}

void orderedArray_insert ( type_t item, ordered_array_t *array )
{
	uint32_t i;
	type_t tmp, tmp2;

	// Iterate till find value greater or equal to item
	i = 0;
	while( i < array -> size &&
	       array -> less_than( array -> array[ i ], item ) )  // array[i] < item
	{
		i += 1;
	}

	// If reached end of array, just append the item
	if ( i == array -> size )
	{
		array -> array[ array -> size ] = item;

		array -> size += 1;
	}

	// Else, insert
	else
	{
		tmp = array -> array[ i ];

		array -> array[ i ] = item;

		while ( i < array -> size )
		{
			i += 1;

			tmp2 = array -> array[ i ];

			array -> array[ i ] = tmp;

			tmp = tmp2;
		}

		array -> size += 1;
	}
}

void orderedArray_destroy ( ordered_array_t *array )
{
	kfree_( array -> array );
}

type_t orderedArray_lookup ( uint32_t i, ordered_array_t *array )
{
	return array -> array[ i ];
}

void orderedArray_remove ( uint32_t i, ordered_array_t *array )
{
	while ( i < array -> size )
	{
		array -> array[ i ] = array -> array[ i + 1 ];

		i += 1;
	}

	array -> size -= 1;
}