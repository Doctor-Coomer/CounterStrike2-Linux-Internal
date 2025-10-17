#include "../../interfaces/input_system.hpp"
#include "../../interfaces/game_entity_system.hpp"

#include "../../classes/pawn.hpp"

#include "../../gui/config.hpp"

#include "../../math.hpp"

void draw_halo() {
  if (config.visuals.hat.enabled == false) return;

  if (!input->is_thirdperson()) return;

  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  draw_list->Flags |= ImDrawListFlags_AntiAliasedLines;

  Pawn* localpawn = entity_system->get_localpawn();
  if (localpawn == nullptr) return;

  Vec3 location = localpawn->get_bone_location(Bone::head);

  Vec3 prev_pos = {0, 0, 0};
  for (float rotation = 0.0; rotation <= M_PI*2 + ((M_PI*2)/45); rotation+=((M_PI*2)/45)) {
    Vec3 pos = {location.x + config.visuals.hat.radius * cos(rotation), location.y + config.visuals.hat.radius * sin(rotation), location.z + config.visuals.hat.z_offset_base};

    Vec3 screen_pos;
    if (!world_to_screen(pos, &screen_pos)) continue;
	
    if (prev_pos != Vec3{0, 0, 0}) {
      draw_list->AddLine(ImVec2(prev_pos.x, prev_pos.y), ImVec2(screen_pos.x, screen_pos.y), config.visuals.hat.hat_color.to_ImU32(), 2);

      if (config.visuals.hat.rice == true) {
	Vec3 tip_pos = location;
	tip_pos.z += config.visuals.hat.z_offset_base + config.visuals.hat.z_offset_tip;

	Vec3 screen_tip_pos;
	world_to_screen(tip_pos, &screen_tip_pos);

	draw_list->AddLine(ImVec2(prev_pos.x, prev_pos.y), ImVec2(screen_tip_pos.x, screen_tip_pos.y), config.visuals.hat.hat_color.to_ImU32());
	draw_list->AddLine(ImVec2(screen_pos.x, screen_pos.y), ImVec2(screen_tip_pos.x, screen_tip_pos.y), config.visuals.hat.hat_color.to_ImU32());
      }
    }

    
    
    prev_pos = screen_pos;

  }
}
