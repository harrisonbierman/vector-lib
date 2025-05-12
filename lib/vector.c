#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

typedef struct{
	void *array;
	size_t length;
	size_t capacity;
	size_t data_size;
}Vector;

Vector vector_create(size_t data_size) 
{
	Vector vec = {
		.length = 0,
		.capacity = 2,
		.data_size = data_size,
		.array = malloc(vec.capacity * vec.data_size)
	};
	
	return vec;
}

int vector_push(Vector *vec, const void *item)
{
	if (vec->length >= vec->capacity) 
	{
		size_t new_capacity = vec->capacity * 2;
		size_t new_size     = new_capacity * vec->data_size;
		void *tmp = realloc(vec->array, new_size);
		if (!tmp) return -1; // allocation failed
		vec->array = tmp;
		vec->capacity = new_capacity;
	}
	
	uint8_t *bytes = (uint8_t *)vec->array;
	uint8_t *slot  = bytes + (vec->length * vec->data_size);
	memcpy(slot, item, vec->data_size);
	vec->length++;
	return 0;
}

void* vector_get(Vector *vec, size_t index)
{
	void *tmp = vec	
}


int main() 
{
	Vector my_vec = vector_create(sizeof (int));

	int num3 = 3;

	vector_push(&my_vec, &num3);

	printf("my vector number is %d",); 
}
