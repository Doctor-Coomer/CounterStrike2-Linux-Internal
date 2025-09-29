#include <unistd.h>

#include "../print.hpp"

long (*something_original)(long) = NULL;

long something_hook(long a) {
  print("test\n");
  return something_original(a);
}
