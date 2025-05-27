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
 * @return  Pointer to item slot, NULL on allocation failure
 */
void *vector_emplace_back(Vector *vec) ;
inline void* vector_get_idx(Vector *vec, size_t index);
static inline void* vector_unsafe_get_idx(Vector *vec, size_t index);
ssize_t vector_idx_of(Vector *vec, void *item) ;
int vector_remove_idx_ordered(Vector *vec, size_t index) ;
int vector_remove_idx_unordered(Vector *vec, size_t index) ;
int vector_remove_ptr_ordered(Vector *vec, void *item);
int vector_remove_ptr_unordered(Vector *vec, void *item);
void vector_clear(Vector *vec);
void vector_free(Vector *vec);

#define vector_foreach(T, item, vec)\
	for(size_t _i_##item = 0; _i_##item < vec.length; _i_##item++)\
		for(T *item = (T *)vector_unsafe_get_idx(&vec, _i_##item); item; item = NULL)
#endif
