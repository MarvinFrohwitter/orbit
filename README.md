# The orbit header only library provides a hash map implementation.

1. First include the header and enable the `ORBIT_IMPLEMENTATION` macro to get the references for the linker.

### An example to get started:
```C
#define ORBIT_IMPLEMENTATION
#include "orbit.h"

typedef struct {
    char *elements;
    size_t count;
} String_View;

#define SV_ARG(sv) (int) (sv).count, (sv).elements
#define SV_FMT "%.*s"

int inner_ht_cmp(const void *s1, const void *s2, size_t n) {
    (void) n;
    const String_View *lhs = s1;
    const String_View *rhs = s2;
    if (lhs->count != rhs->count) {
        return lhs->count < rhs->count ? -1 : 1;
    }
    return memcmp(lhs->elements, rhs->elements, lhs->count);
}

int main(void) {
    Orbit_HT(char *, Orbit_HT(String_View, int)) ht = {
        .default_value.compare = inner_ht_cmp,
        .default_value.init_size = 4,
    };

    typeof(ht.elements->kv) *person = orbit_put_default(&ht, "alice");
    assert(person);

    typeof(ht.elements->kv.value) *attributes = &person->value;
    assert(attributes->compare == inner_ht_cmp);
    assert(attributes->init_size == 4);

    // String_View keys are hashed by their raw bytes, so use the same
    // String_View values for insertion and lookup.
    String_View age = {.elements = "age", .count = 3};
    String_View height = {.elements = "height", .count = 6};

    assert(orbit_put(attributes, age, 42));
    assert(orbit_put(attributes, height, 180));

    int *age_value = orbit_get(attributes, age);
    assert(age_value && *age_value == 42);

    orbit_print(attributes, false, SV_FMT, "%d", SV_ARG, (int) );

    // Look up the inner table through the outer table.
    typeof(ht.elements->kv.value) *found = orbit_get(&ht, "alice");
    assert(found == attributes);
    assert(found->count == 2);

    orbit_foreach(it, &ht) {
        orbit_free(&it->value);
    }
    orbit_free(&ht);
    return 0;
}
```
