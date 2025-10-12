#ifndef INPUT_SYSTEM_HPP
#define INPUT_SYSTEM_HPP

#include "../vec.hpp"

class Input {
public:
  void set_thirdperson(bool value) {
    *(bool*)(this + 0x261) = value;
  }
  
  bool is_thirdperson(void) {
    return *(bool*)(this + 0x261);
  }

  void set_view_angles(Vec3 angles) {
    *(Vec3*)(this + 0x6C0) = angles;
  }

  Vec3 get_view_angles(void) {
    return *(Vec3*)(this + 0x6C0);
  }

  void set_shoot(bool value) {
    *(bool*)(this + 0x288) = value; 
  }
  
};

inline static Input* input;

#endif
