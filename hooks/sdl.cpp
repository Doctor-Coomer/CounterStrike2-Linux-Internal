#include <SDL3/SDL_events.h>
#include <SDL3/SDL.h>

#include "../imgui/imgui_impl_sdl3.h"

#include "../gui/menu.hpp"

int  (*peep_events_original)(SDL_Event*, int, SDL_EventAction, int, int) = NULL;
void (*get_window_size_original)(SDL_Window* window, int* w, int* h) = NULL;

int peep_events_hook(SDL_Event* events, int numevents, SDL_EventAction action, int min, int max) {
  int ret = peep_events_original(events, numevents, action, min, max);

  if(ret != -1 && sdl_window != nullptr && ImGui::GetCurrentContext())
    ImGui_ImplSDL3_ProcessEvent(events);

  return ret;
}

void get_window_size_hook(SDL_Window* window, int* w, int* h) {

  sdl_window = window;
  
  get_window_size_original(window, w, h);
}

