#ifndef CRUDER_H
#define CRUDER_H 1

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// version
#define CRUDER_VERSION "0.0.1"

typedef intptr_t int_t;
typedef uintptr_t uint_t;

#if INTPTR_MAX == INT64_MAX
typedef double float_t;
#else
typedef float float_t;
#endif


#define cruder(type) \
    struct type##_list_t { \
        uint_t capacity; \
        uint_t size; \
        type data[]; \
    }

#define cruder_type(type) \
    struct type##_list_t

#define cruder_new(type) \
    ({ \
        cruder_type(type) *list = malloc(sizeof(cruder_type(type)) + sizeof(type)); \
        if (list) { \
            list->capacity = 0; \
            list->size = 0; \
        } \
        list; \
    })

#define cruder_free(list) \
    do { \
        if (list) { \
            free(list); \
        } \
    } while (0)

#define cruder_double(list) \
    do { \
        if (list->capacity == 0) { \
            list->capacity = 1; \
        } else { \
            list->capacity *= 2; \
        } \
        typeof(list) new_list = malloc(sizeof(typeof(*list)) + sizeof(typeof(list->data[0])) * list->capacity); \
        if (!new_list) { \
            fprintf(stderr, "CRUDER_ERROR: failed to allocate memory for list\n"); \
            exit(EXIT_FAILURE); \
        } \
        memcpy(new_list->data, list->data, sizeof(typeof(list->data[0])) * list->size); \
        new_list->capacity = list->capacity; \
        new_list->size = list->size; \
        free(list); \
        list = new_list; \
    } while (0)

#define cruder_half(list) \
    do { \
        if (list->size < list->capacity / 4 && list->capacity > 1) { \
            list->capacity /= 2; \
            typeof(list) new_list = malloc(sizeof(typeof(*list)) + sizeof(typeof(list->data[0])) * list->capacity); \
            if (!new_list) { \
                fprintf(stderr, "CRUDER_ERROR: failed to allocate memory for list\n"); \
                exit(EXIT_FAILURE); \
            } \
            memcpy(new_list->data, list->data, sizeof(typeof(list->data[0])) * list->size); \
            new_list->capacity = list->capacity; \
            new_list->size = list->size; \
            free(list); \
            list = new_list; \
        } \
    } while (0)

#define cruder_push(list, value) \
    do { \
        cruder_double(list); \
        list->data[list->size++] = value; \
    } while (0)

#define cruder_unshift(list, value) \
    do { \
        cruder_double(list); \
        memmove(&list->data[1], &list->data[0], sizeof(typeof(list->data[0])) * list->size); \
        list->data[0] = value; \
        list->size++; \
    } while (0)

#define cruder_insert(list, i, value) \
    do { \
        if (i < 0 || i > list->size) { \
            fprintf(stderr, "CRUDER_ERROR: index %d out of range in list of size %d\n", i, list->size); \
            exit(EXIT_FAILURE); \
        } \
        cruder_double(list); \
        memmove(&list->data[i + 1], &list->data[i], sizeof(typeof(list->data[0])) * (list->size - i)); \
        list->data[i] = value; \
        list->size++; \
    } while (0)

#define cruder_pop(list) \
    ({ \
        if (list->size == 0) { \
            fprintf(stderr, "CRUDER_ERROR: cannot pop from empty list\n"); \
            exit(EXIT_FAILURE); \
        } \
        list->data[--list->size]; \
    })

#define cruder_shift(list) \
    ({ \
        if (list->size == 0) { \
            fprintf(stderr, "CRUDER_ERROR: cannot shift from empty list\n"); \
            exit(EXIT_FAILURE); \
        } \
        typeof(list->data[0]) value = list->data[0]; \
        memmove(&list->data[0], &list->data[1], sizeof(typeof(list->data[0])) * (list->size - 1)); \
        list->size--; \
        value; \
    })

#define cruder_remove(list, i) \
    do { \
        if (i < 0 || i >= list->size) { \
            fprintf(stderr, "CRUDER_ERROR: index %d out of range in list of size %d\n", i, list->size); \
            exit(EXIT_FAILURE); \
        } \
        memmove(&list->data[i], &list->data[i + 1], sizeof(typeof(list->data[0])) * (list->size - i - 1)); \
        list->size--; \
        cruder_half(list); \
    } while (0)

#define cruder_swap(list, i, j) \
    do { \
        if (i < 0 || i >= list->size || j < 0 || j >= list->size) { \
            fprintf(stderr, "CRUDER_ERROR: index out of range in list of size %d\n", list->size); \
            exit(EXIT_FAILURE); \
        } \
        typeof(list->data[0]) temp = list->data[i]; \
        list->data[i] = list->data[j]; \
        list->data[j] = temp; \
    } while (0)

// just swap and pop
#define cruder_fast_remove(list, i) \
    do { \
        if (i < 0 || i >= list->size) { \
            fprintf(stderr, "CRUDER_ERROR: index %d out of range in list of size %d\n", i, list->size); \
            exit(EXIT_FAILURE); \
        } \
        cruder_swap(list, i, list->size - 1); \
        list->size--; \
        cruder_half(list); \
    } while (0)

#define cruder_reverse(list) \
    do { \
        for (int_t i = 0; i < list->size / 2; i++) { \
            cruder_swap(list, i, list->size - i - 1); \
        } \
    } while (0)

#define cruder_find(list, value) \
    ({ \
        int_t index = -1; \
        for (int_t i = 0; i < list->size; i++) { \
            if (list->data[i] == value) { \
                index = i; \
                break; \
            } \
        } \
        index; \
    })

#define cruder_copy(list) \
    ({ \
        typeof(list) *copy = cruder_init(typeof(list->data[0]), list->capacity); \
        if (!copy) { \
            fprintf(stderr, "CRUDER_ERROR: failed to allocate memory for list copy\n"); \
            exit(EXIT_FAILURE); \
        } \
        copy->size = list->size; \
        memcpy(copy->data, list->data, sizeof(typeof(list->data[0])) * list->size); \
        copy; \
    })

#define cruder_get(list, i) \
    ({ \
        if (i < 0 || i >= list->size) { \
            fprintf(stderr, "CRUDER_ERROR: index %d out of range in list of size %d\n", i, list->size); \
            exit(EXIT_FAILURE); \
        } \
        list->data[i]; \
    })

#define cruder_set(list, i, value) \
    do { \
        if (i < 0 || i >= list->size) { \
            fprintf(stderr, "CRUDER_ERROR: index %d out of range in list of size %d\n", i, list->size); \
            exit(EXIT_FAILURE); \
        } \
        list->data[i] = value; \
    } while (0)

#define cruder_size(list) (list->size)

#endif // CRUDER_H