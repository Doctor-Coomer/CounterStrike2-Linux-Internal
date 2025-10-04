#include <unistd.h>

#include "../print.hpp"

unsigned long (*something_original)(unsigned long, unsigned long) = NULL;

unsigned long something_hook(unsigned long a, unsigned long b) {
  print("test\n");
  return something_original(a, b);
}
