#ifndef GAME_ENTITY_SYSTEM_HPP
#define GAME_ENTITY_SYSTEM_HPP

#include <unistd.h>

#include "../classes/entity.hpp"

class Pawn;

const unsigned long entity_identity_size = 0x70;

static Entity** localentity_ptr = nullptr;  

// https://github.com/avitran0/deadlocked/blob/rust/src/cs2/player.rs#L26
class GameEntitySystem {
public:
  Entity* entity_from_index(unsigned int i) {
    void* bucket_ptr = *(void**)(this + 0x10 + 0x8 * (i >> 9));
    if (bucket_ptr == nullptr) {
      return nullptr;
    }

    return *(Entity**)((unsigned long)bucket_ptr + entity_identity_size * (i & 0x1FF));
  }

  Pawn* pawn_from_pawn_handle(int handle) {
    void* bucket_ptr = *(void**)(this + 0x10 + 0x8 * ((handle & 0x7FFF) >> 9));
    if (bucket_ptr == nullptr) {
      return nullptr;
    }
    
    return *(Pawn**)((unsigned long)bucket_ptr + entity_identity_size * (handle & 0x1FF));
  }

  Pawn* pawn_from_index(unsigned int i) {
    Entity* entity = this->entity_from_index(i);
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
