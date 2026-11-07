#ifndef ORBIT_H_
#define ORBIT_H_

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __Satellite(key_type, value_type)                                                                              \
    struct {                                                                                                           \
        void *_next;                                                                                                   \
        bool occupied;                                                                                                 \
        key_type key;                                                                                                  \
        value_type value;                                                                                              \
    }

#define Orbit_HT(key_type, value_type, ...)                                                                            \
    struct {                                                                                                           \
        __Satellite(key_type, value_type) * elements;                                                                  \
        size_t count;                                                                                                  \
        size_t capacity;                                                                                               \
        size_t filled_slots;                                                                                           \
        size_t init_size;                                                                                              \
        int (*compare)(const void *, const void *, size_t n);                                                          \
                                                                                                                       \
        size_t satellite_index_to_visit_next;                                                                          \
        __Satellite(key_type, value_type) * satellite_to_visit_next;                                                   \
        __VA_ARGS__;                                                                                                   \
    }

typedef Orbit_HT(void *, void *) Orbit_Generic_HT;

#define orbit_sizeof_comparison(x)                                                                                     \
    (_Generic((x),                                                                                                     \
         char *: orbit_strlen(*(char **) ((char *[]){(char *) *((intptr_t *) &x)})),                                   \
         const char *: orbit_strlen(*(char **) ((char *[]){(char *) *((intptr_t *) &x)})),                             \
         volatile char *: orbit_strlen(*(char **) ((char *[]){(char *) *((intptr_t *) &x)})),                          \
         volatile const char *: orbit_strlen(*(char **) ((char *[]){(char *) *((intptr_t *) &x)})),                    \
                                                                                                                       \
         signed char *: orbit_strlen(*(char **) ((char *[]){(char *) *((intptr_t *) &x)})),                            \
         const signed char *: orbit_strlen(*(char **) ((char *[]){(char *) *((intptr_t *) &x)})),                      \
         volatile signed char *: orbit_strlen(*(char **) ((char *[]){(char *) *((intptr_t *) &x)})),                   \
         volatile const signed char *: orbit_strlen(*(char **) ((char *[]){(char *) *((intptr_t *) &x)})),             \
                                                                                                                       \
         unsigned char *: orbit_strlen(*(char **) ((char *[]){(char *) *((intptr_t *) &x)})),                          \
         const unsigned char *: orbit_strlen(*(char **) ((char *[]){(char *) *((intptr_t *) &x)})),                    \
         volatile unsigned char *: orbit_strlen(*(char **) ((char *[]){(char *) *((intptr_t *) &x)})),                 \
         volatile const unsigned char *: orbit_strlen(*(char **) ((char *[]){(char *) *((intptr_t *) &x)})),           \
                                                                                                                       \
         default: sizeof(x)))

#define orbit_key_data(ht, _key)                                                                                       \
    (_Generic((ht)->elements->key,                                                                                     \
         char *: (void *) (_key),                                                                                      \
         const char *: (void *) (_key),                                                                                \
         volatile char *: (void *) (_key),                                                                             \
         volatile const char *: (void *) (_key),                                                                       \
         signed char *: (void *) (_key),                                                                               \
         const signed char *: (void *) (_key),                                                                         \
         volatile signed char *: (void *) (_key),                                                                      \
         volatile const signed char *: (void *) (_key),                                                                \
         unsigned char *: (void *) (_key),                                                                             \
         const unsigned char *: (void *) (_key),                                                                       \
         volatile unsigned char *: (void *) (_key),                                                                    \
         volatile const unsigned char *: (void *) (_key),                                                              \
         default: (void *) ((typeof((ht)->elements->key)[]){(_key)})))

