#ifndef ORBIT_H_
#define ORBIT_H_

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ORBIT_HT_INIT_DEFAULT_CAPACITY (64)
#define __Satellite(key_type, value_type)                                                                              \
    struct {                                                                                                           \
        void *_next;                                                                                                   \
        bool occupied;                                                                                                 \
        struct {                                                                                                       \
            key_type key;                                                                                              \
            value_type value;                                                                                          \
        } kv;                                                                                                          \
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

#define ORBIT_GET_COMPARE_(_default, _provided, ...) _provided
#define ORBIT_GET_COMPARE(_default, ...) ORBIT_GET_COMPARE_(_default __VA_OPT__(, ) __VA_ARGS__, _default)
#define orbit_sizeof_comparison(x)                                                                                     \
    (_Generic((x),                                                                                                     \
         char *: orbit_strlen(*(char **) ((char *[]){(char *) *((intptr_t *) &x)})),                                   \
         const char *: orbit_strlen(*(char **) ((char *[]){(char *) *((intptr_t *) &x)})),                             \
         volatile char *: orbit_strlen(*(char **) ((char *[]){(char *) *((intptr_t *) &x)})),                          \
         volatile const char *: orbit_strlen(*(char **) ((char *[]){(char *) *((intptr_t *) &x)})),                    \
         signed char *: orbit_strlen(*(char **) ((char *[]){(char *) *((intptr_t *) &x)})),                            \
         const signed char *: orbit_strlen(*(char **) ((char *[]){(char *) *((intptr_t *) &x)})),                      \
         volatile signed char *: orbit_strlen(*(char **) ((char *[]){(char *) *((intptr_t *) &x)})),                   \
         volatile const signed char *: orbit_strlen(*(char **) ((char *[]){(char *) *((intptr_t *) &x)})),             \
         unsigned char *: orbit_strlen(*(char **) ((char *[]){(char *) *((intptr_t *) &x)})),                          \
         const unsigned char *: orbit_strlen(*(char **) ((char *[]){(char *) *((intptr_t *) &x)})),                    \
         volatile unsigned char *: orbit_strlen(*(char **) ((char *[]){(char *) *((intptr_t *) &x)})),                 \
         volatile const unsigned char *: orbit_strlen(*(char **) ((char *[]){(char *) *((intptr_t *) &x)})),           \
                                                                                                                       \
         default: sizeof(x)))

#define orbit_is_cstr(X)                                                                                               \
    (_Generic((X),                                                                                                     \
         char *: true,                                                                                                 \
         const char *: true,                                                                                           \
         volatile char *: true,                                                                                        \
         volatile const char *: true,                                                                                  \
         signed char *: true,                                                                                          \
         const signed char *: true,                                                                                    \
         volatile signed char *: true,                                                                                 \
         volatile const signed char *: true,                                                                           \
         unsigned char *: true,                                                                                        \
         const unsigned char *: true,                                                                                  \
         volatile unsigned char *: true,                                                                               \
         volatile const unsigned char *: true,                                                                         \
         default: false))

#define orbit_key_compare(ht)                                                                                          \
    (_Generic((ht)->elements->kv.key,                                                                                  \
         char *: orbit_strcmp,                                                                                         \
         const char *: orbit_strcmp,                                                                                   \
         volatile char *: orbit_strcmp,                                                                                \
         volatile const char *: orbit_strcmp,                                                                          \
         signed char *: orbit_strcmp,                                                                                  \
         const signed char *: orbit_strcmp,                                                                            \
         volatile signed char *: orbit_strcmp,                                                                         \
         volatile const signed char *: orbit_strcmp,                                                                   \
         unsigned char *: orbit_strcmp,                                                                                \
         const unsigned char *: orbit_strcmp,                                                                          \
         volatile unsigned char *: orbit_strcmp,                                                                       \
         volatile const unsigned char *: orbit_strcmp,                                                                 \
         default: memcmp))

#define orbit_key_data(ht, _key)                                                                                       \
    (_Generic((ht)->elements->kv.key,                                                                                  \
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
         default: (void *) (typeof(ht->elements->kv.key)[]){(_key)}))

#define orbit_key_size(ht, _key)                                                                                       \
    (_Generic((ht)->elements->kv.key,                                                                                  \
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
         default: sizeof((ht)->elements->kv.key)))

#define orbit_put(ht, _key, _value)                                                                                    \
    orbit_put_impl(ht, _key, _value,                                                                                   \
                   orbit_hash(orbit_key_data(ht, _key), orbit_key_size(ht, _key),                                      \
                              ht->capacity    ? ht->capacity                                                           \
                              : ht->init_size ? ht->init_size                                                          \
                                              : ORBIT_HT_INIT_DEFAULT_CAPACITY))

void *orbit__put(Orbit_Generic_HT *ht, void *key, void *value, size_t sizeof_key, size_t sizeof_value,
                 size_t offsetof__next, size_t offsetof_occupied, size_t offsetof_key, size_t offsetof_value,
                 size_t offsetof_kv, size_t sizeof_comparison, size_t satellite_size, size_t hash_index,
                 bool is_key_cstr);

#define orbit_put_impl(ht, _key, _value, hash_index)                                                                   \
    (assert(ht), (typeof(ht->elements->kv) *) orbit__put(                                                              \
                     (Orbit_Generic_HT *) ht, ((typeof(ht->elements->kv.key)[]){(_key)}),                              \
                     ((typeof(ht->elements->kv.value)[]){(_value)}), sizeof(_key), sizeof(_value),                     \
                     offsetof(typeof(*ht->elements), _next), offsetof(typeof(*ht->elements), occupied),                \
                     offsetof(typeof(*ht->elements), kv.key), offsetof(typeof(*ht->elements), kv.value),               \
                     offsetof(typeof(*ht->elements), kv),                                                              \
                     ht ? ht->elements ? orbit_sizeof_comparison(ht->elements[hash_index].kv.key) : 0 : 0,             \
                     sizeof(*ht->elements), hash_index, orbit_is_cstr(ht->elements->kv.key)))

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
                if ((ht)->compare(((typeof((ht)->elements->kv.key)[]){(ht)->elements[ht_index].kv.key}),               \
                                  ((typeof((ht)->elements->kv.key)[]){(_key)}),                                        \
                                  orbit_sizeof_comparison((ht)->elements[ht_index].kv.key)) == 0) {                    \
                    if ((ht)->elements[ht_index]._next == NULL) {                                                      \
                        memset(&(ht)->elements[ht_index], 0, sizeof(*(ht)->elements));                                 \
                        (ht)->filled_slots--;                                                                          \
                    } else {                                                                                           \
                        (ht)->elements[ht_index].occupied =                                                            \
                            ((typeof(*(ht)->elements) *) (ht)->elements[ht_index]._next)->occupied;                    \
                        (ht)->elements[ht_index].kv.key =                                                              \
                            ((typeof(*(ht)->elements) *) (ht)->elements[ht_index]._next)->kv.key;                      \
                        (ht)->elements[ht_index].kv.value =                                                            \
                            ((typeof(*(ht)->elements) *) (ht)->elements[ht_index]._next)->kv.value;                    \
                        (ht)->elements[ht_index]._next =                                                               \
                            ((typeof(*(ht)->elements) *) (ht)->elements[ht_index]._next)->_next;                       \
                        free(current_satellite);                                                                       \
                    }                                                                                                  \
                    (ht)->count--;                                                                                     \
                } else {                                                                                               \
                    while (current_satellite &&                                                                        \
                           (ht)->compare(((typeof((ht)->elements->kv.key)[]){current_satellite->kv.key}),              \
                                         ((typeof((ht)->elements->kv.key)[]){(_key)}),                                 \
                                         orbit_sizeof_comparison((ht)->elements[ht_index].kv.key)) != 0) {             \
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
                    printf("%zu:" key_fmt " -> " value_fmt "\n", i, key_args((ht)->elements[i].kv.key),                \
                           value_args((ht)->elements[i].kv.value));                                                    \
                                                                                                                       \
                    typeof(*(ht)->elements) *current_satellite = &(ht)->elements[i];                                   \
                    int indent = 0;                                                                                    \
                    while (current_satellite->_next && current_satellite->occupied) {                                  \
                        current_satellite = current_satellite->_next;                                                  \
                        printf("%*s%zu:" key_fmt " -> " value_fmt "\n", ++indent * 4, "", i,                           \
                               key_args(current_satellite->kv.key), value_args(current_satellite->kv.value));          \
                    }                                                                                                  \
                } else {                                                                                               \
                    if (print_all) {                                                                                   \
                        printf("%zu -> empty\n", i);                                                                   \
                    }                                                                                                  \
                }                                                                                                      \
            }                                                                                                          \
        }                                                                                                              \
    } while (0);

