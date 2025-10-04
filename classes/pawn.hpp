#ifndef PAWN_HPP
#define PAWN_HPP

#include <vector>

#include "../vec.hpp"

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
  
  bool get_lifestate(void) {
    return *(bool*)(this + 0x4C8);
  }

  enum cs_team get_cs_team(void) {
    return (cs_team)*(int*)(this + 0x563);
  }

  int get_health(void) {
    return *(int*)(this + 0x4C4);
  }
};

#endif