#define orbit_key_size(ht, _key)                                                                                       \
    (_Generic((ht)->elements->key,                                                                                     \
         char *: orbit_strlen((const char *) (_key)),                                                                  \
         const char *: orbit_strlen((const char *) (_key)),                                                            \
         volatile char *: orbit_strlen((const char *) (_key)),                                                         \
         volatile const char *: orbit_strlen((const char *) (_key)),                                                   \
         signed char *: orbit_strlen((const char *) (_key)),                                                           \
         const signed char *: orbit_strlen((const char *) (_key)),                                                     \
         volatile signed char *: orbit_strlen((const char *) (_key)),                                                  \
         volatile const signed char *: orbit_strlen((const char *) (_key)),                                            \
         unsigned char *: orbit_strlen((const char *) (_key)),                                                         \
         const unsigned char *: orbit_strlen((const char *) (_key)),                                                   \
         volatile unsigned char *: orbit_strlen((const char *) (_key)),                                                \
         volatile const unsigned char *: orbit_strlen((const char *) (_key)),                                          \
         default: sizeof((ht)->elements->key)))

#define orbit_put(ht, _key, _value)                                                                                    \
    do {                                                                                                               \
        ; /* if (!(ht)) { */ /* return NULL; */ /* } */                                                                \
        assert(ht != NULL);                                                                                            \
        if ((ht)->filled_slots >= (ht)->capacity) {                                                                    \
            size_t old_capacity = (ht)->capacity;                                                                      \
            (ht)->capacity = (ht)->capacity ? (ht)->capacity * 2 : (ht)->init_size ? (ht)->init_size : 64;             \
            (ht)->elements = realloc((ht)->elements, (ht)->capacity * sizeof(*(ht)->elements));                        \
            if ((ht)->elements == NULL) {                                                                              \
                fprintf(stderr, "ERROR: Allocation has failed!\n");                                                    \
                abort();                                                                                               \
                ; /* return NULL;  */                                                                                  \
            }                                                                                                          \
            memset((ht)->elements + old_capacity, 0, ((ht)->capacity - old_capacity) * sizeof(*(ht)->elements));       \
        }                                                                                                              \
        size_t ht_index = orbit_hash(orbit_key_data(ht, _key), orbit_key_size(ht, _key), (ht)->capacity);              \
        bool contatins_value = false;                                                                                  \
        if (!(ht)->elements[ht_index].occupied) {                                                                      \
            (ht)->elements[ht_index].value = (_value);                                                                 \
            (ht)->elements[ht_index].key = (_key);                                                                     \
            (ht)->elements[ht_index].occupied = true;                                                                  \
            (ht)->filled_slots++;                                                                                      \
        } else {                                                                                                       \
            typeof(*(ht)->elements) *current_satellite = &(ht)->elements[ht_index];                                    \
            if ((ht)->compare == NULL) {                                                                               \
                (ht)->compare = orbit_memcmp;                                                                          \
            }                                                                                                          \
            if ((ht)->compare(((typeof((ht)->elements->value)[]){current_satellite->value}),                           \
                              ((typeof((ht)->elements->value)[]){(_value)}),                                           \
                              orbit_sizeof_comparison((ht)->elements[ht_index].value)) == 0) {                         \
                contatins_value = true;                                                                                \
            }                                                                                                          \
            while (!contatins_value && current_satellite->_next) {                                                     \
                if ((ht)->compare(((typeof((ht)->elements->value)[]){                                                  \
                                      ((typeof(*(ht)->elements) *) current_satellite->_next)->value}),                 \
                                  ((typeof((ht)->elements->value)[]){(_value)}),                                       \
                                  orbit_sizeof_comparison((ht)->elements[ht_index].value)) == 0) {                     \
                    contatins_value = true;                                                                            \
                }                                                                                                      \
                current_satellite = current_satellite->_next;                                                          \
            }                                                                                                          \
            if (contatins_value) {                                                                                     \
                current_satellite->value = (_value);                                                                   \
            } else {                                                                                                   \
                current_satellite->_next = malloc(sizeof(*(ht)->elements));                                            \
                if (current_satellite->_next == NULL) {                                                                \
                    fprintf(stderr, "ERROR: Allocation for collision resolution has failed!\n");                       \
                    abort();                                                                                           \
                    ; /* return NULL;  */                                                                              \
                }                                                                                                      \
                memset(current_satellite->_next, 0, sizeof(*(ht)->elements));                                          \
                ((typeof(*(ht)->elements) *) current_satellite->_next)->value = (_value);                              \
                ((typeof(*(ht)->elements) *) current_satellite->_next)->key = (_key);                                  \
                ((typeof(*(ht)->elements) *) current_satellite->_next)->occupied = true;                               \
            }                                                                                                          \
        }                                                                                                              \
        if (!contatins_value) {                                                                                        \
            (ht)->count++;                                                                                             \
        }                                                                                                              \
                                                                                                                       \
        ; /* return &ht->elements[ht_index]; */                                                                        \
    } while (0)

