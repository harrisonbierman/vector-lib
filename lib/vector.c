#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <time.h>


typedef struct{
	void *array;
	size_t length;
	size_t capacity;
	size_t data_size;
}Vector;


Vector vector_create(size_t data_size) 
{
	Vector vec;
	vec.length = 0;
	vec.capacity = 1;
	vec.data_size = data_size;
	vec.array = malloc(vec.capacity * vec.data_size);
	
	return vec;
}


//need to also add an emplace function for faster pushing
int vector_push(Vector *vec, const void *item)
{

	if (vec->length >= vec->capacity) 
	{
		vec->capacity *= 2;
		void *tmp = realloc(vec->array, (vec->capacity * vec->data_size));
		if (!tmp) return -1; // allocation failed
		vec->array = tmp;
	}
	
	 void *slot  = (vec->array) + (vec->length * vec->data_size);
	 memcpy(slot, item, vec->data_size);
	 
	vec->length++;
	return 0;
}


void* vector_get(Vector *vec, size_t index)
{
	if (index >= vec->length) {
		return NULL; // access out of bounds 
	}
	void *tmp = vec->array + (index * vec->data_size);
	return tmp;
}


int vector_remove_ordered(Vector *vec, size_t index) 
{

	if (index >= vec->length)
	{
		return -1; // out of bounds error
	}

	
	// uint8_t *slot = vec->array + (index * vec->data_size);

	
	/*
	 * this is 8x slower than the next solution
	 * b/c I call memcpy for each element instead
	 * of shifting the whole thing down in one memcpy
	size_t shift_count = vec->length - index + 1;
	for (;shift_count != 0; shift_count--) 
	{
		uint8_t *next = slot + vec->data_size;
		*slot = *next;
		memcpy(slot, next, vec->data_size);
	}
	*/

	// shift the whole end of the array at once
	// size_t shift_amount = vec->length - index + 1;
	/*
	memcpy(vec->array + (index * vec->data_size), 
			(vec->array + (index * vec->data_size)) + vec->data_size,
			(vec->length - index + 1) * vec->data_size);
	*/

	/*
	 * using memmove instead (safer for overlaping memory)
	memmove(vec->array + (index * vec->data_size), 
			(vec->array + (index * vec->data_size)) + vec->data_size,
			(vec->length - index + 1) * vec->data_size);
	*/

	// caching a precalulated value
	
	void *empty_slot = vec->array + (index * vec->data_size);
	memmove(empty_slot, 
			empty_slot + vec->data_size,
			(vec->length - index + 1) * vec->data_size);

	vec->length--;

	if (vec->length <= vec->capacity / 2)
	{
		vec->capacity /= 2;
		vec->array = realloc(vec->array, (vec->capacity * vec->data_size));
	}

	return 0;

}

int vector_remove_unordered(Vector *vec, size_t index) 
{

	if (index >= vec->length)
	{
		return -1; // out of bounds error
	}

	memmove(vec->array + (index * vec->data_size),
			(vec->array + ((vec->length - 1) * vec->data_size)),
			 vec->data_size
			 );

	vec->length--;

	if (vec->length <= vec->capacity / 2)
	{
		vec->capacity /= 2;
		vec->array = realloc(vec->array, (vec->capacity * vec->data_size));
	}

	return 0;

}
void* vector_peek(Vector *vec)
{
	if (vec->length == 0) 
	{
		return NULL; // nothing to peak
	}
	void *item = vec->array + ((vec->length - 1) * vec->data_size);
	return item;
}

int vector_pop(Vector *vec)
{
	if (vec->length < 1) {
		return -1; // tried to pop empty array
	}
	vec->length--;
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
	vec->capacity = 1;
}

typedef struct MyStruct {
	int datai;
	float dataf;
	char *name;
}MyStruct;

int main() 
{
	MyStruct *billy = calloc(1, sizeof *billy);

	Vector vec_my_struct = vector_create(sizeof *billy);
	

	struct timespec start, end, start_push, end_push;
    const int runs = 1000000000;

    clock_gettime(CLOCK_MONOTONIC, &start_push);

    for (int i = 0; i < runs; i++) {
		vector_push(&vec_my_struct, billy);
    }

    clock_gettime(CLOCK_MONOTONIC, &end_push);

    // start timer
    clock_gettime(CLOCK_MONOTONIC, &start);

    // run many times
	for (int i = 0; i < runs; i++) {
		vector_remove_unordered(&vec_my_struct, 0);
	}


    // stop timer
    clock_gettime(CLOCK_MONOTONIC, &end);

	vector_free(&vec_my_struct);

    // Compute elapsed seconds
    double elapsed = (end.tv_sec - start.tv_sec)
                   + (end.tv_nsec - start.tv_nsec) / 1e9;
    double elapsed_push = (end_push.tv_sec - start_push.tv_sec)
                   + (end_push.tv_nsec - start_push.tv_nsec) / 1e9;
    printf("Push Total: %.6f s for %d runs → %.3f ns/run\n",
           elapsed_push, runs, (elapsed_push / runs) * 1e9);
    printf("Total: %.6f s for %d runs → %.3f ns/run\n",
           elapsed, runs, (elapsed / runs) * 1e9);
    return 0;
}
