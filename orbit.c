#define ORBIT_IMPLEMENTATION
#include "orbit.h"

size_t orbit_cstr_array_size(char **array, size_t size) {
    size_t result = 0;
    for (size_t i = 0; i < size; ++i) {
        result += strlen(array[i]) + 1;
    }
    return result;
}

typedef struct {
    char *elements;
    size_t count;
} String_View;

#define SV_ARG(sv) (int) (sv).count, (sv).elements
#define SV_FMT "%.*s"

int inner_ht_cmp(const void *s1, const void *s2, size_t n) {
    printf("HY from inner ht cmp\n");
    return memcmp(s1, s2, n);
}

int main() {
    // TODO: The comparison function has to be set for every value.
    // Orbit_HT(char *, Orbit_HT(int, int)) *ht = calloc(1, sizeof(*ht));

    Orbit_HT(char **, char *) *ht = calloc(1, sizeof(*ht));
    assert(ht);

    char *a[] = {strdup("hello")};
    typeof(ht->elements->kv) *kv = orbit_put(ht, a, NULL);
    assert(kv);

    typeof(ht->elements->kv.value) *value = orbit_get(ht, a);
    assert(value);
    (void) value;

    orbit_print(ht, false, "%s", "%s", *(char **), (char *) );
    orbit_delete(ht, a, NULL);

    orbit_foreach(it, ht) {
        if (!it->key) continue;
        printf("<%zu> %s -> %s\n", ht->satellite_index_to_visit_next, *it->key, it->value);
    }

    orbit_free(ht);

    free(ht);

    return 0;
}
