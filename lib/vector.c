#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>


typedef struct{
	void *array;
	size_t length;
	size_t capacity;
	size_t reserve;
	size_t data_size;
}Vector;


Vector vector_create(size_t data_size, size_t reserve_pow_2) 
{
	// reserve must be a power of 2
	assert((reserve_pow_2 & (reserve_pow_2 - 1)) == 0);

	Vector vec;
	vec.length = 0;
	vec.reserve = reserve_pow_2;
	vec.capacity = reserve_pow_2;
	vec.data_size = data_size;
	vec.array = malloc(vec.capacity * vec.data_size);
	
	return vec;
}


int vector_push_back(Vector *vec, const void *item)
{
	// check if vector needs to grow
	if (vec->length >= vec->capacity) 
	{
		vec->capacity *= 2;
		void *tmp = realloc(vec->array, (vec->capacity * vec->data_size));
		if (!tmp) return -1; // allocation failed
		vec->array = tmp;
	}
	
	void *empty_slot  = (vec->array) + (vec->length * vec->data_size);
	memcpy(empty_slot, item, vec->data_size);
	 
	vec->length++;
	return 0;
}


void *vector_emplace_back(Vector *vec) 
{
	// check if vector needs to grow
	if (vec->length >= vec->capacity) 
	{
		vec->capacity *= 2;
		void *tmp = realloc(vec->array, (vec->capacity * vec->data_size));
		if (!tmp) 
			return NULL; // allocation failed
		vec->array = tmp;
	}

	void *empty_slot = vec->array + (vec->length * vec->data_size);

	vec->length++;
	return empty_slot;
}


inline void* vector_get_idx(Vector *vec, size_t index)
{
	if (index >= vec->length) 
		return NULL; // out of bounds 

	return vec->array + (index * vec->data_size);
}


static inline void* vector_unsafe_get_idx(Vector *vec, size_t index)
{
	return vec->array + (index * vec->data_size);
}


ssize_t vector_idx_of(Vector *vec, void *item) 
{
	uint8_t *base = vec->array;
	uint8_t *ptr = item;

	size_t total_bytes = vec->length * vec->data_size;

	if(ptr < base || ptr >= (base + (total_bytes))) 
		return -1; // out of bounds
	
	size_t offset = (ptr - base);

	if(offset % vec->data_size) 
		return -1; // data not aligned
	return (offset / vec->data_size);
}


int vector_remove_idx_ordered(Vector *vec, size_t index) 
{
	if (index >= vec->length) 
		return -1; // out of bounds	

	uint8_t *empty_slot = vec->array + (index * vec->data_size);
	uint8_t *next_slot = empty_slot + vec->data_size;
	size_t bytes_to_shift = (vec->length - index + 1) * vec->data_size;

	memmove(empty_slot, next_slot, bytes_to_shift);

	vec->length--;

	// check if vector needs to shrink
	if (vec->length <= (vec->capacity / 2) && vec->capacity > vec->reserve)
	{
		vec->capacity /= 2;
		vec->array = realloc(vec->array, (vec->capacity * vec->data_size));
	}

	return 0;
}


int vector_remove_idx_unordered(Vector *vec, size_t index) 
{
	if (index >= vec->length)
		return -1; // out of bounds 

	uint8_t *empty_slot = vec->array + (index * vec->data_size);
	uint8_t *last_slot = vec->array + ((vec->length - 1) * vec->data_size);
	
	memcpy(empty_slot, last_slot, vec->data_size);

	vec->length--;

	// check if vector needs to shrink
	if (vec->length <= (vec->capacity / 2) && vec->capacity > vec->reserve)
	{
		vec->capacity /= 2;
		vec->array = realloc(vec->array, (vec->capacity * vec->data_size));
	}

	return 0;
}

int vector_remove_ptr_ordered(Vector *vec, void *item)
{
	uint8_t *base = vec->array;
	uint8_t *empty_slot = item;

	size_t total_bytes = vec->length * vec->data_size;

	if(empty_slot < base || empty_slot >= (base + (total_bytes))) 
		return -1; // out of bounds
	
	size_t offset = empty_slot - base;

	if(offset % vec->data_size) 
		return -1; // data not aligned
	
	size_t index = (offset / vec->data_size);
				   
	uint8_t *next_slot = empty_slot + vec->data_size;
	size_t bytes_to_shift = (vec->length - index + 1) * vec->data_size;

	memmove(empty_slot, next_slot, bytes_to_shift);

	vec->length--;

	// check if vector needs to shrink
	if (vec->length <= (vec->capacity / 2) && vec->capacity > vec->reserve)
	{
		vec->capacity /= 2;
		vec->array = realloc(vec->array, (vec->capacity * vec->data_size));
	}

	return 0;
}

int vector_remove_ptr_unordered(Vector *vec, void *item)
{
	uint8_t *base = vec->array;
	uint8_t *empty_slot = item;

	size_t total_bytes = vec->length * vec->data_size;

	if(empty_slot < base || empty_slot >= (base + (total_bytes))) 
		return -1; // out of bounds
	
	size_t offset = empty_slot - base;

	if(offset % vec->data_size) 
		return -1; // data not aligned

	uint8_t *last_slot = vec->array + ((vec->length - 1) * vec->data_size);
	
	memcpy(empty_slot, last_slot, vec->data_size);

	vec->length--;

	// check if vector needs to shrink
	if (vec->length <= vec->capacity / 2 && vec->capacity > vec->reserve)
	{
		vec->capacity /= 2;
		vec->array = realloc(vec->array, (vec->capacity * vec->data_size));
	}

	return 0;
}


void vector_free(Vector *vec)
{
	free(vec->array);
	vec->array = NULL; // avoid dangling pointer
	vec->length = 0;
	vec->capacity = 0;
	vec->data_size = 0;
}


void vector_clear(Vector *vec)
{
	vec->length = 0;
	vec->capacity = vec->reserve;
}

