#ifndef MENU_HPP
#define MENU_HPP

#include <SDL2/SDL.h>

#include "../imgui/dearimgui.hpp"

inline static SDL_Window* sdl_window = NULL;
inline static bool menu_focused = false;

static void draw_watermark() {
  ImGui::SetNextWindowPos(ImVec2(10, 10)); 
  ImGui::SetNextWindowSize(ImVec2(150, 30));
  ImGui::Begin("##Watermark", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    
  ImGui::TextCentered("I Use Arch BTW!!!");
  ImGui::End();
}

static void draw_esp_tab() {  
  ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 27);

  static bool test = true;
  ImGui::Checkbox("Master", &test);

  ImGui::EndGroup();

  ImGui::SameLine();
  ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
  ImGui::SameLine();

  /* ESP */
  ImGui::BeginChild("##EspMasterChild");

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
    draw_tab(style, "ESP", &tab, 0);

    switch (tab) {
    case 0:
      draw_esp_tab();
      break;      
    }
  }
  
  ImGui::End();  
}

#endif
