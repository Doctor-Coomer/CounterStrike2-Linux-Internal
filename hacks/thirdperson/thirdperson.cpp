#include "../../interfaces/input_system.hpp"
#include "../../interfaces/game_entity_system.hpp"

#include "../../gui/config.hpp"

void thirdperson(void) {

  Pawn* localpawn = entity_system->get_localpawn();
  if (localpawn == nullptr) return;

  if (config.visuals.thirdperson.enabled == false) {
    input->set_thirdperson(false);
    return;
  }

  
  static bool was_pressed = false;
  static bool do_thirdperson = false;
  if (config.visuals.thirdperson.enabled == true) {
    if (!was_pressed && is_button_down(config.visuals.thirdperson.key)) {
      do_thirdperson = !do_thirdperson;
      was_pressed = true;
    } else if (was_pressed && !is_button_down(config.visuals.thirdperson.key)) {
      was_pressed = false;
    }
  }

  if (do_thirdperson == true) input->set_thirdperson(true);
  else input->set_thirdperson(false);

}
