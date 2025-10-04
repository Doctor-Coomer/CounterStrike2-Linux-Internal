#include "../../imgui/dearimgui.hpp"

#include "../../math.hpp"

#include "../../gui/config.hpp"

#include "../../interfaces/game_entity_system.hpp"

#include "../../classes/pawn.hpp"

void draw_players() {
  if (config.esp.master == false) return;

  ImDrawList* draw_list = ImGui::GetForegroundDrawList();

  Pawn* localpawn = entity_system->get_localpawn();
  if (localpawn == nullptr) {
    return;
  }
  
  for (unsigned int i = 1; i <= 64; ++i) {
  
    Pawn* pawn = entity_system->pawn_from_index(i);

    if (pawn == nullptr   ||
	pawn == localpawn ||
	pawn->get_lifestate() != 0 ||
	pawn->is_dormant())
      {
	continue;
      }

    Vec3 location = pawn->get_abs_origin();
    Vec3 screen;
    if (!world_to_screen(location, &screen)) continue;

    Vec2 screen_size = {3440, 1440};

    //draw_list->AddLine(ImVec2(screen_size.x * 0.5, screen_size.y), ImVec2(screen.x, screen.y), IM_COL32(255, 255, 255, 255), 2);
    draw_list->AddText(ImVec2(screen.x, screen.y + 15), IM_COL32(255, 0, 255, 255), std::to_string(pawn->get_health()).c_str());
    
    for (unsigned int i = Bone::hip; i < Bone::head; ++i) {
      Vec3 bone_location = pawn->get_bone_location(i);
      Vec3 bone_screen;
      if (!world_to_screen(bone_location, &bone_screen)) continue;
      Vec3 bone_location2 = pawn->get_bone_location(i+1);
      Vec3 bone_screen2;
      if (!world_to_screen(bone_location2, &bone_screen2)) continue;
      draw_list->AddLine(ImVec2(bone_screen.x, bone_screen.y), ImVec2(bone_screen2.x, bone_screen2.y), IM_COL32(255, 255, 255, 255), 1);
    }

    Vec3 bone_location = pawn->get_bone_location(Bone::spine4);
    Vec3 bone_screen;
    Vec3 bone_location2 = pawn->get_bone_location(Bone::left_shoulder);
    Vec3 bone_screen2;
    if (world_to_screen(bone_location2, &bone_screen2) && world_to_screen(bone_location, &bone_screen)) {
      draw_list->AddLine(ImVec2(bone_screen.x, bone_screen.y), ImVec2(bone_screen2.x, bone_screen2.y), IM_COL32(255, 255, 255, 255), 1);      
    }
      

    for (unsigned int i = Bone::left_shoulder; i < Bone::left_hand; ++i) {
      Vec3 bone_location = pawn->get_bone_location(i);
      Vec3 bone_screen;
      if (!world_to_screen(bone_location, &bone_screen)) continue;
      Vec3 bone_location2 = pawn->get_bone_location(i+1);
      Vec3 bone_screen2;
      if (!world_to_screen(bone_location2, &bone_screen2)) continue;
      draw_list->AddLine(ImVec2(bone_screen.x, bone_screen.y), ImVec2(bone_screen2.x, bone_screen2.y), IM_COL32(255, 255, 255, 255), 1);
    }
    
    bone_location = pawn->get_bone_location(Bone::spine4);
    bone_location2 = pawn->get_bone_location(Bone::right_shoulder);
    if (world_to_screen(bone_location2, &bone_screen2) && world_to_screen(bone_location, &bone_screen)) {
      draw_list->AddLine(ImVec2(bone_screen.x, bone_screen.y), ImVec2(bone_screen2.x, bone_screen2.y), IM_COL32(255, 255, 255, 255), 1);      
    }

    
    for (unsigned int i = Bone::right_shoulder; i < Bone::right_hand; ++i) {
      Vec3 bone_location = pawn->get_bone_location(i);
      Vec3 bone_screen;
      if (!world_to_screen(bone_location, &bone_screen)) continue;
      Vec3 bone_location2 = pawn->get_bone_location(i+1);
      Vec3 bone_screen2;
      if (!world_to_screen(bone_location2, &bone_screen2)) continue;
      draw_list->AddLine(ImVec2(bone_screen.x, bone_screen.y), ImVec2(bone_screen2.x, bone_screen2.y), IM_COL32(255, 255, 255, 255), 1);
    }
    
    for (unsigned int i = Bone::left_hip; i < Bone::left_foot; ++i) {
      Vec3 bone_location = pawn->get_bone_location(i);
      Vec3 bone_screen;
      if (!world_to_screen(bone_location, &bone_screen)) continue;
      Vec3 bone_location2 = pawn->get_bone_location(i+1);
      Vec3 bone_screen2;
      if (!world_to_screen(bone_location2, &bone_screen2)) continue;
      draw_list->AddLine(ImVec2(bone_screen.x, bone_screen.y), ImVec2(bone_screen2.x, bone_screen2.y), IM_COL32(255, 255, 255, 255), 1);
    }
    
    for (unsigned int i = Bone::right_hip; i < Bone::right_foot; ++i) {
      Vec3 bone_location = pawn->get_bone_location(i);
      Vec3 bone_screen;
      if (!world_to_screen(bone_location, &bone_screen)) continue;
      Vec3 bone_location2 = pawn->get_bone_location(i+1);
      Vec3 bone_screen2;
      if (!world_to_screen(bone_location2, &bone_screen2)) continue;
      draw_list->AddLine(ImVec2(bone_screen.x, bone_screen.y), ImVec2(bone_screen2.x, bone_screen2.y), IM_COL32(255, 255, 255, 255), 1);
    }

    bone_location = pawn->get_bone_location(Bone::hip);
    bone_location2 = pawn->get_bone_location(Bone::left_hip);
    if (world_to_screen(bone_location2, &bone_screen2) && world_to_screen(bone_location, &bone_screen)) {
      draw_list->AddLine(ImVec2(bone_screen.x, bone_screen.y), ImVec2(bone_screen2.x, bone_screen2.y), IM_COL32(255, 255, 255, 255), 1);      
    }

    bone_location2 = pawn->get_bone_location(Bone::right_hip);
    if (world_to_screen(bone_location2, &bone_screen2) && world_to_screen(bone_location, &bone_screen)) {
      draw_list->AddLine(ImVec2(bone_screen.x, bone_screen.y), ImVec2(bone_screen2.x, bone_screen2.y), IM_COL32(255, 255, 255, 255), 1);      
    }

  }

}
