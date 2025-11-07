#pragma once
#include <cstdint>
#include <vector>

enum class ComponentType : uint8_t {
  POSITION = 0,
  VELOCITY = 1,
  SCALE    = 2,
  ROTATION = 3,
  HITBOX   = 4,
  PLAYER_TAG = 5,
  DAMAGE   = 6,
  CONTROLLABLE = 7,
  ENEMY_TAG = 8,
  MISSILE_TAG = 9,
  PARALLAX_LAYER = 10,
  AI = 11,
  HEALTH = 12,
  SCORE = 13,
  DEAD = 14,
  SPRITE = 15,
  ANIMATION = 16,
  HEALTH_BAR = 17,
  RECTANGLE_SHAPE = 18,
  CIRCLE_SHAPE = 19,
  SOUND_EFFECT = 20,
  UNKNOWN   = 255
};

struct Components {
  ComponentType type;
  union {
    struct { int16_t x_q, y_q; } pos;
    struct { int16_t texture_id, z_index, frame_width, frame_height; } sprite;
    struct { int16_t start_frame, num_frames, frame_duration, loops; } animation;
    struct { int16_t hp; } health;
    struct { int16_t owner, offset_x, offset_y; } health_bar;
    struct { int16_t x_q, y_q, w_q, h_q; } rect_shape;
    struct { int16_t radius, outline_thickness; } circle_shape;
    struct { int16_t x_q, y_q; } vel;
    struct { int16_t x_q, y_q; } scale;
    struct { int16_t angle; } rotation;
    struct { int16_t w_q, h_q; } hitbox;
    struct { int16_t damage; } damage;
    struct { int16_t x_q, y_q; } controllable;
    struct { int16_t id_player; } player_tag;
    struct { int16_t type; } enemy_tag; 
    struct { int16_t owner; } missile_tag;
    struct { int16_t scroll_speed; } parallax_layer;
    //  struct { int16_t x_q, y_q; } ai;
    struct { int16_t value; } score;
    struct { int16_t dead; } dead;
    struct { int16_t sound_id, to_play; } sound_effect;

  };

  Components() : type(ComponentType::UNKNOWN) {}
  Components(ComponentType t) : type(t) {}
};

struct EntitySnapshot {
    uint32_t id;
    std::vector<Components> components;
    uint8_t component_count;

    EntitySnapshot() : id(0) {}
    EntitySnapshot(uint32_t id) : id(id) {}
};

struct Snapshot {
  // Header
  uint32_t tick;

  // Entities
  std::vector<EntitySnapshot> entities;

  Snapshot() : tick(0) {}

  // Serialization methods
  std::vector<uint8_t> serialize() const;

  // Helper methods
  size_t calculateSize() const;
};