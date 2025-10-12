#include <unistd.h>

#include "../interfaces/input_system.hpp"
#include "../interfaces/engine.hpp"

//#include "../hacks/aimbot/aimbot.cpp"

void (*allow_camera_angle_change_original)(void*, int) = NULL;

void allow_camera_angle_change_hook(void* me, int n) {
  allow_camera_angle_change_original(me, n);
}