#define orbit_delete(ht, _key, _value)                                                                                 \
    do {                                                                                                               \
        if ((ht) && (ht)->elements && (ht)->capacity) {                                                                \
            size_t ht_index = orbit_hash(orbit_key_data(ht, _key), orbit_key_size(ht, _key), (ht)->capacity);          \
            if ((ht)->compare == NULL) {                                                                               \
                (ht)->compare = orbit_memcmp;                                                                          \
            }                                                                                                          \
            typeof(*(ht)->elements) *current_satellite = (ht)->elements[ht_index]._next;                               \
            typeof(*(ht)->elements) *prev_satellite = &(ht)->elements[ht_index];                                       \
            if ((ht)->elements[ht_index].occupied) {                                                                   \
                if ((ht)->compare(((typeof((ht)->elements->value)[]){(ht)->elements[ht_index].value}),                 \
                                  ((typeof((ht)->elements->value)[]){(_value)}),                                       \
                                  orbit_sizeof_comparison((ht)->elements[ht_index].value)) == 0) {                     \
                    if ((ht)->elements[ht_index]._next == NULL) {                                                      \
                        memset(&(ht)->elements[ht_index], 0, sizeof(*(ht)->elements));                                 \
                        (ht)->filled_slots--;                                                                          \
                    } else {                                                                                           \
                        (ht)->elements[ht_index].occupied =                                                            \
                            ((typeof(*(ht)->elements) *) (ht)->elements[ht_index]._next)->occupied;                    \
                        (ht)->elements[ht_index].key =                                                                 \
                            ((typeof(*(ht)->elements) *) (ht)->elements[ht_index]._next)->key;                         \
                        (ht)->elements[ht_index].value =                                                               \
                            ((typeof(*(ht)->elements) *) (ht)->elements[ht_index]._next)->value;                       \
                        (ht)->elements[ht_index]._next =                                                               \
                            ((typeof(*(ht)->elements) *) (ht)->elements[ht_index]._next)->_next;                       \
                        free(current_satellite);                                                                       \
                    }                                                                                                  \
                    (ht)->count--;                                                                                     \
                } else {                                                                                               \
                    while (current_satellite &&                                                                        \
                           (ht)->compare(((typeof((ht)->elements->value)[]){current_satellite->value}),                \
                                         ((typeof((ht)->elements->value)[]){(_value)}),                                \
                                         orbit_sizeof_comparison((ht)->elements[ht_index].value)) != 0) {              \
                        prev_satellite = current_satellite;                                                            \
                        current_satellite = current_satellite->_next;                                                  \
                    }                                                                                                  \
                    if (current_satellite) {                                                                           \
                        prev_satellite->_next = current_satellite->_next;                                              \
                        free(current_satellite);                                                                       \
                        (ht)->count--;                                                                                 \
                    } else {                                                                                           \
                        /* Value not found */                                                                          \
                    }                                                                                                  \
                }                                                                                                      \
            }                                                                                                          \
        }                                                                                                              \
    } while (0)

