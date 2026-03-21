#ifndef ORBIT_H_
#define ORBIT_H_

#include <stdlib.h>

typedef struct {
  void *elements;
  size_t count;
  size_t capacity;

} Satellite;

void orbit_put(void *orbit_hash_map, void *key, void *value);

#endif // ORBIT_H_

// ===========================================================================

#ifdef ORBIT_IMPLEMENTATION

#endif // ORBIT_IMPLEMENTATION
