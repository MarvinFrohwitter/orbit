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
         char *: (ht)->compare ? (ht)->compare : ((ht)->compare = orbit_strcmp),                                       \
         const char *: (ht)->compare ? (ht)->compare : ((ht)->compare = orbit_strcmp),                                 \
         volatile char *: (ht)->compare ? (ht)->compare : ((ht)->compare = orbit_strcmp),                              \
         volatile const char *: (ht)->compare ? (ht)->compare : ((ht)->compare = orbit_strcmp),                        \
         signed char *: (ht)->compare ? (ht)->compare : ((ht)->compare = orbit_strcmp),                                \
         const signed char *: (ht)->compare ? (ht)->compare : ((ht)->compare = orbit_strcmp),                          \
         volatile signed char *: (ht)->compare ? (ht)->compare : ((ht)->compare = orbit_strcmp),                       \
         volatile const signed char *: (ht)->compare ? (ht)->compare : ((ht)->compare = orbit_strcmp),                 \
         unsigned char *: (ht)->compare ? (ht)->compare : ((ht)->compare = orbit_strcmp),                              \
         const unsigned char *: (ht)->compare ? (ht)->compare : ((ht)->compare = orbit_strcmp),                        \
         volatile unsigned char *: (ht)->compare ? (ht)->compare : ((ht)->compare = orbit_strcmp),                     \
         volatile const unsigned char *: (ht)->compare ? (ht)->compare : ((ht)->compare = orbit_strcmp),               \
         default: (ht)->compare ? (ht)->compare : ((ht)->compare = memcmp)))

#define orbit_key_data(ht, _key)                                                                                       \
    (_Generic((ht)->elements->kv.key,                                                                                  \
         char *: orbit__key_data_cstr,                                                                                 \
         const char *: orbit__key_data_cstr,                                                                           \
         volatile char *: orbit__key_data_cstr,                                                                        \
         volatile const char *: orbit__key_data_cstr,                                                                  \
         signed char *: orbit__key_data_cstr,                                                                          \
         const signed char *: orbit__key_data_cstr,                                                                    \
         volatile signed char *: orbit__key_data_cstr,                                                                 \
         volatile const signed char *: orbit__key_data_cstr,                                                           \
         unsigned char *: orbit__key_data_cstr,                                                                        \
         const unsigned char *: orbit__key_data_cstr,                                                                  \
         volatile unsigned char *: orbit__key_data_cstr,                                                               \
         volatile const unsigned char *: orbit__key_data_cstr,                                                         \
         default: orbit__key_data_bytes)((const void *) (typeof((ht)->elements->kv.key)[]){(_key)}))

#define orbit_key_size(ht, _key)                                                                                       \
    (_Generic((ht)->elements->kv.key,                                                                                  \
         char *: orbit__key_size_cstr,                                                                                 \
         const char *: orbit__key_size_cstr,                                                                           \
         volatile char *: orbit__key_size_cstr,                                                                        \
         volatile const char *: orbit__key_size_cstr,                                                                  \
         signed char *: orbit__key_size_cstr,                                                                          \
         const signed char *: orbit__key_size_cstr,                                                                    \
         volatile signed char *: orbit__key_size_cstr,                                                                 \
         volatile const signed char *: orbit__key_size_cstr,                                                           \
         unsigned char *: orbit__key_size_cstr,                                                                        \
         const unsigned char *: orbit__key_size_cstr,                                                                  \
         volatile unsigned char *: orbit__key_size_cstr,                                                               \
         volatile const unsigned char *: orbit__key_size_cstr,                                                         \
         default: orbit__key_size_bytes)((const void *) (typeof((ht)->elements->kv.key)[]){(_key)},                    \
                                         sizeof((ht)->elements->kv.key)))

void *orbit__put(Orbit_Generic_HT *ht, int (*key_compare)(const void *, const void *, size_t), void *key, void *value,
                 size_t sizeof_key, size_t sizeof_value, size_t offsetof__next, size_t offsetof_occupied,
                 size_t offsetof_key, size_t offsetof_value, size_t offsetof_kv, size_t sizeof_comparison,
                 size_t satellite_size, bool is_key_cstr);

#define orbit_put(ht, _key, _value, ...)                                                                               \
    (assert(ht),                                                                                                       \
     (typeof(ht->elements->kv) *) orbit__put(                                                                          \
         (Orbit_Generic_HT *) ht, ORBIT_GET_COMPARE(orbit_key_compare(ht), __VA_ARGS__),                               \
         ((typeof(ht->elements->kv.key)[]){(_key)}), ((typeof(ht->elements->kv.value)[]){(_value)}),                   \
         sizeof((ht)->elements->kv.key), sizeof((ht)->elements->kv.value), offsetof(typeof(*ht->elements), _next),     \
         offsetof(typeof(*ht->elements), occupied), offsetof(typeof(*ht->elements), kv.key),                           \
         offsetof(typeof(*ht->elements), kv.value), offsetof(typeof(*ht->elements), kv), orbit_key_size(ht, _key),     \
         sizeof(*ht->elements), orbit_is_cstr(ht->elements->kv.key)))