#define orbit_free(ht)                                                                                                 \
    do {                                                                                                               \
        if ((ht) && (ht)->elements && (ht)->capacity) {                                                                \
            for (size_t i = 0; i < (ht)->capacity; ++i) {                                                              \
                if (!(ht)->elements[i].occupied) continue;                                                             \
                typeof(*(ht)->elements) *current_satellite = (ht)->elements[i]._next;                                  \
                while (current_satellite && current_satellite->occupied) {                                             \
                    void *_next = current_satellite->_next;                                                            \
                    free(current_satellite);                                                                           \
                    current_satellite = _next;                                                                         \
                }                                                                                                      \
            }                                                                                                          \
            free((ht)->elements);                                                                                      \
        }                                                                                                              \
    } while (0)

#define orbit_print(ht, print_all, key_fmt, value_fmt, key_args, value_args)                                           \
    do {                                                                                                               \
        if ((ht) && (ht)->elements && (ht)->capacity) {                                                                \
            printf("----------<<< Table >>>----------\n");                                                             \
            for (size_t i = 0; i < (ht)->capacity; ++i) {                                                              \
                if ((ht)->elements[i].occupied) {                                                                      \
                    printf("%zu:" key_fmt " -> " value_fmt "\n", i, key_args((ht)->elements[i].key),                   \
                           value_args((ht)->elements[i].value));                                                       \
                                                                                                                       \
                    typeof(*(ht)->elements) *current_satellite = &(ht)->elements[i];                                   \
                    int indent = 0;                                                                                    \
                    while (current_satellite->_next && current_satellite->occupied) {                                  \
                        current_satellite = current_satellite->_next;                                                  \
                        printf("%*s%zu:" key_fmt " -> " value_fmt "\n", ++indent * 4, "", i,                           \
                               key_args(current_satellite->key), value_args(current_satellite->value));                \
                    }                                                                                                  \
                } else {                                                                                               \
                    if (print_all) {                                                                                   \
                        printf("%zu -> empty\n", i);                                                                   \
                    }                                                                                                  \
                }                                                                                                      \
            }                                                                                                          \
        }                                                                                                              \
    } while (0);

#define orbit_get(ht, _key, _value)                                                                                    \
    (assert((ht)), assert((ht)->elements),                                                                             \
     (typeof((ht)->elements->value) *) orbit__get(                                                                     \
         (Orbit_Generic_HT *) ht, orbit_key_data(ht, _key), ((typeof((ht)->elements->value)[]){(_value)}),             \
         orbit_key_size(ht, _key), orbit_sizeof_comparison((ht)->elements->value),                                     \
         offsetof(typeof(*(ht)->elements), value), sizeof(*(ht)->elements)))

#define orbit_itterator(ht)                                                                                            \
    ((ht)->satellite_index_to_visit_next = 0, (ht)->satellite_to_visit_next = NULL, orbit_next((ht)))

#define orbit_next(ht)                                                                                                 \
    (assert((ht)), assert((ht)->elements),                                                                             \
     (typeof(*(ht)->elements) *) orbit_get_next_valid_entry(                                                           \
         (Orbit_Generic_HT *) ht, offsetof(typeof(*ht->elements), _next), offsetof(typeof(*ht->elements), occupied),   \
         sizeof(*(ht)->elements)))

#define orbit_foreach(it, ht, ...)                                                                                     \
    for (typeof(*(ht)->elements) *(it) = orbit_itterator(ht); (it); (it) = orbit_next((ht)))

#define orbit_is_empty(ht) (!(ht) || (ht)->count == 0)

uintptr_t orbit_djb2_hash(void *data, size_t size);
size_t orbit_hash(void *buffer, size_t size, size_t max_index);
void *orbit__get(Orbit_Generic_HT *ht, void *key, void *value, size_t key_size, size_t value_size,
                 size_t offsetof_value, size_t satellite_size);
void *orbit_get_next_valid_entry(Orbit_Generic_HT *ht, size_t offsetof__next, size_t offsetof_occupied,
                                 size_t satellite_size);
