#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "scv.h"

#define SCV_AT(v, i) ((void *) ((char *) (v)->data + (i) * (v)->objsize))

/**
 * Minimum allocation size in bytes.
 */
#define SCV_MIN_ALLOC (64u)

/**
 * Grow the capacity of `v` to at least `capacity`.
 *
 * If more space is needed, grow `v` to `capacity`, but at least by a factor
 * of 1.5.
 *
 * @param v pointer to `scv_vector`
 * @param capacity requested capacity
 * @return zero on success, error code on error
 */
static int scv_i_grow(struct scv_vector *v, size_t capacity)
{
	void *newdata;
	size_t newcapacity;

	assert(v != NULL);
	assert(v->data != NULL);

	if (capacity <= v->capacity) {
		return SCV_OK;
	}

	assert(v->objsize > 0);

	if (capacity >= (size_t) -1 / v->objsize) {
		return SCV_ERANGE;
	}

	/* Growth factor 2 for small vectors, 1.5 for larger */
	if (v->capacity < 4096 / v->objsize) {
		newcapacity = v->capacity + v->capacity + 1;
	}
	else {
		newcapacity = v->capacity + v->capacity / 2 + 1;
	}

	if (capacity > newcapacity || newcapacity >= (size_t) -1 / v->objsize) {
		newcapacity = capacity;
	}

	newdata = realloc(v->data, newcapacity * v->objsize);

	if (newdata == NULL) {
		return SCV_ENOMEM;
	}

	v->data = newdata;
	v->capacity = newcapacity;

	return SCV_OK;
}

struct scv_vector *scv_new(size_t objsize, size_t capacity)
{
	struct scv_vector *v;

	if (capacity == 0) {
		capacity = 1;
	}

	if (objsize == 0 || capacity >= (size_t) -1 / objsize) {
		return NULL;
	}

	v = (struct scv_vector *) malloc(sizeof *v);

	if (v == NULL) {
		return NULL;
	}

	/* Minimum capacity is SCV_MIN_ALLOC bytes or 1 element */
	if (capacity * objsize < SCV_MIN_ALLOC) {
		capacity = (SCV_MIN_ALLOC + (objsize - 1)) / objsize;
	}

	v->data = malloc(capacity * objsize);

	if (v->data == NULL) {
		free(v);
		return NULL;
	}

	v->objsize = objsize;
	v->size = 0;
	v->capacity = capacity;

	return v;
}

void scv_delete(struct scv_vector *v)
{
	if (v == NULL) {
		return;
	}

	if (v->data != NULL) {
		free(v->data);
		v->data = NULL;
	}

	v->objsize = 0;
	v->size = 0;
	v->capacity = 0;

	free(v);
}

void *scv_data(struct scv_vector *v)
{
	assert(v != NULL);
	assert(v->data != NULL);

	if (v->size == 0) {
		return NULL;
	}

	return v->data;
}

int scv_empty(const struct scv_vector *v)
{
	assert(v != NULL);
	assert(v->data != NULL);

	return v->size == 0;
}



int scv_clear(struct scv_vector *v)
{
	assert(v != NULL);
	assert(v->data != NULL);

	v->size = 0;

	return SCV_OK;
}

int scv_assign(struct scv_vector *v, const void *data, size_t nobj)
{
	assert(v != NULL);
	assert(v->data != NULL);

	assert(v->objsize > 0);

	if (nobj >= (size_t) -1 / v->objsize) {
		return SCV_ERANGE;
	}

	if (nobj > v->capacity) {
		int res = scv_i_grow(v, nobj);

		if (res != SCV_OK) {
			return res;
		}
	}

	if (data != NULL && nobj > 0) {
		memcpy(v->data, data, nobj * v->objsize);
	}

	v->size = nobj;

	return SCV_OK;
}

int scv_replace(struct scv_vector *v, size_t i, size_t j, const void *data, size_t nobj)
{
	assert(v != NULL);
	assert(v->data != NULL);

	if (i > j || j > v->size) {
		return SCV_ERANGE;
	}

	assert(v->objsize > 0);

	if (nobj >= (size_t) -1 / v->objsize - (v->size - (j - i))) {
		return SCV_ERANGE;
	}

	if (v->size - (j - i) + nobj > v->capacity) {
		int res = scv_i_grow(v, v->size - (j - i) + nobj);

		if (res != SCV_OK) {
			return res;
		}
	}

	if (j < v->size && i + nobj != j) {
		memmove(SCV_AT(v, i + nobj), SCV_AT(v, j), (v->size - j) * v->objsize);
	}

	if (data != NULL && nobj > 0) {
		memcpy(SCV_AT(v, i), data, nobj * v->objsize);
	}

	v->size = v->size - (j - i) + nobj;

	return SCV_OK;
}

int scv_insert(struct scv_vector *v, size_t i, const void *data, size_t nobj)
{
	return scv_replace(v, i, i, data, nobj);
}

int scv_erase(struct scv_vector *v, size_t i, size_t j)
{
	return scv_replace(v, i, j, NULL, 0);
}