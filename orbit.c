#define ORBIT_IMPLEMENTATION
#include "orbit.h"

typedef struct {
  const char *key;
} Key;

typedef struct {
  const char *value;
} Value;

typedef struct {
  Key key;
  Value value;
} Map;

int main() {
  Map map = {0};
  orbit_put(&map, "hello", 420);
  return 0;
}