size_t orbit_strlen(const char *str);
int orbit_strcmp(const void *s1, const void *s2, size_t n);
int orbit_memcmp(const void *s1, const void *s2, size_t n);

#endif  // ORBIT_H_

// ===========================================================================

#ifdef ORBIT_IMPLEMENTATION

uintptr_t orbit_djb2_hash(void *data, size_t size) {
    const uint8_t *bytes = data;
    uintptr_t hash = 5381U;
    for (size_t i = 0; i < size; ++i) {
        hash = ((hash << 5) + hash) + (uintptr_t) bytes[i];
    }
    return hash;
}

size_t orbit_hash(void *buffer, size_t size, size_t max_index) {
    if (max_index == 0) {
        return 0;
    }
    return orbit_djb2_hash(buffer, size) % max_index;
}

void *orbit__get(Orbit_Generic_HT *ht, void *key, void *value, size_t key_size, size_t value_size,
                 size_t offsetof_value, size_t satellite_size) {
    if (ht && ht->elements && ht->capacity) {
        size_t ht_index = orbit_hash(key, key_size, ht->capacity);
        if ((ht)->compare == NULL) {
            (ht)->compare = orbit_memcmp;
        }

        uint8_t *current_satellite = (uint8_t *) ht->elements + ht_index * satellite_size;
        do {
            if ((ht)->compare(current_satellite + offsetof_value, value, value_size) == 0) {
                return current_satellite + offsetof_value;
            }
            current_satellite = (void *) (uint64_t) *(current_satellite + 0);
        } while (current_satellite);
    }
    return NULL;
}

void *orbit_get_next_valid_entry(Orbit_Generic_HT *ht, size_t offsetof__next, size_t offsetof_occupied,
                                 size_t satellite_size) {
    if (!ht || !ht->elements || ht->capacity == 0 || ht->count == 0) {
        return NULL;
    }

    if (ht->satellite_to_visit_next) {
        void *next_satellite = *(void **) ((char *) ht->satellite_to_visit_next + offsetof__next);
        if (next_satellite) {
            ht->satellite_to_visit_next = next_satellite;
            return next_satellite;
        }
        ht->satellite_to_visit_next = NULL;
        ht->satellite_index_to_visit_next++;
    }

    for (; ht->satellite_index_to_visit_next < ht->capacity; ht->satellite_index_to_visit_next++) {
        uint8_t *satellite = (uint8_t *) ht->elements + ht->satellite_index_to_visit_next * satellite_size;
        bool occupied = *(uint8_t *) (satellite + offsetof_occupied);
        if (!occupied) {
            continue;
        }

        ht->satellite_to_visit_next = (void *) satellite;
        return satellite;
    }
    return NULL;
}

size_t orbit_strlen(const char *str) {
    if (!str) {
        return 0;
    }
    const char *start = str;
    for (; *str; str++);
    return str - start;
}

int orbit_memcmp(const void *s1, const void *s2, size_t n) {
    if (!s1 || !s2) {
        return -1;
    }
    const unsigned char *data1 = (const unsigned char *) *(void *const *) s1;
    const unsigned char *data2 = (const unsigned char *) *(void *const *) s2;
    if (!data1 && !data2) {
        return 0;
    }
    if (!data1 || !data2) {
        return -1;
    }
    for (; n && *data1 == *data2; n--, data1++, data2++);
    return n ? *data1 - *data2 : 0;
}

int orbit_strcmp(const void *s1, const void *s2, size_t n) {
    if (!s1 || !s2) {
        return -1;
    }
    const unsigned char *str1 = (const unsigned char *) *(char *const *) s1;
    const unsigned char *str2 = (const unsigned char *) *(char *const *) s2;
    if (!str1 && !str2) {
        return 0;
    }
    if (!str1 || !str2) {
        return -1;
    }
    if (!n--) {
        return 0;
    }
    for (; n && *str1 && *str2 && *str1 == *str2; str1++, str2++, n--);
    return *str1 - *str2;
}

#endif  // ORBIT_IMPLEMENTATION