#define orbit_get_comp(ht, _key, key_compare)                                                                          \
    (assert((ht)), assert((ht)->elements),                                                                             \
     (typeof((ht)->elements->kv.value) *) orbit__get(                                                                  \
         (Orbit_Generic_HT *) ht, orbit_key_data(ht, _key), orbit_key_size(ht, _key),                                  \
         ((typeof((ht)->elements->kv.key)[]){(_key)}), key_compare, offsetof(typeof(*(ht)->elements), _next),          \
         offsetof(typeof(*(ht)->elements), occupied), offsetof(typeof(*(ht)->elements), kv.key),                       \
         offsetof(typeof(*(ht)->elements), kv.value), sizeof(*(ht)->elements)))

#define orbit_get(ht, _key, ...) orbit_get_comp(ht, _key, ORBIT_GET_COMPARE(orbit_key_compare(ht), __VA_ARGS__))

#define orbit_itterator_reset(ht) (ht)->satellite_index_to_visit_next = 0, (ht)->satellite_to_visit_next = NULL
#define orbit_itterator(ht) (assert(ht), orbit_itterator_reset(ht), orbit_next((ht)))

#define orbit_next(ht)                                                                                                 \
    (assert((ht)),                                                                                                     \
     (typeof((ht)->elements->kv) *) orbit_get_next_valid_entry(                                                        \
         (Orbit_Generic_HT *) ht, offsetof(typeof(*ht->elements), _next), offsetof(typeof(*ht->elements), occupied),   \
         offsetof(typeof(*ht->elements), kv), sizeof(*(ht)->elements)))

