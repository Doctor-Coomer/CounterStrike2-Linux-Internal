#include "aimbot.hpp"
#include "../../gui/config.hpp"
#include "../../math.hpp"
#include "../../interfaces/input_system.hpp"
#include "../../interfaces/game_entity_system.hpp"
#include "../../interfaces/cvar_system.hpp"
#include "../../classes/pawn.hpp"
#include "../../gui/menu.hpp"
#include <random>

bool shoot_next_tick = false;

// helper random float generator
static float random_float(float min, float max) {
  static std::mt19937 rng(std::random_device{}());
  std::uniform_real_distribution<float> dist(min, max);
  return dist(rng);
}

void aimbot(Vec3 original_view_angles) {
  if (config.aimbot.master == false||menu_focused) return;

  Pawn* localpawn = entity_system->get_localpawn();
  if (localpawn == nullptr) {
    return;
  }

  static Convar* team_is_enemy = cvar_system->get_convar("mp_teammates_are_enemies");
  if (team_is_enemy == nullptr) {
    return;
  }

  float smallest_fov_angle = __FLT_MAX__;

  for (unsigned int i = 1; i <= 64; ++i) {
    Pawn* pawn = entity_system->pawn_from_index(i);
    if (pawn == nullptr
      || pawn == localpawn
      || localpawn->get_lifestate() != 0
      || pawn->is_dormant()
      || (pawn->get_cs_team() == localpawn->get_cs_team() && team_is_enemy->get_value<bool>() == false)
      || pawn->get_lifestate() != 0)
    {
      continue;
    }

    Vec3 bone = pawn->get_bone_location(6);
    Vec3 shoot_location = localpawn->get_eye_position();
    Vec3 diff = {
      bone.x - shoot_location.x,
      bone.y - shoot_location.y,
      bone.z - shoot_location.z
    };

    float yaw_hyp = sqrt((diff.x * diff.x) + (diff.y * diff.y));
    float pitch_angle = atan2(diff.z, yaw_hyp) * radpi;
    float yaw_angle = atan2(diff.y, diff.x) * radpi;

    Vec3 desired_angles = {
      .x = -pitch_angle,
      .y = yaw_angle,
      .z = 0
    };

    // recoil compensation
    if (config.aimbot.recoil == true) {
      desired_angles -= (localpawn->get_aim_punch() * 2);
    }

    float x_diff = desired_angles.x - original_view_angles.x;
    float y_diff = desired_angles.y - original_view_angles.y;

    float x = remainderf(x_diff, 360.0f);
    float y = remainderf(y_diff, 360.0f);

    float clamped_x = x > 89.0f ? 89.0f : x < -89.0f ? -89.0f : x;
    float clamped_y = y > 180.0f ? 180.0f : y < -180.0f ? -180.0f : y;

    float fov = hypotf(clamped_x, clamped_y);

    if (fov <= config.aimbot.fov && fov < smallest_fov_angle) {
      target_pawn = pawn;
      smallest_fov_angle = fov;
    }

    if (target_pawn == pawn && (fov > config.aimbot.fov)) {
      target_pawn = nullptr;
    }

    if (is_button_down(config.aimbot.key) && target_pawn == pawn) {
      float smooth_factor = config.aimbot.smooth;

      // --- RANDOMNESS SECTION ---
      // random smoothing variance (e.g. ±15%)
      float smooth_random = smooth_factor * random_float(0.85f, 1.15f);


      // --- END RANDOMNESS SECTION ---

      if (smooth_random < 1.0f)
        smooth_random = 1.0f;

      float applied_x = clamped_x;
      float applied_y = clamped_y;

      if (smooth_random > 1.0f) {
        applied_x = clamped_x / smooth_random;
        applied_y = clamped_y / smooth_random;
      }

      Vec3 final_angles = {
        .x = original_view_angles.x + applied_x,
        .y = original_view_angles.y + applied_y,
        .z = 0
      };

      // clamp final angles
      final_angles.x = final_angles.x > 89.0f ? 89.0f : final_angles.x < -89.0f ? -89.0f : final_angles.x;
      final_angles.y = remainderf(final_angles.y, 360.0f);

      input->set_view_angles(final_angles);

      if (config.aimbot.auto_shoot == true) {
        input->set_shoot(true);
      }
    }
  }
}
