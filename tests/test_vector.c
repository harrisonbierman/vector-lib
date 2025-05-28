// Testing Suite for vector.c
#include "../include/vector.h"
#include <stdio.h>


void test_push_and_get() {
	int num = 42;
	Vector *vec_num = vector_create(sizeof num, 4);

	vector_push_back(vec_num, &num);
	int *result = (int*)vector_get_idx(vec_num, 0);

	printf("Test push and get: ");
	if (num == *result) {
		printf("PASS\n");
	} else {
		printf("FAIL\n");
	}
	vector_free(vec_num);
}

void test_out_of_bounds() {
	int num = 42;
	Vector *vec_num = vector_create(sizeof num, 4);

	vector_push_back(vec_num, &num);
	int *result = (int*)vector_get_idx(vec_num, -1);

	printf("Test out of bounds (lower): ");
	if(result == NULL) {
		printf("PASS\n");
	} else {
		printf("FAIL\n");
	}

	result = (int*)vector_get_idx(vec_num, 1);

	printf("Test out of bounds (upper): ");
	if(result == NULL) {
		printf("PASS\n");
	} else {
		printf("FAIL\n");
	}
	vector_free(vec_num);

}


int main() {
	test_push_and_get();
	test_out_of_bounds();
}
