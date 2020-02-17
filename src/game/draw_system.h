#pragma once
#include <array>
#include <cstdint>
#include <ddraw.h>
#include <memory>
#include <windef.h> // HWND etc
#include <wingdi.h> // PALETTEENTRY

/// Include "draw_area.h" to use this class's methods.
class DrawArea;

/// Include "draw_area.h" to use this class's methods.
struct DrawAreaNode;

class DrawSystem {
private:
  HINSTANCE hinstance_ = nullptr;
  HWND hwnd_ = nullptr;
  HPALETTE main_palette_ = nullptr;
  DrawArea* back_buffer_ = nullptr;
  DrawArea* primary_buffer_ = nullptr;
  DrawAreaNode* draw_nodes = nullptr;
  IDirectDraw* directdraw_ = nullptr;
  IDirectDrawSurface* primary_surface_ = nullptr;
  IDirectDrawClipper* dd_clipper_ = nullptr;
  IDirectDrawPalette* dd_palette_ = nullptr;
  int32_t use_system_memory_ = 0;
  int32_t window_width_ = 0;
  int32_t window_height_ = 0;
  int32_t color_depth_ = 0;
  int32_t init_status_ = 0;
  int8_t draw_type_ = 0;
  int8_t fullscreen_mode_ = 0;
  int8_t _unknownValue1b = 0;
  int8_t _unknownValue1c = 0;
  int32_t custom_dd_display_mode_ = 0;
  int32_t is_bank_switched_ = 0;
  int32_t _transparentColorKeyBlittingForSourceEnabled = 0;
  int32_t _unknown2a = 0;
  int32_t _unknown2b = 0;
  int32_t supports_640x480 = 0;
  int32_t supports_800x600 = 0;
  int32_t supports_1024x768 = 0;
  int32_t supports_1280x1024 = 0;
  int32_t supports_1600x1200 = 0;
  int32_t _has_8Bit_320x200_Surface = 0;
  int32_t _has_8Bit_320x240_Surface = 0;
  int32_t _has_16Bit_320x200_Surface = 0;
  int32_t _has_16Bit_320x240_Surface = 0;
  int32_t _has_16Bit_640x480_Surface = 0;
  int32_t _has_16Bit_800x600_Surface = 0;
  int32_t _has_16Bit_1024x768_Surface = 0;
  int32_t _has_16Bit_1600x1200_Surface = 0;
  int32_t _has_24Bit_320x200_Surface = 0;
  int32_t _has_24Bit_320x240_Surface = 0;
  int32_t _has_24Bit_640x480_Surface = 0;
  int32_t _has_24Bit_800x600_Surface = 0;
  int32_t _has_24Bit_1024x768_Surface = 0;
  int32_t _has_24Bit_1600x1200_Surface = 0;
  int32_t _has_32Bit_320x200_Surface = 0;
  int32_t _has_32Bit_320x240_Surface = 0;
  int32_t _has_32Bit_640x480_Surface = 0;
  int32_t _has_32Bit_800x600_Surface = 0;
  int32_t _has_32Bit_1024x768_Surface = 0;
  int32_t _has_32Bit_1600x1200_Surface = 0;
  std::array<PALETTEENTRY, 256> palette_;

public:
  inline auto drawType() const { return this->draw_type_; }
};