#define orbit_delete(ht, _key, _value)                                                                                 \
    do {                                                                                                               \
        if ((ht) && (ht)->elements && (ht)->capacity) {                                                                \
            size_t ht_index = orbit_hash(orbit_key_data(ht, _key), orbit_key_size(ht, _key), (ht)->capacity);          \
            if ((ht)->compare == NULL) {                                                                               \
                (ht)->compare = orbit_key_compare(ht);                                                                 \
            }                                                                                                          \
            typeof(*(ht)->elements) *current_satellite = (ht)->elements[ht_index]._next;                               \
            typeof(*(ht)->elements) *prev_satellite = &(ht)->elements[ht_index];                                       \
            if ((ht)->elements[ht_index].occupied) {                                                                   \
                if ((ht)->compare(((typeof((ht)->elements->kv.key)[]){(ht)->elements[ht_index].kv.key}),               \
                                  ((typeof((ht)->elements->kv.key)[]){(_key)}), orbit_key_size((ht), (_key))) == 0) {  \
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
                                         orbit_key_size((ht), (_key))) != 0) {                                         \
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
         (Orbit_Generic_HT *) ht, key_compare, orbit_key_data(ht, _key), orbit_key_size(ht, _key),                     \
         ((typeof((ht)->elements->kv.key)[]){(_key)}), offsetof(typeof(*(ht)->elements), _next),                       \
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
void *orbit__get(Orbit_Generic_HT *ht, int (*key_compare)(const void *, const void *, size_t), void *key,
                 size_t key_size, void *key_object, size_t offsetof__next, size_t offsetof_occupied,
                 size_t offsetof_key, size_t offsetof_value, size_t satellite_size);
void *orbit_get_next_valid_entry(Orbit_Generic_HT *ht, size_t offsetof__next, size_t offsetof_occupied,
                                 size_t offsetof_kv, size_t satellite_size);

void *orbit__ht_insert(Orbit_Generic_HT *ht, int (*key_compare)(const void *, const void *, size_t), void *key,
                       void *value, size_t sizeof_key, size_t sizeof_value, size_t offsetof__next,
                       size_t offsetof_occupied, size_t offsetof_key, size_t offsetof_value, size_t offsetof_kv,
                       size_t sizeof_comparison, size_t satellite_size, size_t hash_index);
void orbit__free_chain_nodes(void *elements, size_t capacity, size_t offsetof__next, size_t satellite_size);
size_t orbit_strlen(const char *str);
int orbit_strcmp(const void *s1, const void *s2, size_t n);

static inline void *orbit__key_data_cstr(const void *key_object) {
    return (void *) *(const char *const *) key_object;
}

static inline void *orbit__key_data_bytes(const void *key_object) {
    return (void *) key_object;
}

static inline size_t orbit__key_size_cstr(const void *key_object, size_t key_field_size) {
    (void) key_field_size;
    return orbit_strlen(*(const char *const *) key_object);
}

static inline size_t orbit__key_size_bytes(const void *key_object, size_t key_field_size) {
    (void) key_object;
    return key_field_size;
}

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

void orbit__free_chain_nodes(void *elements, size_t capacity, size_t offsetof__next, size_t satellite_size) {
    if (!elements) {
        return;
    }
    for (size_t i = 0; i < capacity; ++i) {
        uint8_t *satellite = (uint8_t *) elements + i * satellite_size;
        void *next = *(void **) (satellite + offsetof__next);
        while (next) {
            uint8_t *node = (uint8_t *) next;
            next = *(void **) (node + offsetof__next);
            free(node);
        }
    }
}

void *orbit__ht_insert(Orbit_Generic_HT *ht, int (*key_compare)(const void *, const void *, size_t), void *key,
                       void *value, size_t sizeof_key, size_t sizeof_value, size_t offsetof__next,
                       size_t offsetof_occupied, size_t offsetof_key, size_t offsetof_value, size_t offsetof_kv,
                       size_t sizeof_comparison, size_t satellite_size, size_t hash_index) {

    bool contatins_key = false;
    void *current_satellite = (char *) ht->elements + hash_index * satellite_size;
    if (!*(((char *) current_satellite) + offsetof_occupied)) {
        memcpy((char *) current_satellite + offsetof_value, value, sizeof_value);
        memcpy((char *) current_satellite + offsetof_key, key, sizeof_key);
        memcpy((char *) current_satellite + offsetof_occupied, ((bool[]){true}), sizeof(bool));
        ht->filled_slots++;
    } else {
        if (key_compare((char *) current_satellite + offsetof_key, key, sizeof_comparison) == 0) {
            contatins_key = true;
        }
        while (!contatins_key && *((char **) ((char *) current_satellite + offsetof__next))) {
            if (key_compare((*(char **) ((char *) current_satellite + offsetof__next)) + offsetof_key, key,
                            sizeof_comparison) == 0) {
                contatins_key = true;
            }
            current_satellite = *(char **) ((char *) current_satellite + offsetof__next);
        }
        if (contatins_key) {
            memcpy((char *) current_satellite + offsetof_value, value, sizeof_value);
        } else {
            char *new_satellite = malloc(satellite_size);
            if (new_satellite == NULL) {
                return NULL;
            }
            memset(new_satellite, 0, satellite_size);
            memcpy(new_satellite + offsetof_key, key, sizeof_key);
            memcpy(new_satellite + offsetof_value, value, sizeof_value);
            memcpy(new_satellite + offsetof_occupied, ((bool[]){true}), sizeof(bool));
            *(char **) ((char *) current_satellite + offsetof__next) = new_satellite;
            current_satellite = new_satellite;
        }
    }
    if (!contatins_key) {
        ht->count++;
    }

    return (char *) current_satellite + offsetof_kv;
}

void *orbit__put(Orbit_Generic_HT *ht, int (*key_compare)(const void *, const void *, size_t), void *key, void *value,
                 size_t sizeof_key, size_t sizeof_value, size_t offsetof__next, size_t offsetof_occupied,
                 size_t offsetof_key, size_t offsetof_value, size_t offsetof_kv, size_t sizeof_comparison,
                 size_t satellite_size, bool is_key_cstr) {
    if (!ht) {
        return NULL;
    }

    if (ht->filled_slots >= ht->capacity) {
        size_t old_capacity = ht->capacity;
        void *old_elements = ht->elements;
        size_t new_capacity = old_capacity    ? old_capacity * 2
                              : ht->init_size ? ht->init_size
                                              : ORBIT_HT_INIT_DEFAULT_CAPACITY;
        void *new_ptr = calloc(1, new_capacity * satellite_size);
        if (new_ptr == NULL) {
            return NULL;
        }

        Orbit_Generic_HT new_ht = *ht;
        new_ht.elements = new_ptr;
        new_ht.count = 0;
        new_ht.filled_slots = 0;
        new_ht.capacity = new_capacity;
        orbit_itterator_reset(&new_ht);

        for (size_t i = 0; i < old_capacity; ++i) {
            uint8_t *satellite = (uint8_t *) old_elements + i * satellite_size;
            while (satellite) {
                if (*(bool *) (satellite + offsetof_occupied)) {
                    void *key_ptr = satellite + offsetof_key;
                    void *value_ptr = satellite + offsetof_value;
                    void *hash_data = is_key_cstr ? *(void **) key_ptr : key_ptr;
                    size_t hash_size = is_key_cstr ? orbit_strlen(*(char **) key_ptr) : sizeof_key;
                    size_t new_hash_index = orbit_hash(hash_data, hash_size, new_capacity);

                    if (orbit__ht_insert(&new_ht, key_compare, key_ptr, value_ptr, sizeof_key, sizeof_value,
                                         offsetof__next, offsetof_occupied, offsetof_key, offsetof_value, offsetof_kv,
                                         hash_size, satellite_size, new_hash_index) == NULL) {
                        orbit__free_chain_nodes(new_ht.elements, new_capacity, offsetof__next, satellite_size);
                        free(new_ht.elements);
                        orbit_itterator_reset(ht);
                        return NULL;
                    }
                }
                satellite = *(uint8_t **) (satellite + offsetof__next);
            }
        }

        *ht = new_ht;

        orbit__free_chain_nodes(old_elements, old_capacity, offsetof__next, satellite_size);
        if (old_elements) free(old_elements);
        orbit_itterator_reset(ht);
    }

    size_t hash_index = orbit_hash(is_key_cstr ? *(void **) key : key,
                                   is_key_cstr ? orbit_strlen(*(void **) key) : sizeof_key, ht->capacity);

    return orbit__ht_insert(ht, key_compare, key, value, sizeof_key, sizeof_value, offsetof__next, offsetof_occupied,
                            offsetof_key, offsetof_value, offsetof_kv, sizeof_comparison, satellite_size, hash_index);
}

void *orbit__get(Orbit_Generic_HT *ht, int (*key_compare)(const void *, const void *, size_t), void *key,
                 size_t key_size, void *key_object, size_t offsetof__next, size_t offsetof_occupied,
                 size_t offsetof_key, size_t offsetof_value, size_t satellite_size) {
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
