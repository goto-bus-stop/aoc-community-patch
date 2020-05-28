#include "draw_area.h"
#include "draw_system.h"
#include "../auto_hook.h"
#include <windows.h>

HDC DrawArea::getDeviceContext(const char* name) {
  if (this->draw_system_ && this->draw_system_->drawType() == 1) {
    return this->_gdiContext;
  }

  if (this->surface_ == nullptr)
    return nullptr;
  if (this->_gdiContext == nullptr) {
    if (this->bits_ != nullptr)
      return nullptr;

    this->surface_->GetDC(&this->_gdiContext);
  }

  return this->_gdiContext;
}

void DrawArea::releaseDeviceContext(const char* name) {
  if (this->draw_system_ && this->draw_system_->drawType() == 1)
    return;
  if (this->surface_ == nullptr)
    return;
  if (this->_gdiContext == nullptr)
    return;

  this->surface_->ReleaseDC(this->_gdiContext);
  this->_gdiContext = nullptr;
}

void DrawArea::setClipRect(RECT* clip_rect) {
  auto original = getMethod<void, DrawArea*, RECT*>(0x5A2FC0);
  original(this, clip_rect);
}

void DrawArea::clear(RECT* rect, uint8_t color) {
  auto original = getMethod<void, DrawArea*, RECT*, uint8_t>(0x5A2AF0);
  original(this, rect, color);
}
