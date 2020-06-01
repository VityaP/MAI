#ifndef SCV_H_INCLUDED
#define SCV_H_INCLUDED
#include <stddef.h>


struct scv_vector {
	void *data;      /**< Pointer to the underlying memory. */
	size_t objsize;  /**< Size of each element in bytes. */
	size_t size;     /**< Used size in number of elements. */
	size_t capacity; /**< Capacity in number of elements. */
};

typedef enum {
	SCV_OK = 0,      /**< Success. */
	SCV_ERROR = -1,  /**< Generic error. */
	SCV_ENOMEM = -2, /**< Out of memory. */
	SCV_ERANGE = -3, /**< Overflow or out of range. */
	SCV_EINVAL = -4  /**< Invalid argument. */
} scv_error_code;

/**
 * Create a new `scv_vector`.
 *
 * `capacity` is in number of elements.
 *
 * @param objsize size of each element in bytes
 * @param capacity initial capacity in number of elements
 * @return pointer to `scv_vector`, `NULL` on error
 */
struct scv_vector *scv_new(size_t objsize, size_t capacity);

/**
 * Destroy `v`, freeing the associated memory.
 *
 * @param v pointer to `scv_vector`.
 */
void scv_delete(struct scv_vector *v);

/**
 * Return a pointer to element number `i` of `v`.
 *
 * @param v pointer to `scv_vector`
 * @param i index
 * @return pointer to element `i`, `NULL` on error
 */

void *scv_data(struct scv_vector *v);

/**
 * Check if `v` is empty.
 *
 * @param v pointer to `scv_vector`
 * @return non-zero if empty
 */
int scv_empty(const struct scv_vector *v);

/**
 * Remove all elements from `v`.
 *
 * @param v pointer to `scv_vector`
 * @return zero on success, error code on error
 */
int scv_clear(struct scv_vector *v);


/**
 * Insert `nobj` elements from `data` before element number `i` of `v`.
 *
 * If `data` is `NULL`, inserted elements are not initialized.
 *
 * `i` can be `scv_size(v)`, in which case elements are added at the end.
 *
 * `data` must not point inside `v`.
 *
 * @param v pointer to `scv_vector`
 * @param i index
 * @param data pointer to data to copy into new elements
 * @param nobj number of elements to insert
 * @return zero on success, error code on error
 */
int scv_insert(struct scv_vector *v, size_t i, const void *data, size_t nobj);



#endif /* SCV_H_INCLUDED */