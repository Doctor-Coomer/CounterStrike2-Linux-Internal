#ifndef CVAR_SYSTEM_HPP
#define CVAR_SYSTEM_HPP

#include <string.h>

#include "../classes/convar.hpp"

class CvarSystem {
public:
  Convar* get_convar(const char* convar_name) {
    unsigned long objects = *(unsigned long*)(this + 0x48);
    unsigned long length = *(unsigned long*)(this + 0xA0);

    for (unsigned int i = 0; i < length; ++i) {
      unsigned long object = *(unsigned long*)(objects + i * 0x10);
      if (object == 0) break;

      const char* name = *(const char**)(object);
      if (strstr(name, convar_name)) {
	return (Convar*)object;
      }
    }

    return nullptr;
  }
};

inline static CvarSystem* cvar_system;

#endif
