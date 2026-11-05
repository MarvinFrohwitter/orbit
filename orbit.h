#ifndef ORBIT_H_
#define ORBIT_H_

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __Satellite(type)                                                                                              \
    struct {                                                                                                           \
        bool occupied;                                                                                                 \
        type value;                                                                                                    \
        void *_next;                                                                                                   \
    }

#define Orbit_HT(key_type, value_type, ...)                                                                            \
    struct {                                                                                                           \
        __Satellite(value_type) * elements;                                                                            \
        size_t count;                                                                                                  \
        size_t capacity;                                                                                               \
        size_t init_size;                                                                                              \
        int (*compare)(const void *, const void *, size_t n);                                                          \
                                                                                                                       \
        key_type __key_type;                                                                                           \
        __VA_ARGS__;                                                                                                   \
    }

#define orbit_put(ht, _key, _value)                                                                                    \
    do {                                                                                                               \
        ; /* if (!(ht)) { */ /* return NULL; */ /* } */                                                                \
        assert(ht != NULL);                                                                                            \
        if ((ht)->count >= (ht)->capacity) {                                                                           \
            bool _init = false;                                                                                        \
            if ((ht)->capacity == 0) {                                                                                 \
                _init = true;                                                                                          \
            }                                                                                                          \
            (ht)->capacity = (ht)->capacity ? (ht)->capacity * 2 : (ht)->init_size ? (ht)->init_size : 64;             \
            (ht)->elements = realloc((ht)->elements, (ht)->capacity * sizeof(*(ht)->elements));                        \
            if ((ht)->elements == NULL) {                                                                              \
                fprintf(stderr, "ERROR: Allocation has failed!\n");                                                    \
                abort();                                                                                               \
                ; /* return NULL;  */                                                                                  \
            }                                                                                                          \
            if (_init) {                                                                                               \
                memset((ht)->elements, 0, (ht)->capacity * sizeof(*(ht)->elements));                                   \
            }                                                                                                          \
        }                                                                                                              \
        (ht)->__key_type = (_key); /*This is to make the key type save.*/                                              \
        size_t ht_index =                                                                                              \
            orbit_hash(((typeof((ht)->__key_type)[]){(_key)}), sizeof((ht)->__key_type), (ht)->capacity);              \
        bool contatins_value = false;                                                                                  \
        if (!(ht)->elements[ht_index].occupied) {                                                                      \
            (ht)->elements[ht_index].value = (_value);                                                                 \
            (ht)->elements[ht_index].occupied = true;                                                                  \
        } else {                                                                                                       \
            typeof(*(ht)->elements) *current_satellite = &(ht)->elements[ht_index];                                    \
            if ((ht)->compare == NULL) {                                                                               \
                (ht)->compare = memcmp;                                                                                \
            }                                                                                                          \
            if ((ht)->compare(((typeof((ht)->elements->value)[]){current_satellite->value}),                           \
                              ((typeof((ht)->elements->value)[]){(_value)}),                                           \
                              sizeof((ht)->elements[ht_index].value)) == 0) {                                          \
                contatins_value = true;                                                                                \
            }                                                                                                          \
            while (!contatins_value && current_satellite->_next) {                                                     \
                if ((ht)->compare(((typeof((ht)->elements->value)[]){                                                  \
                                      ((typeof(*(ht)->elements) *) current_satellite->_next)->value}),                 \
                                  ((typeof((ht)->elements->value)[]){(_value)}),                                       \
                                  sizeof((ht)->elements[ht_index].value)) == 0) {                                      \
                    contatins_value = true;                                                                            \
                }                                                                                                      \
                current_satellite = current_satellite->_next;                                                          \
            }                                                                                                          \
            if (!contatins_value) {                                                                                    \
                current_satellite->_next = malloc(sizeof(*(ht)->elements));                                            \
                if (current_satellite->_next == NULL) {                                                                \
                    fprintf(stderr, "ERROR: Allocation for collision resolution has failed!\n");                       \
                    abort();                                                                                           \
                    ; /* return NULL;  */                                                                              \
                }                                                                                                      \
                memset(current_satellite->_next, 0, sizeof(*(ht)->elements));                                          \
                ((typeof(*(ht)->elements) *) current_satellite->_next)->value = (_value);                              \
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
            (ht)->__key_type = (_key); /*This is to make the key type save.*/                                          \
            size_t ht_index =                                                                                          \
                orbit_hash(((typeof((ht)->__key_type)[]){(_key)}), sizeof((ht)->__key_type), (ht)->capacity);          \
            if ((ht)->compare == NULL) {                                                                               \
                (ht)->compare = memcmp;                                                                                \
            }                                                                                                          \
            typeof(*(ht)->elements) *current_satellite = (ht)->elements[ht_index]._next;                               \
            typeof(*(ht)->elements) *prev_satellite = &(ht)->elements[ht_index];                                       \
            if ((ht)->elements[ht_index].occupied) {                                                                   \
                if ((ht)->compare(((typeof((ht)->elements->value)[]){(ht)->elements[ht_index].value}),                 \
                                  ((typeof((ht)->elements->value)[]){(_value)}),                                       \
                                  sizeof((ht)->elements[ht_index].value)) == 0) {                                      \
                    if ((ht)->elements[ht_index]._next == NULL) {                                                      \
                        (ht)->elements[ht_index].occupied = false;                                                     \
                    } else {                                                                                           \
                        (ht)->elements[ht_index].occupied =                                                            \
                            ((typeof(*(ht)->elements) *) (ht)->elements[ht_index]._next)->occupied;                    \
                        (ht)->elements[ht_index].value =                                                               \
                            ((typeof(*(ht)->elements) *) (ht)->elements[ht_index]._next)->value;                       \
                        (ht)->elements[ht_index]._next =                                                               \
                            ((typeof(*(ht)->elements) *) (ht)->elements[ht_index]._next)->_next;                       \
                        free(current_satellite);                                                                       \
                    }                                                                                                  \
                } else {                                                                                               \
                    while (current_satellite &&                                                                        \
                           (ht)->compare(((typeof((ht)->elements->value)[]){current_satellite->value}),                \
                                         ((typeof((ht)->elements->value)[]){(_value)}),                                \
                                         sizeof((ht)->elements[ht_index].value)) != 0) {                               \
                        prev_satellite = current_satellite;                                                            \
                        current_satellite = current_satellite->_next;                                                  \
                    }                                                                                                  \
                    if (current_satellite) {                                                                           \
                        prev_satellite->_next = current_satellite->_next;                                              \
                        free(current_satellite);                                                                       \
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

#define orbit_print(ht, print_all, fmt, ...)                                                                           \
    do {                                                                                                               \
        if ((ht) && (ht)->elements && (ht)->capacity) {                                                                \
            printf("----------<<< Table >>>----------\n");                                                             \
            for (size_t i = 0; i < (ht)->capacity; ++i) {                                                              \
                if ((ht)->elements[i].occupied) {                                                                      \
                    printf("%zu -> " fmt "\n", i, __VA_ARGS__((ht)->elements[i].value));                               \
                                                                                                                       \
                    typeof(*(ht)->elements) *current_satellite = &(ht)->elements[i];                                   \
                    while (current_satellite->_next && current_satellite->occupied) {                                  \
                        current_satellite = current_satellite->_next;                                                  \
                        printf("%zu -> " fmt "\n", i, __VA_ARGS__(current_satellite->value));                          \
                    }                                                                                                  \
                } else {                                                                                               \
                    if (print_all) {                                                                                   \
                        printf("%zu -> empty\n", i);                                                                   \
                    }                                                                                                  \
                }                                                                                                      \
            }                                                                                                          \
        }                                                                                                              \
    } while (0);

uintptr_t orbit_djb2_hash(void *data, size_t size);
size_t orbit_hash(void *buffer, size_t size, size_t max_index);

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

#endif  // ORBIT_IMPLEMENTATION
