#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <SDL3/SDL.h>

struct button {
  int button;
  bool waiting = false;
};

struct Aimbot {
  bool master = true;

  struct button key = {.button = -SDL_BUTTON_X1};

  float fov = 45;
  bool draw_fov = false;
  
  bool recoil = true;

  bool auto_shoot = false;
};

struct ESP {
  bool master = true;

  struct {
    bool box = false;
    bool health_bar = true;
    bool health_text = true;
    bool name = true;
    bool skeleton = true;

    struct {
      bool target_indicator = true;
    } flags;
    
  } player;
};

struct Visuals {
  bool override_fov = false;
  float custom_fov = 90;
};

struct {
  Aimbot aimbot;
  ESP esp;
  Visuals visuals;
} static config;


static bool is_button_down(struct button button) {
  if (button.button >= 0) {
    const bool* keys = SDL_GetKeyboardState(NULL);
  
    if (keys[button.button] == 1)
      return true;

    return false;
  } else {
    Uint32 mouse_state = SDL_GetMouseState(NULL, NULL);

    if (mouse_state & SDL_BUTTON_MASK(-button.button))
      return true;

    return false;
  }  

  return false;
}

#endif
