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
        size_t ht_index =                                                                                              \
            orbit_hash(((typeof((ht)->elements->key)[]){(_key)}), sizeof((ht)->elements->key), (ht)->capacity);        \
        bool contatins_value = false;                                                                                  \
        if (!(ht)->elements[ht_index].occupied) {                                                                      \
            (ht)->elements[ht_index].value = (_value);                                                                 \
            (ht)->elements[ht_index].key = (_key);                                                                     \
            (ht)->elements[ht_index].occupied = true;                                                                  \
            (ht)->filled_slots++;                                                                                      \
        } else {                                                                                                       \
            typeof(*(ht)->elements) *current_satellite = &(ht)->elements[ht_index];                                    \
            if ((ht)->compare == NULL) {                                                                               \
                (ht)->compare = memcmp;                                                                                \
            }                                                                                                          \
            if ((ht)->compare(((typeof((ht)->elements->key)[]){current_satellite->key}),                               \
                              ((typeof((ht)->elements->key)[]){(_key)}), sizeof((ht)->elements[ht_index].key)) == 0) { \
                contatins_value = true;                                                                                \
            }                                                                                                          \
            while (!contatins_value && current_satellite->_next) {                                                     \
                if ((ht)->compare(((typeof((ht)->elements->key)[]){                                                    \
                                      ((typeof(*(ht)->elements) *) current_satellite->_next)->key}),                   \
                                  ((typeof((ht)->elements->key)[]){(_key)}),                                           \
                                  sizeof((ht)->elements[ht_index].key)) == 0) {                                        \
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

#define orbit_delete(ht, _key)                                                                                         \
    do {                                                                                                               \
        if ((ht) && (ht)->elements && (ht)->capacity) {                                                                \
            size_t ht_index =                                                                                          \
                orbit_hash(((typeof((ht)->elements->key)[]){(_key)}), sizeof((ht)->elements->key), (ht)->capacity);    \
            if ((ht)->compare == NULL) {                                                                               \
                (ht)->compare = memcmp;                                                                                \
            }                                                                                                          \
            typeof(*(ht)->elements) *current_satellite = (ht)->elements[ht_index]._next;                               \
            typeof(*(ht)->elements) *prev_satellite = &(ht)->elements[ht_index];                                       \
            if ((ht)->elements[ht_index].occupied) {                                                                   \
                if ((ht)->compare(((typeof((ht)->elements->key)[]){(ht)->elements[ht_index].key}),                     \
                                  ((typeof((ht)->elements->key)[]){(_key)}),                                           \
                                  sizeof((ht)->elements[ht_index].key)) == 0) {                                        \
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
                           (ht)->compare(((typeof((ht)->elements->key)[]){current_satellite->key}),                    \
                                         ((typeof((ht)->elements->key)[]){(_key)}),                                    \
                                         sizeof((ht)->elements[ht_index].key)) != 0) {                                 \
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
                    while (current_satellite->_next && current_satellite->occupied) {                                  \
                        current_satellite = current_satellite->_next;                                                  \
                        printf("%zu: " key_fmt " -> " value_fmt "\n", i, key_args(current_satellite->key),             \
                               value_args(current_satellite->value));                                                  \
                    }                                                                                                  \
                } else {                                                                                               \
                    if (print_all) {                                                                                   \
                        printf("%zu -> empty\n", i);                                                                   \
                    }                                                                                                  \
                }                                                                                                      \
            }                                                                                                          \
        }                                                                                                              \
    } while (0);

#define orbit_get(ht, _key)                                                                                            \
    (assert((ht)), assert((ht)->elements),                                                                             \
     (typeof((ht)->elements->value) *) orbit__get((Orbit_Generic_HT *) ht, ((typeof((ht)->elements->key)[]){(_key)}),  \
                                                  sizeof((ht)->elements->key), offsetof(typeof(*(ht)->elements), key), \
                                                  offsetof(typeof(*(ht)->elements), value), sizeof(*(ht)->elements)))

#define orbit_itterator(ht)                                                                                            \
    ((ht)->satellite_index_to_visit_next = 0,                                                                          \
     (ht)->satellite_to_visit_next = ((typeof(*(ht)->satellite_to_visit_next) *) (ht)->elements), orbit_next((ht)))

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
void *orbit__get(Orbit_Generic_HT *ht, void *key, size_t key_size, size_t offsetof_key, size_t offsetof_value,
                 size_t satellite_size);
void *orbit_get_next_valid_entry(Orbit_Generic_HT *ht, size_t offsetof__next, size_t offsetof_occupied,
                                 size_t satellite_size);

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
    return orbit_djb2_hash(buffer, size) % max_index;
}

void *orbit__get(Orbit_Generic_HT *ht, void *key, size_t key_size, size_t offsetof_key, size_t offsetof_value,
                 size_t satellite_size) {
    if (ht && ht->elements && ht->capacity) {
        size_t ht_index = orbit_hash(key, key_size, ht->capacity);
        if ((ht)->compare == NULL) {
            (ht)->compare = memcmp;
        }

        uint8_t *current_satellite = (uint8_t *) ht->elements + ht_index * satellite_size;
        do {
            if ((ht)->compare(current_satellite + offsetof_key, key, key_size) == 0) {
                return current_satellite + offsetof_value;
            }
            current_satellite = (void *) (uint64_t) *(current_satellite + 0);
        } while (current_satellite);
    }
    return NULL;
}

void *orbit_get_next_valid_entry(Orbit_Generic_HT *ht, size_t offsetof__next, size_t offsetof_occupied,
                                 size_t satellite_size) {
    if (!ht || !ht->elements || ht->capacity == 0) {
        return NULL;
    }

    for (; ht->satellite_index_to_visit_next < ht->capacity;) {
        bool occupied =
            *((uint8_t *) ht->elements + ht->satellite_index_to_visit_next * satellite_size + offsetof_occupied);
        if (!occupied) {
            ht->satellite_index_to_visit_next++;
            continue;
        }
        void *current_satellite = ht->satellite_to_visit_next;
        if (!current_satellite) {
            return NULL;
        }

        void *next_satellite_in_collision_resulution_list = *(void **) (current_satellite + offsetof__next);

        if (next_satellite_in_collision_resulution_list == NULL) {
            ht->satellite_index_to_visit_next++;
            if (ht->satellite_index_to_visit_next < ht->capacity) {
                ht->satellite_to_visit_next =
                    (void *) ((uint8_t *) ht->elements + ht->satellite_index_to_visit_next * satellite_size);
            } else {
                ht->satellite_to_visit_next = NULL;
            }
            return current_satellite;
        } else {
            ht->satellite_to_visit_next = *(void **) (current_satellite + offsetof__next);
            return current_satellite;
        }
    }
    return NULL;
}

#endif  // ORBIT_IMPLEMENTATION
