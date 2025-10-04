#ifndef MATH_HPP
#define MATH_HPP

#include "imgui/dearimgui.hpp"

#include "print.hpp"

#include "vec.hpp"

//180/pi
#define radpi 57.295779513082f

//pi/180
#define pideg 0.017453293f

inline static float(*view_matrix)[4][4];
// https://github.com/Ripper99-x/CS2-Internal/blob/de8d5c9e01befa723bf00ae31355388f0681f9b6/SourceEngine/W2S.h#L10
inline static bool world_to_screen(Vec3 point, Vec3* screen) {
  const float w = (*view_matrix)[3][0] * point.x + (*view_matrix)[3][1] * point.y + (*view_matrix)[3][2] * point.z + (*view_matrix)[3][3];

  if (w <= 0.01)
    return false;

  const float invw = 1.0 / w;
  const Vec3 screen_size = {3440.0, 1440.0};
  const float x = screen_size.x * 0.5;
  const float y = screen_size.y * 0.5;

  screen->x = x + (((*view_matrix)[0][0] * point.x + (*view_matrix)[0][1] * point.y + (*view_matrix)[0][2] * point.z + (*view_matrix)[0][3]) * invw * x);
  screen->y = y - (((*view_matrix)[1][0] * point.x + (*view_matrix)[1][1] * point.y + (*view_matrix)[1][2] * point.z + (*view_matrix)[1][3]) * invw * y);
  
  return true;
}


#endif
