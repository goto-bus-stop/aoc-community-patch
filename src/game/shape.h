#pragma once
#include "draw_area.h"
#include <cstdint>

class DrawFlags {
private:
  int32_t flags_;

public:
  DrawFlags() = delete;
  DrawFlags(const DrawFlags&) = default;

  bool isShaded() const { return (flags_ & 1) == 1; }

  static DrawFlags getInstance() { return *(DrawFlags*)0x6B4CE8; }
};

class Shape {
public:
  enum class ResourceLoadType {
    MemoryInvalid = -1,
    MemoryMapped = 0,
    MemoryAllocated = 1,
  };

  struct ClipMaskLine {
    int32_t start;
    int32_t end;
  };

  struct ClipMask {
    int32_t width;
    int32_t height;
    int32_t node_count;
    ClipMaskLine** line_list;
    ClipMaskLine* heap;
  };

  struct Outline {
    int16_t left;
    int16_t right;
  };

  struct SLPFrame {
    int32_t command_offsets;
    int32_t outline_offset;
    int32_t palette_offset;
    int32_t properties;
    int32_t width;
    int32_t height;
    int32_t anchor_x;
    int32_t anchor_y;
  };

  struct SLPData {
    char version[4];
    int32_t num_frames;
    char comment[24];
    SLPFrame frames[1];
  };

private:
  ResourceLoadType load_type_;
  int32_t load_size_;
  ClipMask** clip_masks_;
  SLPData* slp_data_;
  SLPFrame* frames_;
  bool is_loaded_;
  int32_t num_frames_;

  void fix_stormy_dog(int32_t drs_id);

public:
  Shape(const char* filename, int32_t drs_id) {
    auto original = getMethod<void, Shape*, const char*, int32_t>(0x4DAE00);
    original(this, filename, drs_id);
  }

  ~Shape() {
    auto original = getMethod<void, Shape*>(0x4DB110);
    original(this);
  }

  bool shapeBounds(int32_t frame_id, int32_t& frame_width,
                   int32_t& frame_height);
  bool shapeCheck(int32_t check_x, int32_t check_y, int32_t shape_num,
                  bool mirror, bool outline_valid, bool use_hotspot);
  bool shapeDraw(DrawArea* draw_area, int32_t x, int32_t y, int32_t frame_id,
                 char* color_table);
  bool shapeMirror(DrawArea* draw_area, int32_t x, int32_t y, int32_t frame_id,
                   char* color_table);
  bool shapeMinMax(int32_t& x_min, int32_t& y_min, int32_t& x_max,
                   int32_t& y_max, int32_t shape_num);
  char spriteCheck(SLPData* slp, SLPFrame* frame, int32_t check_x,
                   int32_t check_y, bool mirrored, bool outline_check);
};
