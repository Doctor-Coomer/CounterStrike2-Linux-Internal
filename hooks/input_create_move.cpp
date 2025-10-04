#include "../math.hpp"

#include "../interfaces/game_entity_system.hpp"

#include "../classes/pawn.hpp"

#include "../hacks/aimbot/aimbot.cpp"

#include "../print.hpp"

bool (*input_create_move_original)(void*, int, bool);

bool input_create_move_hook(void* me, int slot, bool active) {
  bool ret = input_create_move_original(me, slot, active);

  aimbot();

  return ret;
}
