#define ORBIT_IMPLEMENTATION
#include "orbit.h"


size_t orbit_array_size(char **array, size_t size) {
    size_t result = 0;
    for (size_t i = 0; i < size; ++i) {
        result += strlen(array[i]) + 1;
    }
    return result;
}

size_t orbit_sizeof_string(char *str, ...) {
    return strlen(str);
}

size_t orbit_sizeof(void *s, ...) {
    return sizeof(*s);
}

int orbit_strcmp(const void *s1, const void *s2, size_t n) {
    (void) n;
    return strcmp(*(char **) s1, *(char **) s2);
}

typedef struct {
    char *elements;
    size_t count;
} String_View;

#define SV_ARG(sv) (int) (sv).count, (sv).elements
#define SV_FMT "%.*s"
int main() {
    // Orbit_HT(char *, Orbit_HT(int, int)) *ht = calloc(1, sizeof(*ht)); // The comparison function has to be set for
    // every value.
    Orbit_HT(char *, char *) *ht = calloc(1, sizeof(*ht));
    assert(ht);
    ht->compare = orbit_strcmp;  // TODO: Auto detect char* type and all variants.
    // String_View s = {.elements = "HELLO", .count = 5};
    // String_View s1 = {.elements = "ME", .count = 2};
    // String_View s2 = {.elements = "you", .count = 3};
    // String_View s3 = {.elements = "du", .count = 2};

    char *a = "hello";
    char *a1 = "me";
    char *a2 = strdup("me");

    // orbit_put(ht, s, a);
    // orbit_put(ht, s1, a1);
    // orbit_put(ht, s2, a2);
    orbit_put(ht, a, a1);
    orbit_put(ht, a, a);
    orbit_put(ht, a, a2);
    orbit_put(ht, a, a2);
    orbit_put(ht, a, a1);
    orbit_put(ht, a, a2);
    orbit_put(ht, a, a2);
    // orbit_put(ht, a1, s1);
    // orbit_put(ht, a2, s2);
    // orbit_put(ht, a2, s3);

    // orbit_print(ht, false, SV_FMT, SV_ARG);
    orbit_print(ht, false, "%s");

    // orbit_delete(ht, s3, strdup("me"));
    // orbit_delete(ht, s3, ("me"));
    orbit_delete(ht, strdup("hello"), a1);  // TODO: For this situation the hash size has to be different as well.
    // orbit_delete(ht, a, a1);

    orbit_print(ht, false, "%s");

    orbit_free(ht);

    free(ht);

    return 0;
}