#define orbit_foreach(it, ht)                                                                                          \
    for (typeof((ht)->elements->kv) *(it) = orbit_itterator(ht); (it); (it) = orbit_next((ht)))

#define orbit_is_empty(ht) (!(ht) || (ht)->count == 0)

uintptr_t orbit_djb2_hash(void *data, size_t size);
size_t orbit_hash(void *buffer, size_t size, size_t max_index);
void *orbit__get(Orbit_Generic_HT *ht, void *key, size_t key_size, void *key_object,
                 int (*key_compare)(const void *, const void *, size_t), size_t offsetof__next,
                 size_t offsetof_occupied, size_t offsetof_key, size_t offsetof_value, size_t satellite_size);
void *orbit_get_next_valid_entry(Orbit_Generic_HT *ht, size_t offsetof__next, size_t offsetof_occupied,
                                 size_t offsetof_kv, size_t satellite_size);

void *orbit__ht_insert(Orbit_Generic_HT *ht, void *key, void *value, size_t sizeof_key, size_t sizeof_value,
                       size_t offsetof__next, size_t offsetof_occupied, size_t offsetof_key, size_t offsetof_value,
                       size_t offsetof_kv, size_t sizeof_comparison, size_t satellite_size, size_t hash_index);
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

void *orbit__ht_insert(Orbit_Generic_HT *ht, void *key, void *value, size_t sizeof_key, size_t sizeof_value,
                       size_t offsetof__next, size_t offsetof_occupied, size_t offsetof_key, size_t offsetof_value,
                       size_t offsetof_kv, size_t sizeof_comparison, size_t satellite_size, size_t hash_index) {

    bool contatins_key = false;
    void *current_satellite = ht->elements + hash_index * satellite_size;
    if (!*(((char *) current_satellite) + offsetof_occupied)) {
        memcpy((char *) current_satellite + offsetof_value, value, sizeof_value);
        memcpy((char *) current_satellite + offsetof_key, key, sizeof_key);
        memcpy((char *) current_satellite + offsetof_occupied, ((bool[]){true}), sizeof(bool));
        ht->filled_slots++;
    } else {
        if (ht->compare == NULL) {
            ht->compare = orbit_memcmp;
        }
        if (ht->compare((char *) current_satellite + offsetof_key, key, sizeof_comparison) == 0) {
            contatins_key = true;
        }
        while (!contatins_key && *((char **) ((char *) current_satellite + offsetof__next))) {
            if (ht->compare((*(char **) ((char *) current_satellite + offsetof__next)) + offsetof_key, key,
                            sizeof_comparison) == 0) {
                contatins_key = true;
            }
            current_satellite = *(char **) ((char *) current_satellite + offsetof__next);
        }
        if (contatins_key) {
            memcpy((char *) current_satellite + offsetof_value, value, sizeof_value);
        } else {
            *(char **) ((char *) current_satellite + offsetof__next) = malloc(satellite_size);
            if (*(char **) ((char *) current_satellite + offsetof__next) == NULL) {
                return NULL;
            }
            memset(*(char **) ((char *) current_satellite + offsetof__next), 0, satellite_size);
            memcpy(*(char **) (((char *) current_satellite) + offsetof__next) + offsetof_key, key, sizeof_key);
            memcpy(*(char **) (((char *) current_satellite) + offsetof__next) + offsetof_value, value, sizeof_value);
            memcpy(*(char **) (((char *) current_satellite) + offsetof__next) + offsetof_occupied, ((bool[]){true}),
                   sizeof(bool));
        }
    }
    if (!contatins_key) {
        ht->count++;
    }

    return (char *) current_satellite + offsetof_kv;
}

