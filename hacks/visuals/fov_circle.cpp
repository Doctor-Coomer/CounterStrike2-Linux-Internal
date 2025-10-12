#include "../../imgui/dearimgui.hpp"

#include "../../gui/config.hpp"

void draw_fov() {
  if (config.aimbot.draw_fov == false) return;

  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  
  ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImVec2 center = ImVec2(viewport->GetCenter().x, viewport->GetCenter().y);

  int radius = (tan(config.aimbot.fov / 180 * M_PI) / tan((90.f / 2) / 180 * M_PI) * (float(viewport->GetCenter().x) / 2))/1.55;
  
  draw_list->AddCircle(center, radius, IM_COL32(255, 255, 255, 210), 64);

}
