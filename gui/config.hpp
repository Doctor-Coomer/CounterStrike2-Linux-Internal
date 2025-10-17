#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <SDL3/SDL.h>

#include "../vec.hpp"

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
    RGBA_float box_color = {.r = 1, .g = 0.501960784, .b = 0, .a = 1};

    bool health_bar = true;
    bool health_text = true;

    bool name = true;
    RGBA_float name_color = {.r = 1, .g = 1, .b = 1, .a = 1};    
    
    bool skeleton = true;
    RGBA_float skeleton_color = {.r = 1, .g = 1, .b = 1, .a = 1};    
    
    struct {
      bool target_indicator = true;
      RGBA_float target_indicator_color = {.r = 1, .g = 0, .b = 1, .a = 1};    
    } flags;
    
  } player;
};

struct Visuals {
  bool override_fov = false;
  float custom_fov = 90;
  int custom_viewmodel_fov = 90;

  struct {
    struct button key = {.button = SDL_SCANCODE_LALT};
    bool enabled = false;
  } thirdperson;

  struct {
    bool enabled = true;
    bool rice = false;
    RGBA_float hat_color = {.r = 1, .g = 1, .b = 0, .a = 1};
    float radius = 10;
    float z_offset_base = 11;
    float z_offset_tip = 14;
  } hat;
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
