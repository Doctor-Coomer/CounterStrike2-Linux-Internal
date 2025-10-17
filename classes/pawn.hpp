#ifndef PAWN_HPP
#define PAWN_HPP

#include <limits.h>
#include <string>

#include "../vec.hpp"

#include "../print.hpp"

enum class cs_team;

enum Bone {
  hip = 0,
  spine1 = 1,
  spine2 = 2,
  spine3 = 3,
  spine4 = 4,
  neck = 5,
  head = 6,
  left_shoulder = 8,
  left_elbow = 9,
  left_hand = 10,
  right_shoulder = 13,
  right_elbow = 14,
  right_hand = 15,
  left_hip = 22,
  left_knee = 23,
  left_foot = 24,
  right_hip = 25,
  right_knee = 26,
  right_foot = 27,
};


class Pawn {
public:
  void* get_game_scene_node(void) {
    return *(void**)(this + 0x4A8);
  }

  void* get_camera_services(void) {
    return *(void**)(this + 0x13F0);
  }

  float get_fov(void) {
    void* camera_services = this->get_camera_services();
    if (camera_services == nullptr) {
      return 0.0;
    }

    return *(int*)((unsigned long)camera_services + 0x290);
  }  

  Vec3 get_abs_origin(void) {
    void* game_scene_node = this->get_game_scene_node();
    if (game_scene_node == nullptr) return Vec3{};

    return *(Vec3*)((unsigned long)game_scene_node + 0xD0);
  }

  bool is_dormant(void) {
    void* game_scene_node = this->get_game_scene_node();
    if (game_scene_node == nullptr) return true;

    return *(bool*)((unsigned long)game_scene_node + 0x10B);
  }

  Vec3 get_bone_location(unsigned int i) {
    void* game_scene_node = this->get_game_scene_node();
    if (game_scene_node == nullptr) return Vec3{};
    
    void* bone_data = *(void**)((unsigned long)game_scene_node + 0x190 + 0x80);
    if (bone_data == nullptr) return Vec3{};

    return *(Vec3*)((unsigned long)bone_data + ((unsigned int)i)*32);
  }

  Vec3 get_eye_position(void) {
    Vec3 location = this->get_abs_origin();
    Vec3 offset = *(Vec3*)(this + 0xD28);
    return Vec3{location.x + offset.x, location.y + offset.y, location.z + offset.z};
  }

  Vec3 get_aim_punch(void) {
    unsigned long length = *(unsigned long*)(this + 0x16D0);
    if (length < 1) {
      return Vec3{};
    }
    
    unsigned long aim_punch_ptr = *(unsigned long*)(this + 0x16D0 + 0x8);
    if ((void*)aim_punch_ptr == nullptr || aim_punch_ptr > ULONG_LONG_MAX - 50000) {
      return Vec3{};
    }
    
    return *(Vec3*)(aim_punch_ptr + (length - 1) * 12);
  }
  
  bool get_lifestate(void) {
    return *(bool*)(this + 0x4CC);
  }

  enum cs_team get_cs_team(void) {
    return (cs_team)*(int*)(this + 0x563);
  }

  int get_health(void) {
    return *(int*)(this + 0x4C4);
  }

  bool get_gun_game_immunity(void) {
    return *(bool*)(this + 0x10);
  }
};

#endif
