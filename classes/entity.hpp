#ifndef ENTITY_HPP
#define ENTITY_HPP

class Entity {
public:
  int get_pawn_handle(void) {
    return *(int*)(this + 0x844);
  }
};

#endif
