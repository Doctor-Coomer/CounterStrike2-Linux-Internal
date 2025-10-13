#ifndef MENU_HPP
#define MENU_HPP

#include <SDL2/SDL.h>
#include <string>

#include "../imgui/dearimgui.hpp"

#include "config.hpp"

inline static SDL_Window* sdl_window = NULL;
inline static bool menu_focused = false;

static void get_input(SDL_Event* event) {
  ImGui::KeybindEvent(event, &config.aimbot.key.waiting, &config.aimbot.key.button);
}

static void draw_watermark() {
  ImGui::SetNextWindowPos(ImVec2(10, 10)); 
  ImGui::SetNextWindowSize(ImVec2(80, 30));
  ImGui::Begin("##Watermark", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
  ImGui::TextCentered("LiGNUx");
  ImGui::End();
}

static void draw_aim_tab() {  
  ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 27);

  ImGui::Checkbox("Master", &config.aimbot.master);

  ImGui::EndGroup();

  ImGui::SameLine();
  ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
  ImGui::SameLine();

  /* AIMBOT */
  ImGui::BeginChild("##AimbotMasterChild");

  ImGui::BeginGroup();
  ImGui::Text("General");
  ImGui::KeybindBox(&config.aimbot.key.waiting, &config.aimbot.key.button);

  ImGui::Text("FOV: ");
  ImGui::SameLine();
  ImGui::SliderFloatHeightPad(" ", &config.aimbot.fov, 0.1f, 180.0f, 1, "%.0f\xC2\xB0");
  ImGui::Checkbox("Draw FOV", &config.aimbot.draw_fov);

  ImGui::Checkbox("Recoil Compensation", &config.aimbot.recoil);
  ImGui::Checkbox("Auto Shoot", &config.aimbot.auto_shoot);
  ImGui::EndGroup();  
  
  ImGui::EndChild();
}

static void draw_esp_tab() {  
  ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 27);

  ImGui::Checkbox("Master", &config.esp.master);

  ImGui::EndGroup();

  ImGui::SameLine();
  ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
  ImGui::SameLine();

  /* ESP */
  ImGui::BeginChild("##EspMasterChild");

  ImGui::BeginGroup();
  ImGui::Text("Player");
  ImGui::Checkbox("Box##Player", &config.esp.player.box);
  ImGui::SameLine();
  ImGui::ColorEdit4("##BoxColorPlayer", config.esp.player.box_color.to_arr(), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs);

  ImGui::Checkbox("Health Bar##Player", &config.esp.player.health_bar);
  ImGui::SameLine();
  ImGui::Checkbox("Health Text##Player", &config.esp.player.health_text);

  ImGui::Checkbox("Name##Player", &config.esp.player.name);
  ImGui::SameLine();
  ImGui::ColorEdit4("##NameColorPlayer", config.esp.player.name_color.to_arr(), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs);  
  
  ImGui::Checkbox("Skeleton##Player", &config.esp.player.skeleton);
  ImGui::SameLine();
  ImGui::ColorEdit4("##SkeletonColorPlayer", config.esp.player.skeleton_color.to_arr(), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs);  
  
  ImGui::NewLine();
  ImGui::Text("Flags");
  ImGui::Checkbox("Target##Player", &config.esp.player.flags.target_indicator);
  ImGui::SameLine();
  ImGui::ColorEdit4("##TargetColorPlayer", config.esp.player.flags.target_indicator_color.to_arr(), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs);    
  ImGui::EndGroup();

  ImGui::EndChild();
}

static void draw_visuals_tab() {
  ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 21);

  ImGui::Text(" ");

  ImGui::EndGroup();

  ImGui::SameLine();
  ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
  ImGui::SameLine();

  /* Visuals */
  ImGui::BeginChild("##VisualsMasterChild");

  /* Camera */
  ImGui::BeginGroup();
  ImGui::Text("Camera");
  ImGui::Checkbox("Override FOV", &config.visuals.override_fov);
  ImGui::SliderFloat(" ", &config.visuals.custom_fov, 30.1f, 150.0f, "%.0f\xC2\xB0");
  ImGui::EndGroup();

  ImGui::EndChild();

}
static void draw_tab(ImGuiStyle* style, const char* name, int* tab, int index) {
  ImVec4 orig_box_color = ImVec4(0.15, 0.15, 0.15, 1);
  
  if (*tab == index) {
    style->Colors[ImGuiCol_Button] = ImVec4(orig_box_color.x + 0.15, orig_box_color.y + 0.15, orig_box_color.z + 0.15, 1.00f);
  } else {
    style->Colors[ImGuiCol_Button] = ImVec4(0.15, 0.15, 0.15, 1);
  }
  
  if (ImGui::Button(name, ImVec2(80, 30))) {
    *tab = index;
  }
  style->Colors[ImGuiCol_Button] = ImVec4(0.15, 0.15, 0.15, 1);
}

static void draw_menu() {
  ImGui::SetNextWindowSize(ImVec2(600, 350));
  if (ImGui::Begin("Counter Strike 2 GNU/Linux", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
    
    ImGuiStyle* style = &ImGui::GetStyle();
    
    static int tab = 0;
    ImGui::BeginGroup();
    draw_tab(style, "Aimbot", &tab, 0);
    draw_tab(style, "ESP", &tab, 1);
    draw_tab(style, "Visuals", &tab, 2);

    switch (tab) {
    case 0:
      draw_aim_tab();
      break;
    case 1:
      draw_esp_tab();
      break;
    case 2:
      draw_visuals_tab();
      break;
    }
  }
  
  ImGui::End();  
}

#endif
