/**
 * @file vector.h
 * @brief Generic dynamic array implementation in C
 *
 * This header declares a Vector type capable of holding 
 * arbitrary plain old data (POD) element. It supports push,
 * emplace, get, remove, clear, and free, with variations of 
 * each one
 *
 * Usage example:
 * @code
 *	Vector vec = vector_create(sizeof (float));
 *	float x = 42;
 *	vector_push_back(&vec, &x);
 *	float *p = vector_get_idx(&vec, 0);
 *  //...
 *  vector_free(&vec);
 * @endcode
 *
 * @author  Harrison Bierman
 * @date    2025-05-25
 * @license Apache 2.0
 */
#ifndef VECTOR_H
#define VECTOR_H
#include <stdlib.h>

/**
 * @struct  Vector
 * @brief  Opaque dynamic array of items of uniform size.
 */
typedef struct Vector Vector;


/**
 * @brief  Initalize a Vector.
 * @param  data_size      Size in bytes of the items to store.
 * @param  reserve_pow_2  Minimum capacity. Must be a power of 2.
 * @return Empty Vector on success
 * @return NULL on allocation failure
 */
Vector *vector_create(size_t data_size, size_t reserve_pow_2);


/**
 * @brief  Get length of the Vector.
 * @param  vec  Pointer to the Vector.
 * @return  length of the Vector.
 */
size_t vector_len(Vector *vec);


/**
 * @brief  Append an item to the end of the Vector.
 * @param  vec   Pointer to the Vector.
 * @param  item  Pointer to the Item to copy in.
 * @return 0 on success.
 * @return -1 on allocation failure.
 */
int vector_push_back(Vector *vec, const void *item);

/**
 * @brief  Make room at end of the Vector for item.
 * @param  vec  Pointer to the Vector.
 * @return  Pointer to item slot
 * @return  NULL on allocation failure
 * @example
 * @code
 * Person *p = (Person *)vector_emplace_back(vector_people);
 * if (p) {
 *		p->name = "Bob";
 *		p->id = 1234;
 * } else {
 *		perror("malloc");
 *		exit(EXIT_FAILURE);
 * }
 * @endcode
 */
void *vector_emplace_back(Vector *vec) ;

/**
 * @brief  Get item from vector with index
 * @param  vec    Pointer to the Vector.
 * @param  index  Index of item to get.
 * @return  Pointer to the item. 
 * @return  NULL if out of bounds
 */
void* vector_get(Vector *vec, size_t index);

/**
 * @brief  Private Function Used in foreach macro do not use this function. No safety checks.
 * @param  vec    Pointer to the Vector.
 * @param  index  Index of item to get.
 * @return  Pointer to the item.
 */
void* vector_unsafe_get(Vector *vec, size_t index);

/**
 * @brief  Finds the index of matching pointers to an item in the Vector.
 * @param  vec   Pointer to the Vector.
 * @param  item  Pointer to the item.
 * @return  Index on success.
 * @return  -1 on out of bounds.
 * @return  -2 on data miss-aligned.
 */
ssize_t vector_idx_of(Vector *vec, void *item) ;

/**
 * @brief  Removes item from the Vector while maintaining order of items. Slower than unordered.
 * @details O(n) time complexity if not allocating
 * @details O(n^2) if allocating.
 * @param  vec    Pointer to the Vector.
 * @param  index  Index of the item.
 * @return  0 on success.
 * @return  -1 on out of bounds.
 */
int vector_remove_idx_ordered(Vector *vec, size_t index) ;

/**
 * @brief  Removes item from the Vector with no regard to order of items. Faster than ordered.
 * @details O(1) time complexity if not allocating
 * @param  vec    Pointer to the Vector.
 * @param  index  Index of the item.
 * @return  0 on success.
 * @return  -1 on out of bounds.
 * @return  -2 on allocation failure
 * @related vector_remove_idx_unordered
 */
int vector_remove_idx_unordered(Vector *vec, size_t index) ;

/*
 * @brief  Removes item from the Vector while maintaining order of items. Slower than unordered.
 * @details O(n) time complexity if not allocating
 * @details O(n^2) if allocating
 * @param  vec   Pointer to the Vector.
 * @param  item  Pointer to the Item.
 * @return  0 on success.
 * @return  -1 on out of bounds.
 * @return  -2 on allocation failure.
 * @returns -3 on data miss-aligned.
 */
int vector_remove_ptr_ordered(Vector *vec, void *item);

/*
 * @brief  Removes item from the Vector with no regard to the order of items. Faster than ordered.
 * @details O(1) time complexity if not allocating
 * @param  vec  Pointer to the Vector.
 * @param  item  Pointer to the Item.
 * @return  0 on success.
 * @return  -1 on out of bounds.
 * @return  -2 on allocation failure.
 * @return  -3 on data miss-aligned.
 */
int vector_remove_ptr_unordered(Vector *vec, void *item);

/*
 * @brief removes all items from the Vector at once. 
 * @details O(1) Time Complexity.
 * @param  vec  Pointer to the Vector.
 * @return  void
 */
void vector_clear(Vector *vec);

/*
 * @brief frees array and vector struct.
 * @param  vec  Pointer to the Vector.
 * @return void
 */
void vector_free(Vector *vec);

/*
 * @brief  Macro that iterates over each item in Vector.
 * @details use _i_##item for index. 
 * @param  T  type of the item in the Vector.
 * @param  item_name  name of item in the Vector.
 * @param  p_vec  Pointer to the Vector.
 * @example 
 * vector_foreach(Person, person, vector_people) {
 *		printf("Index: %zu, Name: %s, ID: %zu", _i_person, person->name, person->id);		
 * }
 */
#define vector_foreach(T, item_name, p_vec)\
	for(size_t _i_##item = 0; _i_##item < vector_len(p_vec); _i_##item++)\
		for(T *item = (T *)vector_unsafe_get(p_vec, _i_##item); item; item = NULL)
#endif
