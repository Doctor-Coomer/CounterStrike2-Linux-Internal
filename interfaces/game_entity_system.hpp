#ifndef GAME_ENTITY_SYSTEM_HPP
#define GAME_ENTITY_SYSTEM_HPP

#include <unistd.h>

#include "../classes/entity.hpp"

class Pawn;

static Entity** localentity_ptr = nullptr;  
class GameEntitySystem {
public:
  Entity* entity_from_index(unsigned int i) {
    unsigned long v1 = (unsigned long)*(void**)(this + 0x8 * (i >> 9) + 0x10);
    return *(Entity**)(v1 + 120 * (i & 0x1FF));
  }

  Pawn* pawn_from_pawn_handle(int handle) {
    void* v2 = *(void**)(this + 0x10 + 0x8 * ((handle & 0x7FFF) >> 9));
    if (v2 == nullptr) return nullptr;
    return *(Pawn**)((unsigned long)v2 + 120 * (handle & 0x1FF));
  }

  Pawn* pawn_from_index(unsigned int i) {
    Entity* entity = entity_from_index(i);
    if (entity == nullptr) {
      return nullptr;
    }

    return pawn_from_pawn_handle(entity->get_pawn_handle());
  }

  Entity* get_localentity(void) {
    return *localentity_ptr;
  }

  Pawn* get_localpawn(void) {
    Entity* localentity = this->get_localentity();
    if (localentity == nullptr) {
      return nullptr;
    }
    
    return this->pawn_from_pawn_handle(localentity->get_pawn_handle());
  }
};

inline static GameEntitySystem* entity_system;

#endif
