#ifndef ORDERED_ARRAY_H
#define ORDERED_ARRAY_H

#include <stdint.h>

#include "memory.h"

typedef void *type_t;

typedef char (*lessthan_predicate_t)(type_t, type_t);

typedef struct
{
	type_t *array;
	uint32_t size;
	uint32_t max_size;
	lessthan_predicate_t less_than;
} ordered_array_t;

#ifdef __cplusplus
extern "C"
{

	// A standard less than predicate
	char standard_lessthan_predicate(type_t a, type_t b);

	// Create an ordered array (uses kmalloc)
	ordered_array_t orderedArray_create(uint32_t max_size, lessthan_predicate_t less_than);

	// Create an ordered array (uses given start location)
	ordered_array_t orderedArray_place(void *addr, uint32_t max_size, lessthan_predicate_t less_than);

	// Destroy an ordered array
	void orderedArray_destroy(ordered_array_t *array);

	// Add an item into the array
	void orderedArray_insert(type_t item, ordered_array_t *array);

	// Lookup the item at index i
	type_t orderedArray_lookup(uint32_t i, ordered_array_t *array);

	// Deletes the item at location i from the array
	void orderedArray_remove(uint32_t i, ordered_array_t *array);
}
#else

// A standard less than predicate
extern char standard_lessthan_predicate(type_t a, type_t b);

// Create an ordered array (uses kmalloc)
extern ordered_array_t orderedArray_create(uint32_t max_size, lessthan_predicate_t less_than);

// Create an ordered array (uses given start location)
extern ordered_array_t orderedArray_place(void *addr, uint32_t max_size, lessthan_predicate_t less_than);

// Destroy an ordered array
extern void orderedArray_destroy(ordered_array_t *array);

// Add an item into the array
extern void orderedArray_insert(type_t item, ordered_array_t *array);

// Lookup the item at index i
extern type_t orderedArray_lookup(uint32_t i, ordered_array_t *array);

// Deletes the item at location i from the array
extern void orderedArray_remove(uint32_t i, ordered_array_t *array);

#endif

#endif // ORDERED_ARRAY_H