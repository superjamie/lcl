/**
 * @file   array.h
 * @brief  Little C Library - array functions
 */
#ifndef LCL_ARRAY_H
#define LCL_ARRAY_H

#include <limits.h>
#include <stddef.h>
#include <stdlib.h>

#if CHAR_BIT != 8
  #error "CHAR_BIT == 8 required"
#endif

/**
 * Dynamically allocate zeroed 2D array from heap.
 *
 * The returned pointer can be addressed like a normal stack-allocated array:
 *
 * ```c
 * int **my_2d = init_array_2d(5, 5, sizeof(**my2d));
 *
 * my_2d[0][2] = 1;
 * ```
 *
 * Caller is responsible to free when finished:
 *
 * ```c
 * free(my_2d);
 * my_2d = NULL;
 * ```
 *
 * @param  rows       The number of horizontal rows in the array
 * @param  cols       The number of vertical columns in the array
 * @param  type_size  The size of the type the array stores, as per sizeof
 *
 * @return            Pointer to array on success, NULL on failure
 */
void *lcl_array2d_init(size_t rows, size_t cols, size_t type_size);

/**
 * Dynamically allocate a zeroed 3D array from heap.
 *
 * The returned pointer can be addressed like a normal stack-allocated array:
 *
 * ```c
 * int ***my_3d = init_array_3d(5, 5, 5, sizeof(***my_3d));
 *
 * my_3d[0][2][4] = 1;
 * ```
 *
 * Caller is responsible to free when finished:
 *
 * ```c
 * free(my_3d);
 * my_3d = NULL;
 * ```
 *
 * @param  layers     The number of layers of "row x column" arrays
 * @param  rows       The number of horizontal rows in the array
 * @param  cols       The number of vertical columns in the array
 * @param  type_size  The size of the type the array stores, as per sizeof
 *
 * @return            Pointer to array on success, NULL on failure
 */
void *lcl_array3d_init(size_t layers, size_t rows, size_t cols, size_t type_size);

void *lcl_array2d_init(size_t rows, size_t cols, size_t type_size)
{
	unsigned char **array = NULL;
	size_t alloc_len = rows * sizeof(unsigned char *) +
			   rows * cols * type_size;

	array = (unsigned char **)calloc(1, alloc_len);
	if (!array)
		return NULL;

	unsigned char *rows_start = (unsigned char *)(array + rows);

	for (size_t row = 0; row < rows; row++)
		array[row] = (rows_start + row * cols * type_size);

	return array;
}

void *lcl_array3d_init(size_t layers, size_t rows, size_t cols, size_t type_size)
{
	unsigned char ***array = NULL;
	size_t alloc_len = layers * sizeof(unsigned char **) +
			   layers * rows * sizeof(unsigned char *) +
			   layers * rows * cols * type_size;

	array = (unsigned char ***)calloc(1, alloc_len);
	if (!array)
		return NULL;

	unsigned char **rows_start = (unsigned char **)(array + layers);

	for (size_t layer = 0; layer < layers; layer++)
		array[layer] = (rows_start + layer * rows);

	unsigned char *cols_start = (unsigned char *) rows_start +
		             (layers * rows * sizeof(unsigned char *));

	for (size_t layer = 0; layer < layers; layer++)
		for (size_t row = 0; row < rows; row++)
			array[layer][row] = (cols_start + type_size *
					    (row + layer * rows) * cols);

	return array;
}

#endif
