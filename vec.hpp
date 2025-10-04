#ifndef VEC_HPP
#define VEC_HPP

struct Vec2 {
  int x = 0, y = 0;
};

struct Vec3 {
  float x = 0.0, y = 0.0, z = 0.0;

  Vec3 operator+(const Vec3 v) {
    return Vec3{x + v.x, y + v.y, z + v.z};
  }

  Vec3 operator*(const Vec3 v) {
    return Vec3{x * v.x, y * v.y, z * v.z};
  }

  Vec3 operator*(float v) {
    return Vec3{x * v, y * v, z * v};
  }

  Vec3 operator*(int v) {
    return Vec3{x * v, y * v, z * v};
  }

  Vec3& operator-=(const Vec3 v) {
    x -= v.x; y -= v.y; z -= v.z; return *this;
  }

  Vec3& operator+=(const Vec3 v) {
    x += v.x; y += v.y; z += v.z; return *this;
  }

  Vec3& operator+=(float v) {
    x += v; y += v; z += v; return *this;
  }

  Vec3 operator-(const Vec3 v) {
    return Vec3{x - v.x, y - v.y, z - v.z};
  }

  bool operator!=(const Vec3 v) {
    return (this->x != v.x && this->y != v.y && this->z != v.z);
  }
};

struct __attribute__((aligned(16))) Vec3_aligned {
  float x = 0.0, y = 0.0, z = 0.0;
};

struct RGBA {
  int r = 255, g = 255, b = 255, a = 255;
};

struct RGBA_float {
  float r = 1.0, g = 1.0, b = 1.0, a = 1.0;

  RGBA to_RGBA() {
    return RGBA{.r = int(r * 255), .g = int(g * 255), .b = int(b * 255), .a = int(a * 255)};
  }

  float* to_arr() {
    return (float*)this;
  }
};

typedef float VMatrix[4][4];

#endif