void *orbit__put(Orbit_Generic_HT *ht, void *key, void *value, size_t sizeof_key, size_t sizeof_value,
                 size_t offsetof__next, size_t offsetof_occupied, size_t offsetof_key, size_t offsetof_value,
                 size_t offsetof_kv, size_t sizeof_comparison, size_t satellite_size, size_t hash_index,
                 bool is_key_cstr) {
    if (!ht) {
        return NULL;
    }

    if (ht->filled_slots >= ht->capacity) {
        size_t old_capacity = ht->capacity;
        void *old_ht_elements = ht->elements;
        ht->capacity = ht->capacity ? ht->capacity * 2 : ht->init_size ? ht->init_size : ORBIT_HT_INIT_DEFAULT_CAPACITY;
        void *new_ptr = calloc(1, ht->capacity * satellite_size);
        if (new_ptr == NULL) {
            ht->capacity = old_capacity;
            return NULL;
        }

        Orbit_Generic_HT new_ht = *ht;
        new_ht.elements = new_ptr;
        new_ht.count = 0;
        new_ht.filled_slots = 0;
        new_ht.capacity = ht->capacity;
        orbit_itterator_reset(&new_ht);

        orbit_foreach(it, ht) {
            size_t new_hash_index = orbit_hash(is_key_cstr ? it->key : &it->key,
                                               is_key_cstr ? orbit_strlen(it->key) : sizeof_key, new_ht.capacity);

            if (orbit__ht_insert(&new_ht, &it->key, &it->value, sizeof_key, sizeof_value, offsetof__next,
                                 offsetof_occupied, offsetof_key, offsetof_value, offsetof_kv,
                                 is_key_cstr ? orbit_strlen(it->key) : sizeof_key, satellite_size,
                                 new_hash_index) == NULL)

            {
                assert(false && "Re insertion failed");
                free(new_ht.elements);
                ht->capacity = old_capacity;
                return NULL;
            }
        }

        *ht = new_ht;

        if (old_ht_elements) free(old_ht_elements);
        orbit_itterator_reset(ht);
    }

    return orbit__ht_insert(ht, key, value, sizeof_key, sizeof_value, offsetof__next, offsetof_occupied, offsetof_key,
                            offsetof_value, offsetof_kv, sizeof_comparison, satellite_size, hash_index);
}

void *orbit__get(Orbit_Generic_HT *ht, void *key, size_t key_size, void *key_object,
                 int (*key_compare)(const void *, const void *, size_t), size_t offsetof__next,
                 size_t offsetof_occupied, size_t offsetof_key, size_t offsetof_value, size_t satellite_size) {
    if (ht && ht->elements && ht->capacity) {
        size_t ht_index = orbit_hash(key, key_size, ht->capacity);

        uint8_t *current_satellite = (uint8_t *) ht->elements + ht_index * satellite_size;
        do {
            bool occupied = *(char *) (current_satellite + offsetof_occupied);
            if (occupied && key_compare(current_satellite + offsetof_key, key_object, key_size) == 0) {
                return current_satellite + offsetof_value;
            }
            current_satellite = *(void **) (current_satellite + offsetof__next);
        } while (current_satellite);
    }
    return NULL;
}

void *orbit_get_next_valid_entry(Orbit_Generic_HT *ht, size_t offsetof__next, size_t offsetof_occupied,
                                 size_t offsetof_kv, size_t satellite_size) {
    if (!ht || !ht->elements || ht->capacity == 0 || ht->count == 0) {
        return NULL;
    }

    if (ht->satellite_to_visit_next) {
        void *next_satellite = *(void **) ((char *) ht->satellite_to_visit_next + offsetof__next);
        if (next_satellite) {
            ht->satellite_to_visit_next = next_satellite;
            return next_satellite + offsetof_kv;
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
        return satellite + offsetof_kv;
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
    for (; n && *str1 && *str2 && *str1 == *str2; str1++, str2++, n--);
    return *str1 - *str2;
}

#endif  // ORBIT_IMPLEMENTATION
