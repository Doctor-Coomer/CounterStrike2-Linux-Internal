#include "../aimbot/aimbot.hpp"

#include "../../imgui/dearimgui.hpp"

#include "../../math.hpp"

#include "../../gui/config.hpp"

#include "../../interfaces/game_entity_system.hpp"
#include "../../interfaces/cvar_system.hpp"

#include "../../classes/pawn.hpp"
#include <cmath>

void draw_players() {
  if (config.esp.master == false) return;

  //ImDrawList* draw_list = ImGui::GetForegroundDrawList();
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  draw_list->Flags &= ~ImDrawListFlags_AntiAliasedLines;

  
  Entity* localentity = entity_system->get_localentity();
  Pawn* localpawn = entity_system->get_localpawn();
  if (localentity == nullptr || localpawn == nullptr) {
    return;
  }

  static Convar* team_is_enemy = cvar_system->get_convar("mp_teammates_are_enemies");
  if (team_is_enemy == nullptr) {
    return;
  }

  static Convar* fov = cvar_system->get_convar("fov_cs_debug");
  if (fov == nullptr) {
    return;
  }
  
  if (config.visuals.override_fov == true) {
    fov->set_value<float>(config.visuals.custom_fov);
    if (localpawn->get_fov() != config.visuals.custom_viewmodel_fov) {
      localentity->set_fov(150*(1.f-(float(config.visuals.custom_viewmodel_fov)/150.f)));
    }
  } else {
    fov->set_value<float>(0);
    localentity->set_fov(90);
  }

  
  
  Vec2 screen_size = {3440, 1440};
  
  for (unsigned int i = 1; i <= 64; ++i) {
  
    Pawn* pawn = entity_system->pawn_from_index(i);
    Entity* entity = entity_system->entity_from_index(i);
    
    if (pawn == nullptr   ||
	pawn == localpawn ||
	pawn->get_lifestate() != 0 ||
	(pawn->get_cs_team() == localpawn->get_cs_team() && team_is_enemy->get_value<bool>() == false) ||
	pawn->is_dormant())
      {
	continue;
      }

    Vec3 location = pawn->get_abs_origin();
    Vec3 screen;
    if (!world_to_screen(location, &screen)) continue;

    Vec3 location_offset = {location.x, location.y, pawn->get_bone_location(Bone::head).z + 8};
    Vec3 screen_offset;
    world_to_screen(location_offset, &screen_offset);

    
    float box_offset = (screen.y - screen_offset.y)*0.25;
    
    if (config.esp.player.box == true) {    
      ImU32 color = config.esp.player.box_color.to_ImU32();
      
      draw_list->AddLine(ImVec2(screen.x + box_offset, screen.y + 2), ImVec2(screen.x + box_offset, screen_offset.y - 2), IM_COL32(0, 0, 0, 255), 4);
      draw_list->AddLine(ImVec2(screen.x - box_offset, screen.y + 2), ImVec2(screen.x - box_offset, screen_offset.y - 2), IM_COL32(0, 0, 0, 255), 4);
      draw_list->AddLine(ImVec2(screen.x - box_offset, screen.y), ImVec2(screen.x + box_offset, screen.y), IM_COL32(0, 0, 0, 255), 4);
      draw_list->AddLine(ImVec2(screen.x - box_offset, screen_offset.y), ImVec2(screen.x + box_offset, screen_offset.y), IM_COL32(0, 0, 0, 255), 4);

    
      draw_list->AddLine(ImVec2(screen.x + box_offset, screen.y + 1), ImVec2(screen.x + box_offset, screen_offset.y - 1), color, 2);
      draw_list->AddLine(ImVec2(screen.x - box_offset, screen.y + 1), ImVec2(screen.x - box_offset, screen_offset.y - 1), color, 2);
      draw_list->AddLine(ImVec2(screen.x - box_offset, screen.y), ImVec2(screen.x + box_offset, screen.y), color, 2);
      draw_list->AddLine(ImVec2(screen.x - box_offset, screen_offset.y), ImVec2(screen.x + box_offset, screen_offset.y), color, 2);
    }

    ImU32 color;
    int ydelta = (screen_offset.y - screen.y) * (1.f - (float(pawn->get_health()) / 100));
    
    int g = (float(pawn->get_health()) / 100) * 255;
    int r = (1.f - (float(pawn->get_health()) / 100)) * 255;

    color = IM_COL32(r, g, 0, 255);
    
    if (pawn->get_health() > 100) {
      color = IM_COL32(0, 255, 255, 255);
      ydelta = 0;
    }

    if (config.esp.player.health_bar == true) {
      draw_list->AddLine(ImVec2(screen.x - box_offset - 6, screen.y + 2), ImVec2(screen.x - box_offset - 6, screen_offset.y - 2 - ydelta), IM_COL32(0, 0, 0, 255), 4);
    
      draw_list->AddLine(ImVec2(screen.x - box_offset - 6, screen.y + 1), ImVec2(screen.x - box_offset - 6, screen_offset.y - 1 - ydelta), IM_COL32(r, g, 0, 255), 2);
    }

    std::string health_str = std::to_string(pawn->get_health());
    unsigned int characters = health_str.length() + 1;

    if (config.esp.player.health_text == true) {
      draw_list->AddTextShadow(ImVec2(screen.x - box_offset - 7*characters - 1, screen_offset.y - 2 - (config.esp.player.health_bar ? ydelta : 0)), color, health_str.c_str());
    }
    
    if (config.esp.player.flags.target_indicator == true && pawn == target_pawn) {
      draw_list->AddTextShadow(ImVec2(screen.x + box_offset + 5, screen_offset.y - 2), config.esp.player.flags.target_indicator_color.to_ImU32(), "Target");
    }

    if (config.esp.player.name == true) {
      const char* name = entity->get_name();
      draw_list->AddTextShadow(ImVec2(screen.x - ((6*strlen(name))*0.5), screen_offset.y - 15), config.esp.player.name_color.to_ImU32(), name);
    }
    
    if (config.esp.player.skeleton == true) {

      ImU32 color = config.esp.player.skeleton_color.to_ImU32();
    
      for (unsigned int i = Bone::hip; i < Bone::head; ++i) {
	Vec3 bone_location = pawn->get_bone_location(i);
	Vec3 bone_screen;
	if (!world_to_screen(bone_location, &bone_screen)) continue;
	Vec3 bone_location2 = pawn->get_bone_location(i+1);
	Vec3 bone_screen2;
	if (!world_to_screen(bone_location2, &bone_screen2)) continue;
	draw_list->AddLine(ImVec2(bone_screen.x, bone_screen.y), ImVec2(bone_screen2.x, bone_screen2.y), color, 1);
      }

      Vec3 bone_location = pawn->get_bone_location(Bone::spine4);
      Vec3 bone_screen;
      Vec3 bone_location2 = pawn->get_bone_location(Bone::left_shoulder);
      Vec3 bone_screen2;
      if (world_to_screen(bone_location2, &bone_screen2) && world_to_screen(bone_location, &bone_screen)) {
	draw_list->AddLine(ImVec2(bone_screen.x, bone_screen.y), ImVec2(bone_screen2.x, bone_screen2.y), color, 1);      
      }
      

      for (unsigned int i = Bone::left_shoulder; i < Bone::left_hand; ++i) {
	Vec3 bone_location = pawn->get_bone_location(i);
	Vec3 bone_screen;
	if (!world_to_screen(bone_location, &bone_screen)) continue;
	Vec3 bone_location2 = pawn->get_bone_location(i+1);
	Vec3 bone_screen2;
	if (!world_to_screen(bone_location2, &bone_screen2)) continue;
	draw_list->AddLine(ImVec2(bone_screen.x, bone_screen.y), ImVec2(bone_screen2.x, bone_screen2.y), color, 1);
      }
    
      bone_location = pawn->get_bone_location(Bone::spine4);
      bone_location2 = pawn->get_bone_location(Bone::right_shoulder);
      if (world_to_screen(bone_location2, &bone_screen2) && world_to_screen(bone_location, &bone_screen)) {
	draw_list->AddLine(ImVec2(bone_screen.x, bone_screen.y), ImVec2(bone_screen2.x, bone_screen2.y), color, 1);      
      }

    
      for (unsigned int i = Bone::right_shoulder; i < Bone::right_hand; ++i) {
	Vec3 bone_location = pawn->get_bone_location(i);
	Vec3 bone_screen;
	if (!world_to_screen(bone_location, &bone_screen)) continue;
	Vec3 bone_location2 = pawn->get_bone_location(i+1);
	Vec3 bone_screen2;
	if (!world_to_screen(bone_location2, &bone_screen2)) continue;
	draw_list->AddLine(ImVec2(bone_screen.x, bone_screen.y), ImVec2(bone_screen2.x, bone_screen2.y), color, 1);
      }
    
      for (unsigned int i = Bone::left_hip; i < Bone::left_foot; ++i) {
	Vec3 bone_location = pawn->get_bone_location(i);
	Vec3 bone_screen;
	if (!world_to_screen(bone_location, &bone_screen)) continue;
	Vec3 bone_location2 = pawn->get_bone_location(i+1);
	Vec3 bone_screen2;
	if (!world_to_screen(bone_location2, &bone_screen2)) continue;
	draw_list->AddLine(ImVec2(bone_screen.x, bone_screen.y), ImVec2(bone_screen2.x, bone_screen2.y), color, 1);
      }
    
      for (unsigned int i = Bone::right_hip; i < Bone::right_foot; ++i) {
	Vec3 bone_location = pawn->get_bone_location(i);
	Vec3 bone_screen;
	if (!world_to_screen(bone_location, &bone_screen)) continue;
	Vec3 bone_location2 = pawn->get_bone_location(i+1);
	Vec3 bone_screen2;
	if (!world_to_screen(bone_location2, &bone_screen2)) continue;
	draw_list->AddLine(ImVec2(bone_screen.x, bone_screen.y), ImVec2(bone_screen2.x, bone_screen2.y), color, 1);
      }

      bone_location = pawn->get_bone_location(Bone::hip);
      bone_location2 = pawn->get_bone_location(Bone::left_hip);
      if (world_to_screen(bone_location2, &bone_screen2) && world_to_screen(bone_location, &bone_screen)) {
	draw_list->AddLine(ImVec2(bone_screen.x, bone_screen.y), ImVec2(bone_screen2.x, bone_screen2.y), color, 1);      
      }

      bone_location2 = pawn->get_bone_location(Bone::right_hip);
      if (world_to_screen(bone_location2, &bone_screen2) && world_to_screen(bone_location, &bone_screen)) {
	draw_list->AddLine(ImVec2(bone_screen.x, bone_screen.y), ImVec2(bone_screen2.x, bone_screen2.y), color, 1);      
      }
    }

    /*
    Vec3 prev_pos = {0, 0, 0};
    for (float rotation = 0.0; rotation <= M_PI*2 + ((M_PI*2)/45); rotation+=((M_PI*2)/45)) {
      Vec3 pos = {location.x + 40 * cos(rotation), location.y + 40 * sin(rotation), location.z};
      
      Vec3 screen_pos;
      if (world_to_screen(pos, &screen_pos)) {
	
	if (prev_pos != Vec3{0, 0, 0}) {
	  draw_list->AddLine(ImVec2(prev_pos.x, prev_pos.y), ImVec2(screen_pos.x, screen_pos.y), IM_COL32_WHITE, 5);
	}
	
	prev_pos = screen_pos;
      }

    }
    */
  }

  
}
