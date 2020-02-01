#pragma once
#include <array>
#include <ddraw.h>
#include <cstdint>
#include <memory>
#include <windef.h> // HWND
#include <wingdi.h> // bitmap stuff

/// Include "draw_system.h" to use this class's methods.
class DrawSystem;

class DrawArea;

class ColorTable;

struct DrawSpan {
  DrawSpan* next;
  DrawSpan* prev;
  int32_t start;
  int32_t end;
};

class SpanListManager {
  uint8_t super_[0x3C];
  DrawSpan** line_heads_;
  DrawSpan** line_tails_;
  int32_t* leftmost_;
  int32_t* rightmost_;
  int32_t* span_count_;
  int32_t num_lines_;
  int32_t num_pixels_;
  int32_t min_span_;
  int32_t max_span_;
  int32_t min_line_;
  int32_t max_line_;

public:
  DrawSpan* lineHead(int32_t y) { return this->line_heads_[y]; }
  DrawSpan* lineTail(int32_t y) { return this->line_tails_[y]; }
  int32_t minSpan() { return this->min_span_; }
  int32_t maxSpan() { return this->max_span_; }
  int32_t minY() { return this->min_line_; }
  int32_t maxY() { return this->max_line_; }
};

struct BITMAPINFO256 {
  BITMAPINFOHEADER bmiHeader;
  RGBQUAD bmiColors[256];
};

struct DrawAreaNode {
  DrawArea* draw_area = nullptr;
  DrawAreaNode* prev = nullptr;
  DrawAreaNode* next = nullptr;

  DrawAreaNode(DrawArea* draw_area) : draw_area(draw_area) {}
};

class DrawArea {
  DrawSystem* draw_system_ = nullptr;
  HWND main_window_ = nullptr;
  uint8_t* bits_ = nullptr;
  std::unique_ptr<BITMAPINFO256> bitmap_info_ = nullptr;
  HGDIOBJ _unknownGdiObject2 = nullptr;
  HGDIOBJ _unknownGdiObject1 = nullptr;
  int32_t width_ = 0;
  int32_t height_ = 0;
  int32_t pitch_ = 0;
  int32_t orientation_ = 1;
  RECT clip_rect_ = {0, 0, 0, 0};
  HDC _gdiContext = nullptr;
  IDirectDrawSurface* surface_ = nullptr;
  DDSURFACEDESC surface_desc_;
  std::unique_ptr<DrawAreaNode> node_;
  char* name_ = nullptr;
  uint8_t* last_bits_ = nullptr;
  int32_t last_pitch_ = 0;
  uint8_t** display_offsets_ = nullptr;
  uint8_t** cur_display_offsets_ = nullptr;
  uint8_t** float_offsets_ = nullptr;
  ColorTable* shadow_table_ = nullptr;
  SpanListManager* span_list_ = nullptr;
  SpanListManager* current_span_list_ = nullptr;
  int32_t x_delta_ = 0;
  int32_t y_delta_ = 0;
  uint32_t display_offsets_size_ = 0;
  uint32_t float_offsets_size_ = 0;
  int32_t system_memory_only_ = 0;
  int32_t using_video_memory_ = 0;
  int32_t use_transparency_ = 0;
  int32_t is_primary_screen_buffer_ = 0;
  int32_t restored_ = 0;
  int32_t extended_lines_ = 0;
  int8_t transparent_color_ = -1;
  uint8_t*** blend_tables_ = nullptr;
  char blend_filename_[257];

public:
  HDC getDeviceContext(const char* name);
  void releaseDeviceContext(const char* name);
  void setClipRect(RECT* clip_rect);
  void clear(RECT* rect, uint8_t color);

  HDC deviceContext() const { return this->_gdiContext; }
};

