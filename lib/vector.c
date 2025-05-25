#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
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

//need to also add an emplace function for faster pushing
int vector_push_back(Vector *vec, const void *item)
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

void *vector_emplace_back(Vector *vec) 
{
	if (vec->length >= vec->capacity) 
	{
		vec->capacity *= 2;
		void *tmp = realloc(vec->array, (vec->capacity * vec->data_size));
		if (!tmp) return NULL; // allocation failed
		vec->array = tmp;
	}

	void *slot = vec->array + (vec->length * vec->data_size);
	vec->length++;
	return slot;
}

inline void* vector_get_idx(Vector *vec, size_t index)
{
	if (index >= vec->length) {
		return NULL; // access out of bounds 
	}

	return vec->array + (index * vec->data_size);
}

inline void* vector_unsafe_get_idx(Vector *vec, size_t index)
{
	void *slot = vec->array + (index * vec->data_size);
	return slot;
}

ssize_t vector_indexof(Vector *vec, void *item) 
{
	uint8_t *base = vec->array;
	uint8_t *ptr = item;

	size_t total_bytes = vec->length * vec->data_size;

	if(ptr < base || ptr >= (base + (total_bytes))) 
		return -1; // out of bounds
	
	size_t offset = ptr - base;

	if(offset % vec->data_size) 
		return -1; // data not aligned
	return offset / vec->data_size;
}

int vector_remove_ordered(Vector *vec, size_t index) 
{
	// out of bounds 
	if (index >= vec->length) return -1; 	

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

int vector_remove_idx_unordered(Vector *vec, size_t index) 
{

	if (index >= vec->length)
	{
		return -1; // out of bounds error or nothing in vector
	}

	void *empty_slot = vec->array + (index * vec->data_size);
	void *last_slot = vec->array + ((vec->length - 1) * vec->data_size);
	
	memcpy(empty_slot, last_slot, vec->data_size);

	vec->length--;

	if (vec->length <= vec->capacity / 2 && vec->capacity > vec->reserve)
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

	void *last_slot = vec->array + ((vec->length - 1) * vec->data_size);
	
	memcpy(empty_slot, last_slot, vec->data_size);

	vec->length--;

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

typedef struct Person {
	int id;
	char *name;
}Person;

#define vector_foreach(T, item, vec)\
	for(size_t _i_##item = 0; _i_##item < vec.length; _i_##item++)\
		for(T *item = (T *)vector_unsafe_get_idx(&vec, _i_##item); item; item = NULL)

ssize_t Tvector_indexof(Vector *vec, void *item)
{
	for(size_t i = 0; i < vec->length; i++) 
	{
		if (vec->array + (i * vec->data_size) == item) 
		{
			return i;
		}
	}
	return -1;
}

int main() 
{
	Vector vector_people = vector_create(sizeof (Person), 1);

	Person *person1 = vector_emplace_back(&vector_people);
	person1->id = 324;
	person1->name = "Bacon";

	Person *person2 = vector_emplace_back(&vector_people);
	if(person2) memset(person2, 0, sizeof (Person));

	Person *person3 = vector_emplace_back(&vector_people);
	if(person3){
		person3->id = 100;
		person3->name = "Satan";
	}

	// vector_remove_ptr_unordered(&vector_people, person2);

	ssize_t index = vector_indexof(&vector_people, person1);
	printf("The index of person3 is: %ld\n", index);

	vector_foreach(Person, person, vector_people){
		printf("index: %zu, value: %d, name: %s\n", _i_person, person->id, person->name);
	}

}
